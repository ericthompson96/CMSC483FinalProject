make:
	mpicc  -g  -Wall  -o  proj1 proj1.c	
##	gcc --std=c99 -Wall -O2 -o hw2 global_sum.c -pthread

run:
	mpiexec  -n  4 ./proj1
##	mpiexec  -n  1 ./proj1	
clean:
	rm -rf proj1
