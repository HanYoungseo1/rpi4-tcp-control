// thread_buzzer.c

#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>

typedef void (*FUNC)(char *);

void *segment_thread(void *arg)
{
    char *buff = (char *)arg;
    FUNC shared;
    
    void *handle = dlopen("/home/veda/build/libs/lib_segment.so", RTLD_LAZY);
    if (handle == NULL)
    {
        perror("dlopen");
        return NULL;
    }

    shared = (FUNC)dlsym(handle, "segment_control");
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

void thread_segment(char* buffer)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, segment_thread, (void*)buffer))
    {
        perror("pthread_segment");
        return;
    }
    pthread_detach(tid);
}

