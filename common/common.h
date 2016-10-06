#ifndef _COMMON_H
#define _COMMON_H

#ifdef __cplusplus
extern "C" {
#endif


void cmn_delay_us(int us);

int cmn_pin_board2bcm(int board_pin);


#ifdef __cplusplus
}
#endif

#endif
