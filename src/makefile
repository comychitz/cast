#
# define RPI=1 to compile for Raspberry Pi B+ V1.2 (Arm)
#
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)
tests = $(wildcard test/*.cpp)

CXXFLAGS=-std=c++14 -I.

ifeq ($(RPI), 1)
    CXXFLAGS += -DRPI
endif

cast: $(obj)
	g++ $(CXXFLAGS) -o $@ $^ 

install: 
	install cast /usr/local/bin
	mkdir -p /usr/local/share/cast/deps
	install -m 664 deps/*.cfg /usr/local/share/cast/deps

check: 
	g++ $(CXXFLAGS) $(tests) $(filter-out main.cpp, $(src)) -o test/cast_test
	(cd test && ./cast_test)

.PHONY: clean
clean:
	rm -f $(obj) cast test/cast_test
