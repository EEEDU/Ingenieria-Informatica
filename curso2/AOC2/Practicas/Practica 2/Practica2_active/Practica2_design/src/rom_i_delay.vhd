----------------------------------------------------------------------------------
-- IMPORTANTE: CADA ESTUDIANTE DEBE COMPLETAR SUS DATOS 
-- Name: Eduardo Guerrero
-- NIA: 816106
-- Create Date: 28-02-2025    
-- Module Name: memoriaROM_I
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity memoriaROM_I is 	
generic (	propagation_delay: time := 8 ns --propagation delay of the ROM
    		);  
port (
		ADDR : in std_logic_vector (6 downto 0); --Dir 
        RE : in std_logic;		-- read enable		  
		Dout : out std_logic_vector (31 downto 0)
		);
end memoriaROM_I;

architecture Behavioral of memoriaROM_I is
type RomType is array(0 to 127) of std_logic_vector(31 downto 0);
signal ROM : RomType := (  			-- Asiganar valores a r1-r7
									X"00202710", -- MOV 10000, r1
									X"00400000", -- MOV 0, r2
									X"00600000", -- MOV 0, r3
									X"00801770", -- MOV 6000, r4
									X"00A00064", -- MOV 100, r5
									X"00C00000", -- MOV 0, r6
									X"00E00006", -- MOV 6, r7

									-- Reiniciar r0
									X"00000000", -- MOV 0, r0

									-- Acumular valores de r1-r7 en r0
									X"40011000", -- ADD r1, r2, r0
									X"40030000", -- ADD r3, r0, r0
									X"40040000", -- ADD r4, r0, r0
									X"40050000", -- ADD r5, r0, r0
									X"40060000", -- ADD r6, r0, r0
									X"40070000", -- ADD r7, r0, r0

									-- HALT
									X"80000000",  -- HALT

									X"00000000", --word 15
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",-- words 16,...
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 64,...
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",--word 72,...
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 80,...
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 88,...
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 96,...
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 104,...
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --word 112,...
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000");--word 120,...

signal internal_Dout : std_logic_vector (31 downto 0);
																		
begin
 	--read port
     internal_Dout <= ROM(conv_integer(ADDR)) when (RE='1') else X"00000000"; --sólo se lee si RE vale 1
     -- Propagation delay
     Dout <= internal_Dout after propagation_delay;

end Behavioral;


