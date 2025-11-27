----------------------------------------------------------------------------------
-- IMPORTANTE: CADA ESTUDIANTE DEBE COMPLETAR SUS DATOS 
-- Name: Eduardo Guerrero
-- NIA: 816106
-- Create Date: 11/02/2025   
-- Module Name: P3
-- Additional Comments: 
--
----------------------------------------------------------------------------------

  LIBRARY ieee;
  USE ieee.std_logic_1164.ALL;
  USE ieee.numeric_std.ALL;

  ENTITY P3 IS
  	Port ( 	clk : in  STD_LOGIC;
           	reset : in  STD_LOGIC;
			start : in  STD_LOGIC;
			Din0 : in  STD_LOGIC_VECTOR (31 downto 0);
			Din1 : in  STD_LOGIC_VECTOR (31 downto 0);
			Din2 : in  STD_LOGIC_VECTOR (31 downto 0);
			Din3 : in  STD_LOGIC_VECTOR (31 downto 0);
			Din4 : in  STD_LOGIC_VECTOR (31 downto 0);
			Din5 : in  STD_LOGIC_VECTOR (31 downto 0);
			Dout : out  STD_LOGIC_VECTOR (31 downto 0)
           	);
  END P3;

  ARCHITECTURE behavior OF P3 IS 
-------------------------------------------------------------
  -- Component Declarations
  -- Register
	COMPONENT reg is
    generic (	Tsup_delay: time := 1 ns; --tiempo de Setup
	   			propagation_delay: time := 5 ns); 	 --retardo del registro  	
	Port ( Din : in  STD_LOGIC_VECTOR (31 downto 0);
           clk : in  STD_LOGIC;
		   reset : in  STD_LOGIC;
           load : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
    END COMPONENT;
    -- Adder
	
	COMPONENT adder32 is
	generic (	propagation_delay: time := 26 ns ---�retardo del sumador  	
    			);
    Port ( Din0 : in  STD_LOGIC_VECTOR (31 downto 0);
           Din1 : in  STD_LOGIC_VECTOR (31 downto 0);
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
	end component;

	-- UC
	   
    COMPONENT UC_P3 is
	generic (	Tsup_delay: time := 1 ns; --tiempo de Setup 		
				propagation_delay: time := 10 ns); -- retardo de la UC  		
	Port ( 	clk : in  STD_LOGIC;
		   	reset : in  STD_LOGIC;
    		start: in STD_LOGIC;
			Load_inputs : out  STD_LOGIC; 	--para los 4 registros iniciales
    		load_suma1 : out  STD_LOGIC;	-- para los dos registros que guardan los resultados de la primera suma
			load_suma2 : out  STD_LOGIC;	-- para los dos registros que guardan los resultados de la primera suma
			load_suma3 : out  STD_LOGIC;	-- para los dos registros que guardan los resultados de la primera suma
			load_output : out  STD_LOGIC	-- carga la salida final. No debemos cargar basura, s�lo los resultados correctos.
		   );
	end component;  
	
	------------------------------------------------           
-- Internal signals:   Define the needed internal signals 
--examples:	
    SIGNAL R0,R1,R2,R3, ,Adder1_output  :  std_logic_vector(31 downto 0);
    SIGNAL Load_inputs, load_suma1, load_output : STD_LOGIC;
--Additional signals are needed
	SIGNAL R4,R5, Adder0_output,Adder2_output, reg_adder0,reg_adder1,reg_adder2  :  std_logic_vector(31 downto 0);
    SIGNAL load_suma2, load_suma3 : STD_LOGIC;

------------------------------------------------       

  BEGIN

-- Component Instantiation
-- TODO: conect the modules assigning the proper signals to the open ports.
-- Example: Four of the needed registers are already instantiated, and one of the adders
	
	
	Reg_adder0_0: reg port map (	Din => Din0, clk => clk, reset => reset, load => Load_inputs, Dout => R0);
	Reg_adder0_1: reg port map (	Din => Din1, clk => clk, reset => reset, load => Load_inputs, Dout => R1);
	Reg_adder0_2: reg port map (	Din => Adder0_output, clk => clk, reset => reset, load => Load_inputs, Dout => reg_adder0);

	Reg_adder1_0: reg port map (	Din => Din2, clk => clk, reset => reset, load => Load_inputs, Dout => R2);
	Reg_adder1_1: reg port map (	Din => Din3, clk => clk, reset => reset, load => Load_inputs, Dout => R3);
	Reg_adder1_2: reg port map (	Din => Adder1_output, clk => clk, reset => reset, load => Load_inputs, Dout => reg_adder1);

	Reg_adder1_0: reg port map (	Din => Din4, clk => clk, reset => reset, load => Load_inputs, Dout => R4);
	Reg_adder1_1: reg port map (	Din => Din5, clk => clk, reset => reset, load => Load_inputs, Dout => R5);
	Reg_adder1_2: reg port map (	Din => Adder2_output, clk => clk, reset => reset, load => Load_inputs, Dout => reg_adder2);
	
			
	adder0: adder32 port map (Din0 => R0, Din1 => R1, Dout => Adder0_output);
	adder1: adder32 port map (Din0 => R2, Din1 => R3, Dout => Adder1_output);
	adder2: adder32 port map (Din0 => R4, Din1 => R5, Dout => Adder2_output);
	
	UC: UC_P3 PORT MAP (clk => clk, reset => reset, start => start, Load_inputs => Load_inputs, load_suma1 => load_suma1, load_suma2 => load_suma2, load_suma3 => load_suma3, load_output => load_output);
	
END;

-- El retardo maximo es de 123ns
