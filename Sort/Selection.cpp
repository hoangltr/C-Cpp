#include <iostream>
#define n 10

void SelectionSort(int A[])
{
    int min;
    for (int i = 0; i < n - 1; i++)
    {
        min = i;
        for (int j = i + 1; j < n; j++)
            if (A[j] < A[min])
                min = j;
        if (min != i)
        {
            A[i] += A[min];
            A[min] = A[i] - A[min];
            A[i] = A[i] - A[min];
        }
    }
}
int main()
{
    int A[n] = { 10,1,3,2,9,7,6,8,5,4 };
    SelectionSort(A);
    for (int i = 0; i < n; i++)
    {
        std::cout << A[i] << " ";
    }
}
