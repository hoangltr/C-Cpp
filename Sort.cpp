#include <iostream>
#define n 10

void BubleSort(int A[]) {
    int temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (A[j] > A[j + 1])
            {
                temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
            }
}
int main()
{
    int A[n] = { 10,1,3,2,9,7,6,8,5,4 };
    BubleSort(A);
    for (int i = 0; i < n; i++)
    {
        std::cout << A[i] << " ";
    }
}
