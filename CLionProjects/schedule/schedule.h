/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */

#ifndef SCHEDULE_SCHEDULE_H
#define SCHEDULE_SCHEDULE_H

#include "queue.h"

struct Event{
    int id;
    bool is_expired;
    void* data;
    TAILQ_ENTRY(Event) event_link;
};

TAILQ_HEAD(Event_list, Event);

extern struct Event_list sched_list;

extern pthread_mutex_t event_fetch_lock;
extern pthread_mutex_t event_exec_lock;
extern pthread_mutex_t event_insert_lock;
extern pthread_cond_t not_full;
extern pthread_cond_t not_empty;
extern pthread_mutex_t event_create_lock;

void init_event_list();

Event* fetch_new_event();

void insert_event(Event* event);

void execute_event(Event* event);

void create_event(int id, void* data, int size, struct Event* event_buf);

void expire_event(Event* event);

void mysleep(int sec);

#endif //SCHEDULE_SCHEDULE_H
