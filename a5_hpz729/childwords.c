#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_BUFF_SIZE 4096
#define MAX_FILENAME_LEN 500
#define MAX_WORD_LEN 150

int main(int argc, char *argv[])
{
    FILE* fp;
    int file_count = (argc - 1);
    int i = 1;
    int iExitStatus;
    int word_count = 0;
    int numSuccess = 0;
    char * delimiter = " ";
    long lForkPid;
    char line[MAX_BUFF_SIZE];

    while(i <= file_count)
    {
        lForkPid = fork();//create child process for each file in argv
        switch (lForkPid)//Use return status of the fork to determine fork success.
        {                //If successful read and count file, if not exit.
        case -1:
            fprintf(stderr, "Fork failed with error code %d: %s\n", errno, strerror(errno));
            return 1;
            break;
        case 0:
            fp = fopen(argv[i], "r");
            if(fp == NULL)
            {
                fprintf(stderr, "ERROR: could not open file %s: %s\n", argv[i], strerror(errno));
                return 1;
            }
            while(fgets(line, MAX_BUFF_SIZE, fp))
            {
                if(strtok(line, delimiter))
                {
                    word_count++;

                    while(strtok(NULL, delimiter))
                    {
                        word_count++;
                    }
                }
                if(strlen(line) == MAX_BUFF_SIZE && line[strlen(line) - 1] != '\n')
                {
                    word_count--;
                    printf("INFO: deducting one word due to no line break\n");
                }
            }
            printf("Words in file %s are %d\n", argv[i], word_count);
            fclose(fp);
            return 0;
         
        default:
            i++;
            break;
        }
    }
    while(wait(&iExitStatus) != -1)
    {
        if(WIFEXITED(iExitStatus) && WEXITSTATUS(iExitStatus) == 0)
        {
            numSuccess++;
        }        
    }
    printf("%d of %d files read successfully\n", numSuccess, (argc - 1));
    return 0;
}//MAIN }