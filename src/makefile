src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)
tests = $(wildcard test/*.cpp)

CXXFLAGS=-std=c++14 -I.

cast: $(obj)
	g++ $(CXXFLAGS) -o $@ $^ 

install: 
	cp cast /usr/local/bin

check: 
	g++ $(CXXFLAGS) $(tests) $(filter-out main.cpp, $(src)) -o test/cast_test
	(cd test && ./cast_test)

.PHONY: clean
clean:
	rm -f $(obj) cast test/cast_test
