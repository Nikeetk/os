parent- 
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 20

void merge_sort(int arr[], int left, int right)
{
    if (left >= right)
        return;

    int mid = (left + right) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);

    int temp[MAX];
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= right)
        temp[k++] = arr[j++];

    for (i = 0; i < k; i++)
        arr[left + i] = temp[i];
}

int main(int argc, char *argv[])
{
    int arr[MAX], n;
    char values[MAX][32], parent_pid[32];
    char *args[MAX + 4];
    char *mode = argc > 1 ? argv[1] : "normal";

    printf("Enter number of elements (1-20): ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX)
    {
        printf("Invalid size.\n");
        return 1;
    }

    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++)
    {
        if (scanf("%d", &arr[i]) != 1)
        {
            printf("Invalid integer.\n");
            return 1;
        }

        snprintf(values[i], sizeof values[i], "%d", arr[i]);
        args[i + 3] = values[i];
    }

    snprintf(parent_pid, sizeof parent_pid, "%ld", (long)getpid());
    args[0] = "./childprocess";
    args[1] = mode;
    args[2] = parent_pid;
    args[n + 3] = NULL;

    fflush(stdout);
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        execv(args[0], args);
        perror("execv");
        _exit(1);
    }

    merge_sort(arr, 0, n - 1);

    printf("Parent sorted array (merge sort):");
    for (int i = 0; i < n; i++)
        printf(" %d", arr[i]);
    printf("\n");

    // Orphan state
    if (strcmp(mode, "orphan") == 0)
    {
        printf("Parent is exiting.\n");
        return 0;
    }

    // Zombie state
    if (strcmp(mode, "zombie") == 0)
    {
        printf("Run this in another terminal within 15 seconds:\n");
        printf("ps -o pid,ppid,stat,cmd -p %ld\n", (long)pid);
        fflush(stdout);
        sleep(15);
    }

    if (wait(NULL) == -1)
    {
        perror("wait");
        return 1;
    }

    printf("Parent collected the child's exit status.\n");
    return 0;
}

child - 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 20

void bubble_sort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int arr[MAX];
    int n = argc - 3;

    if (n < 1 || n > MAX)
        return 1;

    for (int i = 0; i < n; i++)
        arr[i] = atoi(argv[i + 3]);

    bubble_sort(arr, n);

    printf("Child sorted array (bubble sort):");
    for (int i = 0; i < n; i++)
        printf(" %d", arr[i]);
    printf("\n");

    // Orphan state: The child stays alive until its parent PID changes.
    if (strcmp(argv[1], "orphan") == 0)
    {
        long original_parent = atol(argv[2]);

        while ((long)getppid() == original_parent)
            sleep(1);

        printf("Original parent PID = %ld\n", original_parent);
        printf("New parent PID = %ld\n", (long)getppid());
    }

    return 0;
}