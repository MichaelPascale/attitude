/* tt
 * Time Tracker
 *
 * Copyright (c) 2021, Michael Pascale. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "errors.h"
#include "timecvt.h"
#include "file.h"

int start(char* timer);
int stop(char* timer);

int main (int argc, char** argv) {

    if (argc < 2) {
        printf("%s\n", ERR_FIRSTARG);
        return -1;
    }

    if (argc < 3) {
        printf("%s\n", ERR_SECONDARG);
        return -1;
    }

    /*  Get the current time. */
    time_t epoch;
    time(&epoch);

    struct tm* timeinfo;
    timeinfo = localtime(&epoch);
    printf("%s", asctime(timeinfo));

    FILE* project;
    char path[PATH_SIZE];
    open_project(&project, path, argv[2]);

    time_t record;
    int last_was_start;
    last_record(project, &record, &last_was_start);
    /*  printf("last record was a start: %d\n", last_was_start); */


    /*  Parse arguments and pass to subcommands. */
    /*  Start a project timer. */
    if (strcmp(argv[1], "start") == 0) {
        if (last_was_start) {
            printf("Project timer is already running.\n");
        } else {
            fwrite(&epoch, sizeof(epoch), 1, project);
            printf("Timer started.\n");
        }

    /*  Stop a project timer. */
    } else if (strcmp(argv[1], "stop") == 0) {
        if (!last_was_start) {
            printf("Project timer has not been running.\n");
        } else {
            fwrite(&epoch, sizeof(epoch), 1, project);

            double hrs, min, dec_hrs;
            time_t elapsed = (time_t)(long int) difftime(epoch, record);

            sec_to_hours_minutes(elapsed, &hrs, &min, &dec_hrs);
            printf("Timer stopped. %.0f hours and %.2f minutes (%.3fhr) added to %s.\n", hrs, min, dec_hrs, argv[2]);
        }

    /*  Add a time entry manually. */
    } else if (strcmp(argv[1], "add") == 0) {
        printf("Please enter the number of hours and minutes to be added to %s\n", argv[2]);

        time_t sec;
        double hrs, min;
        scanf("%lf %lf", &hrs, &min);

        sec = hrs * HOUR + min * MINUTE;
        sec = epoch - sec;

        fwrite(&sec, sizeof(sec), 1, project);
        fwrite(&epoch, sizeof(epoch), 1, project);

        printf("Added %.3f:%.0f to the project's time.\n", hrs, min);

    /*  Print daily and weekly stats. */
    } else if (strcmp(argv[1], "report") == 0) {

        report_project(project, epoch);

    /*  Delete the last time entry. */
    } else if (strcmp(argv[1], "cancel") == 0) {
        report_project(project, epoch);
        delete_last(&project, path);
        report_project(project, epoch);

    /*  Delete entire project history. */
    } else if (strcmp(argv[1], "erase") == 0) {

    /*  Not a command. */
    } else {
        printf("%s\n", ERR_FIRSTARG);
    }

    /*  printf("close files\n"); */
    fclose(project);
    /*  printf("done\n"); */

    return 0;
}