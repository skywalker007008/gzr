#include <iostream>
#include <thread>
#include "schedule.h"

using namespace std;

void insert_execute(Event* event);

void test_schedule() {
    init_event_list();
    struct Event temp_event[5];
    for (int i = 0; i < 5; i++) {
        create_event(i, nullptr, 0, &temp_event[i]);
        //insert_event(&temp_event[i]);
    }
    expire_event(&temp_event[2]);
    expire_event(&temp_event[4]);
    thread t1(insert_execute, &temp_event[0]);
    t1.detach();
    thread t2(insert_execute, &temp_event[1]);
    t2.detach();
    thread t3(insert_execute, &temp_event[2]);
    t3.detach();
    thread t4(insert_execute, &temp_event[3]);
    t4.detach();
    thread t5(insert_execute, &temp_event[4]);
    t5.detach();

}

void insert_execute(Event* event) {
    insert_event(event);
    if (event->id == 2 || event->id == 4) {
        expire_event(event);
    }
    mysleep(3000);
    Event* temp_event = fetch_new_event();
    execute_event(temp_event);
}

int main() {
    test_schedule();
    while (true) {

    }
    return 0;
}