/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */


#ifndef SCHEDULE_ROUTE_H
#define SCHEDULE_ROUTE_H

#include <string>
#include <vector>

using namespace std;

typedef struct {
    int id;
    uint32_t dest;
    uint32_t gateway;
    int genmask;
    string flags;
    int metric;
    int ref;
    int use;
    string iface;
}Route;

void test_pop();

void splitStr(const string &str, vector<string> &vec, const string &splitChar);

int read_genmask(string& str_mask);

void read_route(vector<Route>& routing_table);

void print_route(Route& route);

void print_routing_table(vector<Route>& routing_table);

#endif //SCHEDULE_POP_H
