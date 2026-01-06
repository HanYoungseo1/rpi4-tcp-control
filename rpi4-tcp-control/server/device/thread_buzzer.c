// thread_buzzer.c

#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>

#define SPKR 25

typedef void (*FUNC)(char *);

void *buzzer_thread(void *arg)
{
    char *buff = (char *)arg;
    FUNC shared;

    void *handle = dlopen("/home/veda/build/libs/lib_buzzer.so", RTLD_LAZY);
    if (handle == NULL)
    {
        perror("dlopen");
        return NULL;
    }

    shared = (FUNC)dlsym(handle, "buzzer_control");
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

void thread_buzzer(char *buffer)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, buzzer_thread, (void *)buffer))
    {
        perror("pthread_buzzer");
        return;
    }
    pthread_detach(tid);
}

