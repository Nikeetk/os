#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int RQ[100], n, initial, size;
int dataEntered = 0;

void getData()
{
   int i;

   printf("Enter the number of requests: ");
   if (scanf("%d", &n) != 1 || n < 1 || n > 100)
   {
       printf("Invalid number of requests\n");
       dataEntered = 0;
       return;
   }

   printf("Enter total disk size: ");
   if (scanf("%d", &size) != 1 || size <= 0)
   {
       printf("Invalid disk size\n");
       dataEntered = 0;
       return;
   }

   printf("Enter the request sequence:\n");
   for (i = 0; i < n; i++)
   {
       if (scanf("%d", &RQ[i]) != 1 ||
           RQ[i] < 0 || RQ[i] >= size)
       {
           printf("Invalid disk request\n");
           dataEntered = 0;
           return;
       }
   }

   printf("Enter initial head position: ");
   if (scanf("%d", &initial) != 1 ||
       initial < 0 || initial >= size)
   {
       printf("Invalid initial head position\n");
       dataEntered = 0;
       return;
   }

   dataEntered = 1;
}

void sstf()
{
   int visited[100] = {0};
   int current = initial;
   int i, j, index, min, d;
   int TotalHeadMoment = 0;

   printf("\nSSTF Head Movement: %d", current);

   for (i = 0; i < n; i++)
   {
       min = INT_MAX;
       index = -1;

       for (j = 0; j < n; j++)
       {
           if (visited[j] == 0)
           {
               d = abs(RQ[j] - current);

               if (d < min)
               {
                   min = d;
                   index = j;
               }
           }
       }

       TotalHeadMoment += min;
       current = RQ[index];
       visited[index] = 1;

       printf(" -> %d", current);
   }

   printf("\nTotal head movement is %d\n",
          TotalHeadMoment);
}

void sort(int arr[])
{
   int i, j, temp;

   for (i = 0; i < n - 1; i++)
   {
       for (j = 0; j < n - i - 1; j++)
       {
           if (arr[j] > arr[j + 1])
           {
               temp = arr[j];
               arr[j] = arr[j + 1];
               arr[j + 1] = temp;
           }
       }
   }
}

void scan()
{
   int arr[100], i, index = n;
   int current = initial;
   int TotalHeadMoment = 0;

   for (i = 0; i < n; i++)
       arr[i] = RQ[i];

   sort(arr);

   for (i = 0; i < n; i++)
   {
       if (arr[i] >= initial)
       {
           index = i;
           break;
       }
   }

   printf("\nSCAN Head Movement: %d", current);

   // Move towards higher cylinder numbers
   for (i = index; i < n; i++)
   {
       TotalHeadMoment += abs(arr[i] - current);
       current = arr[i];
       printf(" -> %d", current);
   }

   // Move to last cylinder
   TotalHeadMoment += abs((size - 1) - current);
   current = size - 1;
   printf(" -> %d", current);

   // Reverse direction
   for (i = index - 1; i >= 0; i--)
   {
       TotalHeadMoment += abs(arr[i] - current);
       current = arr[i];
       printf(" -> %d", current);
   }

   printf("\nTotal head movement is %d\n",
          TotalHeadMoment);
}

void clook()
{
   int arr[100], i, index = n;
   int current = initial;
   int TotalHeadMoment = 0;

   for (i = 0; i < n; i++)
       arr[i] = RQ[i];

   sort(arr);

   for (i = 0; i < n; i++)
   {
       if (arr[i] >= initial)
       {
           index = i;
           break;
       }
   }

   printf("\nC-LOOK Head Movement: %d", current);

   // Move towards higher requests
   for (i = index; i < n; i++)
   {
       TotalHeadMoment += abs(arr[i] - current);
       current = arr[i];
       printf(" -> %d", current);
   }

   // Jump to lowest request and move upwards
   for (i = 0; i < index; i++)
   {
       TotalHeadMoment += abs(arr[i] - current);
       current = arr[i];
       printf(" -> %d", current);
   }

   printf("\nTotal head movement is %d\n",
          TotalHeadMoment);
}

int main()
{
   int choice;
   while (1)
   {
       printf("\n\nDisk Scheduling Algorithms\n");
       printf("1. Enter Data\n");
       printf("2. SSTF\n");
       printf("3. SCAN\n");
       printf("4. C-LOOK\n");
       printf("5. Exit\n");
       printf("Enter your choice: ");

       if (scanf("%d", &choice) != 1)
           return 0;
       if (choice == 1)
       {
           getData();
       }
       else if (choice >= 2 && choice <= 4)
       {
           if (!dataEntered)
           {
               printf("Please enter data first!\n");
               continue;
           }

           switch (choice)
           {
               case 2:
                   sstf();
                   break;
               case 3:
                   scan();
                   break;
               case 4:
                   clook();
                   break;
           }
       }
       else if (choice == 5)
       {
           printf("Exiting program...\n");
           return 0;
       }
       else
       {
           printf("Invalid choice!\n");
       }
   }
}
