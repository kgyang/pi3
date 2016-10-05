#include <stdio.h>
#include <time.h>
#include <bcm2835.h>

int main(int argc, char* argv[])
{
    int wait;
    struct timespec tstart, tstop;

    if (argc != 2) return 0;

    wait = atoi(argv[1]);

    if (wait == 0) return 0;

    bcm2835_init();

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &tstart) != 0) return 0;

    bcm2835_delayMicroseconds(wait);

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &tstop) != 0) return 0;

    printf("start %d %d\n", tstart.tv_sec, tstart.tv_nsec);
    printf("stop  %d %d\n", tstop.tv_sec, tstop.tv_nsec);
    if (tstart.tv_sec == tstop.tv_sec)
    {
        printf("wait %f us\n", (tstop.tv_nsec - tstart.tv_nsec)/1000.0);
    }

    bcm2835_close();

    return 0;
}
