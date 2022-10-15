all: mm mm_parallel mm_vectorised
mm: mm.cpp
	g++ -O2 -fopenmp mm.cpp -o mm
mm_parallel: mm_parallel.cpp
	g++ -O2 -fopenmp mm_parallel.cpp -o mm_parallel
mm_vectorised: mm_vectorised.cpp
	g++ -O2 -fopenmp mm_vectorised.cpp -o mm_vectorised
clean:
	rm -rf mm *~
	rm -rf mm_parallel *~
	rm -rf mm_vectorised *~