#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sortArray(int arr[], int n)
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

int binarySearch(int arr[], int n, int key)
{
    int low = 0;
    int high = n - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (arr[mid] == key)
            return mid;
        if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int main()
{
    int n, i, key, result;
    printf("Enter the number of integers: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter %d integers:\n", n);
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    printf("\nParent process started\n");
    printf("Parent Process ID: %d\n", getpid());
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("Fork failed.\n");
        return 1;
    }
    // ---------------- PARENT PROCESS ----------------
    if (pid > 0)
    {
        printf("Child Process ID: %d\n\n", pid);
        printf("Parent process sorting the integers...\n");
        sortArray(arr, n);
        printf("Parent sorted array: ");
        for (i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n\n");
        printf("Parent process waiting for child process...\n");
        wait(NULL);
        printf("Child process completed.\n");
        printf("\nParent process resumed after waiting for child.\n");
        // ---------------- ZOMBIE PROCESS ----------------
        printf("\nDemonstrating Zombie State:\n");
        pid_t zombie_pid = fork();
        if (zombie_pid == 0)
        {
            printf("Child process terminated.\n");
            exit(0);
        }
        else
        {
            printf("Child process created with PID: %d\n", zombie_pid);
            sleep(2);
            printf("Parent has not called wait(), so child is in ZOMBIE state.\n");
            printf("Zombie Process ID: %d\n", zombie_pid);
            printf("Parent process calling wait()...\n");
            waitpid(zombie_pid, NULL, 0);
            printf("Zombie process removed.\n");
        }
        // ---------------- ORPHAN PROCESS ----------------
        printf("\nDemonstrating Orphan State:\n");
        pid_t orphan_pid = fork();
        if (orphan_pid == 0)
        {
            sleep(2);
            printf("Child process is now an ORPHAN.\n");
            printf("Child Process ID: %d\n", getpid());
            printf("New Parent Process ID: %d\n", getppid());
            printf("Orphan process completed.\n");
        }
        else
        {
            printf("Child process created with PID: %d\n", orphan_pid);
            printf("Parent process terminating before child.\n");
            exit(0);
        }
        printf("\nParent process completed.\n");
    }
    // ---------------- CHILD PROCESS ----------------
    else
    {
        sleep(1);
        printf("Child process started\n");
        printf("Child Process ID: %d\n", getpid());
        printf("Parent Process ID: %d\n", getppid());
        sortArray(arr, n);
        printf("\nChild process performing Binary Search\n");
        printf("Enter element to search: ");
        scanf("%d", &key);
        result = binarySearch(arr, n, key);
        if (result != -1)
            printf("Element %d found at position %d\n", key, result + 1);
        else
            printf("Element %d not found in the array.\n", key);
        printf("\nChild process completed.\n");
        exit(0);
    }
    return 0;
}
