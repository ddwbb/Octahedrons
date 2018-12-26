#include "../Headers/Client.h"

Client::Client() {
	client_instance = this;
	srand(time(0));
	randomaze();

	read_socket = 0;
	write_socket = 0;

	data.octahedron_aspect = 1;

	init();

	scene = new Scene(&data_list, &data);
}

Client::~Client(){

}


//----------------------INITIALIZE----------------------
void Client::init() {
	if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws))) {
		errno = WSAGetLastError();
		return;
	}

	socket_init(&read_socket);
	socket_init(&write_socket);

	if (!read_socket || !write_socket) {
		cout << "Can't connect to Server. Autoexit" << endl;
		cout << "Start Single Pleer" << endl;
	} else {
		receiver = new thread(get_list, read_socket, &data_list);
		sender = new thread(send_data, write_socket, &data);
	}
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


//----------------------RECEIVE FUNCTION----------------------
void get_list(SOCKET sock, list<DATA *> * _list) {
	int count;
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
		for (int i = 0; i < count; i++) {
			remaining = sizeof(DATA);
			received = 0;
			result = 0;
			DATA * data = new DATA;
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
			pool.push_back(data);
		}
		(*_list) = pool;
		Client::view_list(pool);
	}
}


//----------------------SEND DATA----------------------
void send_data(SOCKET sock, DATA * data) {
	while (true) {
		int remaining = sizeof(DATA);
		int sent = 0;
		int result = 0;
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
		Sleep(24);
	}
}


//----------------------VIEW FUNCTION----------------------
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


//----------------------
void Client::randomaze() {
	for (int i = 0; i < 4; i++) {
		name[i] = rand() % 26 + 97;
	}
	name[4] = '\0';
}

void Client::start(int * argc, char ** argv) {
	scene->init(argc, argv);
	glutKeyboardFunc(callback_key_press);
	scene->start();
}

void Client::callback_key_press(unsigned char key, int x, int y) {
	client_instance->key_press(key, x, y);
}

void Client::key_press(unsigned char key, int x, int y) {
	//send_data(write_socket, &data);
	scene->key_press(key, x, y);
}