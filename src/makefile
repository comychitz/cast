src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

CXXFLAGS=-std=c++11

cast: $(obj)
	g++ $(CXXFLAGS) -o $@ $^ 

.PHONY: clean
clean:
	rm -f $(obj) cast
