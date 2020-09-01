#include "4_digit_7_seg_display.h"

#define CS A0
#define CLK A1
#define SER A2

uint8_t digits[4] = {2, 3, 4, 5};

uint8_t segments[11] = {B11111100,
                        B01100000,
                        B11011010,
                        B11110010,
                        B01100110,
                        B10110110,
                        B10111110,
                        B11100000,
                        B11111110,
                        B11100110,
                        B00000001};
                        
void init_7_seg()
{
    int i;
    for(i = 0; i < 4; i++)
    {
        pinMode(digits[i], OUTPUT);
        digitalWrite(digits[i], HIGH);
    }
    pinMode(CS, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(SER, OUTPUT);
    
}

void display_digit(uint8_t pos, uint8_t value)
{
    pos %= 4;
    value %= 11;
    
    digitalWrite(CS, LOW);
    shiftOut(SER, CLK, LSBFIRST, segments[value]);
    digitalWrite(CS, HIGH);
    
    digitalWrite(digits[pos], LOW);
    delay(1);
    digitalWrite(digits[pos], HIGH);
}
