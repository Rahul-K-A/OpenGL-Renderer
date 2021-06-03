#pragma once
#include <GL\glew.h>
class Mesh
{
public:
	//Constructor
	Mesh();
	//Destructor
	~Mesh();
	//Creates mesh from vertices and indices
	void CreateMesh(GLfloat* Vertices, unsigned int* Indices, unsigned int NumberOfVertices, unsigned int NumberOfIndices);
	//Renders the created mesh
	void RenderMesh();
	//Removes the mesh
	void RemoveMesh();
private:
	//VAO,VBO,IBO location ID
	GLuint Vao, Vbo, Ibo;
	//Index count which stores number of indices
	GLsizei IndexCount;
};