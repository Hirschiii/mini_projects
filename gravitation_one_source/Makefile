
output: main.o
	g++ main.o -o output -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp

target: dependencies
	action

clean:
	rm *.o output
