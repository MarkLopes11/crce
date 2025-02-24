#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int choice;
    char name[50], name1[50];
    char buff[500];
    char buff1[500];

up:

    printf("Enter the choice: \n1.Create\n2.Write and Read\n3.Copy\n4.Exit");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1: // create file

        printf("Enter filename: ");
        scanf("%s", name);

        int fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0666);
        if (fd == -1)
        {
            printf("Error");
            exit(1);
        }
        else
        {
            printf("File created successfully");
            close(fd);
            goto up;
        }

    case 2: // write and read

        printf("Enter the name of the file: ");
        scanf("%s", name);

        fd = open(name, O_RDWR);
        if (fd == -1)
        {
            printf("Error");
        }
        printf("Enter the data to be stored: ");
        scanf("%s", buff);

        write(fd, buff, strlen(buff));
        lseek(fd, 0, SEEK_SET);
        read(fd, buff1, sizeof(buff1));

        printf("contents are %s\n", buff1);
        close(fd);
        goto up;

    case 3:
        printf("Enter the name of the file you want to copy from: ");
        scanf("%s", name1);
        printf("Enter the name of the file you want to copy to: ");
        scanf("%s", name);

        fd = open(name1, O_RDWR);
        if (fd == -1)
        {
            printf("Error");
        }
        read(fd, buff1, sizeof(buff1));
        close(fd);

        fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0666);
        if (fd == -1)
        {
            printf("Error");
        }
        write(fd, buff1, strlen(buff1));
        close(fd);
        break;

    case 4: // exit
        printf("Exiting program\n");
        exit(EXIT_SUCCESS);
        break;
    }
  
    return 0;
}
