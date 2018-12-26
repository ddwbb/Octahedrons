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
	void key_press(unsigned char, int, int);
public:
	Client();
	~Client();

	void init();
	void view_list();
	static void view_list(list<DATA *>);
	static void callback_key_press(unsigned char, int, int);

	void start(int *, char **);
};

static Client * client_instance = nullptr;

void send_data(SOCKET, DATA *);
void get_list(SOCKET, list<DATA *> *);