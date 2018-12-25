#include "../Headers/Server.h"


//INITIALIZE
Server::Server() {
	master_socket_init();
}


Server::~Server() {
}

void Server::master_socket_init() {
	if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws))) {
		errno = WSAGetLastError();
		return;
	}

	if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		errno = WSAGetLastError();
		return;
	}

	sockaddr_in addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.56.1");
	addr.sin_port = htons(1234);

	if (::bind(master_socket, (sockaddr *)&addr, (int)sizeof(addr)) == SOCKET_ERROR) {
		errno = WSAGetLastError();
		return;
	}

	if (FAILED(listen(master_socket, SOMAXCONN))) {
		errno = E_FAIL;
		return;
	}
	cout << "Server's main socket created!" << endl;
}

//START SERVER
void Server::start() {
	gargabe_collector = new thread(start_gargabe_collect, &data_list, &list_mutex);
	char name[NAME_SIZE] = "\0";
	char socket_type;
	while (true) {
		SOCKET new_connection;
		sockaddr_in connection_data;
		ZeroMemory(&connection_data, sizeof(connection_data));
		int connection_data_size = sizeof(connection_data);
		if (FAILED(new_connection = accept(master_socket, (sockaddr *)&connection_data, &connection_data_size))) {
			errno = E_FAIL;
			continue;
		}

		DATA * data;
		if (recv(new_connection, name, NAME_SIZE, 0) == SOCKET_ERROR) {
			errno = WSAGetLastError();
			continue;
		}
		if (recv(new_connection, &socket_type, 1, 0) == SOCKET_ERROR) {
			errno = WSAGetLastError();
			continue;
		}

		if (!(data = get_data_by_name(name))) {
			data = new DATA;
			strcpy_s(data->name, name);
			data->working = true;
			data_list.push_back(data);
			data->read_socket = 0;
			data->write_socket = 0;
		} else {
			if (data->write_socket && data->read_socket) {
				cout << "Connection already exist... This name is using by other connection: " << data->name << endl;
			}
		}

		if (socket_type == '0') {
			//write socket 
			//server write data into this socket
			data->write_socket = new_connection;
			data->writer = new thread(echo, data, &data_list, &list_mutex);
		}
		else {
			//read_socket
			//server get data from this socket
			data->read_socket = new_connection;
			data->reader = new thread(read_data, data);
		}
	}
}

DATA * Server::get_data_by_name(char * name) {
	DATA * data;
	for (list<DATA *>::iterator it = data_list.begin(); it != data_list.end(); it++) {
		data = *it;
		if (!strcmp(data->name, name)) return data;
	}
	return nullptr;
}

//GARGABE COLLECTOR'S FUNCTION
void start_gargabe_collect(list<DATA *> * _list, mutex * _mutex) {
	DATA * data;
	while (true) {
		Sleep(10000);
		_mutex->try_lock();
		for (list<DATA *>::iterator it = _list->begin(); it != _list->end();) {
			data = *it;
			if (!data->working) {
				closesocket(data->read_socket);
				closesocket(data->write_socket);
				data->reader->join();
				data->writer->join();
				it++;
				_list->remove(data);
				delete data;
			}
			else {
				cout << data->name << endl;
				it++;
			}
		}
		_mutex->unlock();
		cout << "----------" << endl;
	}
}

//SEND DATA 
void echo(DATA * data, list<DATA *> * _list, mutex * list_mutex) {
	DATA * other;
	while (true) {
		if (!data->working) return;
		//list_mutex->lock();
		int value = _list->size() - 1;
		char * buffer = (char *)&value;
		int remaining = sizeof(value);
		int sent = 0;
		int result = 0;
		while (remaining > 0) {
			if ((result = send(data->write_socket, buffer + sent, remaining, 0)) > 0) {
				remaining -= result;
				sent += result;
			} else {
				data->working = false;
				return;
			}
		}
		//send_to(data, INT_SIZE, 0, &value, INT_TYPE);
		for (list<DATA *>::iterator it = _list->begin(); it != _list->end(); it++) {
			if (*it == data) continue;
			other = *it;
			send_info(data, other);
		}
		//list_mutex->unlock();
		Sleep(3000);
	}
}

//READ DATA 
void read_data(DATA * data) {
	while (true) {
		if (!data->working) return;

		int remaining = sizeof(data->info);
		int received = 0;
		int result = 0;
		char * buffer = (char *)&data->info;
		//recv(data->read_socket, (char *)&data->info, sizeof(DATA_INFO), 0);
		while (remaining > 0) {
			if ((result = recv(data->read_socket, (char *)&data->info + received, remaining, 0)) > 0) {
				remaining -= result;
				received += result;
			}
			else if (result == SOCKET_ERROR) {
				data->working = false;
				return;
			}
		}


		////bool triangle_rotating;
		//read_from(data, BOOL_SIZE, 0, &data->triangle_rotating, BOOL_TYPE);
		////bool blending;
		//read_from(data, BOOL_SIZE, 0, &data->blending, BOOL_TYPE);
		////bool slicing;
		//read_from(data, BOOL_SIZE, 0, &data->slicing, BOOL_TYPE);

		////int texturing;
		//read_from(data, INT_SIZE, 0, &data->texturing, INT_TYPE);
		////int texture_count;
		//read_from(data, INT_SIZE, 0, &data->texture_count, INT_TYPE);

		//float alpha;
		//read_from(data, FLOAT_SIZE, 0, &data->alpha, FLOAT_TYPE);
		////float triangle_angle;
		//read_from(data, FLOAT_SIZE, 0, &data->triangle_angle, FLOAT_TYPE);
		////float triangle_speed;
		//read_from(data, FLOAT_SIZE, 0, &data->triangle_speed, FLOAT_TYPE);
		////float octahedron_angle_x;
		//read_from(data, FLOAT_SIZE, 0, &data->octahedron_angle_x, FLOAT_TYPE);
		////float octahedron_angle_y;
		//read_from(data, FLOAT_SIZE, 0, &data->octahedron_angle_y, FLOAT_TYPE);
		////float octahedron_speed;
		//read_from(data, FLOAT_SIZE, 0, &data->octahedron_speed, FLOAT_TYPE);
		////float octahedron_aspect;
		//read_from(data, FLOAT_SIZE, 0, &data->octahedron_aspect, FLOAT_TYPE);
		////float position[3];
		//for (int i = 0; i < 3; i++)
		//	read_from(data, FLOAT_SIZE, 0, &data->position[i], FLOAT_TYPE);
	}
}

//SUPPORT FUNCTIONS
void send_info(DATA * target, DATA * data) {

	int remaining = sizeof(data->info);
	int sent = 0;
	int result = 0;
	char * buffer = (char *)&data->info;

	//send(target->write_socket, (char *)&data->info, sizeof(DATA_INFO), 0);

	while (remaining > 0) {
		if ((result = send(target->write_socket, (char *)&data->info + sent, remaining, 0)) > 0) {
			remaining -= result;
			sent += result;
		} else {
			target->working = false;
			break;
		}
	}
	


	////bool triangle_rotating;
	//send_to(data, BOOL_SIZE, 0, &data->triangle_rotating, BOOL_TYPE);
	////bool blending;
	//send_to(data, BOOL_SIZE, 0, &data->blending, BOOL_TYPE);
	////bool slicing;
	//send_to(data, BOOL_SIZE, 0, &data->slicing, BOOL_TYPE);

	////int texturing;
	//send_to(data, INT_SIZE, 0, &data->texturing, INT_TYPE);
	////int texture_count;
	//send_to(data, INT_SIZE, 0, &data->texture_count, INT_TYPE);

	//float alpha;
	//send_to(data, FLOAT_SIZE, 0, &data->alpha, FLOAT_TYPE);
	////float triangle_angle;
	//send_to(data, FLOAT_SIZE, 0, &data->triangle_angle, FLOAT_TYPE);
	////float triangle_speed;
	//send_to(data, FLOAT_SIZE, 0, &data->triangle_speed, FLOAT_TYPE);
	////float octahedron_angle_x;
	//send_to(data, FLOAT_SIZE, 0, &data->octahedron_angle_x, FLOAT_TYPE);
	////float octahedron_angle_y;
	//send_to(data, FLOAT_SIZE, 0, &data->octahedron_angle_y, FLOAT_TYPE);
	////float octahedron_speed;
	//send_to(data, FLOAT_SIZE, 0, &data->octahedron_speed, FLOAT_TYPE);
	////float octahedron_aspect;
	//send_to(data, FLOAT_SIZE, 0, &data->octahedron_aspect, FLOAT_TYPE);
	////float position[3];
	//for (int i = 0; i < 3; i++)
	//	send_to(data, FLOAT_SIZE, 0, &data->position[i], FLOAT_TYPE);
}

void read_from(DATA * data, int length, int flags, void * var, int mode) {
	int * int_value;
	bool * bool_value;
	char * char_value;
	float * float_value;
	char * buffer = new char[length];
	buffer[length - 1] = '\0';
	if (recv(data->read_socket, buffer, length, flags) == SOCKET_ERROR) {
		data->working = false;
	}
	else {
		try {
			switch (mode) {
			case INT_TYPE:
				int_value = (int *)var;
				(*int_value) = atoi(buffer);
				break;
			case BOOL_TYPE:
				bool_value = (bool *)var;
				(*bool_value) = (atoi(buffer) == 0) ? false : true;
				break;
			case CHAR_TYPE:
				char_value = (char *)var;
				strcpy_s(char_value, length, buffer);
				break;
			case FLOAT_TYPE:
				float_value = (float *)var;
				(*float_value) = atof(buffer);
				break;
			}
		}
		catch (...) {

		}
	}
}

void send_to(DATA * data, int length, int flags, void * var, int mode) {
	if (!data->working) return;
	int val;
	int * int_value;
	bool * bool_value;
	char * char_value;
	float * float_value;
	char * buffer = new char[length];
	buffer[length - 1] = '\0';
	switch (mode) {
	case INT_TYPE:
		int_value = (int *)var;
		sprintf_s(buffer, length, "%d", *int_value);
		break;
	case BOOL_TYPE:
		bool_value = (bool *)var;
		val = (*bool_value) ? 1 : 0;
		sprintf_s(buffer, length, "%d", val);
		break;
	case CHAR_TYPE:
		char_value = (char *)var;
		strcpy_s(buffer, length, char_value);
		break;
	case FLOAT_TYPE:
		float_value = (float *)var;
		sprintf_s(buffer, length, "%4.3f", *float_value);
		break;
	}
	if (send(data->write_socket, buffer, length, flags) == SOCKET_ERROR) {
		data->working = false;
	}
}