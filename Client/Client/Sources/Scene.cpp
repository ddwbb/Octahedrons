#include "../Headers/Scene.h"



Scene::Scene() {
	instance = this;
	initialized = false;
	full_screen = FULL_SCREEN_VIEW;

	texture_count = texture_paths.size();
	texture_names = new GLuint[texture_count];
	texture_images = new AUX_RGBImageRec *[texture_count];

	slice_count = SLICE_COUNT_DEFAULT;

	light_disabled = LIGHTING_DEFAULT;
	lighting = LIGHTING_DEFAULT;
	light_angle = LIGHT_ANGLE_DEFAULT;
	light_speed = LIGHT_SPEED_DEFAULT;
	light_distance = LIGHT_DISTANCE_DEFAULT;
}

Scene::Scene(list<DATA *> * _list, DATA * _data) {
	data = _data;
	data_list = _list;

	instance = this;
	initialized = false;
	full_screen = FULL_SCREEN_VIEW;

	texture_count = texture_paths.size();
	texture_names = new GLuint[texture_count];
	texture_images = new AUX_RGBImageRec *[texture_count];

	slice_count = SLICE_COUNT_DEFAULT;

	light_disabled = LIGHTING_DEFAULT;
	lighting = LIGHTING_DEFAULT;
	light_angle = LIGHT_ANGLE_DEFAULT;
	light_speed = LIGHT_SPEED_DEFAULT;
	light_distance = LIGHT_DISTANCE_DEFAULT;
}

Scene::~Scene() {

}


//----------------------INITIALIZE----------------------
void Scene::init(int * argc, char ** argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	if (full_screen) {
		glutCreateWindow("OpenGL Octahedron");
		glutFullScreen();
	}
	else {
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(992, 558);
		glutCreateWindow("OpenGL Octahedron");
	}

	glutDisplayFunc(callback_display);
	glutTimerFunc(40, callback_animation, 0);

	init_view();
	init_material();
	init_texture();
	init_sliced_octahedron();

	main = new Octahedron(data, octahedron_list, texture_names);

	initialized = true;
}

void Scene::init_view() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLdouble)16 / 9, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 30, 400, 0, 0, 0, 0, 400, -30);
}

void Scene::init_light() {
	GLfloat position[] = { 0, 0, 0, 1 };
	GLfloat ambient[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat diffuse[] = { 1, 1, 1, 1 };

	if (!lighting) {
		if (light_disabled) return;
		glDisable(GL_LIGHT0);
		light_disabled = true;
	}
	else {
		if (light_disabled) {
			light_disabled = false;
			glEnable(GL_LIGHT0);
		}
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	}
}

void Scene::init_texture() {
	glGenTextures(texture_count, texture_names);
	for (int i = 0; i < texture_count; i++) {
		glBindTexture(GL_TEXTURE_2D, texture_names[i]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		texture_images[i] = auxDIBImageLoad(texture_paths[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_images[i]->sizeX, texture_images[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_images[i]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
}

void Scene::init_material() {
	GLfloat ambient[] = { 0.02, 0.02, 0.02, 1 };
	GLfloat diffuse_front[] = { 1, 1, 1, 1 };
	GLfloat diffuse_back[] = { 0.2, 0.2, 0.2, 1 };

	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_front);
	glMaterialfv(GL_BACK, GL_DIFFUSE, diffuse_back);
}

void Scene::init_sliced_triangles(int color) {
	GLfloat width_step = (GLfloat)20 / (GLfloat)(slice_count);
	GLfloat height_step = (GLfloat)35 / (GLfloat)(slice_count);
	ColorArray color_array;
	color_array.set_color(color);
	glBegin(GL_QUADS);
	glColor4fv(color_array);
	glNormal3f(0, 40, -70);
	for (int i = 1; i < slice_count * 2; i += 2) {
		glVertex3f(-40 + i * width_step, 0 + i * height_step, 40 - i * width_step);
		glVertex3f(-40 + (i + 1) * width_step, 0 + (i + 1) * height_step, 40 - (i + 1) * width_step);
		glVertex3f(40 - (i + 1) * width_step, 0 + (i + 1) * height_step, 40 - (i + 1) * width_step);
		glVertex3f(40 - i * width_step, 0 + i * height_step, 40 - i * width_step);
	}
	glEnd();
}

void Scene::init_sliced_octahedron() {
	octahedron_list = glGenLists(1);
	glNewList(octahedron_list, GL_COMPILE_AND_EXECUTE);
	glPushMatrix();
	for (int i = 0; i < 8; i++) {
		init_sliced_triangles(i);
		glRotatef(90 * (i + 1), 0, 1, 0);
		if (i == 3) glRotatef(180, 0, 0, 1);
	}
	glPopMatrix();
	glEndList();
}


//----------------------MAIN FUNC----------------------
void Scene::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_sphere();
	glRotatef(90, 0, 1, 0);
	glFlush();
}

void Scene::animation(int value) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_sphere();

	main->draw();

	if (data_list->size()) {
		for (list<DATA *>::iterator it = data_list->begin(); it != data_list->end(); it++) {
			Octahedron::draw(*it, octahedron_list, texture_names);
		}
	}

	glFlush();

	glutTimerFunc(40, callback_animation, 0);
}

void Scene::key_press(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(EXIT_SUCCESS);
		break;
	case 'a':
		glRotatef(CAMERA_ROTATE_ANGLE_VALUE, 0, 1, 0);
		break;
	case 'd':
		glRotatef(-CAMERA_ROTATE_ANGLE_VALUE, 0, 1, 0);
		break;
	case ' ':
		lighting = !lighting;
		break;
	case 'w':
		if (light_distance + LIGHT_DISTANCE_DIFFERENCE_VALUE <= 0)
			light_distance += LIGHT_DISTANCE_DIFFERENCE_VALUE;
		break;
	case 's':
		light_distance -= LIGHT_DISTANCE_DIFFERENCE_VALUE;
		break;
	case ',':
		light_speed -= LIGHT_SPEED_VALUE;
		break;
	case '.':
		light_speed += LIGHT_SPEED_VALUE;
		break;
	case 'R':
		lighting = LIGHTING_DEFAULT;
		light_angle = LIGHT_ANGLE_DEFAULT;
		light_speed = LIGHT_SPEED_DEFAULT;
		light_distance = LIGHT_DISTANCE_DEFAULT;
		break;
	default:
		main->key_press(key, x, y);
		break;
	}
}

void Scene::draw_sphere() {
	glPushMatrix();
		glPushAttrib(GL_LINE_WIDTH);
		if (light_angle + light_speed > 360 || light_angle + light_speed < -360) {
			light_angle = 0;
		}
		else {
			light_angle += light_speed;
		}
		glRotatef(light_angle, 0, 1, 0);
		glPushMatrix();
			glTranslatef(0, 0, light_distance);
			init_light();
			if (lighting) {
				glLineWidth(4);
				glColor3f(1, 1, 0);
				glutWireSphere(8, 8, 8);
			}
		glPopMatrix();
		glPopAttrib();
	glPopMatrix();
}

void Scene::start() {
	if (initialized) {
		glutMainLoop();
	}
	else {
		cout << "Not initialized" << endl;
		return;
	}
}


//----------------------CALLBACK----------------------
void Scene::callback_display() {
	instance->display();
}

void Scene::callback_animation(int value) {
	instance->animation(value);
}