#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int wait;
    struct timespec res, tstart, tstop;
    time_t sec;
    long nsec;

    if (argc != 2) return 0;

    wait = atoi(argv[1]);

    if (wait == 0) return 0;

    if (clock_getres(CLOCK_MONOTONIC_RAW, &res) != 0) return 0;

    printf("res %d %d\n", res.tv_sec, res.tv_nsec);

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &tstart) != 0) return 0;

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
    printf("start %d %d\n", tstart.tv_sec, tstart.tv_nsec);
    printf("stop  %d %d\n", tstop.tv_sec, tstop.tv_nsec);
    if (tstart.tv_sec == tstop.tv_sec)
    {
        printf("wait %f us\n", (tstop.tv_nsec - tstart.tv_nsec)/1000.0);
    }

    return 0;
}
