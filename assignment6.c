/ reader
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

    int main() {
  char ch;
  int fd, chars = 0, words = 0, lines = 0, inword = 0;
  int last = '\n';

  mkfifo("fifo1", 0666);
  mkfifo("fifo2", 0666);

  printf("Receiver waiting...\n");

  fd = open("fifo1", O_RDONLY);

  while (read(fd, &ch, 1) > 0 && ch != '@') {
    chars++;
    if (ch == '\n')
      lines++;

    if (isspace((unsigned char)ch))
      inword = 0;
    else if (!inword) {
      words++;
      inword = 1;
    }

    last = ch;
  }
  close(fd);

  if (chars > 0 && last != '\n')
    lines++;

  FILE *fp = fopen("result.txt", "w");
  if (fp == NULL)
    return 1;

  fprintf(fp, "Characters: %d\nWords: %d\nLines: %d\n", chars, words, lines);
  fclose(fp);

  fp = fopen("result.txt", "r");
  if (fp == NULL)
    return 1;

  fd = open("fifo2", O_WRONLY);

  while ((ch = fgetc(fp)) != EOF)
    write(fd, &ch, 1);

  close(fd);
  fclose(fp);

  printf("Result sent successfully.\n");
  return 0;
}
/ writer

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

    int main() {
  char text[5000], ch = '@';
  int fd;

  mkfifo("fifo1", 0666);
  mkfifo("fifo2", 0666);

  printf("Enter sentences (@ alone to finish):\n");

  fd = open("fifo1", O_WRONLY);

  while (fgets(text, sizeof(text), stdin)) {
    if (strcmp(text, "@\n") == 0 || strcmp(text, "@") == 0)
      break;

    write(fd, text, strlen(text));
  }

  write(fd, &ch, 1);
  close(fd);

  printf("\nReceived result:\n");

  fd = open("fifo2", O_RDONLY);

  while (read(fd, &ch, 1) > 0)
    putchar(ch);

  close(fd);
  return 0;
}
