#pragma once

#include"config.h"
#include "ColorArray.h"

class Octahedron {

	DATA * data;
	GLuint draw_list;
	GLuint * textures;

	static void draw_triangle(int, DATA *, GLuint, GLuint *);

	void init();
	void draw_triangle(int);
public:
	Octahedron();
	Octahedron(DATA *, GLuint, GLuint *);
	~Octahedron();

	static void draw(DATA *, GLuint, GLuint *);

	void draw();
	void key_press(unsigned char, int, int);
};