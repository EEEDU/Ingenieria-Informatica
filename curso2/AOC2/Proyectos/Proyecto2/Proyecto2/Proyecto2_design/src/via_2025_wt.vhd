library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all; -- used to convert std_logic to integers

entity Via is 
 	generic ( num_via: integer); -- used for messages. Must set the correct number when instantiated
 	port (	
        CLK : in std_logic;
		reset : in  STD_LOGIC;
 		Dir_word: in std_logic_vector(1 downto 0); -- used to select the word being accessed in a data cache set
 		Dir_cjto: in std_logic_vector(1 downto 0); -- used to select the set
 		Tag: in std_logic_vector(25 downto 0);
 		Din : in std_logic_vector (31 downto 0);
		WE : in  STD_LOGIC; 	-- write enable	
		Tags_WE : in  STD_LOGIC; 	-- write enable for the tag memory 
		-- NEW: signal for fetch_inc
	  	Fetch_inc: in std_logic;
	  	invalidate_bit: in std_logic;
		hit : out STD_LOGIC; -- indicates if there's a hit
		Dout : out std_logic_vector (31 downto 0)			
	) ;
end Via;
 			
Architecture Behavioral of Via is

component reg is
    generic (size: natural := 32);  -- default is 32-bit, but can be any size
	Port ( Din : in  STD_LOGIC_VECTOR (size -1 downto 0);
           clk : in  STD_LOGIC;
		   reset : in  STD_LOGIC;
           load : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (size -1 downto 0));
end component;

-- define the cache data memory as an array of 16 words of 32 bits
type Ram_MC_data is array(0 to 15) of std_logic_vector(31 downto 0);
signal MC_data : Ram_MC_data := (
    X"00000000", X"00000000", X"00000000", X"00000000",
    X"00000000", X"00000000", X"00000000", X"00000000",
    X"00000000", X"00000000", X"00000000", X"00000000",
    X"00000000", X"00000000", X"00000000", X"00000000"
);									
-- define the cache tag memory as an array of 4 words of 26 bits
type Ram_MC_Tags is array(0 to 3) of std_logic_vector(25 downto 0);
signal MC_Tags : Ram_MC_Tags := (
    "00000000000000000000000000", "00000000000000000000000000",
    "00000000000000000000000000", "00000000000000000000000000"
);												
signal valid_bits_in, valid_bits_out, mask_validate, mask_invalidate : std_logic_vector(3 downto 0); -- used to track if a block has valid data. Each bit represents a block										
signal valid_bit, update_valid_bits, internal_hit, validate_bit: std_logic;
signal Dir_MC: std_logic_vector(3 downto 0); -- used to read/write data stored in cache
signal MC_Tags_Dout: std_logic_vector(25 downto 0); 

begin 
-------------------------------------------------------------------------------------------------- 
-----cache_data_memory: RAM memory that stores the 4 blocks of 4 words each the cache can hold
-------------------------------------------------------------------------------------------------- 
Dir_MC <= Dir_cjto & Dir_word;

memoria_cache_D: process (CLK)
begin
    if (CLK'event and CLK = '1') then
        if (WE = '1') then -- only write if WE is 1
            MC_data(conv_integer(Dir_MC)) <= Din;
			-- report prints a message in the simulator console. It tells us what data was written, where, and when
			report "Simulation time : " & time'IMAGE(now) & ".  Data written in via " & integer'image(num_via) & ": " & integer'image(to_integer(unsigned(Din))) & ", in Dir_cjto = " & integer'image(to_integer(unsigned(Dir_cjto)));
        end if;
    end if;
end process;

Dout <= MC_data(conv_integer(Dir_MC)); 

-------------------------------------------------------------------------------------------------- 
-----MC_Tags: RAM memory that stores the 4 tags
-------------------------------------------------------------------------------------------------- 
memoria_cache_tags: process (CLK)
begin
    if (CLK'event and CLK = '1') then
        if (Tags_WE = '1') then -- only write if Tags_WE is 1
            MC_Tags(conv_integer(dir_cjto)) <= Tag;
			-- report prints a message in the simulator console. It tells us what tag was written, where, and when
			report "Simulation time : " & time'IMAGE(now) & ".  Tag written in via " & integer'image(num_via) & ": " & integer'image(to_integer(unsigned(Tag))) & ", in Dir_cjto = " & integer'image(to_integer(unsigned(dir_cjto)));
        end if;
    end if;
end process;

MC_Tags_Dout <= MC_Tags(conv_integer(dir_cjto)); 

-------------------------------------------------------------------------------------------------- 
-- Validity register. On reset, the valid bits are cleared to avoid false positives due to garbage in memory.
-- The valid bit is written at the same time as the tag memory. We must set the correct bit to 1 while keeping the others unchanged, which is done using a mask from a decoder.
-------------------------------------------------------------------------------------------------- 

-- mask_validate: used to validate a set
-- mask_invalidate: used to invalidate a set
mask_validate <= 	"0001" when dir_cjto="00" else
					"0010" when dir_cjto="01" else
					"0100" when dir_cjto="10" else
					"1000" when dir_cjto="11" else
					"0000";

mask_invalidate <= 	"1110" when dir_cjto="00" else
					"1101" when dir_cjto="01" else
					"1011" when dir_cjto="10" else
					"0111" when dir_cjto="11" else
					"0000";						

-- Valid bits are set to '1' when a new block has been stored in MC (Tags_WE = '1')
validate_bit <= '1' when (Tags_WE = '1') else '0';

-- Valid bits are cleared when it's a fetch_inc and there's a hit. In that case, the block is invalidated since memory content will change
update_valid_bits <= validate_bit or (invalidate_bit and internal_hit); -- If we’re told to invalidate, do so only on hit

-- Select the proper mask to validate or invalidate					
valid_bits_in <= (valid_bits_out OR mask_validate) 		when validate_bit = '1' else
				 (valid_bits_out AND mask_invalidate)	when invalidate_bit = '1' else
				 valid_bits_out;

bits_validez: reg 
    generic map (size => 4)
	port map(	
        Din => valid_bits_in, 
        clk => clk, 
        reset => reset, 
        load => update_valid_bits, 
        Dout => valid_bits_out
    );

-------------------------------------------------------------------------------------------------- 
valid_bit <= 	
    valid_bits_out(0) when dir_cjto = "00" else
	valid_bits_out(1) when dir_cjto = "01" else
	valid_bits_out(2) when dir_cjto = "10" else
	valid_bits_out(3) when dir_cjto = "11" else
	'0';

-------------------------------------------------------------------------------------------------- 
-- Hit signal: activated when the tag matches and the valid bit is 1
internal_hit <= '1' when ((MC_Tags_Dout = Tag) AND (valid_bit = '1')) else '0'; -- comparator checks if the tag in MC matches the one from the address, and if the valid bit is active, returns 1
hit <= internal_hit;

-------------------------------------------------------------------------------------------------- 
end Behavioral;