ifeq ($(OS),Windows_NT)
PROG:=main.exe
else
PROG:=main
endif

SRCS:=main.cc

CXX:=g++ -std=c++23 -O2 -Wall -Wfatal-errors

OBJS:=$(addsuffix .o,$(basename $(SRCS)))
DEPS:=$(addsuffix .d,$(basename $(SRCS)))

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cc
	$(CXX) -c -MMD $<

%.o: %.cpp
	$(CXX) -c -MMD $<

clean:
ifeq ($(OS),Windows_NT)
	-@del /q $(PROG) $(OBJS) $(DEPS)
else
	-@rm -f $(PROG) $(OBJS) $(DEPS) core
endif

-include $(DEPS)
