#ifndef FILE_H
#define FILE_H


#define PATH_SIZE 100
#define DIR_NAME "/.tt"
#define PROJ_DIR_NAME "/projects"
#define TASK_DIR_NAME "/tasks"

int create_app_dir(char* path, char* projects_path, size_t size);
int open_project(FILE** file, char* path, char* name);
int last_record(FILE* file, time_t* record, int* last_was_start);
int report_project(FILE* file, time_t epoch);
int delete_last (FILE** file, char* path);

#endif