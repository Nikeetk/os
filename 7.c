#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static int copy_data(FILE *source, FILE *destination)
{
    char buffer[4096];
    size_t n;

    while ((n = fread(buffer, 1, sizeof buffer, source)) > 0) {
        if (fwrite(buffer, 1, n, destination) != n)
            return -1;
    }

    if (ferror(source))
        return -1;

    return fflush(destination);
}

int main(int argc, char *argv[])
{
    int request[2], response[2], status;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    if (pipe(request) == -1 || pipe(response) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t child = fork();

    if (child == -1) {
        perror("fork");
        return 1;
    }

    if (child == 0) {
        close(request[1]);
        close(response[0]);

        FILE *input = fdopen(request[0], "r");
        FILE *output = fdopen(response[1], "w");

        if (input == NULL || output == NULL) {
            perror("fdopen");
            return 1;
        }

        char *pathname = NULL;
        size_t capacity = 0;

        if (getdelim(&pathname, &capacity, '\0', input) == -1) {
            fprintf(stderr, "Could not read the pathname\n");
            free(pathname);
            return 1;
        }

        fclose(input);

        FILE *file = fopen(pathname, "rb");
        if (file == NULL) {
            perror(pathname);
            free(pathname);
            return 1;
        }

        free(pathname);

        int result = copy_data(file, output);
        if (result == -1)
            perror("Copying file");

        fclose(file);
        fclose(output);
        return result == -1;
    }

    close(request[0]);
    close(response[1]);

    if (dprintf(request[1], "%s%c", argv[1], '\0') < 0) {
        perror("Sending pathname");
        return 1;
    }

    close(request[1]);

    FILE *input = fdopen(response[0], "r");
    if (input == NULL) {
        perror("fdopen");
        return 1;
    }

    int result = copy_data(input, stdout);
    if (result == -1)
        perror("Displaying file");

    fclose(input);

    if (waitpid(child, &status, 0) == -1) {
        perror("waitpid");
        return 1;
    }

    return result == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0;
}