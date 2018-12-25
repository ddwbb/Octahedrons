#pragma once
#pragma comment(lib, "WS2_32.lib")

#include <list>
#include <mutex>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <winsock.h>

#define NAME_SIZE 10
#define INT_SIZE 5
#define BOOL_SIZE 5
#define FLOAT_SIZE 9

using namespace std;

enum VAR_TYPE {
	INT_TYPE = 0,
	BOOL_TYPE,
	CHAR_TYPE,
	FLOAT_TYPE
};

typedef struct DATA_INFO {
	bool triangle_rotating;
	bool blending;
	bool slicing;

	int texturing;
	int texture_count;

	float alpha;
	float triangle_angle;
	float triangle_speed;
	float octahedron_angle_x;
	float octahedron_angle_y;
	float octahedron_speed;
	float octahedron_aspect;
	float position[3];
};

typedef struct DATA{
	bool working;

	char name[NAME_SIZE] = "\0";
	
	DATA_INFO info;

	SOCKET read_socket;
	SOCKET write_socket;

	thread * reader;
	thread * writer;

	mutex _m;
};

class Server {
	WSADATA ws;
	
	list<DATA *> data_list;

	SOCKET master_socket;

	thread * gargabe_collector;
	mutex list_mutex;

public:
	Server();
	~Server();

	void start();
	void master_socket_init();
	DATA * get_data_by_name(char *);
};

void read_data(DATA *);
void echo(DATA *, list<DATA *> *, mutex *);
void start_gargabe_collect(list<DATA *> *, mutex *);
void send_to(DATA *, int, int, void *, int);
void read_from(DATA *, int, int, void *, int);
void send_info(DATA *, DATA *);
