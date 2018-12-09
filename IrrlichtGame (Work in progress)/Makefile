CPPFLAGS = -I/usr/include/irrlicht
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter

irrlicht-selection: main.o events.o
	g++ $^ -o $@ -lIrrlicht

main.o events.o: events.h
clean:
	@rm -f irrlicht-selection *.o
