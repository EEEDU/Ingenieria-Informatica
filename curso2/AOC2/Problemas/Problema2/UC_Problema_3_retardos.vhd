----------------------------------------------------------------------------------
-- IMPORTANT: EACH STUDENT MUST FILL THIS WITH HIS PERSONAL INFO
-- Name: Eduardo Guerrero
-- NIA: 816106
-- Create Date: 11/02/2025      
-- Module Name:    UC_P3
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity UC_P3 is
	generic (	propagation_delay: time := 10 ns); -- propagation delay of the UC. The setup will be the default setup of the register
	Port ( 	clk : in  STD_LOGIC;
		   	reset : in  STD_LOGIC;
    		start: in STD_LOGIC;
			Load_inputs : out  STD_LOGIC; 	--Load the four initical values
    		load_suma1 : out  STD_LOGIC;	-- Load the results of the two first adders
    		load_suma2 : out  STD_LOGIC;	-- Load the results of the two first adders
    		load_suma2 : out  STD_LOGIC;	-- Load the results of the two first adders
			load_output : out  STD_LOGIC	-- Final result. We must load only the final result
		   );
end UC_P3;

architecture Behavioral of UC_P3 is

   	type state_type is (Inicio, Suma_inicial, Suma_final ); --Use this three states for your control unit design
	signal state, next_state : state_type; 
	signal internal_load_inputs, internal_load_suma1, internal_load_output: STD_LOGIC; 
	begin
	
	-- State Register
	--We do not use the component register because we do not want to codify the states, but use their names instead.
	State_reg: process (clk)
	   begin
	      if (clk'event and clk = '1') then
	         if (reset = '1') then
	            state <= Inicio;
	         else
	            state <= next_state;
	         end if;        
	      end if;
	   end process;
	
	-- UC mealy, this process includes the logic that generates the next state and the outputs
	-- The sensitivity list ir wrong. What should it be there? Does it make any sense to include clk in this is combinatorial logic?
	UC_outputs : process (clk)--Fix the sensitivity list or nothing will work
	begin 
		-- We set the outpus to '0'. This way, in the case statement we anly need to define the values that are '1' 
		internal_load_inputs <= '0'; 
		internal_load_suma1 <= '0'; 
		internal_load_suma2 <= '0'; 
		internal_load_suma3 <= '0'; 
		internal_load_output <= '0'; 
		next_state <= Inicio;
		CASE state IS
			-- This is an example of possible actions for the first state. They are not correct.
			-- Modify them, including your own actions 
			WHEN  Inicio  	=>  If (start = '1') then next_state <= Inicio; internal_load_output <= '1' ;
								ELSE
									internal_load_output <= '1' ;
									next_state <= Inicio;
								END IF;
			WHEN  Suma_inicial  =>  internal_load_output <= '1'; next_state <= Inicio;
			WHEN  Suma_final  	=>  internal_load_output <= '1'; next_state <= Inicio;
			-- Default value
			WHEN OTHERS 	  	=> 
		END CASE;
	end process;
	-- Next lines assign the value to the outputs after the propagation delay
	load_inputs <= internal_load_inputs after propagation_delay;
	load_suma1  <= internal_load_suma1 after propagation_delay;
	load_suma2  <= internal_load_suma2 after propagation_delay;
	load_suma3  <= internal_load_suma3 after propagation_delay;
	load_output <= internal_load_output after propagation_delay;
	
	

end Behavioral;



