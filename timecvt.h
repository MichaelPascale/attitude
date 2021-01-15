#ifndef TIMECVT_H
#define TIMECVT_H

#define MINUTE 60
#define HOUR 3600
#define DAY 86400
#define WEEK 604800
#define FORTNIGHT 1209600

time_t now(struct tm** timeinfo);

int this_day(time_t sec);
int this_week(time_t sec);
int this_fortnight(time_t sec);

void sec_to_hours_minutes(time_t sec, double* hrs, double* min, double* dec_hrs);

#endif