NAME=OPENACC

all: OPENACC

clean:
	rm -rf *.o

OPENACC:
	gcc -c *.c
	gcc -o main *.o -fopenmp -fopenacc -foffload=disable
