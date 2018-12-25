#include "../Headers/Client.h"

Client::Client() {
	srand(time(0));
	randomaze();

	data.texturing = 2;
	data.texture_count = 12;

	data.position[0] = (float)(rand() % 1000 - 500) / 1000;
	data.position[1] = (float)(rand() % 1000 - 500) / 1000;
	data.position[2] = (float)(rand() % 1000 - 500) / 1000;

	init();

	scene = new Scene(&data_list);
	//scene->view_list();
}


Client::~Client(){

}

void Client::init() {
	if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws))) {
		errno = WSAGetLastError();
		return;
	}

	socket_init(&read_socket);
	socket_init(&write_socket);

	receiver = new thread(get_list, read_socket, &data_list);
	sender = new thread(send_data, write_socket, &data);
}

void Client::socket_init(SOCKET * sock) {
	if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		errno = WSAGetLastError();
		return;
	}

	sockaddr_in addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("192.168.56.1");
	addr.sin_port = htons(1234);

	if (connect(*sock, (sockaddr *)&addr, (int)sizeof(addr)) == SOCKET_ERROR) {
		errno = WSAGetLastError();
		return;
	}
	cout << "Cleint's socket created and connected!" << endl;

	char socket_type = (*sock == read_socket) ? '0' : '1';
	if (send(*sock, name, NAME_SIZE, 0) == SOCKET_ERROR) {
		errno = WSAGetLastError();
		return;
	}
	if (send(*sock, &socket_type, 1, 0) == SOCKET_ERROR) {
		errno = WSAGetLastError();
		return;
	}
}

void get_list(SOCKET sock, list<DATA *> * _list) {
	int count;
	//char buffer[10] = "\0";
	while (true) {
		int remaining = sizeof(count);
		int received = 0;
		int result = 0;
		char * buffer = (char *)&count;
		while (remaining > 0) {
			if ((result = recv(sock, buffer + received, remaining, 0)) > 0) {
				remaining -= result;
				received += result;
			}
			else if (result == SOCKET_ERROR) {
				cout << "Disconnected" << endl;
				closesocket(sock);
				return;
			}
		}
		list<DATA *> pool;
		//read_from(sock, INT_SIZE, 0, &count, INT_TYPE);
		for (int i = 0; i < count; i++) {
			remaining = sizeof(DATA);
			received = 0;
			result = 0;
			DATA * data = new DATA;
			//char * buffer = (char *)data;
			//recv(sock, (char *)data, sizeof(DATA), 0);

			while (remaining > 0) {
				if ((result = recv(sock, (char *)data + received, remaining, 0)) > 0) {
					remaining -= result;
					received += result;
				} else if (result == SOCKET_ERROR) {
					cout << "Disconnected" << endl;
					closesocket(sock);
					return;
				}
			}
			////bool triangle_rotating;
			//read_from(sock, BOOL_SIZE, 0, &data->triangle_rotating, BOOL_TYPE);
			////bool blending;
			//read_from(sock, BOOL_SIZE, 0, &data->blending, BOOL_TYPE);
			////bool slicing;
			//read_from(sock, BOOL_SIZE, 0, &data->slicing, BOOL_TYPE);

			////int texturing;
			//read_from(sock, INT_SIZE, 0, &data->texturing, INT_TYPE);
			////int texture_count;
			//read_from(sock, INT_SIZE, 0, &data->texture_count, INT_TYPE);

			////float alpha;
			//read_from(sock, FLOAT_SIZE, 0, &data->alpha, FLOAT_TYPE);
			////float triangle_angle;
			//read_from(sock, FLOAT_SIZE, 0, &data->triangle_angle, FLOAT_TYPE);
			////float triangle_speed;
			//read_from(sock, FLOAT_SIZE, 0, &data->triangle_speed, FLOAT_TYPE);
			////float octahedron_angle_x;
			//read_from(sock, FLOAT_SIZE, 0, &data->octahedron_angle_x, FLOAT_TYPE);
			////float octahedron_angle_y;
			//read_from(sock, FLOAT_SIZE, 0, &data->octahedron_angle_y, FLOAT_TYPE);
			////float octahedron_speed;
			//read_from(sock, FLOAT_SIZE, 0, &data->octahedron_speed, FLOAT_TYPE);
			////float octahedron_aspect;
			//read_from(sock, FLOAT_SIZE, 0, &data->octahedron_aspect, FLOAT_TYPE);
			////float position[3];
			//for (int i = 0; i < 3; i++)
			//	read_from(sock, FLOAT_SIZE, 0, &data->position[i], FLOAT_TYPE);

			pool.push_back(data);
		}
		(*_list) = pool;
		Client::view_list(pool);
	}
}

void send_data(SOCKET sock, DATA * data) {
	while (true) {
		int remaining = sizeof(DATA);
		int sent = 0;
		int result = 0;
		char * buffer = (char *)&data;

		data->position[0] = (float)(rand() % 1000 - 500) / 1000;
		data->position[1] = (float)(rand() % 1000 - 500) / 1000;
		data->position[2] = (float)(rand() % 1000 - 500) / 1000;

		//send(sock, (char *)data, sizeof(DATA), 0);
		while (remaining > 0) {
			if ((result = send(sock, (char *)data + sent, remaining, 0)) > 0) {
				remaining -= result;
				sent += result;
			}
			else {
				closesocket(sock);
				return;
			}
		}
		//left_byte = sizeof(DATA_INFO);


		////bool triangle_rotating;
		//send_to(sock, BOOL_SIZE, 0, &data->triangle_rotating, BOOL_TYPE);
		////bool blending;
		//send_to(sock, BOOL_SIZE, 0, &data->blending, BOOL_TYPE);
		////bool slicing;
		//send_to(sock, BOOL_SIZE, 0, &data->slicing, BOOL_TYPE);

		////int texturing;
		//send_to(sock, INT_SIZE, 0, &data->texturing, INT_TYPE);
		////int texture_count;
		//send_to(sock, INT_SIZE, 0, &data->texture_count, INT_TYPE);

		////float alpha;
		//send_to(sock, FLOAT_SIZE, 0, &data->alpha, FLOAT_TYPE);
		////float triangle_angle;
		//send_to(sock, FLOAT_SIZE, 0, &data->triangle_angle, FLOAT_TYPE);
		////float triangle_speed;
		//send_to(sock, FLOAT_SIZE, 0, &data->triangle_speed, FLOAT_TYPE);
		////float octahedron_angle_x;
		//send_to(sock, FLOAT_SIZE, 0, &data->octahedron_angle_x, FLOAT_TYPE);
		////float octahedron_angle_y;
		//send_to(sock, FLOAT_SIZE, 0, &data->octahedron_angle_y, FLOAT_TYPE);
		////float octahedron_speed;
		//send_to(sock, FLOAT_SIZE, 0, &data->octahedron_speed, FLOAT_TYPE);
		////float octahedron_aspect;
		//send_to(sock, FLOAT_SIZE, 0, &data->octahedron_aspect, FLOAT_TYPE);
		////float position[3];
		//for (int i = 0; i < 3; i++)
		//	send_to(sock, FLOAT_SIZE, 0, &data->position[i], FLOAT_TYPE);

		Sleep(5000);
	}
}

void Client::view_list() {
	Client::view_list(data_list);
}

void Client::view_list(list<DATA *> data_list) {
	DATA * data;
	cout << "START SCANING" << endl;
	for (list<DATA *>::iterator it = data_list.begin(); it != data_list.end(); it++) {
		cout << "---------------------" << endl;
		data = *it;
		cout << "Triangle Rotating: " << ((data->triangle_rotating) ? "Yes" : "No") << endl;
		cout << "Blending: " << ((data->blending) ? "Yes" : "No") << endl;
		cout << "Slicing: " << ((data->blending) ? "Yes" : "No") << endl;
		cout << "Texture mode: " << data->texturing << endl;
		cout << "Texture count: " << data->texture_count << endl;
 		cout << "Alpha: " << data->alpha << endl;
		cout << "Triangle angle: " << data->triangle_angle << endl;
		cout << "Triangle speed: " << data->triangle_speed << endl;
		cout << "Angle_X: " << data->octahedron_angle_x << endl;
		cout << "Angle_Y: " << data->octahedron_angle_y << endl;
		cout << "Aspect: " << data->octahedron_aspect << endl;
		cout << "Speed: " << data->octahedron_speed << endl;
		cout << "Position: ";
		for (int i = 0; i < 3; i++)
			cout << data->position[i] << " ";
		cout << endl;
		cout << "---------------------" << endl;
	}
	cout << "END OF SCANING" << endl << endl;
}

void Client::randomaze() {
	for (int i = 0; i < 4; i++) {
		name[i] = rand() % 26 + 97;
	}
	name[4] = '\0';
}

void read_from(SOCKET sock, int length, int flags, void * var, int mode) {
	int * int_value;
	bool * bool_value;
	char * char_value;
	float * float_value;
	char * buffer = new char[length];
	buffer[length - 1] = '\0';
	if (recv(sock, buffer, length, flags) == SOCKET_ERROR) {
		//data->working = false;
	}
	else {
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
}

void send_to(SOCKET sock, int length, int flags, void * var, int mode) {
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
		sprintf_s(buffer, length, "%.3f", *float_value);
		break;
	}
	if (send(sock, buffer, length, flags) == SOCKET_ERROR) {
		//data->working = false;
	}
}