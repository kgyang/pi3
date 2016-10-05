#include <time.h>
#include "common.h"

// test shows bcm delay is worse than clock_gettime, so implement following for DHT
// test shows the clock_gettime works in most cases (not every time),
// and could reach 1us resolution especially in root mode

void wait_us(int wait)
{
    struct timespec tstart, tstop;
    time_t sec;
    long nsec;

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &tstart) != 0) return;

    sec = tstart.tv_sec;
    nsec = tstart.tv_nsec + wait*1000;

    if (nsec >= 1000000000)
    {
        sec++;
        nsec -= 1000000000;
    }
    while (clock_gettime(CLOCK_MONOTONIC_RAW, &tstop) == 0)
    {
        if (tstop.tv_sec > sec) break;
        else if (tstop.tv_sec == sec)
        {
            if (tstop.tv_nsec >= nsec) break;
        }
    }
}

