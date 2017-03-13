#ifndef ORB_PUBLIC_H
#define ORB_PUBLIC_H
#include <mbed.h>
#include "orb_public.h"

///The orb status
typedef enum {
    ORB_ACTIVE=1,///<orb is active
    ORB_DEACTIVE=0///<orb is no longer active
} ORB_STATUS;

typedef struct {
    int x;                   ///< The x-coordinate of missile current position
    int y;                   ///< The y-coordinate of missile current position
    int tick;
    bool b;
    ORB_STATUS status;   ///< The missile status, see MISSILE_STATUS
} ORB;

class orb_public {
public:
    //void orb_setup();
    //void orb_create();
    //void orb_update_position();
    //void orb_generator();
    
    orb_public();
    orb_public(int x, int y);
    int get_posX();
    int get_posY();
    void set_posX(int x);
    void set_posY(int y);
    bool get_b();
    ORB_STATUS get_status();
private:
    int x_pos;
    int y_pos;
    bool b;
    ORB_STATUS status;
};


#define MAX_NUM_ORB  2


#endif 