library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- This module monitors a system input and writes it to a memory address in the scratchpad through the bus.
-- In the current version, it always attempts to write (IO_M_Req = '1') the received data (IO_input), which represents a system input.
-- It writes it to word 0 of the scratchpad. This way, the processor can access the input value.

entity IO_master is
    Port ( 	
        clk: in  STD_LOGIC; 
		reset: in  STD_LOGIC; 
		IO_M_bus_Grant: in std_logic; 
		IO_input: in STD_LOGIC_VECTOR (31 downto 0);
		bus_TRDY : in  STD_LOGIC; -- indicates that the slave cannot perform the requested operation in this cycle
		Bus_DevSel: in  STD_LOGIC; -- indicates that the slave recognized the address as within its range
		IO_M_ERROR: out std_logic; -- Activated if the slave does not respond to its address
		IO_M_Req: out std_logic; 
		IO_M_Read: out std_logic; 
		IO_M_Write: out std_logic;
		IO_M_Fetch_inc: out std_logic;
		IO_M_bus_Frame: out std_logic; 
		IO_M_send_Addr: out std_logic;
		IO_M_send_Data: out std_logic;
		IO_M_last_word: out std_logic;
		IO_M_Addr: out STD_LOGIC_VECTOR (31 downto 0);
		IO_M_Data: out STD_LOGIC_VECTOR (31 downto 0)
    ); 
end IO_master;

Architecture Behavioral of IO_master is

type state_type is (Inicio, send_addr, send_data); 
type error_type is (addr_error, No_error); 
signal state, next_state : state_type; 
signal error_state, next_error_state : error_type; 
signal internal_IO_M_send_Addr: std_logic;

begin

-- Master state machine
--------------------------------------------------------------------------- 
-- State machine for writing to scratch memory 
---------------------------------------------------------------------------

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
   
--------------------------------------------------------------------------- 
-- 2023
-- State machine for the error bit 
---------------------------------------------------------------------------

Error_reg: process (clk)
begin
    if (clk'event and clk = '1') then
        if (reset = '1') then           
            error_state <= No_error;
        else
            error_state <= next_error_state;
        end if;   
    end if;
end process;

-- Mealy State-Machine - Outputs based on state and inputs
--------------------------------------------------------------------------- 
-- This state machine manages writing the IO input to the last word of the scratchpad 
---------------------------------------------------------------------------

OUTPUT_DECODE: process (state, IO_M_bus_Grant, bus_TRDY, Bus_DevSel)
begin
	-- Default values, used unless overwritten in a given state
	
	IO_M_Req <= '0'; -- if we don't have it, request the bus
	IO_M_bus_Frame <= '0';	
	IO_M_send_Addr <= '0';	
	IO_M_send_Data <= '0';	
	IO_M_last_word <= '0';	
	IO_M_Read <= '0'; -- never reads
	IO_M_Write <= '0';
	IO_M_Fetch_inc <= '0'; -- Currently does not use fetch_inc
	IO_M_Addr <= X"10000000"; -- Always send the same address: word 0 of scratchpad. Allows processor to read external input
	IO_M_Data <= IO_input; -- write the input data
	next_state <= Inicio; -- By default, remain in Inicio
	next_error_state <= error_state; -- by default, retain error state

	-- Initial state: wait
	If (state = Inicio) then
		IO_M_Req <= '1';
		If (IO_M_bus_Grant = '0') then -- if bus is not granted, do nothing
			next_state <= Inicio;
  		else  -- if bus is granted, send the address
     		next_state <= send_addr;
  		end if;

  	-- Bus has been granted. Activate frame and send address.  
	elsif state = send_addr then 	
  		IO_M_bus_Frame <= '1'; -- activate bus_frame to disable arbiter
		IO_M_send_Addr <= '1';
     	IO_M_Write <= '1';
		If (Bus_DevSel = '1') then -- if slave recognizes the address, proceed to data phase
			next_state <= send_data;
			next_error_state <= No_error; -- record that slave acknowledged the last address
		else
			next_error_state <= addr_error; -- record that slave did not acknowledge the last address
			next_state <= Inicio; -- If slave doesn't respond, end transfer
		end if;

  	elsif state = send_data then 	
     	IO_M_send_Addr <= '0';
     	IO_M_Write <= '1';
		IO_M_bus_Frame <= '1'; -- activate bus_frame to disable arbiter
		IO_M_send_Data <= '1'; -- send the input data
		IO_M_last_word <= '1'; -- indicate it's the last word

    	If (bus_TRDY = '1') then  -- if slave is ready, write to scratchpad
			next_state <= Inicio;		
		else
			next_state <= send_data;
		end if;   				
	end if;
end process;   

IO_M_ERROR <= '1' when (error_state = addr_error) else '0';

end Behavioral;

		