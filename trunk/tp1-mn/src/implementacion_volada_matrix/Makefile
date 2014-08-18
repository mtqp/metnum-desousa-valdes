OBJS =  matrix_exceptions.cpp warp_cannon.cpp linear_system.cpp
OBJSo =  matrix_exceptions.o warp_cannon.o linear_system.o
MATH = -lm
CFLAGS = -ggdb 

all: tp2

tp2: main.cpp $(OBJSo)
	g++ -o tp2 $(CFLAGS) $(MATH) main.cpp $(OBJSo) 
	
%.o: %.cpp
	g++ -c $< $(MATH)
	
clean:
	rm -f tp2
	rm -f *.o
