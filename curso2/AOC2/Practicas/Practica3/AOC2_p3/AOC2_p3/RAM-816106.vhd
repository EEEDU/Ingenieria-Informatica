----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:12:11 04/04/2014 
-- Design Name: 
-- Module Name:    memoriaRAM - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;
-- Memoria RAM de 128 oalabras de 32 bits
entity Data_Memory_Subsystem is port (
	CLK : in std_logic;
	reset: in std_logic;
	ADDR : in std_logic_vector (31 downto 0); --Dir solicitada por el Mips
	Din : in std_logic_vector (31 downto 0);--entrada de datos desde el Mips
	WE : in std_logic;		-- write enable	del MIPS
	RE : in std_logic;		-- read enable del MIPS	
	Mem_ready: out std_logic; -- indica si podemos hacer la operación solicitada en el ciclo actual. En esta memoria vale siempre '1'.
	Dout : out std_logic_vector (31 downto 0)); --dato que se envía al Mips
end Data_Memory_Subsystem;

architecture Behavioral of Data_Memory_Subsystem is
type RamType is array(0 to 127) of std_logic_vector(31 downto 0);
signal RAM : RamType := (  	X"00000000", 
							X"0000001C", -- @1: 7*4 = 28 en hexadecimal
							X"00000030", -- @2: 12*4 = 48 en hexadecimal
							X"00000004", -- @3: 4 en hexadecimal
							X"00000000", X"00000000", X"00000000", 
							X"000C4B40", -- @7: 800000 en hexadecimal -- palabras 0,1,2,3,4,5,6,7
							X"000186A0", -- @8: 10000 en hexadecimal
							X"00001770", -- @9: 6000 en hexadecimal
							X"00000064", -- @10: 100 en hexadecimal
							X"00000000", -- @11: 0 en hexadecimal
							X"00000006", -- @12: 6 en hexadecimal
							X"00000000", X"00000000", X"00000000", --palabras 8,9,...
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
									X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000");				  

signal dir_7:  std_logic_vector(6 downto 0); 
begin
 
 dir_7 <= ADDR(8 downto 2); -- como la memoria es de 128 plalabras no usamos la dirección completa sino sólo 7 bits. Como se direccionan los bytes, pero damos palabras no usamos los 2 bits menos significativos
 MEM_ready <= '1'; --En esta versión la memoria siempre está preparada
 process (CLK)
    begin
        if (CLK'event and CLK = '1') then
            if (WE = '1') then -- sólo se escribe si WE vale 1
                RAM(conv_integer(dir_7)) <= Din;
                -- la siguiente línea saca un aviso por pantalla con el dato que se ha escrito
                report "Simulation time : " & time'IMAGE(now) & ".  Data written: " & integer'image(to_integer(unsigned(Din))) & ", in ADDR = " & integer'image(to_integer(unsigned(ADDR)));
            end if;
        end if;
    end process;

    Dout <= RAM(conv_integer(dir_7)) when (RE='1') else "00000000000000000000000000000000"; --sólo se lee si RE vale 1

end Behavioral;

