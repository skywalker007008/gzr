#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <iostream>
#include <vector>
#include "route.h"
#include "arpa/inet.h"

#define bit_8(value, byte_num) ((value) >> ((byte_num) * 8) & 0xFF)

/*
 * Copyright G410 Studio
 * Author: Skywalker007008, Liu Zihao
 * Github Link: https://github.com/skywalker007008/marvelCoding
 * 
 */
using namespace std;

void test_pop() {
    vector<Route> routing_table;
    read_route(routing_table);
    print_routing_table(routing_table);
}

void splitStr(const string &str, vector<string> &vec, const string &splitChar)
{
    string::size_type pos = str.find(splitChar);
    string::size_type pos1 = 0;
    while(string::npos !=  pos)
    {
        vec.push_back(str.substr(pos1, pos -pos1));
        pos1 = pos + splitChar.size();
        pos = str.find(splitChar, pos1);
    }
    if(pos1 != str.length())
    {
        vec.push_back(str.substr(pos1));
    }
}

int read_genmask(string& str_mask) {
    vector<string> mask;
    splitStr(str_mask, mask, ".");
    int genmask = 0;
    genmask = genmask | (atoi(mask[0].c_str()) << 24);
    genmask = genmask | (atoi(mask[1].c_str()) << 16);
    genmask = genmask | (atoi(mask[2].c_str()) << 8);
    genmask = genmask | (atoi(mask[3].c_str()));
    return genmask;
}

void read_route(vector<Route>& routing_table) {
    FILE* read_fp;
    char buffer[512];
    int chars_read;
    memset(buffer, 0, sizeof(buffer));
    read_fp = popen("route -n", "r");
    if (read_fp != NULL) {
        chars_read = fread(buffer, sizeof(char), 512, read_fp);
        if (chars_read > 0) {
            printf("Output:\n%s\n", buffer);
            /*for (int i = 0; i < chars_read; i++) {
                printf("%d ", (__uint8_t)buffer[i]);
            }*/
        } else {
            printf("Output Error.\n");
        }
        pclose(read_fp);
    }
    string str(buffer);
    vector<string> vecstr;
    vector<string> typestr;
    splitStr(str, vecstr, "\n");
    for (int i = 2; i < vecstr.size(); i++) {

        splitStr(vecstr[i], typestr, " ");
        int state = 0;
        Route route ;
        route.id = i - 2;
        for (int j = 0; j < typestr.size(); j++) {
            if (typestr[j].size() == 0) {
                continue;
            }
            switch (state) {
                case 0: {
                    route.dest = inet_addr(typestr[j].c_str());
                    state++;
                    break;
                }
                case 1: {
                    route.gateway = inet_addr(typestr[j].c_str());
                    state++;
                    break;
                }
                case 2: {
                    //routing_table[i - 1].genmask = read_genmask(typestr[j]);
                    uint32_t a = (inet_addr(typestr[j].c_str()));
                    route.genmask = bit_8(a, 0) << 24 | bit_8(a, 1) << 16 | bit_8(a, 2) << 8 | bit_8(a, 3);
                    //route.genmask = (inet_addr(typestr[j].c_str()));
                    printf("%d %d\n", a, route.genmask);
                    state++;
                    break;
                }
                case 3: {
                    route.flags = typestr[j];
                    state++;
                    break;
                }
                case 4: {
                    route.metric = atoi(typestr[j].c_str());
                    state++;
                    break;
                }
                case 5: {
                    route.ref = atoi(typestr[j].c_str());
                    state++;
                    break;
                }
                case 6: {
                    route.use = atoi(typestr[j].c_str());
                    state++;
                    break;
                }
                case 7: {
                    route.iface = typestr[j];
                    state++;
                    break;
                }
                default: {
                    break;
                }
            }
        }
        routing_table.push_back(route);
        typestr.clear();
    }
}

void print_route(Route& route) {
    cout << "Route " << route.id << endl;
    //cout << "Dest: " << route.dest << endl;
    cout << "Dest: " << ((route.dest % 65536) % 256) << "." \
                        << (route.dest % 65536) / 256 << "." \
                        << ((route.dest % (256 * 256 * 256)) / 65536) << "." \
                        << route.dest / (256 * 65536) << endl;
    //cout << "Gateway: " << route.gateway << endl;
    cout << "Gateway: " << ((route.gateway % 65536) % 256) << "." \
                        << (route.gateway % 65536) / 256 << "." \
                        << ((route.gateway % (256 * 256 * 256)) / 65536) << "." \
                        << route.gateway / (256 * 65536) << endl;
    cout << "Genmask: " << route.genmask / (256 * 65536) << "." \
                        << ((route.genmask % (256 * 256 * 256)) / 65536) << "." \
                        << (route.genmask % 65536) / 256 << "." \
                        << ((route.genmask % 65536) % 256) << endl;
    cout << "Flags: " << route.flags << endl;
    cout << "Metric: " << route.metric << endl;
    cout << "Ref: " << route.ref << endl;
    cout << "Use: " << route.use << endl;
    cout << "Iface: " << route.iface << endl;
}

void print_routing_table(vector<Route>& routing_table) {
    for (int i = 0; i < routing_table.size(); i++) {
        print_route(routing_table[i]);
    }
}