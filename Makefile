LIBS= -lsfml-graphics -lsfml-window -lsfml-system 

Binary_name=topdown_shooter
Compiler=g++
CFLAGS=-Wall -std=c++11


all: topdown_shooter

topdown_shooter: main.o 
	@echo "Building the game!"

	$(Compiler) main.o -o $(Binary_name) $(LIBS)


main.o: main.cpp 
	$(Compiler) $(CFLAGS) -c main.cpp -o main.o 



Player_class.o: Player_class.h
	$(Compiler) $(CFLAGS) -c Player_class.h $(LIBS)

Enemy_class.o: Enemy_class.h
	$(Compiler) $(CFLAGS) -c Enemy_class.h $(LIBS)


clean:
	@echo "** REMOVING THE GAME ** "   
	rm *.o $(Binary_name)

install:
	@echo "** INSTALLING ..."
	cp $(Binary_name) /usr/bin


uninstall:
	@echo "** UNINSTALLING..."
	rm $(Binary_name)
