/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#include <zconf.h>
#include <pthread.h>
#include "schedule.h"
#include <iostream>
#include <cstring>

struct Event_list sched_list;

#define MAX_EVENT_NUM 4

pthread_mutex_t event_fetch_lock;
pthread_mutex_t event_exec_lock;
pthread_mutex_t event_insert_lock;
pthread_cond_t not_full;
pthread_cond_t not_empty;
pthread_mutex_t event_create_lock;

int event_num;

void init_event_list() {
    pthread_mutex_init(&event_fetch_lock, NULL);
    pthread_mutex_init(&event_exec_lock, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_mutex_init(&event_insert_lock, NULL);
    pthread_mutex_init(&event_create_lock, NULL);
    event_num = 0;
    TAILQ_INIT(&sched_list);
}

Event* fetch_new_event() {
    Event* event_buf;
    pthread_mutex_lock(&event_fetch_lock);
    if (event_num <= 0) {
        pthread_cond_wait(&not_empty, &event_fetch_lock);
    }
    event_buf = TAILQ_FIRST(&sched_list);
    TAILQ_FIRST(&sched_list) = TAILQ_NEXT(event_buf, event_link);
    TAILQ_REMOVE(&sched_list, event_buf, event_link);
    event_num--;
    pthread_cond_signal(&not_full);
    printf("Event %d Fetched.\n", event_buf->id);
    pthread_mutex_unlock(&event_fetch_lock);
    return event_buf;
}

void insert_event(Event* event) {
    pthread_mutex_lock(&event_insert_lock);
    if (event_num >= MAX_EVENT_NUM) {
        pthread_cond_wait(&not_full, &event_insert_lock);
    }
    TAILQ_INSERT_TAIL(&sched_list, event, event_link);
    event_num++;
    pthread_cond_signal(&not_empty);
    printf("Event %d Inserted.\n", event->id);
    pthread_mutex_unlock(&event_insert_lock);
}

void execute_event(Event* event) {
    pthread_mutex_lock(&event_exec_lock);
    if (event->is_expired) {
        printf("Event %d Expired.\n", event->id);
        pthread_mutex_unlock(&event_exec_lock);
        return;
    }
    printf("Event %d Executed.\n", event->id);

    pthread_mutex_unlock(&event_exec_lock);
}

void create_event(int id, void* data, int size, struct Event* event_buf) {

    pthread_mutex_lock(&event_create_lock);
    event_buf->id = id;
    //strcpy((char*)event_buf -> data, (char*)data);
    event_buf->is_expired = false;
    pthread_mutex_unlock(&event_create_lock);
    printf("Event %d Created.\n", id);
}

void expire_event(Event* event) {
    event->is_expired = true;
}

void mysleep(int sec) {
    struct timeval tv;
    tv.tv_sec = sec / 1000;
    tv.tv_usec = sec % 1000;
    select(0, nullptr, nullptr, nullptr, &tv);
}



