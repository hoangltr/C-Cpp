#include<malloc.h>
#include<windows.h>
#include<stdio.h>
#include<string.h> 
struct SP //declare function struct
{
	char Name[30];
	char ID[10];
	int Price;
	int numS;
	int numL;
};
//Function 1
int maxp(struct SP *p, int n) //Find the most expensive product
{
	int i, max = (p + 0)->Price;
	for (i = 0; i < n; i++)
	{
		if (max < (p + i)->Price)
			max = (p + i)->Price;
	}
	return(max);
}
//Function 2
int minp(struct SP *p, int n)  //Find the cheapest product
{
	int i, min = (p + 0)->Price;
	for (i = 0; i < n; i++)
	{
		if (min >(p + i)->Price)
			min = (p + i)->Price;
	}
	return(min);
}
//Function 3
int maxS(struct SP *p, int n) //Find the best sold product
{
	int i, max = (p + 0)->numS;
	for (i = 0; i < n; i++)
	{
		if (max < (p + i)->numS)
			max = (p + i)->numS;
	}
	return(max);
}
//Function 4
int minS(struct SP *p, int n) //Find the least sold product
{
	int i, min = (p + 0)->numS;
	for (i = 0; i < n; i++)
	{
		if (min >(p + i)->numS)
			min = (p + i)->numS;
	}
	return(min);
}
//Function 5
int maxL(struct SP *p, int n) //Find the most inventory product
{
	int i, max = (p + 0)->numL;
	for (i = 0; i < n; i++)
	{
		if (max < (p + i)->numL)
			max = (p + i)->numL;
	}
	return(max);
}
//Function 6
int minL(struct SP *p, int n)  //Find the least inventory product
{
	int i, min = (p + 0)->numL;
	for (i = 0; i < n; i++)
	{
		if (min >(p + i)->numL)
			min = (p + i)->numL;
	}
	return(min);
}
//Function 7
void printrs(struct SP *p, int n, int m, int s) //Print results to the screen
{
	int i;
	switch (s)
	{
	case 1:
	{
			  for (i = 0; i < n; i++)
			  {
				  if ((p + i)->Price == m)
				  {
					  printf("|%-8d|%-10s|%-10s|%-12d%-3s|%-15d|%-18d|\n", i + 1, (p + i)->Name, (p + i)->ID, (p + i)->Price,"VND", (p + i)->numS, (p + i)->numL);
				  }
			  }
			  break;
	}
	case 2:
	{
			  for (i = 0; i < n; i++)
			  {
				  if ((p + i)->numS == m)
				  {
					  printf("|%-8d|%-10s|%-10s|%-12d%-3s|%-15d|%-18d|\n", i + 1, (p + i)->Name, (p + i)->ID, (p + i)->Price,"VND", (p + i)->numS, (p + i)->numL);
				  }
			  }
			  break;
	}
	case 3:
	{
			  for (i = 0; i < n; i++)
			  {
				  if ((p + i)->numL == m)
				  {
					  printf("|%-8d|%-10s|%-10s|%-12d%-3s|%-15d|%-18d|\n", i + 1, (p + i)->Name, (p + i)->ID, (p + i)->Price,"VND", (p + i)->numS, (p + i)->numL);
				  }
			  }
			  break;
	}
	}
}
//Main program
void main()
{
	system("color 1");
	printf("\t%70s","Welcome to the product management program!");
	Sleep(2000);
	system("color 7");
	fflush(stdin);
	int i, n, st;
	char test[2],check[100];
	struct SP *p;
	//Loop 1
	do
	{
		system("cls");
		printf("Enter the number of product : ");
		scanf_s("%d", &n);
		while (n < 0)
		{
			printf("You enter wrong! Please enter again : ");
			scanf_s("%d", &n);
		}
		system("cls");
		p = (SP*)malloc(n*sizeof(SP));
		printf("Enter the product information : \n");
		for (i = 0; i < n; i++)
		{
			printf("Product %d : \n", i + 1);
			printf("Name : ");
			fflush(stdin);
			gets_s(check);
			while (strlen(check)>30)
			{
				printf("You enter wrong! Please enter again : ");
				gets_s(check);
			}
			strcpy_s((p + i)->Name, check);
			printf("ID : ");
			gets_s(check);
			while (strlen(check)>10)
			{
				printf("You enter wrong! Please enter again : ");
				gets_s(check);
			}
			strcpy_s((p + i)->ID, check);
			printf("Price : ");
			scanf_s("%d", &(p + i)->Price);
			while ((p + i)->Price < 0)
			{
				printf("You enter wrong! Please enter again : ");
				fflush(stdin);
				scanf_s("%d", &(p + i)->Price);
			}
			printf("Number of product sold : ");
			scanf_s("%d", &(p + i)->numS);
			while ((p + i)->numS < 0)
			{
				printf("You enter wrong! Please enter again : ");
				fflush(stdin);
				scanf_s("%d", &(p + i)->numS);
			}
			printf("Number of inventory product : ");
			scanf_s("%d", &(p + i)->numL);
			while ((p + i)->numL < 0)
			{
				printf("You enter wrong! Please enter again : ");
				fflush(stdin);
				scanf_s("%d", &(p + i)->numL);
			}
		}
		system("cls");
		printf("|%-8s|%-10s|%-10s|%-15s|%-15s|%-18s|\n", "Number", "Name", "ID", "Price", "Number sold", "Inventory product");
		for (i = 0; i < n; i++)
		{
			printf("|%-8d|%-10s|%-10s|%-12d%-3s|%-15d|%-18d|\n", i + 1, (p + i)->Name, (p + i)->ID, (p + i)->Price,"VND", (p + i)->numS, (p + i)->numL);
		}
		printf("Choose the application you want to do :\n");
		printf("1.Find the most expensive product.\n");
		printf("2.Find the cheapest product.\n");
		printf("3.Find the best sold product.\n");
		printf("4.Find the least sold product.\n");
		printf("5.Find the most inventory product.\n");
		printf("6.Find the least inventory product.\n");
		//Loop 2
		do
		{
			//Loop 3
			do
			{	
				st = 0;
				printf("The application : ");
				scanf_s("%d", &st);
				//Switch - case
				switch (st)
				{
					case 1:
					{
						printf("The information of product : \n");
						printf("|%-8s|%-10s|%-10s|%-15s|%-15s|%-18s|\n", "Number", "Name", "ID", "Price", "Number sold", "Inventory product");
						printrs(p, n, maxp(p, n), 1);
						 break;
					}
					case 2:
					{
						printf("The information of product : \n");
						printf("|%-8s|%-10s|%-10s|%-15s|%-15s|%-18s|\n", "Number", "Name", "ID", "Price", "Number sold", "Inventory product");
						printrs(p, n, minp(p, n), 1);
						break;
					}
					case 3:
					{
						printf("The information of product : \n");
						printf("|%-8s|%-10s|%-10s|%-15s|%-15s|%-18s|\n", "Number", "Name", "ID", "Price", "Number sold", "Inventory product");
						printrs(p, n, maxS(p, n), 2);
						break;
					}
					case 4:
					{
						printf("The information of product : \n");
						printf("|%-8s|%-10s|%-10s|%-15s|%-10s|%-18s|\n", "Number", "Name", "ID", "Price", "Number sold", "Inventory product");
						printrs(p, n, minS(p, n), 2);
						break;
					}
					case 5:
					{
						printf("The information of product : \n");
						printf("|%-8s|%-10s|%-10s|%-15s|%-10s|%-18s|\n", "Number", "Name", "ID", "Price", "Number sold", "Inventory product");
						printrs(p, n, maxL(p, n), 3);
						break;
					}
					case 6:
					{
						printf("The information of product : \n");
						printf("|%-8s|%-10s|%-10s|%-15s|%-10s|%-18s|\n", "Number", "Name", "ID", "Price", "Number sold", "Inventory product");
						printrs(p, n, minL(p, n), 3);
						break;
					}
					default:
					{
						printf("You entered wrong! Please enter again: \n");
						st = -1;
					}
				}
			fflush(stdin);
			} while (st == -1);
			fflush(stdin);
			printf("Do you want to do application again?(Y/N) ");
			gets_s(test);
			while ((strcmp(test, "Y") != 0) && (strcmp(test, "N") != 0))
			{
				printf("You enter wrong! Please enter again : ");
				gets_s(test);
			}
		} while (strcmp(test, "Y") == 0);
		free(p);
		fflush(stdin);
		system("cls");
		printf("Do you want to do program again?(Y/N) ");
		gets_s(test);
		while ((strcmp(test, "Y") != 0) && (strcmp(test, "N") != 0))
		{
			printf("You enter wrong! Please enter again : ");
			gets_s(test);
		}
	} while (strcmp(test, "Y") == 0);
}
/*
Note:
Library <string.h>
strcpy(a,b); The command copy string b to string a;
strcmp(a,b); The command compare string a with string b;
strlen(a);	 The command give the length of string a;

Library <windows.h>
Sleep(200);		The command stop screen 2000 milliseconds;
system("cls"); The command clear screen;
system("color x"); x is a integer number 1,2,3,..,15, the command change font color;

Library <malloc.h> -> allocates memory
*/
