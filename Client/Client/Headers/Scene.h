#pragma once

#include <list>
#include <cstdlib>
#include <iostream>
#include <winsock.h>

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
}DATA;

class Scene {
	list<DATA *> * data_list;

public:
	Scene(list<DATA *> *);
	~Scene();

	void view_list();
};

