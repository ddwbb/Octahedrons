#include "Scene.h"



Scene::Scene(list<DATA *> * _list) {
	data_list = _list;
}


Scene::~Scene()
{
}

void Scene::view_list() {
	while (true) {
		DATA * data;
		cout << "START SCANING" << endl;
		for (list<DATA *>::iterator it = data_list->begin(); it != data_list->end(); it++) {
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
		Sleep(2000);
	}
}