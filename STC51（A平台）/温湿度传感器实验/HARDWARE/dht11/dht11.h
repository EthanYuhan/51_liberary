
#ifndef  DHT11_H
#define  DHT11_H

#include "sys.h"
#include  "stc15f2k60s2.h"

extern u8 U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;

void  COM(void);
void RH(void);


#endif
