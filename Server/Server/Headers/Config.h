#pragma once

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

typedef struct {
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
}DATA_INFO;

typedef struct {
	bool working;

	char name[NAME_SIZE] = "\0";

	DATA_INFO info;

	SOCKET read_socket;
	SOCKET write_socket;

	thread * reader;
	thread * writer;

	mutex _m;
}DATA;