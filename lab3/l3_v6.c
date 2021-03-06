#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>

char *execfile, *fullpath;
int max_num_proc, cur_num_proc;

int open_dir(char *);

char *getfullname(char *, char *);

int getprocess(char *);

int getcountofwords(char *);

int main(int argc, char *argv[]) {
    int ret;
    max_num_proc++;
    char *dirname = argv[1];

    execfile = basename(argv[0]);

    max_num_proc = atoi(argv[2]);
    cur_num_proc = 1;

    open_dir(dirname);

    while (cur_num_proc != 0) {
        ret = wait(NULL);
        cur_num_proc--;
    }

    return 0;
}

char *getfullname(char *rootdirname, char *dirname) {
    char *filepath = (char *) malloc(sizeof(char) * PATH_MAX);

    strcpy(filepath, rootdirname);
    strcat(filepath, "/");
    strcat(filepath, dirname);
    return filepath;
}

int getcountofwords(char *filepath) {
    FILE *file;
    char c;
    long int readingbytes;
    int countofwords = 0;

    file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "%d : %s : %s :%s\n", getpid(), execfile, strerror(errno), filepath);
        return 1;
    }
    do {
        c = fgetc(file);
        readingbytes++;
        if (c != '\n' && c != ' ' && c != EOF && c != '\t') {
            c = fgetc(file);
            readingbytes++;
            countofwords++;
            while (c != '\n' && c != ' ' && c != EOF && c != '\t') {
                c = fgetc(file);
                readingbytes++;
            }
        }
    } while (c != EOF);
    if (fclose(file) == EOF)
        fprintf(stderr, "%d : %s : %s : %s\n", getpid(), execfile, strerror(errno), filepath);

    printf("%d : %s : %ld %d\n", getpid(), filepath, readingbytes, countofwords);
    return 0;
}

int getprocess(char *filepath) {
    int result;
    pid_t pid;
    if (cur_num_proc >= max_num_proc) {
        if ((result = wait(NULL)) > 0)
            cur_num_proc--;
        if (result == -1) {
            fprintf(stderr, "%d %s: %s\n", getpid(), execfile, strerror(errno));
            exit(0);
        }
    }

    if (cur_num_proc < max_num_proc) {
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "%d : %s : %s\n", getpid(), execfile, strerror(errno));
            return 1;
        }
        if (pid == 0) {
            getcountofwords(filepath);
            exit(0);
        }
        if (pid > 0)
            cur_num_proc++;
    }

    return 0;
}

int open_dir(char *dirname) {
    DIR *dfd;
    struct dirent *dir;
    char *filepath;

    if ((dfd = opendir(dirname)) == NULL) {
        fprintf(stderr, "%d : %s : %s : %s\n", getpid(), execfile, dirname, strerror(errno));
        return 1;
    }

    while ((dir = readdir(dfd)) != NULL) {
        filepath = getfullname(dirname, dir->d_name);
        if (((dir->d_type == DT_DIR))
            && ((strcmp(dir->d_name, ".")) != 0)
            && ((strcmp(dir->d_name, "..")) != 0))
            open_dir(filepath);
        else if (dir->d_type == DT_REG)
            getprocess(filepath);


        free(filepath);
    }
    if (errno == EBADF) {
        fprintf(stderr, "%d %s : %s\n", getpid(), execfile, strerror(errno));
        return 1;
    }
    if (closedir(dfd) != 0) {
        fprintf(stderr, "%d %s : %s\n", getpid(), execfile, strerror(errno));
        return 1;
    }
    return 0;
}
