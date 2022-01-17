a.exe: main.o checkers.o 
	g++ -o a.exe main.o checkers.o 

main.o: main.cpp checkers.h game.h
	g++ -c main.cpp

checkers.o: checkers.cpp checkers.h game.h colormod.h
	g++ -c checkers.cpp 

debug:
	g++ -g -o a.exe main.cpp checkers.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups