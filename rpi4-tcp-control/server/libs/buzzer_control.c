// buzzer_control.c

#include <string.h>
#include <softTone.h>
#include <wiringPi.h>

#define BUZZER_PIN 25

void buzzer_control(char *buf)
{
    if (strcmp(buf, "buzzer on") == 0)
    {
        pinMode(BUZZER_PIN, OUTPUT);
        softToneCreate(BUZZER_PIN);
    }
    else if (strcmp(buf, "buzzer off") == 0)
    {
        pinMode(BUZZER_PIN, INPUT);
    }
}