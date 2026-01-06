// segment_control.c

#include <stdio.h>
#include <softTone.h>
#include <unistd.h>
#include <wiringPi.h>

#define BUZZER_PIN 25
#define TOTAL 25

#define NOTE_C5  523
#define NOTE_E5   659
#define NOTE_G5   784
#define REST     0

int melody[] = {
    NOTE_G5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
    NOTE_E5, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_E5,
    NOTE_G5, NOTE_E5, NOTE_E5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5,
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_C5,
    NOTE_E5, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_E5,
    NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5,
    NOTE_G5, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_E5,
    NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5,
    NOTE_C5, NOTE_C5, NOTE_E5, NOTE_E5,
    NOTE_G5, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_E5,
    NOTE_C5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_G5,
};


int durations[] = {
    300, 300, 150, 150, 300,
    150, 150, 150, 150, 150, 80, 80, 150, 150,
    150, 150, 300, 150, 80, 80, 150,
    150, 150, 300, 150, 150, 150, 150,
    150, 80, 80, 150, 150, 150, 150, 300,
    150, 80, 80, 450,
    150, 80, 80, 150, 150,
    80, 80, 80, 80, 450,
    150, 150, 150, 150,
    150, 80, 80, 150, 150,
    80, 80, 80, 80, 450,
};

int musicPlay() {
    softToneCreate(BUZZER_PIN);
    int total = sizeof(melody) / sizeof(melody[0]);
    for (int i = 0; i < total; ++i) {
        softToneWrite(BUZZER_PIN, melody[i]);
        delay(durations[i]);
        softToneWrite(BUZZER_PIN, 0);
        delay(50);
    }
    return 0;
}

int gpiopins[4] = { 23, 18, 15, 14 };

int number[11][4] = {
    {0, 0, 0, 0},
    {0, 0, 0, 1},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 1, 0, 0},
    {0, 1, 0, 1},
    {0, 1, 1, 0},
    {0, 1, 1, 1},
    {1, 0, 0, 0},
    {1, 0, 0, 1},
    {1, 1, 1, 1},
};

void fndDisplay(int num)
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(gpiopins[i], number[num][i] ? HIGH : LOW);
    }
}

void segment_control(const char* buffer)
{
    int seconds = 0;
    if (sscanf(buffer, "segment %d", &seconds) != 1 || seconds < 0 || seconds > 9)
    {
        printf("유효한 숫자를 입력해주세요. (0~9) : %s\n", buffer);
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        pinMode(gpiopins[i], OUTPUT);
    }

    for (int i = seconds; i >= 0; i--)
    {
        fndDisplay(i);
        sleep(1);
    }

    fndDisplay(10);
    musicPlay();
}
