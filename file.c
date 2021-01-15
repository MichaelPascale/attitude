#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "errors.h"
#include "timecvt.h"
#include "file.h"

int create_app_dir(char* path, char* projects_path, size_t size) {

    /*  Get home directory. */
    char* homedir = getenv("HOME");
    if (homedir == NULL)
        return -1;

    /*  Get name of path. */
    if (strlen(homedir) + strlen(DIR_NAME) + strlen(PROJ_DIR_NAME) + 1 > size)
        return -1;

    strcpy(path, homedir);
    strcat(path, DIR_NAME);

    /*  Create tt directory. */
    if (mkdir(path, 0700) < 0 && errno != EEXIST) {
        printf("%s\n", ERR_CREATEFILE);
        return -1;
    }

    /*  Create projects directory. */
    strcpy(projects_path, path);
    strcat(projects_path, PROJ_DIR_NAME);
    if (mkdir(path, 0700) < 0 && errno != EEXIST) {
        printf("%s\n", ERR_CREATEFILE);
        return -1;
    }

    return 0;
}

int open_project(FILE** file, char* path, char* name) {
    char projects[PATH_SIZE];
    if (create_app_dir(path, projects, PATH_SIZE) < 0)
        return -1;

    if (strlen(projects) + strlen(name) + 2 > PATH_SIZE)
        return -1;

    /*  printf("projects  %s\n", projects); */

    strcpy(path, projects);
    strcat(path, "/");
    strcat(path, name);


    /*  printf("open path: %s\n", path); */

    *file = fopen(path, "ab+");

    if (*file == NULL) {
        printf("%s\n", ERR_OPENFILE);
        return -1;
    }

    return 0;
}

long int file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

int last_record(FILE* file, time_t* record, int* last_was_start) {

    /*  Determine the filesize. */
    *last_was_start = file_size(file) / sizeof(time_t) % 2;

    /*  Seek to end of file, read backwards. */
    fseek(file, -sizeof(time_t), SEEK_END);
    fread(record, sizeof(time_t), 1, file);

    return 0;
}

int delete_last (FILE** file, char* path) {

    /*  Determine the filesize. */
    long int size0 = file_size(*file);
    long int length0 = size0 / sizeof(time_t);

    long int length1 = length0 - !(length0 % 2) - 1;
    long int size1 = length1 * sizeof(time_t);

    printf("\nfilesize: %ld\nno. records: %ld\nnew records: %ld\nnew size: %ld\n\n", size0, length0, length1, size1);

    void *buffer = malloc(size1);
    if (buffer == NULL)
        return -1;

    rewind(*file);
    fread(buffer, sizeof(time_t), length1, *file);


    freopen(path, "wb+", *file);
    if (*file == NULL) {
        printf("%s\n", ERR_OPENFILE);
        return -1;
    }

    fwrite(buffer, sizeof(time_t), length1, *file);

    freopen(path, "ab+", *file);
    if (*file == NULL) {
        printf("%s\n", ERR_OPENFILE);
        return -1;
    }

    free(buffer);

    return 0;
}

int delete_project(char* name) {
    return 0;
}

int report_project(FILE* file, time_t epoch) {
    long int len = file_size(file) / sizeof(time_t);

    if (!len % 2) {
        printf("No records.\n");
        return -1;
    }

    long int entries = len / 2;

    rewind(file);

    /*  struct tm* lt; */
    /*  lt = localtime(&epoch); */

    time_t records[2];
    double total = 0, today = 0, week = 0, fortnight = 0;

    printf("complete entries: %ld\nno. records: %ld\n", entries, len);

    int i;
    for (i = 0; i < entries; ++i) {
        fread(records, sizeof(time_t), 2, file);
        double dt = difftime(records[1], records[0]);
        total += dt;

        if (this_day(records[0]))
            today += dt;

        if (this_week(records[0]))
            week += dt;

        if (this_fortnight(records[0]))
            fortnight += dt;
    }

    printf("total: %f, today: %f, week: %f\n", total, today, week);

    return 0;
}

/*  int readdir() */