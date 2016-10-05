#include <string.h>
#include "common.h"

int board_to_bcm_pin(int board_pin)
{
    int pinmap[40+1];

    memset(pinmap, -1, sizeof(pinmap));

    pinmap[3] = 2;
    pinmap[5] = 3;
    pinmap[7] = 4;
    pinmap[8] = 14;
    pinmap[10] = 15;
    pinmap[11] = 17;
    pinmap[12] = 18;
    pinmap[13] = 27;
    pinmap[15] = 22;
    pinmap[16] = 23;
    pinmap[18] = 24;
    pinmap[19] = 10;
    pinmap[21] = 9;
    pinmap[22] = 25;
    pinmap[23] = 11;
    pinmap[24] = 8;
    pinmap[26] = 7;
    pinmap[29] = 5;
    pinmap[31] = 6;
    pinmap[32] = 12;
    pinmap[33] = 13;
    pinmap[35] = 19;
    pinmap[36] = 16;
    pinmap[37] = 26;
    pinmap[38] = 20;
    pinmap[40] = 21;

    if (board_pin < 1 || board_pin > 40)
    {
        return -1;
    }

    return pinmap[board_pin];
}

