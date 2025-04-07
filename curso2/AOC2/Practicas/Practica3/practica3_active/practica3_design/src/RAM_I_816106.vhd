----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:38:16 04/08/2014 
-- Design Name: 
-- Module Name:    memoriaRAM_I - Behavioral 
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
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity memoriaRAM_I is port (
		  CLK : in std_logic;
		  ADDR : in std_logic_vector (31 downto 0); --Dir 
        Din : in std_logic_vector (31 downto 0);--entrada de datos para el puerto de escritura
        WE : in std_logic;		-- write enable	
		  RE : in std_logic;		-- read enable		  
		  Dout : out std_logic_vector (31 downto 0));
end memoriaRAM_I;

architecture Behavioral of memoriaRAM_I is
type RamType is array(0 to 127) of std_logic_vector(31 downto 0);

-- NIA.txt file
-- signal RAM : RamType := (  		
--                             X"08010004", -- LW R1, 4(R0)
--                             X"08020008", -- LW R2, 8(R0)
--                             X"0803000C", -- LW R3, 12(R0)
-- 							X"00000000", -- NOP
-- 							X"00000000", -- NOP
-- 							X"04431000", -- ADD R2, R2, R3
--                             X"14040001", -- JAL R4, 0  			-- palabras 0,1,2,3,4,5,6,7
-- 							X"00000000", -- NOP
--                             X"08250000", -- LW R5, 0(R1)
-- 							X"00000000", -- NOP	 
-- 							X"00000000", -- NOP	 
--                             X"04C53000", -- ADD R6, R6, R4
-- 							X"04230800", -- ADD R1, R1, R3
-- 							X"00000000", -- NOP	 
-- 							X"00000000", -- NOP	 
-- 							X"10220003", -- BEQ R1, R2, @fin (3)  						-- palabras 8,9,...	 15
-- 							X"00000000", -- NOP	 
--                             X"18800000", -- RET R4
-- 							X"00000000", -- NOP
-- 							X"0C060000", -- SW R6, 0(R0)	
-- 							X"1400FFFF", -- JAL R0,-1; Bucle infinito 
-- 								  X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
-- 								X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000");

-- NIA-R.txt file
signal RAM : RamType := (  		
                            X"08020008", -- LW R2, 8(R0)
                            X"0803000C", -- LW R3, 12(R0)
                            X"08010004", -- LW R1, 4(R0)
							X"00000000", -- NOP
							X"04431000", -- ADD R2, R2, R3	 
							X"14040001", -- JAL R4, 0
                            X"08250000", -- LW R5, 0(R1)
							X"10220005", -- BEQ R1, R2, @fin (3) 
							X"00000000", -- NOP	  			-- palabras 0,1,2,3,4,5,6,7
                            X"04C53000", -- ADD R6, R6, R4
                            X"04230800", -- ADD R1, R1, R3
                            X"18800000", -- RET R4
							X"00000000", -- NOP
							X"0C060000", -- SW R6, 0(R0)	
							X"1400FFFF", -- JAL R0,-1; Bucle infinito 
								X"00000000", 						-- palabras 8,9,...	 15
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
 process (CLK)
    begin
        if (CLK'event and CLK = '1') then
            if (WE = '1') then -- sólo se escribe si WE vale 1
                RAM(conv_integer(dir_7)) <= Din;
            end if;
        end if;
    end process;

    Dout <= RAM(conv_integer(dir_7)) when (RE='1') else "00000000000000000000000000000000"; --sólo se lee si RE vale 1

end Behavioral;


