PROG:=main
SRCS:=main.cc shape.cc collide.cc

CXX:= g++ -std=c++20 -Wall

OBJS:=$(SRCS:.cc=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $(OBJS)

.cc.o:
	$(CXX) -c $<

.cc.s:
	$(CXX) -c -S $<

edit:
	geany -i -s *.h *.cc &

clean:
	@rm -f $(PROG) $(OBJS) core *.s *.gch

# Dependencias
main.o: shape.h collide.h
shape.o: shape.h
collide.o: collide.h shape.h
