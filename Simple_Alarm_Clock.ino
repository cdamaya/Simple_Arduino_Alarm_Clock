#include <TimeLib.h>
#include <TimeAlarms.h>

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

    setTime(0, 0, 0, 0 ,0, 2020);     //(Hour, Minute, Second, Day, Month, Year(XX or XXX)
}

void loop() 
{
    AlarmId id;
    STATE state = START;
    id = Alarm.alarmRepeat(0, (void*)alert_state(&state));    //CAUTION: Not sure if this works!
    
    while(1)
    {
        switch(state)
        {
            case(START):
                state = start_state(&state);
                break;
            case(DISPLAY_TIME):
                state = display_state(&state);
                break;
            case(ADJ_TIME):
                state = adj_time_state(&state);
                break;
            case(ADJ_ALARM):
                state = adj_alarm_state(&state);
                break;
            case(ALERT):
                state = alert_state(&state);
                break;
            default:
                state = -1;
        }
    }
}

void display_time_helper(int duration)
{
    int i;
    if(isAM())
    {
        digitalWrite(AM_IND, HIGH);
    }
    else
    {
        digitalWrite(AM_IND, LOW);
    }
    for(i = 0; i < duration; i++)
    {
        display_digit(0, hourFormat12()/10, 0);
        display_digit(1, hourFormat12()%10, 1);
        display_digit(2, minute()/10, 0);
        display_digit(3, minute()%10, 0);
    }
}

STATE start_state(STATE* next_state)
{
    *next_state = START;

    if((millis()/500)%2)
    {
        display_time_helper(1);
    }

    while(!(digitalRead(H_BTN)&&
            digitalRead(M_BTN)&&
            digitalRead(T_BTN)&&
            digitalRead(A_BTN)))
    {
        *next_state = DISPLAY_TIME;
    }
    return *next_state;
}

STATE display_state(STATE* next_state)
{
    *next_state = DISPLAY_TIME;

    display_time_helper(1);
    
    if(!digitalRead(T_BTN))
    {
        *next_state = ADJ_TIME;
    }
    else if(!digitalRead(A_BTN))
    {
        *next_state = ADJ_ALARM;
    }
    return *next_state;
}

STATE adj_time_state(STATE* next_state)
{
    *next_state = ADJ_TIME;

    display_time_helper(10);

    if(!digitalRead(T_BTN))
    {
        if(!digitalRead(H_BTN))
        {
            setTime((hour()+1)%24, minute(), second(), day(), month(), year());
        }
        else if(!digitalRead(M_BTN))
        {
            setTime(hour(), (minute()+1)%60, second(), day(), month(), year());
        }
    }
    else
    {
        *next_state = DISPLAY_TIME;
    }
    
    return *next_state;
}

STATE adj_alarm_state(STATE* next_state)
{
    *next_state = ADJ_ALARM;
    if(!digitalRead(A_BTN))
    {
        //TODO
    }
    else
    {
        *next_state = DISPLAY_TIME;
    }
    return *next_state;
}

STATE alert_state(STATE* next_state)
{
    *next_state = ALERT;

    while(!(digitalRead(H_BTN)&&
            digitalRead(M_BTN)&&
            digitalRead(T_BTN)&&
            digitalRead(A_BTN)))
    {
        *next_state = DISPLAY_TIME;
    }
    return *next_state;
}
