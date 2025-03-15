begin
	--*********** Ejercicio 1: Gestión del IRQs ***********
	IRQ_accepted <= 	'1' WHEN (IRQ = '1' and (IRQ_disabled = '0' and Parar_MIPS = '0')) else
						'0'; -- IRQ_accept = IRQ cuando IRQ_disabled no sea 1 y Parar_MIPS no sea 1



	--*********** Ejercicio 2: Gestión del PC ***********

	PC_src <= 	"00" WHEN (IRQ_accepted = '1') else -- Si hay excepcion PC_in = x"00000004"
				"01" WHEN (RTE = '1') else -- Si estamos en RTE PC_in = IRQ_LR
				"10" WHEN (Branch = '1' and Z = '1') else -- Si hay salto PC_in = Dirsalto_ID
				"11"; -- Si no PC_in = PC4
    
	Mux_PC: mux4_1 port map  ( DIn0 => x"00000004", DIn1 => IRQ_LR, DIn2 => Dirsalto_ID, DIn3 => PC4, ctrl => PC_src, Dout => PC_in)

    -- Ejercicio 2. Cargar en load_PC 1 menos excepciones
    Load_PC <= 	'1' WHEN (parar_ID = '0' or IRQ_accepted = '1') and parar_MIPS = '0' else 
				'0'; 

	-- IRQ tiene prioridad al codigo


	--*********** Ejercicio 3: Dirección retorno de IRQs ***********
	-- Ejercicio 3. Mux de IRQ_LR

	IRQ_LR_in <= 	PC_EX 		when (Valid_ex = '1' and RTE = '0') else -- IRQ_LR_in = PC_EX cuando Valid_ex = 1
					PC_ID 		when (Valid_id = '1') else -- IRQ_LR_in = PC_ID cuando valid_id = 1
					PC_out; -- Si no utilizar PC_out

	Load_LR <= 	'1' WHEN (IRQ_accepted = '1' and Parar_MIPS = '0') else -- Si hay excepcion Load_LR = 1. ¿EL PARAR_MIPS ES NECESARIO?
				'0'; -- Si no Load_LR = 0



	--*********** Ejercicio 4: Registro de estados (SR) *********** 
	SR_input <=		"11" 		when (IRQ_accepted = '1' and Parar_MIPS = '0')	 else -- Si estamos IRQ ha sido aceptado SR_inp = 11
					"00"; -- Si no SR_inp = 00

	Update_SR <= 	'1' 		when ((IRQ_accepted = '1' and Parar_MIPS = '1') or RTE = '1') else -- Si hay excepcion Update_SR = 1
					'0'; -- Si no Update_SR = 0
