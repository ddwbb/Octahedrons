#include "../Headers/Octahedron.h"

Octahedron::Octahedron() {
	init();
}

Octahedron::Octahedron(DATA * _data, GLuint _list, GLuint * _textures) {
	data = _data;
	draw_list = _list;
	textures = _textures;

	init();
}

Octahedron::~Octahedron() {

}

void Octahedron::init() {
	data->texturing = TEXTURE_SINGLE;

	data->slicing = SLICING_DEFAULT;
	data->blending = BLENDING_DEFAULT;

	data->texture_count = texture_paths.size();

	data->triangle_angle = TRIANGLE_ANGLE_DEFAULT;
	data->triangle_speed = TRIANGLE_SPEED_DEFAULT;
	data->triangle_rotating = TRIANGLE_ROTATING_DEFAULT;

	data->octahedron_speed = OCTAHEDRON_SPEED_DEFAULT;
	data->octahedron_aspect = OCTAHEDRON_ASPECT_DEFAULT;

	data->octahedron_angle_x = OCTAHEDRON_ANGLE_X_DEFAULT;
	data->octahedron_angle_y = OCTAHEDRON_ANGLE_Y_DEFAULT;

	data->position[0] = 0;
	data->position[1] = 0;
	data->position[2] = 0;
}

void Octahedron::draw() {
	Octahedron::draw(data, draw_list, textures);
}

void Octahedron::draw_triangle(int color) {
	Octahedron::draw_triangle(color, data, draw_list, textures);
}

void Octahedron::draw(DATA * data, GLuint draw_list, GLuint * textures) {
	if (data->octahedron_angle_x + data->octahedron_speed > 360 || data->octahedron_angle_x + data->octahedron_speed < -360) {
		data->octahedron_angle_x = 0;
	}
	else {
		data->octahedron_angle_x += data->octahedron_speed;
	}

	glPushMatrix();
	glTranslatef(data->position[0], data->position[1], data->position[2]);
	glRotatef(data->octahedron_angle_x, 0, 1, 0);
	glRotatef(data->octahedron_angle_y, 1, 0, 0);
	if (data->slicing) glCallList(draw_list);
	else {
		glPushMatrix();
		for (int i = 0; i < 8; i++) {
			if (data->texturing == TEXTURE_SINGLE) draw_triangle(TEXTURE_ID_DEFAULT, data, draw_list, textures);
			else draw_triangle(i, data, draw_list, textures);
			glRotatef(90 * (i + 1), 0, 1, 0);
			if (i == 3) glRotatef(180, 0, 0, 1);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Octahedron::draw_triangle(int color, DATA * data, GLuint draw_list, GLuint * textures) {
	GLfloat vertexes[] = {
		-40, 0, 40,
		40, 0, 40,
		0, 70, 0
	};

	GLubyte indeces[] = { 0, 1, 2 };
	ColorArray color_array;

	glEnableClientState(GL_VERTEX_ARRAY);

	if (!data->texturing) {
		glEnableClientState(GL_COLOR_ARRAY);
		color_array.set_alpha(data->alpha);
		color_array.set_color(color);
		glColorPointer(4, GL_FLOAT, 0, color_array);
	}
	else {
		GLfloat texture_coord[] = {
			0, 0,
			1, 0,
			0.5, 1
		};
		glColor4f(1, 1, 1, data->alpha);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, texture_coord);
		if (color >= data->texture_count) color = data->texture_count - 1;
		glBindTexture(GL_TEXTURE_2D, textures[color]);
	}

	if (data->blending) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);

	}

	glVertexPointer(3, GL_FLOAT, 0, vertexes);
	glNormal3f(0, 40, 70);

	if (data->triangle_angle + data->triangle_speed > 360 || data->triangle_angle + data->triangle_speed < -360) {
		data->triangle_angle = 0;
		if (!data->triangle_rotating)
			data->triangle_speed = 0;
	}
	else {
		data->triangle_angle += data->triangle_speed;
	}

	glPushMatrix();
	glTranslatef(0, 40 * data->octahedron_aspect, 70 * data->octahedron_aspect);
	glPushMatrix();
	glTranslatef(0, 18.5, 0);
	glPopMatrix();
	glRotatef(data->triangle_angle, 0, 40, 70);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indeces);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);

	if (!data->texturing)
		glDisableClientState(GL_COLOR_ARRAY);
	else {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void Octahedron::key_press(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(EXIT_SUCCESS);
		break;
	case 'q':
		data->octahedron_aspect += OCTAHEDRON_ASPECT_DIFFERENCE_VALUE;
		break;
	case 'e':
		if (data->octahedron_aspect - OCTAHEDRON_ASPECT_DIFFERENCE_VALUE >= 0)
			data->octahedron_aspect -= OCTAHEDRON_ASPECT_DIFFERENCE_VALUE;
		break;
	case 'a':
		glRotatef(CAMERA_ROTATE_ANGLE_VALUE, 0, 1, 0);
		break;
	case 'd':
		glRotatef(-CAMERA_ROTATE_ANGLE_VALUE, 0, 1, 0);
		break;
	case 'c':
		data->triangle_rotating = true;
		data->triangle_speed += TRIANGLE_SPEED_VALUE;
		break;
	case 'v':
		data->triangle_rotating = true;
		data->triangle_speed -= TRIANGLE_SPEED_VALUE;
		break;
	case 'g':
		data->triangle_rotating = false;
		break;
	case 'G':
		data->triangle_rotating = false;
		data->triangle_angle = 0;
		data->triangle_speed = 0;
		break;
	case 'r':
		data->blending = BLENDING_DEFAULT;
		data->slicing = SLICING_DEFAULT;
		data->triangle_angle = TRIANGLE_ANGLE_DEFAULT;
		data->triangle_speed = TRIANGLE_SPEED_DEFAULT;
		data->triangle_rotating = TRIANGLE_ROTATING_DEFAULT;
		data->octahedron_angle_x = OCTAHEDRON_ANGLE_X_DEFAULT;
		data->octahedron_angle_y = OCTAHEDRON_ANGLE_Y_DEFAULT;
		data->octahedron_speed = OCTAHEDRON_SPEED_DEFAULT;
		data->octahedron_aspect = OCTAHEDRON_ASPECT_DEFAULT;
		break;
	case '1':
		data->octahedron_speed -= OCTAHEDRON_SPEED_VALUE;
		break;
	case '2':
		data->octahedron_speed += OCTAHEDRON_SPEED_VALUE;
		break;
	case 't':
		if (data->texture_count && data->texturing == TEXTURE_SINGLE)
			data->texturing = TEXTURE_NONE;
		else
			data->texturing++;
		break;
	case '-':
		data->alpha = (data->alpha - ALPHA_DIFFERENCE_VALUE <= 0) ? 0 : data->alpha - ALPHA_DIFFERENCE_VALUE;
		break;
	case '=':
		data->alpha = (data->alpha + ALPHA_DIFFERENCE_VALUE >= 1) ? 1 : data->alpha + ALPHA_DIFFERENCE_VALUE;
		break;
	case 'b':
		data->blending = !data->blending;
		break;
	case 'l':
		data->slicing = !data->slicing;
		break;
	default:
		break;
	}
}