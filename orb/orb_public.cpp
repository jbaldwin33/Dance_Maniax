#include "orb_public.h"


    orb_public::orb_public() {}
    orb_public::orb_public(int x, int y) { x_pos = x; y_pos = y;}
    int orb_public::get_posX() {return x_pos;}
    int orb_public::get_posY() { return y_pos;}
    void orb_public::set_posX(int x) {x_pos = x;}
    void orb_public::set_posY(int y) {y_pos = y;}
    bool orb_public::get_b() {return b;}
    ORB_STATUS orb_public::get_status() {return status;}




//ORB orb_record[MAX_NUM_ORB];
//int orb_tick=0;
//int orb_interval = 10;
//int score = 0;
//uLCD_4DGL uLCD(p9,p10,p20);


/*void orb_public::orb_setup(void) {
    stdio_mutex.lock();
    uLCD.color(0xFFFFFF);
    uLCD.locate(SIZE_X - 5, 0);
    uLCD.printf("Score:");
    uLCD.color(0xFFFFFF);
    uLCD.locate(SIZE_X - 2, 1);
    uLCD.printf("%d", score);
    stdio_mutex.unlock();
}
void orb_public::orb_generator(void){
    orb_tick++;
    // only fire the missile at certain ticks
    if((orb_tick % orb_interval)==1 || orb_tick==0){
        orb_create();
    }
    
    // update the missiles and draw them
    orb_update_position();
}
void orb_public::orb_create(void){
    int i;
    for(i=0;i<MAX_NUM_ORB;i++){
        if(orb_record[i].status == ORB_DEACTIVE && (rand() % 2 == 0)){
            orb_record[i].y = SIZE_Y;
            //each missile has its own tick
            orb_record[i].tick = 0;
            //the missile starts at its source
            orb_record[i].x = SIZE_X / 2;
            orb_record[i].status = ORB_ACTIVE;
            orb_record[i].b = true;
            break;
        }
    }
}
void orb_public::orb_update_position(void) {
    int i;
    for(i=0;i<MAX_NUM_ORB;i++){
        if(orb_record[i].status == ORB_ACTIVE){
            // update missile position
            stdio_mutex.lock();
            uLCD.circle(SIZE_X / 2, SIZE_Y / 2, 8, WHITE);
            uLCD.filled_circle(orb_record[i].x, orb_record[i].y, 8, RED);
            uLCD.filled_circle(orb_record[i].x, orb_record[i].y, 8, BLACK);
            stdio_mutex.unlock();
            orb_record[i].y = orb_record[i].y - 5;
            //update missile's internal tick
            orb_record[i].tick++;
            
            if (ain > 0.6f && (orb_record[i].y < (SIZE_Y/2) + 5 && orb_record[i].y > (SIZE_Y/2) - 10)) {
                stdio_mutex.lock();
                uLCD.filled_circle(SIZE_X / 2, SIZE_Y / 2, 8, BLUE);
                stdio_mutex.unlock();
                if(orb_record[i].b) {
                    orb_record[i].b = false;
                    score = score + 1;
                    stdio_mutex.lock();
                    uLCD.color(0xFFFFFF);
                    uLCD.text_width(1);
                    uLCD.text_height(1);
                    uLCD.locate(SIZE_X - 2, 1);
                    uLCD.printf("%d", score);
                    stdio_mutex.unlock();
                }
                
            } else if (ain > 0.6f && !(orb_record[i].y < (SIZE_Y/2) + 5 && orb_record[i].y > (SIZE_Y/2) - 10)) {
                orb_record[i].b = true;
                stdio_mutex.lock();
                uLCD.filled_circle(SIZE_X / 2, SIZE_Y / 2, 8, BLACK);
                stdio_mutex.unlock();
            }
            
            if (orb_record[i].y <= -5) {
                orb_record[i].y = SIZE_Y;
                orb_record[i].status = ORB_DEACTIVE;
            }
        }
    }
}*/