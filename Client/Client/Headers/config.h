#pragma once

#ifndef CONFIG
#define CONFIG

#include <vector>

#include <glaux.h>
#include <GL/glut.h>

using namespace std;

static enum colors {
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	LIGHT_BLUE,
	DARK_BLUE,
	PURPLE,
	MIXED
};

static enum texturing {
	TEXTURE_NONE,
	TEXTURE_ALL,
	TEXTURE_SINGLE
};

static enum slicing {
	SLICING_OFF,
	SLICING_COMMON,
	SLICING_TRANSPARENT
};

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

static vector<const char *> texture_paths = {
	"./resources/stone_texture_light.bmp",
	"./resources/stone_texture_gray.bmp",
	"./resources/wood_texture.bmp",
	"./resources/ice_texture_1.bmp",
	"./resources/ice_texture_2.bmp",
	"./resources/ice_texture_white.bmp",
	"./resources/metall_texture_gray_1.bmp",
	"./resources/metall_texture_gray_2.bmp",
	"./resources/metall_texture_gray_3.bmp",
	"./resources/metall_texture_dark.bmp"
};

#define TEXTURE_ID_DEFAULT 1

#define FULL_SCREEN_VIEW true

#define ALPHA_DEFAULT 1
#define ALPHA_DIFFERENCE_VALUE 0.05

#define BLENDING_DEFAULT true

#define SLICING_DEFAULT SLICING_OFF

#define LIGHTING_DEFAULT true
#define LIGHT_ANGLE_DEFAULT 0
#define LIGHT_SPEED_VALUE 0.5
#define LIGHT_SPEED_DEFAULT 0
#define LIGHT_DISTANCE_DEFAULT -300
#define LIGHT_DISTANCE_DIFFERENCE_VALUE 10

#define TRIANGLE_ANGLE_DEFAULT 0
#define TRIANGLE_SPEED_VALUE 0.05
#define TRIANGLE_SPEED_DEFAULT 0
#define TRIANGLE_ROTATING_DEFAULT false

#define OCTAHEDRON_ANGLE_X_DEFAULT 0
#define OCTAHEDRON_ANGLE_Y_DEFAULT 0
#define OCTAHEDRON_SPEED_DEFAULT 0
#define OCTAHEDRON_SPEED_VALUE 0.5
#define OCTAHEDRON_ASPECT_DEFAULT 0
#define OCTAHEDRON_ASPECT_DIFFERENCE_VALUE 0.005

#define CAMERA_ROTATE_ANGLE_VALUE 6

#define SLICE_COUNT_DEFAULT 9

#endif