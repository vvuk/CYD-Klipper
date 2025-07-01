#include "semaphore.h"

#if defined(_WIN32)
#include <windows.h>
static HANDLE freezeRenderThreadSemaphore = NULL;
static HANDLE freezeRequestThreadSemaphore = NULL;
#elif defined(__unix__) || defined(__APPLE__)
#include <semaphore.h>
#include <pthread.h>
static sem_t freezeRenderThreadSemaphore;
static sem_t freezeRequestThreadSemaphore;
#else
// Assume FreeRTOS or similar embedded
#include <UrlEncode.h>
#ifndef NATIVE_SDL
#include <esp_task_wdt.h>
#endif
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
static SemaphoreHandle_t freezeRenderThreadSemaphore;
static SemaphoreHandle_t freezeRequestThreadSemaphore;
#endif

void semaphore_init() {
#if defined(_WIN32)
    freezeRenderThreadSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
    freezeRequestThreadSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
#elif defined(__unix__) || defined(__APPLE__)
    sem_init(&freezeRenderThreadSemaphore, 0, 1);
    sem_init(&freezeRequestThreadSemaphore, 0, 1);
#else
    freezeRenderThreadSemaphore = xSemaphoreCreateMutex();
    freezeRequestThreadSemaphore = xSemaphoreCreateMutex();
    xSemaphoreGive(freezeRenderThreadSemaphore);
    xSemaphoreGive(freezeRequestThreadSemaphore);
#endif
}

void freeze_request_thread() {
#if defined(_WIN32)
    WaitForSingleObject(freezeRequestThreadSemaphore, INFINITE);
#elif defined(__unix__) || defined(__APPLE__)
    sem_wait(&freezeRequestThreadSemaphore);
#else
    xSemaphoreTake(freezeRequestThreadSemaphore, portMAX_DELAY);
#endif
}

void unfreeze_request_thread() {
#if defined(_WIN32)
    ReleaseSemaphore(freezeRequestThreadSemaphore, 1, NULL);
#elif defined(__unix__) || defined(__APPLE__)
    sem_post(&freezeRequestThreadSemaphore);
#else
    xSemaphoreGive(freezeRequestThreadSemaphore);
#endif
}

void freeze_render_thread() {
#if defined(_WIN32)
    WaitForSingleObject(freezeRenderThreadSemaphore, INFINITE);
#elif defined(__unix__) || defined(__APPLE__)
    sem_wait(&freezeRenderThreadSemaphore);
#else
    xSemaphoreTake(freezeRenderThreadSemaphore, portMAX_DELAY);
#endif
}

void unfreeze_render_thread() {
#if defined(_WIN32)
    ReleaseSemaphore(freezeRenderThreadSemaphore, 1, NULL);
#elif defined(__unix__) || defined(__APPLE__)
    sem_post(&freezeRenderThreadSemaphore);
#else
    xSemaphoreGive(freezeRenderThreadSemaphore);
#endif
}