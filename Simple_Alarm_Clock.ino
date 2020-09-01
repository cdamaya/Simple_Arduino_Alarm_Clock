#include "State.h"
#include "4_digit_7_seg_display.h"

#define AM_IND 6
#define ALARM_IND 7

#define H_BTN 8
#define M_BTN 9
#define T_BTN 10
#define A_BTN 11
#define A_SWITCH 12

void setup() 
{
    init_7_seg();
    
    pinMode(AM_IND, OUTPUT);
    pinMode(ALARM_IND, OUTPUT);
  
    pinMode(H_BTN, INPUT_PULLUP);
    pinMode(M_BTN, INPUT_PULLUP);
    pinMode(T_BTN, INPUT_PULLUP);
    pinMode(A_BTN, INPUT_PULLUP);
    pinMode(A_SWITCH, INPUT_PULLUP);
}

void loop() 
{
    STATE state = START;
    
    while(1)
    {
        switch(state)
        {
            case(START):
                state = start_state();
                break;
            case(DISPLAY_TIME):
                state = display_state();
                break;
            case(ADJ_TIME):
                state = adj_time_state();
                break;
            case(ADJ_ALARM):
                state = adj_alarm_state();
                break;
            case(ALERT):
                state = alert_state();
                break;
            default:
                state = -1;
        }
    }
}

STATE start_state()
{
    STATE next_state = START;

    if((millis()/500)%2)
    {
        display_digit(0, 1);
        display_digit(1, 2);
        display_digit(1, 10);
        display_digit(2, 0);
        display_digit(3, 0);
    }

    while(!(digitalRead(H_BTN)&&
            digitalRead(M_BTN)&&
            digitalRead(T_BTN)&&
            digitalRead(A_BTN)))
    {
        next_state = DISPLAY_TIME;
    }
    return next_state;
}

STATE display_state()
{
    STATE next_state = DISPLAY_TIME;

    if(!digitalRead(T_BTN))
    {
        next_state = ADJ_TIME;
    }
    else if(!digitalRead(A_BTN))
    {
        next_state = ADJ_ALARM;
    }
    return next_state;
}

STATE adj_time_state()
{
    STATE next_state = ADJ_TIME;

    if(!digitalRead(T_BTN))
    {
        //TODO
    }
    else
    {
        next_state = DISPLAY_TIME;
    }
    return next_state;
}

STATE adj_alarm_state()
{
    STATE next_state = ADJ_ALARM;
    if(!digitalRead(A_BTN))
    {
        //TODO
    }
    else
    {
        next_state = DISPLAY_TIME;
    }
    return next_state;
}

STATE alert_state()
{
    STATE next_state = ALERT;

    while(!(digitalRead(H_BTN)&&
            digitalRead(M_BTN)&&
            digitalRead(T_BTN)&&
            digitalRead(A_BTN)))
    {
        next_state = DISPLAY_TIME;
    }
    return next_state;
}
