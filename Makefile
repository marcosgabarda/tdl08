all: AFD.o AFN.o GIC.o
	g++ test.cpp AFN.o AFD.o GIC.o -o test -lm
AFD.o:
	g++ -c AFD.cpp
AFN.o:
	g++ -c AFN.cpp
GIC.o:
	g++ -c GIC.cpp
debug:
	g++ test.cpp AFD.cpp AFN.cpp GIC.cpp -o test -lm -g
clean:
	rm -f *~ test *.o