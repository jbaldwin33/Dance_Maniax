#include "mbed.h"
#include "rtos.h"
#include "uLCD_4DGL.h"
#include "SDFileSystem.h"
#include "wave_player.h"
#include "DebounceIn.h"
#include <string>

SDFileSystem sd(p5, p6, p7, p8, "sd");
uLCD_4DGL uLCD(p9,p10,p20); // serial tx, serial rx, reset pin;
SPI spi(p11, p12, p13);
DigitalOut latch(p15);
DigitalOut enable(p16);
AnalogIn ain(p17);          //ir sensor pins
AnalogOut DACout(p18);
DebounceIn pb1(p21);
DebounceIn pb2(p22);
Mutex stdio_mutex;
wave_player waver(&DACout);

bool fileClosed;
void makeSound(void const *args) {
    fileClosed = false;
    FILE *wave_file;
    wave_file=fopen("/sd/mySongs/I_Miss_You2.wav","r");
    waver.play(wave_file);
    fclose(wave_file);
    fileClosed = true;
}

int highScore = 0;
int highCombo;
int main() {
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    //increase baud rate for faster printing
    uLCD.baudrate(3000000);
    
Start:
    //main menu
    uLCD.cls();
    while(1) {
        uLCD.color(0xFF00FF);
        uLCD.text_width(2);
        uLCD.text_height(3);
        uLCD.locate(1, 1);
        uLCD.printf("uDance");
        uLCD.locate(1, 2);
        uLCD.printf("Maniax");
        uLCD.text_width(1);
        uLCD.text_height(1);
        uLCD.locate(7, 11);
        uLCD.printf("START");
        wait(0.1);
        uLCD.color(0xFFFFFF);
        uLCD.locate(7, 11);
        uLCD.printf("START");
        wait(0.1);
        if (!pb1) {
            goto Game;
        }
    }

Game:
    highCombo = 0;
    uLCD.cls();
    Thread thread2(makeSound);
    int orbX1 = SIZE_X / 2;
    int orbY1 = SIZE_Y;
    int score = 0;
    int combo = 0;
    //use bools to save previous state for "if" statements
    bool preventExcessPrinting = true;
    bool preventMultipleScoreIncrement = true;
    //use to see if hand has passed over sensor at all
    bool sensed = false;
    //setup display
    stdio_mutex.lock();
    uLCD.color(WHITE);
    uLCD.locate(SIZE_X - 5, 0);
    uLCD.printf("Score:");
    uLCD.color(WHITE);
    uLCD.locate(SIZE_X - 3, 1);
    uLCD.printf("%d", score);
    uLCD.circle(SIZE_X / 2, SIZE_Y / 2, 8, WHITE);
    uLCD.color(WHITE);
    uLCD.text_width(1);
    uLCD.text_height(1);
    uLCD.locate(0, 0);
    uLCD.printf("Top");
    uLCD.locate(0, 1);
    uLCD.printf("Score:");
    uLCD.locate(0, 2);
    uLCD.printf("%d", highScore);
    stdio_mutex.unlock();
    //wait for song to start
    Thread::wait((10000)*1.48);
    while(1) {
        if(fileClosed) {
            if (score > highScore) {
                highScore = score;
            }
            goto End;
        }
        //draw orb and keep white circle in center
        stdio_mutex.lock();
        uLCD.circle(SIZE_X / 2, SIZE_Y / 2, 8, WHITE);
        uLCD.filled_circle(orbX1, orbY1, 8, RED);
        uLCD.filled_circle(orbX1, orbY1, 8, BLACK);
        stdio_mutex.unlock();
        //update orb's position
        orbY1 = orbY1 - 5;
        //check sensor while orb is within bounds
        if ((ain > 0.6f) && (orbY1 < (SIZE_Y/2) + 5 && orbY1 > (SIZE_Y/2) - 10)) {
            sensed = true;
            stdio_mutex.lock();
            uLCD.filled_circle(SIZE_X / 2, SIZE_Y / 2, 8, BLUE);
            //compensate for lag from printing
            orbY1 = orbY1 - 2;
            //compensate for lag from printing
            if (combo >= 5) {
                orbY1 = orbY1 - 5;
            }
            stdio_mutex.unlock();
            if (preventMultipleScoreIncrement) {
                preventMultipleScoreIncrement = false;
                score++;
                combo++;
                //double score if combo is 5
                if (combo >= 5) {
                    score++;
                }
                preventExcessPrinting = true;
                //print the current combo and multiplier
                if (preventExcessPrinting && combo >=5 ) {
                    preventExcessPrinting = false;
                    if (combo == 5) {
                        stdio_mutex.lock();
                        uLCD.color(0xFFFF00);
                        uLCD.text_width(2);
                        uLCD.text_height(2);
                        uLCD.locate(1, 6);
                        uLCD.printf("x2!");
                        stdio_mutex.unlock();
                    }
                    stdio_mutex.lock();
                    uLCD.color(WHITE);
                    uLCD.text_width(1);
                    uLCD.text_height(1);
                    uLCD.locate(0, 14);
                    uLCD.printf("COMBO");
                    uLCD.color(WHITE);
                    uLCD.text_width(1);
                    uLCD.text_height(1);
                    uLCD.locate(0, 15);
                    uLCD.printf("%d", combo);
                    stdio_mutex.unlock();
                }
                //update printed score
                stdio_mutex.lock();
                uLCD.color(WHITE);
                uLCD.text_width(1);
                uLCD.text_height(1);
                uLCD.locate(SIZE_X - 3, 1);
                uLCD.printf("%d", score);
                stdio_mutex.unlock();
            }
        } else {
            //after hand is sensed and blue circle is printed, draw over it with black
            preventMultipleScoreIncrement = true;
            stdio_mutex.lock();
            uLCD.filled_circle(SIZE_X / 2, SIZE_Y / 2, 8, BLACK);
            stdio_mutex.unlock();
        }
        //C-C-C-COMBOBREAKER if player keeps hand in front of sensor when orb isn't within bounds
        //or if player doesn't have their hand in front of the sensor when the orb is within bounds
        if ((combo != 0 && (ain > 0.6f) && (orbY1 > (SIZE_Y/2) + 10 || orbY1 < (SIZE_Y/2) - 15))
            || (combo != 0 && (orbY1 < (SIZE_Y/2) - 8) && (!sensed))) {
            if (combo > highCombo) {
                highCombo = combo;
            }
            combo = 0;
            stdio_mutex.lock();
            uLCD.color(BLACK);
            uLCD.text_width(1);
            uLCD.text_height(1);
            uLCD.locate(0, 14);
            uLCD.printf("COMBO");
            uLCD.color(BLACK);
            uLCD.text_width(1);
            uLCD.text_height(1);
            uLCD.locate(0, 15);
            uLCD.printf("%s", "000");
            uLCD.color(BLACK);
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.locate(1, 6);
            uLCD.printf("x2!");
            stdio_mutex.unlock();
        }
        //when orb reaches top of screen start back at the bottom and check for sensor again
        if (orbY1 <= -5) {
            orbY1 = SIZE_Y;
            sensed = false;
        }
    }
End:
    //select if player wants to play again or not
    uLCD.cls();
    uLCD.color(0xFF00FF);
    uLCD.text_width(2);
    uLCD.text_height(3);
    uLCD.locate(0, 1);
    uLCD.printf("Score:%d", score);
    wait(3.0);
    uLCD.cls();
    uLCD.text_width(1);
    uLCD.text_height(2);
    uLCD.locate(1, 2);
    uLCD.printf("Highest combo:%d", highCombo);
    wait(3.0);
    uLCD.cls();
    uLCD.color(0xFF00FF);
    uLCD.text_width(1);
    uLCD.text_height(3);
    uLCD.locate(3, 1);
    uLCD.printf("Play Again?");
    uLCD.text_width(1);
    uLCD.text_height(1);
    uLCD.locate(7, 9);
    uLCD.printf("Yes");
    uLCD.locate(7, 11);
    uLCD.printf("No");
    int x = 7;  //use to highlight "Yes" or "No"
    int y = 9;
    string s = "Yes";
    int counter = 0;
    int old_pb2 = 1;    //use this so the option only changes once when a button is pressed and held
    int new_pb2;
    while(1) {
        uLCD.color(WHITE);
        uLCD.text_width(1);
        uLCD.text_height(1);
        uLCD.locate(x, y);
        uLCD.printf("%s", s.c_str());
        wait(0.1);
        uLCD.color(0xFF00FF);
        uLCD.text_width(1);
        uLCD.text_height(1);
        uLCD.locate(x, y);
        uLCD.printf("%s", s.c_str());
        wait(0.1);
        new_pb2 = pb2;
        if (new_pb2 == 0 && old_pb2 == 1) {
            counter++;
            if (counter % 2 == 1) {
                x = 7;
                y = 11;
                s = "No";
            } else if (counter % 2 == 0) {
                x = 7;
                y = 9;
                s = "Yes";
            }
        }
        old_pb2 = new_pb2;
        if (!pb1 && y == 9) {
            goto Game;
        } else if (!pb1 && y == 11) {
            goto Start;
        }
    }
}
