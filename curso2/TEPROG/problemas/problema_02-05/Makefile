PROG:=main
SRCS:=Cuenta.cc CuentaCorriente.cc PlazoFijo.cc main.cc

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
main.o: main.cc Cuenta.h CuentaCorriente.h PlazoFijo.h
main.s: main.cc Cuenta.h CuentaCorriente.h PlazoFijo.h
