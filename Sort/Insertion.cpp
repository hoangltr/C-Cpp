#include <iostream>
#define n 10

void InsertionSort(int A[])
{
    int pos, x;
    for (int i = 1; i < n; i++)
    {
        x = A[i];
        pos = i - 1;
        while (pos >= 0 && A[pos] > x)
        {
            A[pos + 1] = A[pos];
            pos--;
        }
        A[pos + 1] = x;
    }
}
int main()
{
    int A[n] = { 10,1,3,2,9,7,6,8,5,4 };
    InsertionSort(A);
    for (int i = 0; i < n; i++)
    {
        std::cout << A[i] << " ";
    }
}
