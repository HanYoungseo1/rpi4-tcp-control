// thread_led.c

#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>

#define BUF_SIZE 1024

typedef void (*FUNC)(char *);

void *led_thread(void *arg)
{
    char *buff = (char *)arg;
    FUNC shared;

    void *handle = dlopen("/home/veda/build/libs/lib_led.so", RTLD_LAZY);
    if (handle == NULL)
    {
        perror("dlopen");
        return NULL;
    }

    shared = (FUNC)dlsym(handle, "led_control");
    if (shared == NULL)
    {
        perror("dlsym");
        dlclose(handle);
        return NULL;
    }
    
    shared(buff);
    dlclose(handle);
    return NULL;
}

void thread_led(char *buffer)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, led_thread, (void *)buffer))
    {
        perror("pthread_led");
        return;
    }
    pthread_detach(tid);
}
