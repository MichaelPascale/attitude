#include <string.h>
#include <math.h>
#include <time.h>

#include "timecvt.h"

time_t now(struct tm** timeinfo) {
    /*  Get the current time. */
    time_t epoch;
    time(&epoch);

    *timeinfo = localtime(&epoch);
    return epoch;
}

int same_day (time_t t0, time_t t1) {

}

int same_week (time_t t0, time_t t1) {

}

int same_fortnight (time_t t0, time_t t1) {

}

int this_day(time_t sec) {
    struct tm *t0, *t1;
    time_t epoch1 = now(&t1);

    time_t epoch0 = (time_t)(long int) sec;
    t0 = localtime(&epoch0);

    return difftime(epoch1, epoch0) < DAY && t0->tm_mday == t1->tm_mday;
}

int this_week(time_t sec) {
    struct tm *t0, *t1;
    time_t epoch1 = now(&t1);

    time_t epoch0 = (time_t)(long int) sec;
    t0 = localtime(&epoch0);

    return difftime(epoch1, epoch0) < WEEK && t0->tm_wday <= t1->tm_wday;
}

int this_fortnight(time_t sec) {
    struct tm *t0, *t1;
    time_t epoch1 = now(&t1);

    time_t epoch0 = (time_t)(long int) sec;
    t0 = localtime(&epoch0);

    return difftime(epoch1, epoch0) < FORTNIGHT && t0->tm_wday <= t1->tm_wday; /*  TODO: logic for fortnight. */
}

time_t beginning_of_week(time_t sec) {
    struct tm *t = localtime(&sec);
}

void sec_to_hours_minutes(time_t sec, double* hrs, double* min, double* dec_hrs) {
    *min = ((double) sec) / 60;
    *hrs = floor(*min / 60);
    *min -= *hrs * 60;
    *dec_hrs = ((double) sec) / HOUR;

    return;
}
