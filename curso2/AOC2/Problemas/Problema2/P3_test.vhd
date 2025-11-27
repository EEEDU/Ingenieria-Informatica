-- TestBench Template 

  LIBRARY ieee;
  USE ieee.std_logic_1164.ALL;
  USE ieee.numeric_std.ALL;

  ENTITY P3_testbench IS
  END P3_testbench;

  ARCHITECTURE behavior OF P3_testbench IS 

  -- Component Declaration
	COMPONENT P3 is
		Port ( 	clk : in  STD_LOGIC;
           	reset : in  STD_LOGIC;
			start : in  STD_LOGIC;
			Din0 : in  STD_LOGIC_VECTOR (31 downto 0);
			Din1 : in  STD_LOGIC_VECTOR (31 downto 0);
			Din2 : in  STD_LOGIC_VECTOR (31 downto 0);
			Din3 : in  STD_LOGIC_VECTOR (31 downto 0);
			Dout : out  STD_LOGIC_VECTOR (31 downto 0)
           	);
	END COMPONENT;

          SIGNAL clk, reset, start:  std_logic;
		  SIGNAL Dout: STD_LOGIC_VECTOR (31 downto 0);
         
          
  -- Clock period definitions
   constant CLK_period : time := 100 ns;
  BEGIN

  -- Component Instantiation
   uut: P3 PORT MAP(clk => clk, reset => reset, start => start, Din0 => x"00000001", Din1 => x"00000002", Din2 => x"00000003", Din3 => x"00000004", Dout => Dout);

-- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;

 stim_proc: process
   begin		
      	-- activamos reset
   		reset <= '1';
		start <= '0';
    	wait for CLK_period*2;
		-- desactivamos reset.
		reset <= '0';
		wait for CLK_period*2;
		-- activamos start
   		start <= '1';
		
		wait;
   end process;

  END;
