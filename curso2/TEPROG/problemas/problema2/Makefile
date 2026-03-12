PROG:=main
SRCS:=Store.cc StoreMax.cc StoreMin.cc StoreLast.cc StoreAvg.cc main.cc

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

Store.o: Store.cc Store.h
StoreMax.o: StoreMax.cc StoreMax.h
StoreMin.o: StoreMin.cc StoreMin.h
StoreLast.o: StoreLast.cc StoreLast.h
StoreAvg.o: StoreAvg.cc StoreAvg.h
main.o: main.cc Store.h StoreMax.h StoreMin.h StoreLast.h StoreAvg.h
main.s: main.cc Store.h StoreMax.h StoreMin.h StoreLast.h StoreAvg.h
