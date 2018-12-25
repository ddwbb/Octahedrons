#pragma once
#pragma comment(lib, "WS2_32.lib")

#include <time.h>
#include <list>
#include <thread>
#include <iostream>
#include <winsock.h>

#include "Scene.h"

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

//typedef struct DATA{
//	bool triangle_rotating;
//	bool blending;
//	bool slicing;
//
//	int texturing;
//	int texture_count;
//
//	float alpha;
//	float triangle_angle;
//	float triangle_speed;
//	float octahedron_angle_x;
//	float octahedron_angle_y;
//	float octahedron_speed;
//	float octahedron_aspect;
//	float position[3];
//};

class Client {

	WSADATA ws;

	list<DATA *> data_list;

	char name[NAME_SIZE];

	DATA data;

	SOCKET read_socket;
	SOCKET write_socket;

	thread * receiver;
	thread * sender;

	Scene * scene;

	void randomaze();
	void socket_init(SOCKET *);
public:
	Client();
	~Client();

	void init();
	void view_list();
	static void view_list(list<DATA *>);
};

void send_to(SOCKET, int, int, void *, int);
void read_from(SOCKET, int, int, void *, int);
void send_data(SOCKET, DATA *);
void get_list(SOCKET, list<DATA *> *);