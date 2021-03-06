#pragma once
#include "Mesh.h"

struct RoomData{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
	//GLfloat s;
	//GLfloat t;
};

class Room :
	public Mesh
{
public:
	Room()
	{
		vao = 0;
		vertexbuffer = 0;
		indexbuffer = 0;
		vertexArray = NULL;
		indexArray = NULL;
		nVerts = 0;
		nTris = 0;
	};

	Room(glm::dvec3 _pos, glm::dvec3 _dim, glm::dvec3 _emission, glm::dvec3 _brdf, double _P);

	//TODO: do it properly
	double getArea() {return dim[0]*dim[1];}

	void createBuffers();
	~Room();

	void render();

private:

	texST* stArray;

	float dim[3];

};
