## Supercomputers and parallel programming

### Supercomputers and Parallel Data Processing course

A Supercomputers and Parallel Data Processing course on Computational Mathematics and Cybernetics Faculty of Moscow State University is held for the 3rd grade students. 

### Tasks
During the course students are offered to parallelize some algorithm using two technologies: OpenMP and MPI. Then they run their algorithms on MSU supercomputers: IBM Polus and IBM Bluegene. The result of the work is a report (with 3D-graphics) on the effectiveness of the proposed parallelization.

I was offered to parallelize 1-dimensional jacobi algorithm. Initial realization of the algorithm you can see in **[jacobi-1d.c](jacobi-1d.c)**.

In the directories **[OMP](OMP)** and **[MPI](MPI)** you can see jacobi-algorithm parallelization using each techology. Both solutions are written on C. Graphics are built using Python Matplotlib library.
