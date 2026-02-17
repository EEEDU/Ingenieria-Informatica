PROG:=main
SRCS:=Cuenta.cc CuentaCorriente.cc PlazoFijo.cc CuentaNomina.cc CuentaDivisa.cc main.cc

CXX:= g++ -O2

OBJS:=$(SRCS:.cc=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $(OBJS)

.cc.o:
	$(CXX) -c $<

edit:
	geany -s *.h *.cc &

clean:
	@rm -f $(PROG) $(OBJS) core *.s

cuenta.o: Cuenta.cc Cuenta.h
CuentaCorriente.o: CuentaCorriente.cc Cuenta.h
PlazoFijo.o: PlazoFijo.cc Cuenta.h
CuentaNomina.o: CuentaNomina.cc CuentaNomina.h
CuentaDivisa.o: CuentaDivisa.cc CuentaDivisa.h
main.o: main.cc Cuenta.h CuentaCorriente.h PlazoFijo.h CuentaNomina.h CuentaDivisa.h
main.s: main.cc Cuenta.h CuentaCorriente.h PlazoFijo.h CuentaNomina.h CuentaDivisa.h
