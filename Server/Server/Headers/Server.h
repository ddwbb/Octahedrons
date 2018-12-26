#pragma once
#pragma comment(lib, "WS2_32.lib")

#include "Config.h"

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
void send_info(DATA *, DATA *);
