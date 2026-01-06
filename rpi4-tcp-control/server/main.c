// main.c

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wiringPi.h>

#define BUF_SIZE 1024
#define BACKLOG 10

int network();
void daemonP();

int main()
{
	daemonP();

	if (wiringPiSetupGpio() == -1)
		return 1;

	int cmd = 0, new_fd, sin_size;
	struct sockaddr_in client_addr;
	char buffer[BUF_SIZE];
	int sock_fd = network();
	sin_size = sizeof(struct sockaddr_in);

	if ((new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &sin_size)) == -1)
	{
		perror("accept");
		exit(1);
	}

	while (1)
	{
		int read_len = recv(new_fd, buffer, BUF_SIZE - 1, 0);
		if (read_len <= 0)
			continue;
		buffer[read_len] = '\0';

		printf("받은 메시지: %s\n", buffer);

		if (strstr(buffer, "led"))
		{
			thread_led(buffer);
		}
		else if (strstr(buffer, "buzzer"))
		{
			thread_buzzer(buffer);
		}
		else if (strstr(buffer, "sensor"))
		{
			thread_sensor(buffer);
		}
		else if (strstr(buffer, "segment"))
		{
			thread_segment(buffer);
		}
		else if (strstr(buffer, "temp"))
		{
			thread_temperature(buffer);
		}
		else if (strcmp(buffer, "exit") == 0)
			break;
	}
	close(new_fd);
	close(sock_fd);
	return 0;
}

int network()
{
	int sockfd;
	struct sockaddr_in server_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(60000);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(server_addr.sin_zero), '\0', 8);
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}
	return sockfd;
}

void daemonP()
{
    struct sigaction sa;
    struct rlimit rl;
    pid_t pid;
    int fd0, fd1, fd2;

    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    {
        perror("getrlimit");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid > 0)
    {
        exit(0);
    }

    if (setsid() < 0)
    {
        perror("setsid");
        exit(1);
    }

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
    {
        perror("sigaction");
        exit(1);
    }

    if (chdir("/") < 0)
    {
        perror("chdir");
        exit(1);
    }

    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (int i = 0; i < rl.rlim_max; i++)
        close(i);

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
}