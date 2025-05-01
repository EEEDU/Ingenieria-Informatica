-- TestBench Template 

  LIBRARY ieee;
  USE ieee.std_logic_1164.ALL;
  USE ieee.numeric_std.ALL;
  use IEEE.std_logic_arith.all;
  use IEEE.std_logic_unsigned.all;


  ENTITY testbench_MD_mas_MC IS
  END testbench_MD_mas_MC;

  ARCHITECTURE behavior OF testbench_MD_mas_MC IS 

  -- Component Declaration
  COMPONENT RAM_128_32 is port (
		CLK : in std_logic;
		Reset : in std_logic;
		ADDR : in std_logic_vector (31 downto 0); --Dir 
        Din : in std_logic_vector (31 downto 0);--entrada de datos para el puerto de escritura
        enable: in std_logic; -- If enable is 0 WE and RE are ignored
        WE : in std_logic;		-- write enable	
		RE : in std_logic;		-- read enable		
		Fetch_inc: in std_logic;		-- NUEVO: señal que indica que hay que hacer una FETCH_inc atómica usando el paradigma in-memory processing  
		Mem_ready: out std_logic; -- indica si puede hacer la operación solicitada en el ciclo actual
		Dout : out std_logic_vector (31 downto 0));
 end COMPONENT;

        SIGNAL clk, reset, RE, WE, Mem_ready, enable, Fetch_inc :  std_logic;
        signal ADDR, Din, Dout : std_logic_vector (31 downto 0);
        signal test_id : std_logic_vector (7 downto 0); --Para saber por qué prueba vamos
       
			           
  -- Clock period definitions
   constant CLK_period : time := 10 ns;
  BEGIN

  -- Component Instantiation
   uut: RAM_128_32 PORT MAP(clk=> clk, reset => reset, ADDR => ADDR, Din => Din, enable => enable, RE => RE, WE => WE, Fetch_inc=> Fetch_inc, Mem_ready => Mem_ready, Dout => Dout);

-- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;

 stim_proc: process
   		 variable done: integer :=0; -- la vamos a usar para la simulación
 	begin		
      	enable <= '0';
      	Fetch_inc <= '0';	
    	reset <= '1';
    	--conv_std_logic_vector convierte el primer número (un 0) a un vector de tantos bits como se indiquen (en este caso 32 bits)
    	addr <= conv_std_logic_vector(0, 32);
  	   	Din <= conv_std_logic_vector(0, 32);
		RE <= '0';
		WE <= '0';
	  	wait for 20 ns;	
	  	--Test 0---------------------------------------------------------------------------------------------------------
	  	-- Read without enable. Nothing should happend
	  	test_id <= conv_std_logic_vector(0, 8); --Test 0
	  	reset <= '0';
	  	RE <= '1';
	  	enable <= '0';	
	  	Addr <= conv_std_logic_vector(64, 32); 
	  	-- Esperamos a que la memoria esté preparada para atender otra solicitud
	  	-- a veces un pulso espureo (en este caso en mem_ready) puede hacer que vuestro banco de pruebas se adelante. 
        -- si esperamos un ns desaparecerá el pulso espureo, pero no el real
	  	done := 0;
        wait for 1 ns;
	  	while done < 1 loop
        	if Mem_ready = '0' then 
				wait until Mem_ready ='1'; --Este wait espera hasta que se ponga Mem_ready a uno
        		wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		else
	  		 	wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		end if;
	  	end loop;
    	wait until clk'event;
    	wait until clk'event;--esperamos al siguiente pulso de reloj
		--Test 1---------------------------------------------------------------------------------------------------------
		test_id <= conv_std_logic_vector(1, 8); --Test 1
      	-- Read with enable. 
      	RE <= '1';
      	enable <= '1';
      	Addr <= conv_std_logic_vector(64, 32); 
	  	-- Esperamos a que la memoria esté preparada para atender otra solicitud
	  	-- a veces un pulso espureo (en este caso en mem_ready) puede hacer que vuestro banco de pruebas se adelante. 
        -- si esperamos un ns desaparecerá el pulso espureo, pero no el real
	  	done := 0;
        wait for 1 ns;
	  	while done < 1 loop
        	if Mem_ready = '0' then 
				wait until Mem_ready ='1'; --Este wait espera hasta que se ponga Mem_ready a uno
        		wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		else
	  		 	wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		end if;
	  	end loop;
    	wait until clk'event;
    	wait until clk'event;--esperamos al siguiente pulso de reloj
		--Test 2---------------------------------------------------------------------------------------------------------
		test_id <= conv_std_logic_vector(2, 8); --Test 2
		-- write without enable. 
      	RE <= '0';
      	WE <= '1';
      	enable <= '0';
      	Addr <= conv_std_logic_vector(64, 32); 
      	Din <= conv_std_logic_vector(255, 32);
		-- Esperamos a que la memoria esté preparada para atender otra solicitud
	  	-- a veces un pulso espureo (en este caso en mem_ready) puede hacer que vuestro banco de pruebas se adelante. 
        -- si esperamos un ns desaparecerá el pulso espureo, pero no el real
	  	done := 0;
        wait for 1 ns;
	  	while done < 1 loop
        	if Mem_ready = '0' then 
				wait until Mem_ready ='1'; --Este wait espera hasta que se ponga Mem_ready a uno
        		wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		else
	  		 	wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		end if;
	  	end loop;
    	wait until clk'event;
    	wait until clk'event;--esperamos al siguiente pulso de reloj
		--Test 3---------------------------------------------------------------------------------------------------------
		test_id <= conv_std_logic_vector(3, 8); --Test 3
		-- write with enable. 
      	RE <= '0';
      	WE <= '1';
      	enable <= '1';
      	Addr <= conv_std_logic_vector(64, 32); 
      	Din <= conv_std_logic_vector(255, 32);
		-- Esperamos a que la memoria esté preparada para atender otra solicitud
	  	-- a veces un pulso espureo (en este caso en mem_ready) puede hacer que vuestro banco de pruebas se adelante. 
        -- si esperamos un ns desaparecerá el pulso espureo, pero no el real
	  	done := 0;
        wait for 1 ns;
	  	while done < 1 loop
        	if Mem_ready = '0' then 
				wait until Mem_ready ='1'; --Este wait espera hasta que se ponga Mem_ready a uno
        		wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		else
	  		 	wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		end if;
	  	end loop;
    	wait until clk'event;
    	wait until clk'event;--esperamos al siguiente pulso de reloj
		--Test 4---------------------------------------------------------------------------------------------------------
		test_id <= conv_std_logic_vector(4, 8); --Test 4: fetch-inc without enable
		Fetch_inc <= '1';	
		RE <= '0';
      	WE <= '0';
      	enable <= '0';
      	Addr <= conv_std_logic_vector(64, 32); 
      	Din <= conv_std_logic_vector(255, 32);
		-- Esperamos a que la memoria esté preparada para atender otra solicitud
	  	-- a veces un pulso espureo (en este caso en mem_ready) puede hacer que vuestro banco de pruebas se adelante. 
        -- si esperamos un ns desaparecerá el pulso espureo, pero no el real
	  	done := 0;
        wait for 1 ns;
	  	while done < 1 loop
        	if Mem_ready = '0' then 
				wait until Mem_ready ='1'; --Este wait espera hasta que se ponga Mem_ready a uno
        		wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		else
	  		 	wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		end if;
	  	end loop;
    	wait until clk'event;
    	wait until clk'event;--esperamos al siguiente pulso de reloj
		--Test 5---------------------------------------------------------------------------------------------------------
		test_id <= conv_std_logic_vector(5, 8); --Test 5: fetch-inc witht enable
		Fetch_inc <= '1';	
		RE <= '0';
      	WE <= '0';
      	enable <= '1';
      	Addr <= conv_std_logic_vector(64, 32); 
      	Din <= conv_std_logic_vector(255, 32);
		-- Esperamos a que la memoria esté preparada para atender otra solicitud
	  	-- a veces un pulso espureo (en este caso en mem_ready) puede hacer que vuestro banco de pruebas se adelante. 
        -- si esperamos un ns desaparecerá el pulso espureo, pero no el real
	  	done := 0;
        wait for 1 ns;
	  	while done < 1 loop
        	if Mem_ready = '0' then 
				wait until Mem_ready ='1'; --Este wait espera hasta que se ponga Mem_ready a uno
        		wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		else
	  		 	wait for 1 ns; -- para evitar avanzar en pulsos espurios
        		if Mem_ready ='1' then
	  				done := 1;
	  			end if;
	  		end if;
	  	end loop;
    	wait until clk'event;
    	wait until clk'event;--esperamos al siguiente pulso de reloj
		--Test 6---------------------------------------------------------------------------------------------------------
		-- Read with enable. 
      	Fetch_inc <= '0';	
		RE <= '1';
      	WE <= '0';
      	enable <= '1';
      	Addr <= conv_std_logic_vector(64, 32); 
		test_id <= conv_std_logic_vector(6, 8); 
		
	  	wait;
	  	
   end process;


  END;
