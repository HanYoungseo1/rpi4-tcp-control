// thread_temperature.c

#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>
#include <stdint.h>

typedef void (*FUNC)(int);

void *temperature_thread(void *arg)
{
    int temperature = (int)(intptr_t)arg;
    FUNC shared;
    
    void *handle = dlopen("/home/veda/build/libs/lib_temperature.so", RTLD_LAZY);
    if (handle == NULL)
    {
        perror("dlopen");
        return NULL;
    }

    shared = (FUNC)dlsym(handle, "temperature_control");
    if (shared == NULL)
    {
        perror("dlsym");
        dlclose(handle);
        return NULL;
    }

    shared(temperature);
    dlclose(handle);
    return NULL;
}


void thread_temperature(char *buffer)
{
    pthread_t tid;
    int temperature = 0;
    sscanf(buffer, "temp %d", &temperature);

    if (pthread_create(&tid, NULL, temperature_thread, (void*)(intptr_t)temperature))
    {
        perror("pthread_temperature");
        return;
    }
    pthread_detach(tid);
}
