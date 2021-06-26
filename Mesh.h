#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>
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
	//Calculates average normals.Average normals mean that for a given face, the normals of all points in that face point in the same direction
	void CalculateAverageNormals(unsigned int* Indices, unsigned int IndiceCount, GLfloat* Vertices, unsigned int VerticeCount, unsigned int vLength, unsigned int normalOffset);
};