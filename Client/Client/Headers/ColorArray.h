#pragma once

#ifndef COLOR_ARRAY
#define COLOR_ARRAY
#include "config.h"

//��� ������� � ���� GLfloat
#include <GL/glut.h> 

class ColorArray {
	int size;
	GLfloat * array;
	GLfloat alpha;

	void copy_values(GLfloat *);
public:
	ColorArray();
	ColorArray(int);
	ColorArray(int, GLfloat *);
	~ColorArray();
	
	operator GLfloat *();

	int length();
	void set_color(int);
	void set_color(GLfloat *);
	void set_alpha(GLfloat);
	void set_component(int, int, GLfloat);
	void reset();
};

//���������� ������� �������� ������� �������
int array_size(GLfloat *);

#endif