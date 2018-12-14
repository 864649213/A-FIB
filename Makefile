OPCIONS = -D_GLIBCXX_DEBUG -O2 -Wall -Wextra -Werror -Wno-uninitialized -Wno-sign-compare -std=c++11


program.exe: program.o fileManager.o
	g++ -o program.exe program.o fileManager.o

fileManager.o: fileManager.cc 
	g++ -c fileManager.cc $(OPCIONS)

program.o: program.cc
	g++ -c program.cc $(OPCIONS)

tar:
	tar -cvf practica.tar *.cc *.hh Makefiles

clean:
	rm *.o
	rm *.exe
	rm *.tar
