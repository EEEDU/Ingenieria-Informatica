----------------------------------------------------------------------------------
-- Description: Mips segmentado tal y como lo hemos estudiado en clase. Sus características son:
-- Saltos 1-retardados
-- instrucciones aritméticas, LW, SW y BEQ
-- MI y MD de 128 palabras de 32 bits
-- 32 bits de entrada y otros 32 de salida para IO mapeados en las direcciones 0xffff0000 (entrada) y 0xffff0004 (salida)
-- Línea de IRQ
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity MIPs_segmentado is
    Port ( clk : in  STD_LOGIC;
           	reset : in  STD_LOGIC
           	);
end MIPs_segmentado;

architecture Behavioral of MIPs_segmentado is

component reg is
    generic (size: natural := 32);  -- por defecto son de 32 bits, pero se puede usar cualquier tamaño
	Port ( Din : in  STD_LOGIC_VECTOR (size -1 downto 0);
	clk : in  STD_LOGIC;
	reset : in  STD_LOGIC;
	load : in  STD_LOGIC;
	Dout : out  STD_LOGIC_VECTOR (size -1 downto 0));
end component;
---------------------------------------------------------------

component adder32 is
    Port ( Din0 : in  STD_LOGIC_VECTOR (31 downto 0);
           Din1 : in  STD_LOGIC_VECTOR (31 downto 0);
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component mux4_1 is
  Port (   DIn0 : in  STD_LOGIC_VECTOR (31 downto 0);
           DIn1 : in  STD_LOGIC_VECTOR (31 downto 0);
           DIn2 : in  STD_LOGIC_VECTOR (31 downto 0);
           DIn3 : in  STD_LOGIC_VECTOR (31 downto 0);
		   ctrl : in  STD_LOGIC_VECTOR (1 downto 0);
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component mux2_1 is
  Port (   DIn0 : in  STD_LOGIC_VECTOR (31 downto 0);
           DIn1 : in  STD_LOGIC_VECTOR (31 downto 0);
			  ctrl : in  STD_LOGIC;
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component Data_Memory_Subsystem is port (
	CLK : in std_logic;
	reset: in std_logic; 
	ADDR : in std_logic_vector (31 downto 0); --Dir solicitada por el Mips
	Din : in std_logic_vector (31 downto 0);--entrada de datos desde el Mips
	WE : in std_logic;	-- write enable	del MIPS
	RE : in std_logic;	-- read enable del MIPS	
	Mem_ready: out std_logic; -- indica si podemos hacer la operación solicitada en el ciclo actual. Cuando se complique el subsistema a veces habrá que esperar
	Dout : out std_logic_vector (31 downto 0)); --dato que se envía al Mips
end component;


component memoriaRAM_I is port (
	CLK : in std_logic;
	ADDR : in std_logic_vector (31 downto 0); --Dir 
	Din : in std_logic_vector (31 downto 0);--entrada de datos para el puerto de escritura
	WE : in std_logic;		-- write enable	
	RE : in std_logic;		-- read enable		  
	Dout : out std_logic_vector (31 downto 0));
end component;

component Banco_ID is
 Port ( IR_in : in  STD_LOGIC_VECTOR (31 downto 0); -- instrucción leida en IF
        PC4_in:  in  STD_LOGIC_VECTOR (31 downto 0); -- PC+4 sumado en IF
		clk : in  STD_LOGIC;
		reset : in  STD_LOGIC;
        load : in  STD_LOGIC;
        IR_ID : out  STD_LOGIC_VECTOR (31 downto 0); -- instrucción en la etapa ID
        PC4_ID:  out  STD_LOGIC_VECTOR (31 downto 0);
        --bits de validez. Permiten inhabilitar instrucciones. No se hace en la práctica 3, pero sí en los proyecto
        valid_I_IF: in STD_LOGIC;
        valid_I_ID: out STD_LOGIC); 

end component;

COMPONENT BReg is
    PORT(
        clk : IN  std_logic;
	 	reset : in  STD_LOGIC;
        RA : IN  std_logic_vector(4 downto 0);
        RB : IN  std_logic_vector(4 downto 0);
        RW : IN  std_logic_vector(4 downto 0);
        BusW : IN  std_logic_vector(31 downto 0);
        RegWrite : IN  std_logic;
        BusA : OUT  std_logic_vector(31 downto 0);
        BusB : OUT  std_logic_vector(31 downto 0)
        );
END COMPONENT;

component Ext_signo is
    Port ( inm : in  STD_LOGIC_VECTOR (15 downto 0);
           inm_ext : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component two_bits_shifter is
    Port ( Din : in  STD_LOGIC_VECTOR (31 downto 0);
           Dout : out  STD_LOGIC_VECTOR (31 downto 0));
end component;

component UC is
	Port ( valid_I_ID : in  STD_LOGIC; --indica si es una instrucción válida			
		IR_op_code : in  STD_LOGIC_VECTOR (5 downto 0);
        Branch : out  STD_LOGIC;
        RegDst : out  STD_LOGIC;
        ALUSrc : out  STD_LOGIC;
		MemWrite : out  STD_LOGIC;
        MemRead : out  STD_LOGIC;
        MemtoReg : out  STD_LOGIC;
        RegWrite : out  STD_LOGIC;
        --nuevas señales jal y ret
        jal : out  STD_LOGIC; --indica que es una instrucción jal 
        ret : out  STD_LOGIC; --indica que es una instrucción ret
		undef: out STD_LOGIC --indica que el código de operación no pertenence a una instrucción conocida. En este procesador se usa sólo para depurar
           );
end component;

COMPONENT Banco_EX is
        Port (  clk : in  STD_LOGIC;
			reset : in  STD_LOGIC;
			load : in  STD_LOGIC;
	        busA : in  STD_LOGIC_VECTOR (31 downto 0);
           	busB : in  STD_LOGIC_VECTOR (31 downto 0);
			busA_EX : out  STD_LOGIC_VECTOR (31 downto 0);
           	busB_EX : out  STD_LOGIC_VECTOR (31 downto 0);
           	RegDst_ID : in  STD_LOGIC;
           	ALUSrc_ID : in  STD_LOGIC;
           	MemWrite_ID : in  STD_LOGIC;
           	MemRead_ID : in  STD_LOGIC;
           	MemtoReg_ID : in  STD_LOGIC;
           	RegWrite_ID : in  STD_LOGIC;
			inm_ext: IN  std_logic_vector(31 downto 0);
			inm_ext_EX: OUT  std_logic_vector(31 downto 0);
           	RegDst_EX : out  STD_LOGIC;
           	ALUSrc_EX : out  STD_LOGIC;
           	MemWrite_EX : out  STD_LOGIC;
           	MemRead_EX : out  STD_LOGIC;
           	MemtoReg_EX : out  STD_LOGIC;
           	RegWrite_EX : out  STD_LOGIC;
			ALUctrl_ID: in STD_LOGIC_VECTOR (2 downto 0);
			ALUctrl_EX: out STD_LOGIC_VECTOR (2 downto 0);
           	Reg_Rt_ID : in  STD_LOGIC_VECTOR (4 downto 0);
           	Reg_Rd_ID : in  STD_LOGIC_VECTOR (4 downto 0);
           	Reg_Rt_EX : out  STD_LOGIC_VECTOR (4 downto 0);
           	Reg_Rd_EX : out  STD_LOGIC_VECTOR (4 downto 0);
           	--bits de validez. Permiten inhabilitar instrucciones. No se hace en la práctica 3, pero sí en los proyecto
        	valid_I_EX_in: in STD_LOGIC;
        	valid_I_EX: out STD_LOGIC;
        	-- Puertos para añadir señales
			-- Estos puertos se utilizan para añadir funcionalidades al MIPS que requieran enviar información de una etapa a las etapas siguientes
			-- El banco permite enviar dos señales de un bit (ext_signal_1 y 2) y dos palabras de 32 bits ext_word_1 y 2)
			ext_signal_1_ID: in  STD_LOGIC;
			ext_signal_1_EX: out  STD_LOGIC;
			ext_signal_2_ID: in  STD_LOGIC;
			ext_signal_2_EX: out  STD_LOGIC;
			ext_word_1_ID:  IN  STD_LOGIC_VECTOR (31 downto 0);
			ext_word_1_EX:  OUT  STD_LOGIC_VECTOR (31 downto 0);
			ext_word_2_ID:  IN  STD_LOGIC_VECTOR (31 downto 0);
			ext_word_2_EX:  OUT  STD_LOGIC_VECTOR (31 downto 0)
			--fin puertos extensión
        	);
end component;
        
COMPONENT ALU
    PORT(
         DA : IN  std_logic_vector(31 downto 0);
         DB : IN  std_logic_vector(31 downto 0);
         ALUctrl : IN  std_logic_vector(2 downto 0);
         Dout : OUT  std_logic_vector(31 downto 0)
         );
END COMPONENT;
	 
component mux2_5bits is
    Port ( 
	DIn0 : in  STD_LOGIC_VECTOR (4 downto 0);
	DIn1 : in  STD_LOGIC_VECTOR (4 downto 0);
	ctrl : in  STD_LOGIC;
	Dout : out  STD_LOGIC_VECTOR (4 downto 0));
end component;
	
COMPONENT Banco_MEM is
	Port ( 		ALU_out_EX : in  STD_LOGIC_VECTOR (31 downto 0); 
	ALU_out_MEM : out  STD_LOGIC_VECTOR (31 downto 0); -- instrucción leida en IF
	clk : in  STD_LOGIC;
	reset : in  STD_LOGIC;
	load : in  STD_LOGIC;
	MemWrite_EX : in  STD_LOGIC;
	MemRead_EX : in  STD_LOGIC;
	MemtoReg_EX : in  STD_LOGIC;
	RegWrite_EX : in  STD_LOGIC;
	MemWrite_MEM : out  STD_LOGIC;
	MemRead_MEM : out  STD_LOGIC;
	MemtoReg_MEM : out  STD_LOGIC;
	RegWrite_MEM : out  STD_LOGIC;
	BusB_EX: in  STD_LOGIC_VECTOR (31 downto 0); -- para los store
	BusB_MEM: out  STD_LOGIC_VECTOR (31 downto 0); -- para los store
	RW_EX : in  STD_LOGIC_VECTOR (4 downto 0); -- registro destino de la escritura
	RW_MEM : out  STD_LOGIC_VECTOR (4 downto 0);    	
	--bits de validez. Permiten inhabilitar instrucciones. No se hace en la práctica 3, pero sí en los proyecto
	valid_I_EX: in STD_LOGIC;
	valid_I_MEM: out STD_LOGIC;
	-- Puertos para añadir señales
	-- Estos puertos se utilizan para añadir funcionalidades al MIPS que requieran enviar información de una etapa a las etapas siguientes
	-- El banco permite enviar dos señales de un bit (ext_signal_1 y 2) y dos palabras de 32 bits ext_word_1 y 2)
	ext_signal_1_EX: in  STD_LOGIC;
	ext_signal_1_MEM: out  STD_LOGIC;
	ext_signal_2_EX: in  STD_LOGIC;
	ext_signal_2_MEM: out  STD_LOGIC;
	ext_word_1_EX:  IN  STD_LOGIC_VECTOR (31 downto 0);
	ext_word_1_MEM:  OUT  STD_LOGIC_VECTOR (31 downto 0);
	ext_word_2_EX:  IN  STD_LOGIC_VECTOR (31 downto 0);
	ext_word_2_MEM:  OUT  STD_LOGIC_VECTOR (31 downto 0)
	--fin puertos extensión
	);
    END COMPONENT;
 
    COMPONENT Banco_WB is
	Port ( 	ALU_out_MEM : in  STD_LOGIC_VECTOR (31 downto 0); 
		ALU_out_WB : out  STD_LOGIC_VECTOR (31 downto 0); 
		MEM_out : in  STD_LOGIC_VECTOR (31 downto 0); 
		MDR : out  STD_LOGIC_VECTOR (31 downto 0); --memory data register
        clk : in  STD_LOGIC;
		reset : in  STD_LOGIC;
        load : in  STD_LOGIC;
		MemtoReg_MEM : in  STD_LOGIC;
        RegWrite_MEM : in  STD_LOGIC;
        MemtoReg_WB : out  STD_LOGIC;
        RegWrite_WB : out  STD_LOGIC;
        RW_MEM : in  STD_LOGIC_VECTOR (4 downto 0); -- registro destino de la escritura
        RW_WB : out  STD_LOGIC_VECTOR (4 downto 0); -- PC+4 en la etapa IDend Banco_WB;
        --bits de validez. Permiten inhabilitar instrucciones. No se hace en la práctica 3, pero sí en los proyecto
        valid_I_WB_in: in STD_LOGIC;
        valid_I_WB: out STD_LOGIC;
        -- Puertos para añadir señales
		-- Estos puertos se utilizan para añadir funcionalidades al MIPS que requieran enviar información de una etapa a las etapas siguientes
		-- El banco permite enviar dos señales de un bit (ext_signal_1 y 2) y dos palabras de 32 bits ext_word_1 y 2)
		ext_signal_1_MEM: in  STD_LOGIC;
		ext_signal_1_WB: out  STD_LOGIC;
		ext_signal_2_MEM: in  STD_LOGIC;
		ext_signal_2_WB: out  STD_LOGIC;
		ext_word_1_MEM:  IN  STD_LOGIC_VECTOR (31 downto 0);
		ext_word_1_WB:  OUT  STD_LOGIC_VECTOR (31 downto 0);
		ext_word_2_MEM:  IN  STD_LOGIC_VECTOR (31 downto 0);
		ext_word_2_WB:  OUT  STD_LOGIC_VECTOR (31 downto 0)
		--fin puertos extensión
		);
    END COMPONENT; 
    
    COMPONENT counter 
 	generic (
   		size : integer := 10);
	Port ( clk : in  STD_LOGIC;
       		reset : in  STD_LOGIC;
       		count_enable : in  STD_LOGIC;
       		count : out  STD_LOGIC_VECTOR (size-1 downto 0));
	end COMPONENT;
-- Señales internas MIPS	
	CONSTANT ARIT : STD_LOGIC_VECTOR (5 downto 0) := "000001";
	signal load_PC, RegWrite_ID, RegWrite_EX, RegWrite_MEM, RegWrite_WB, RegWrite, Z, Branch_ID, RegDst_ID, RegDst_EX, ALUSrc_ID, ALUSrc_EX: std_logic;
	signal MemtoReg_ID, MemtoReg_EX, MemtoReg_MEM, MemtoReg_WB, MemWrite_ID, MemWrite_EX, MemWrite_MEM, MemRead_ID, MemRead_EX, MemRead_MEM, WE, RE: std_logic;
	signal PC_in, PC_out, PC4, Dirsalto_ID, IR_in, IR_ID, PC4_ID, inm_ext_EX, ALU_Src_out : std_logic_vector(31 downto 0);
	signal BusW, BusA, BusB, BusA_EX, BusB_EX, BusB_MEM, inm_ext, inm_ext_x4, ALU_out_EX, ALU_out_MEM, ALU_out_WB, Mem_out, MDR : std_logic_vector(31 downto 0);
	signal RW_EX, RW_MEM, RW_WB, Reg_Rs_ID, Reg_Rt_ID, Reg_Rd_EX, Reg_Rt_EX: std_logic_vector(4 downto 0);
	signal ALUctrl_ID, ALUctrl_EX : std_logic_vector(2 downto 0);
	signal IR_op_code: std_logic_vector(5 downto 0);
	signal reset_ID, reset_MEM, load_ID, load_EX, load_Mem : std_logic;
	signal undef : std_logic;
--señales de control de los muxes 4 a 1 que hemos añadido apra el jal y el ret(en el mips original eran 2 a 1)
		signal ctrl_Mux4a1_escritura_BR, PCSrc: std_logic_vector (1 downto 0);
-- Instrucciones jal y ret
	signal jal_ID, ret_ID : std_logic; --si necesitáis propagar las señales a otras etapas, definid las señales necesarias. Ejemplo: jal_EX, jal_MEM...
-- Bit validez etapas. Permiten inhabilitar instrucciones. No se hace en la práctica 3, pero sí en los proyecto
	signal valid_I_IF, valid_I_ID, valid_I_EX_in, valid_I_EX, valid_I_MEM, valid_I_WB_in, valid_I_WB: std_logic;
-- contadores
	signal cycles: std_logic_vector(15 downto 0);
	signal Ins : std_logic_vector(7 downto 0);
	signal inc_cycles, inc_I : std_logic;
--interfaz con memoria
	signal Mem_ready : std_logic;
	

begin
	pc: reg generic map (size => 32)
			port map ( Din => PC_in, clk => clk, reset => reset, load => load_PC, Dout => PC_out);
	
	------------------------------------------------------------------------------------
	-- vale '1' porque en la versión actual el procesador no para nunca
	-- Si queremos detener una instrucción en la etapa fetch habrá que ponerlo a '0'
	load_PC <=  '1'; 
	------------------------------------------------------------------------------------
	 -- la x en x"00000004" indica que está en hexadecimal
	adder_4: adder32 port map (Din0 => PC_out, Din1 => x"00000004", Dout => PC4);
	------------------------------------------------------------------------------------
	-- mux de entrada al PC (PC+4 o direccion de salto si se detecta salto tomado en D)
	-- Tiene dos entradas que no se usan. Se pueden utilizar para dar soporte a nuevas intrucciones
	muxPC: mux4_1 port map (Din0 => PC4, DIn1 => Dirsalto_ID, Din2 => x"00000000", DIn3 => x"00000000", ctrl => PCSrc, Dout => PC_in);							
	------------------------------------------------------------------------------------
	Mem_I: memoriaRAM_I PORT MAP (CLK => CLK, ADDR => PC_out, Din => x"00000000", WE => '0', RE => '1', Dout => IR_in);
	------------------------------------------------------------------------------------
	reset_ID <= reset;
	-- Las señal valid_I_IF es siempre '1' porque nunca eliminamos una instrucción en este procesador
	valid_I_IF <= '1';	
	-- el load vale uno porque este procesador no para nunca. Si queremos que una instrucción no avance habrá que poner el load a '0'
	load_ID <= '1';
-----------------------------------------------------------------
	Banco_IF_ID: Banco_ID port map ( 
				IR_in => IR_in, PC4_in => PC4, clk => clk, reset => reset_ID, load => load_ID, 
				IR_ID => IR_ID, PC4_ID => PC4_ID,
				valid_I_IF => valid_I_IF, valid_I_ID => valid_I_ID);
	------------------------------------------Etapa ID-------------------------------------------------------------------
	Reg_Rs_ID <= IR_ID(25 downto 21);
	Reg_Rt_ID <= IR_ID(20 downto 16);
	--------------------------------------------------
	-- BANCOS DE REGISTROS
	
	-- sólo se escribe si la instrucción en WB es válida
	RegWrite <= RegWrite_WB and valid_I_WB;
	
	INT_Register_bank: BReg PORT MAP (
				clk => clk, reset => reset, RA => Reg_Rs_ID, RB => Reg_Rt_ID, RW => RW_WB, BusW => BusW, 
				RegWrite => RegWrite, BusA => BusA, BusB => BusB
			);
	
	-------------------------------------------------------------------------------------
	sign_ext: Ext_signo port map (inm => IR_ID(15 downto 0), inm_ext => inm_ext);
	
	two_bits_shift: two_bits_shifter port map (Din => inm_ext, Dout => inm_ext_x4);
	
	adder_dir: adder32 port map (Din0 => inm_ext_x4, Din1 => PC4_ID, Dout => Dirsalto_ID);
	
	Z <= '1' when (busA=busB) else '0';
	
	-------------------------------------------------------------------------------------
	IR_op_code <= IR_ID(31 downto 26);
	
	-- Si la Instrucción en ID no es válida, todas las señales son 0
	UC_seg: UC port map (
				valid_I_ID => valid_I_ID, IR_op_code => IR_op_code, Branch => Branch_ID, RegDst => RegDst_ID,  
				ALUSrc => ALUSrc_ID, MemWrite => MemWrite_ID,  
				MemRead => MemRead_ID, MemtoReg => MemtoReg_ID, RegWrite => RegWrite_ID,
				undef => undef,
				jal => jal_ID, ret => ret_ID
			);
				
	-- Salto tomado se debe activar cada vez que la instrucción en D produzca un salto en la ejecución.
	-- Eso incluye los saltos tomados en los BEQs (Z AND Branch_ID)
	-- Como en la versión inicial las entradas 2 y 3 no se usan, PCSrc(1) empieza con valor '0'
	PCSrc(1) <= '0';
	PCSrc(0) <= Z AND Branch_ID;
								
	-- En este procesador no se invalidan instrucciones en ID. En el proyecto lo haremos
	valid_I_EX_in	<=  valid_I_ID;
				
	-------------------------------------------------------------------------------------
	-- si la operación es aritmética (es decir: IR_op_code= "000001") miro el campo funct
	-- como sólo hay 4 operaciones en la alu, basta con los bits menos significativos del campo func de la instrucción	
	-- si no es aritmética le damos el valor de la suma (000)
	ALUctrl_ID <= IR_ID(2 downto 0) when IR_op_code= ARIT else "000"; 
	
	
	-- Banco ID/EX parte de enteros
	load_EX <= '1';
	Banco_ID_EX: Banco_EX PORT MAP (
		clk => clk, reset => reset, load => load_EX, busA => busA, busB => busB, busA_EX => busA_EX, busB_EX => busB_EX,
		RegDst_ID => RegDst_ID, ALUSrc_ID => ALUSrc_ID, MemWrite_ID => MemWrite_ID, MemRead_ID => MemRead_ID,
		MemtoReg_ID => MemtoReg_ID, RegWrite_ID => RegWrite_ID, RegDst_EX => RegDst_EX, ALUSrc_EX => ALUSrc_EX,
		MemWrite_EX => MemWrite_EX, MemRead_EX => MemRead_EX, MemtoReg_EX => MemtoReg_EX, RegWrite_EX => RegWrite_EX,
		ALUctrl_ID => ALUctrl_ID, ALUctrl_EX => ALUctrl_EX, inm_ext => inm_ext, inm_ext_EX=> inm_ext_EX,
		Reg_Rt_ID => IR_ID(20 downto 16), Reg_Rd_ID => IR_ID(15 downto 11), Reg_Rt_EX => Reg_Rt_EX, Reg_Rd_EX => Reg_Rd_EX, 
		valid_I_EX_in => valid_I_EX_in, valid_I_EX => valid_I_EX,
		-- Puertos de extensión. Inicialmente están desconectados
		ext_word_1_ID => x"00000000", ext_word_2_ID => x"00000000", ext_signal_1_ID => '0', ext_signal_2_ID => '0',
		ext_word_1_EX => open, ext_word_2_EX => open, ext_signal_1_EX => open, ext_signal_2_EX => open
		); 	
	
	------------------------------------------Etapa EX-------------------------------------------------------------------
	
	muxALU_src: mux2_1 port map (Din0 => BusB_EX, DIn1 => inm_ext_EX, ctrl => ALUSrc_EX, Dout => ALU_Src_out);
	
	ALU_MIPs: ALU PORT MAP ( DA => BusA_Ex, DB => ALU_Src_out, ALUctrl => ALUctrl_EX, Dout => ALU_out_EX);
	
	
	mux_dst: mux2_5bits port map (Din0 => Reg_Rt_EX, DIn1 => Reg_Rd_EX, ctrl => RegDst_EX, Dout => RW_EX);
	
	
	reset_MEM <= (reset);
	load_MEM <= '1'; -- este procesador no para; cambiar por otra señal para implementar paradas
	Banco_EX_MEM: Banco_MEM PORT MAP ( ALU_out_EX => ALU_out_EX, ALU_out_MEM => ALU_out_MEM, clk => clk, 
		reset => reset_MEM, load => load_MEM, MemWrite_EX => MemWrite_EX,
		MemRead_EX => MemRead_EX, MemtoReg_EX => MemtoReg_EX, RegWrite_EX => RegWrite_EX, 
		MemWrite_MEM => MemWrite_MEM, MemRead_MEM => MemRead_MEM,
		MemtoReg_MEM => MemtoReg_MEM, RegWrite_MEM => RegWrite_MEM, 
		BusB_EX => BusB_EX, BusB_MEM => BusB_MEM, 
		RW_EX => RW_EX, RW_MEM => RW_MEM,
		valid_I_EX => valid_I_EX, valid_I_MEM => valid_I_MEM,
		-- Puertos de extensión. Inicialmente están desconectados
		ext_word_1_EX => x"00000000", ext_word_2_EX => x"00000000", ext_signal_1_EX => '0', ext_signal_2_EX => '0',
		ext_word_1_MEM => open, ext_word_2_MEM => open, ext_signal_1_MEM => open, ext_signal_2_MEM => open
		);
													
	
	--
	------------------------------------------Etapa MEM-------------------------------------------------------------------
	--
	
	WE <= MemWrite_MEM and valid_I_MEM; --sólo se escribe si es una instrucción válida
	RE <= MemRead_MEM and valid_I_MEM; --sólo se lee si es una instrucción válida
	
	Mem_D: Data_Memory_Subsystem PORT MAP (
			CLK => CLK, ADDR => ALU_out_MEM, Din => BusB_MEM, WE => MemWrite_MEM, 
			RE => MemRead_MEM, reset => reset, Mem_ready => Mem_ready, Dout => Mem_out
			);

	
	-- La instrucción en WB será válida el próximo ciclo si la instrucción en Mem es válida
	valid_I_WB_in <= valid_I_MEM;
	
	Banco_MEM_WB: Banco_WB PORT MAP (
			ALU_out_MEM => ALU_out_MEM, ALU_out_WB => ALU_out_WB, Mem_out => Mem_out, MDR => MDR, 
			clk => clk, reset => reset, load => '1', 
			MemtoReg_MEM => MemtoReg_MEM, RegWrite_MEM => RegWrite_MEM, MemtoReg_WB => MemtoReg_WB, RegWrite_WB => RegWrite_WB, 
			RW_MEM => RW_MEM, RW_WB => RW_WB,
			valid_I_WB_in => valid_I_WB_in, valid_I_WB => valid_I_WB,
			-- Puertos de extensión. Inicialmente están desconectados
			ext_word_1_MEM => x"00000000", ext_word_2_MEM => x"00000000", ext_signal_1_MEM => '0', ext_signal_2_MEM => '0',
			ext_word_1_WB => open, ext_word_2_WB => open, ext_signal_1_WB => open, ext_signal_2_WB => open
			);
	
	--
	------------------------------------------Etapa WB-------------------------------------------------------------------						
	--	Mux 4 a 1. Inicialmente sólo se usan dos entradas, y las otras dos están desconectadas, pero se pueden usar para las nuevas instrucciones	
	--  Para ello hay que realizar las conexiones necesarias, y ajustar la señal de control del multiplexor			
	ctrl_Mux4a1_escritura_BR <= '0'&MemtoReg_WB	;
	mux_busW: mux4_1 port map (Din0 => ALU_out_WB, DIn1 => MDR, DIn2 => x"00000000", DIn3 => x"00000000", ctrl => ctrl_Mux4a1_escritura_BR, Dout => busW);
	
--------------------------------------------------------------------------------------------------
----------- Contadores de eventos. Nos permiten calcular métricas de rendimiento como el CPI
-------------------------------------------------------------------------------------------------- 
	-- Contador de ciclos totales
	cont_cycles: counter 	generic map (size => 16)
							port map (clk => clk, reset => reset, count_enable => inc_cycles, count => cycles);
	-- Contador de Instrucciones ejecutadas
	cont_I: counter 		generic map (size => 8)
							port map (clk => clk, reset => reset, count_enable => inc_I, count => Ins);
	
	inc_cycles <= '1';--Done
	inc_I <= valid_I_WB; --Cuenta las instrucciones válidas que llegan a WB 
		
end Behavioral;

