# OpenMP

Here is a short description of the solution:

1. Since 1-dimensional jacobi algorithm is pretty simple, first thing I tried to do was use simple OpenMP clause ***#pragma omp parallel for*** before two cycles: **[jacobi1d_omp_v1.c](jacobi1d_omp_v1.c)**.
It gives some speedup, but obviously we need in some way modificate cycles to get a significant speedup.
2. So I rewrote the cycles and made them more efficient: **[jacobi1d_omp_v2.c](jacobi1d_omp_v2.c)**. 
3. Then I used OpenMP clause ***#pragma omp parallel for*** before modified cycle again and get the final OpenMP-parallelilzation for jacobi algorithm: **[jacobi1d_omp_final.c](jacobi1d_omp_final.c)**.

The final version were run on supercomputers. 

Jupyter-notebook with graphics: **[graphics.ipynb](graphics.ipynb)**.
