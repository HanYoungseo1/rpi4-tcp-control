// client.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXDATASIZE 100
#define SERVER_PORT 60000

void led_select(int sockfd, char *message);
void buzzer_select(int sockfd, char *message);
void sensor_select(int sockfd, char *message);
void segment_select(int sockfd, char *message);
void temperature_select(int sockfd, char *message);

void mainMenu()
{
    printf("\n");
    printf("-----메인 메뉴-----\n");
    printf("1. LED\n");
    printf("2. BUZZER\n");
    printf("3. SENSOR\n");
    printf("4. SEGMENT\n");
    printf("5. TEMPERATURE\n");
    printf("6. EXIT\n");
    printf(">> ");
}

void ledMenu()
{
    printf("\n");
    printf("-----LED 메뉴-----\n");
    printf("1. ON\n");
    printf("2. OFF\n");
    printf("3. BRIGHT\n");
    printf("4. BACK\n");
    printf(">> ");
}

void brightnessMenu()
{
    printf("\n");
    printf("-----밝기 메뉴-----\n");
    printf("1. 최대 밝기\n");
    printf("2. 중간 밝기\n");
    printf("3. 최소 밝기\n");
    printf("4. BACK\n");
    printf(">> ");
}

void buzzerMenu()
{
    printf("\n");
    printf("-----부저 메뉴-----\n");
    printf("1. ON\n");
    printf("2. OFF\n");
    printf("3. BACK\n");
    printf(">> ");
}

int network()
{
    int sockfd;
    struct hostent *he;
    struct sockaddr_in server_addr;

    char hostname[10] = "veda";

    sigset_t set;

    sigfillset(&set);
    sigdelset(&set, SIGINT);
    sigprocmask(SIG_SETMASK, &set, NULL);

    if ((he = gethostbyname(hostname)) == NULL)
    {
        perror("gethostbyname");
        return -1;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(server_addr.sin_zero), '\0', 8);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int send_to_server(int sockfd, const char *message)
{
    send(sockfd, message, strlen(message), 0);
    return 0;
}

int main(int argc, char *argv[])
{
    int sockfd = network();

    if (sockfd < 0)
        return 1;

    char message[10] = "";
    int choice;

    while (1)
    {
        mainMenu();
        scanf("%d", &choice);
        getchar();
        sprintf(message, "%d", choice);

        if (choice == 1)
        {
            led_select(sockfd, message);
        }
        else if (choice == 2)
        {
            buzzer_select(sockfd, message);
        }
        else if (choice == 3)
        {
            sensor_select(sockfd, message);
        }
        else if (choice == 4)
        {
            segment_select(sockfd, message);
        }
        else if (choice == 5)
        {
            temperature_select(sockfd, message);
        }
        else if (choice == 6)
            break;
        else
        {
            printf("잘못된 선택입니다.\n");
        }
    }
    close(sockfd);
    printf("프로그램 종료\n");
    return 0;
}

void led_select(int sockfd, char *message)
{
    int choice;
    while (1)
    {
        ledMenu();
        scanf("%d", &choice);
        getchar();
        snprintf(message, sizeof(message), "%d", choice);

        if (choice == 1)
            strcpy(message, "led on");
        else if (choice == 2)
            strcpy(message, "led off");
        else if (choice == 3)
        {
            brightnessMenu();
            scanf("%d", &choice);
            getchar();
            snprintf(message, sizeof(message), "%d", choice);

            if (choice == 1)
                strcpy(message, "led bright high");
            else if (choice == 2)
                strcpy(message, "led bright mid");
            else if (choice == 3)
                strcpy(message, "led bright low");
            else if (choice == 4)
                strcpy(message, "back");
            else
            {
                strcpy(message, "wrong");
                printf("잘못된 선택입니다.\n");
            }
        }
        else if (choice == 4)
            break;
        else if (choice > 4)
        {
            strcpy(message, "wrong");
            printf("잘못된 선택입니다.\n");
        }
        if (strstr(message, "led"))
            send_to_server(sockfd, message);
    }
}

void buzzer_select(int sockfd, char *message)
{
    int choice;
    while (1)
    {
        buzzerMenu();
        scanf("%d", &choice);
        getchar();
        snprintf(message, sizeof(message), "%d", choice);

        if (choice == 1)
            strcpy(message, "buzzer on");
        else if (choice == 2)
            strcpy(message, "buzzer off");
        else if (choice == 3)
            break;
        else if (choice > 3)
        {
            strcpy(message, "wrong");
            printf("잘못된 선택입니다.\n");
        }
        if (strstr(message, "buzzer"))
            send_to_server(sockfd, message);
    }
}

void sensor_select(int sockfd, char *message)
{
    while (1)
    {
        int threshold = 0;
        printf("조도 센서 값 (0~1024) (ADC 값) >> ");
        scanf("%d", &threshold);
        getchar();

        if (threshold < 0 && threshold > 1024)
            continue;
        sprintf(message, "sensor %d", threshold);

        send_to_server(sockfd, message);
        break;
    }
}

void segment_select(int sockfd, char *message)
{
    while (1)
    {
        int segmentVal = 0;
        printf("세그먼트 값 (0~9) >> ");
        scanf("%d", &segmentVal);
        getchar();

        if (segmentVal < 0 && segmentVal > 9)
            continue;
        sprintf(message, "segment %d", segmentVal);

        send_to_server(sockfd, message);
        break;
    }
}

void temperature_select(int sockfd, char *message)
{
    while (1)
    {
        int temperature = 0;
        printf("온도 센서 값 (0~255) (ADC 값) >> ");
        scanf("%d", &temperature);
        getchar();

        if (temperature < 0 && temperature > 255)
            continue;
        sprintf(message, "temp %d", temperature);

        send_to_server(sockfd, message);
        break;
    }
}