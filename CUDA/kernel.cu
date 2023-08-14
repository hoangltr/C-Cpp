#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuda.h"
#include <stdio.h>
#include <time.h>
__global__ void FPN(int *s, int n)//Find prime numbers
{
	int i = blockIdx.x * blockDim.x + threadIdx.x, j;
	bool t;
	if ((i >= 2) && (i <=n))
	{
		if (i == 2)
		{
			t = true;
		}
		else
		{
			for (j = 2; j < i; j++)
			{
				t = true;
				if (i % j == 0)
				{
					t = false;
					break;
				}
			}
		}
		if (t == true)
		{

			s[i] = i;
		}
		else
		{
			s[i] = 0;
		}
	}
}
__global__ void ttgpu(int *s, int *tt, int n)//Calculate the sum of prime numbers
{
	int i;
	tt[0] = 0;
	for (i = 2; i <= n; i++)
	{
		tt[0] = tt[0] + s[i];
	}
}
int main(void)
{
	//Introduce program
	printf("Program : Find the sum of prime numbers from 1 to n using GPU \n\n");
	//Declare variable
	int n, i, block, * s, * a, * tt;
	clock_t start, end;
	double time_use;
	//Enter the value n (1000 < n < 9999)
	printf_s("Enter n : \nn=");
	scanf_s("%d", &n);
	//Memory allocation
	cudaMallocManaged((void**)&s, (n + 1) * sizeof(int));
	cudaMallocManaged((void**)&tt, sizeof(int));
	//Start recording time
	start = clock();
	//Find primes and calculate the sum of primes from 1 to n 
	FPN <<<n+1, 1 >>> (s, n);
	ttgpu <<<1, 1 >>> (s, tt, n);
	cudaDeviceSynchronize();
	//Finish recording timeinish recording time
	end = clock();
	//Find time GPU used
	time_use = (double)(end - start) / CLOCKS_PER_SEC;
	//Print results
	printf_s("Total number of primes from 1 to %d : %d\n", n, tt[0]);
	printf_s("Time used : %lfs\n",time_use);
	//Release memory
	cudaFree(s);
	cudaFree(tt);
	return;
}