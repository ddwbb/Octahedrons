#pragma once

#include <list>
#include <cstdlib>
#include <iostream>
#include <winsock.h>

#include "config.h"
#include "Octahedron.h"

using namespace std;

class Scene {
	bool initialized;
	bool lighting;
	bool light_disabled;
	bool full_screen;

	int slice_count;
	int texture_count;

	GLfloat light_angle;
	GLfloat light_speed;
	GLfloat light_distance;

	DATA * data;

	Octahedron * main;

	list<DATA *> * data_list;

	GLuint octahedron_list;
	GLuint * texture_names;
	AUX_RGBImageRec ** texture_images;

	void init_view();
	void init_light();
	void init_texture();
	void init_material();

	static void callback_display();
	static void callback_animation(int);

public:
	Scene();
	Scene(list<DATA *> *, DATA *);
	~Scene();

	void init(int *, char **);
	void display();
	void animation(int);
	void key_press(unsigned char, int, int);
	void start();
	void draw_sphere();

	void init_sliced_octahedron();
	void init_sliced_triangles(int);
};

static Scene * instance = nullptr;