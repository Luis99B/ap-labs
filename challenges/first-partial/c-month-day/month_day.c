#include <stdio.h>
#include <stdlib.h>

static char *month_name[] = {
    "Illegal month",
    "Jan", "Feb", "Mar",
    "Apr", "May", "Jun",
    "Jul", "Aug", "Sep",
    "Oct", "Nov", "Dec"
};

int months[] = {
    0,
    1, 2, 3,
    4, 5, 6,
    7, 8, 9,
    10, 11, 12
};

int days[] = {
    0,
    31, 28, 31,
    30, 31, 30,
    31, 31, 30,
    31, 30, 31
};

/* month_day function's */
void month_day(int year, int yearday, int *pmonth, int *pday){
    pmonth = &months[0];
    pday = &days[0];
    int leap;
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) { // is a leap year
        pday[2] = 29;
        leap = 1;
    }
    if (leap) {
        if (yearday < 1 || yearday > 366) {
            printf("ERROR: the yearday must be between 1 and 366\n");
            return;
        }
    } else {
        if (yearday < 1 || yearday > 365) {
            printf("ERROR: the yearday must be between 1 and 365\n");
            return;
        }
    }
    int day = yearday;
    int m = 0;
    while(yearday > 0 && (yearday - pday[m]) > 0) {
        yearday -= pday[m];
        day = yearday;
        m++;
    }
    printf("%s %02d, %d\n", month_name[m], day, year);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("ERROR: Not enough arguments\n");
        return -1;
    }
    int year = atoi(argv[1]);
    int yearday = atoi(argv[2]);
    if (year < 1) {
        printf("ERROR: the year must be greater than 0\n");
        return -1;
    }
    int pmonth, pday;
    month_day(year, yearday, &pmonth, &pday);
    return 0;
}
