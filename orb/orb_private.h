#ifndef ORB_PRIVATE_H
#define ORB_PRIVATE_H
#include "mbed.h"
#include "uLCD_4DGL.h"
#include "orb_public.h"
#include "rtos.h"

//uLCD_4DGL uLCD(p9,p10,p20);

void orb_setup(void);
void orb_create(void);
void orb_update_position(void);

#endif 