#include <stdio.h>

int main()
{
   int Max[10][10], need[10][10], alloc[10][10];
   int avail[10], completed[10], safeSequence[10];

   int p, r, i, j, process, count = 0;

   // Step 1: Take number of processes
   printf("Enter the no of processes: ");
   scanf("%d", &p);

   for (i = 0; i < p; i++)
   {
       completed[i] = 0;
   }

   // Step 2: Take number of resources
   printf("Enter the no of resources: ");
   scanf("%d", &r);

   // Step 3: Take Max matrix
   printf("\nEnter the Max Matrix for each process:\n");

   for (i = 0; i < p; i++)
   {
       printf("For process %d: ", i + 1);

       for (j = 0; j < r; j++)
       {
           scanf("%d", &Max[i][j]);
       }
   }

   // Step 4: Take Allocation matrix
   printf("\nEnter the Allocation Matrix for each process:\n");

   for (i = 0; i < p; i++)
   {
       printf("For process %d: ", i + 1);

       for (j = 0; j < r; j++)
       {
           scanf("%d", &alloc[i][j]);
       }
   }

   // Step 5: Take Available vector
   printf("\nEnter the Available Resources: ");

   for (i = 0; i < r; i++)
   {
       scanf("%d", &avail[i]);
   }

   // Step 6: Calculate Need matrix
   for (i = 0; i < p; i++)
   {
       for (j = 0; j < r; j++)
       {
           need[i][j] = Max[i][j] - alloc[i][j];
       }
   }

   do
   {
       process = -1;

       // Find a process that can execute
       for (i = 0; i < p; i++)
       {
           if (completed[i] == 0)
           {
               process = i;

               for (j = 0; j < r; j++)
               {
                   if (avail[j] < need[i][j])
                   {
                       process = -1;
                       break;
                   }
               }

               if (process != -1)
               {
                   break;
               }
           }
       }

       if (process != -1)
       {
           printf("\nProcess %d runs to completion.", process + 1);

           safeSequence[count] = process + 1;
           count++;

           // Release resources held by process
           for (j = 0; j < r; j++)
           {
               avail[j] += alloc[process][j];
               alloc[process][j] = 0;
           }

           completed[process] = 1;
       }

   } while (count != p && process != -1);

   // Check safe/unsafe state
   if (count == p)
   {
       printf("\n\nThe system is in a SAFE state.\n");
       printf("Safe Sequence: < ");

       for (i = 0; i < p; i++)
       {
           printf("P%d ", safeSequence[i]);
       }

       printf(">\n");
   }
   else
   {
       printf("\n\nThe system is in an UNSAFE state.\n");
   }

   return 0;
}
