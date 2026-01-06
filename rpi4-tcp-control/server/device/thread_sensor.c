// thread_sensor.c

#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>
#include <stdint.h>

typedef void (*FUNC)(int);

void *sensor_thread(void *arg)
{
    int threshold = (int)(intptr_t)arg;
    FUNC shared;
    
    void *handle = dlopen("/home/veda/build/libs/lib_sensor.so", RTLD_LAZY);
    if (handle == NULL)
    {
        perror("dlopen");
        return NULL;
    }

    shared = (FUNC)dlsym(handle, "sensor_control");
    if (shared == NULL)
    {
        perror("dlsym");
        dlclose(handle);
        return NULL;
    }

    shared(threshold);
    dlclose(handle);
    return NULL;
}


void thread_sensor(char *buffer)
{
    pthread_t tid;
    int threshold = 0;
    sscanf(buffer, "sensor %d", &threshold);

    if (pthread_create(&tid, NULL, sensor_thread, (void*)(intptr_t)threshold))
    {
        perror("pthread_sensor");
        return;
    }
    pthread_detach(tid);
}
