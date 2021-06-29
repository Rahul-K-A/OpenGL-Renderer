#include "Mesh.h"

//Constructor
Mesh::Mesh()
	:Vao{ 0 },
	Vbo{ 0 },
	Ibo{ 0 },
	IndexCount{ 0 }{}

//Creates Mesh from indices and vertices
void Mesh::CreateMesh(GLfloat* Vertices, unsigned int* Indices, unsigned int NumberOfVertices, unsigned int NumberOfIndices)
{
	//CalculateAverageNormals(Indices, NumberOfIndices, Vertices, NumberOfVertices, 8, 5);
	IndexCount = NumberOfIndices;
	glGenVertexArrays(1, &Vao);
	//Links Vao to OpenGL state machine as the vertex array object
	glBindVertexArray(Vao);

	glGenBuffers(1, &Ibo);
	//GL_ELEMENT_ARRAY_BUFFER is used to indicate the buffer you're presenting contains
	//the indices of each element in the buffer tagged asGL_ARRAY_BUFFER
	//Links Ibo to the OpenGL state machine.Ibo is directly linked to Vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * NumberOfIndices, Indices, GL_STATIC_DRAW);

	glGenBuffers(1, &Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	//Links Vbo to OpenGL State machine
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * NumberOfVertices, Vertices, GL_STATIC_DRAW);

	//Set mesh data

	//For attrib Pointer, 0 in the first value of Vbo,
	//5th param is the start position od the first vertex
	//2cd param is number of data points to be read consecutively
	//5th param is the offset to the address of the first data point.By default 0
	//4th param is the offset between sets of data points
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 8, 0);
	//glEnableVertexAttribArray enables the generic vertex attribute array specified by index. 0 is the default index but you can have many
	glEnableVertexAttribArray(0);

	//Set texture data
	//Texture only needs UV data to wrap the image around the mesh so 2cd param is 2
	//first UV value
	//We set the texture info in index 1 of the attribute array
	//Since we need the 4th and 5th points in a index of vertices the 5th param offset is Vectrices[0]*3
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 8, (void*)(sizeof(Vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 8, (void*)(sizeof(Vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	//Unlinks Vbo from OpenGL state machine
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Unlinks Vao from state machine
	glBindVertexArray(0);
	//Unlinks Ibo from state machine
	//binding of an IBO is effectively an action recorded to Vao
	//When you bind an IBO, you are saying "bind this IBO to this VAO", but if you then unbind it,
	//it tells the VAO "unbind this IBO from this VAO" which is undesireable
	//Thus we first unbind Vao so that the OpenGl state machine will not remove Ibo from Vao
	//Then we unbind Ibo so that it gets unlinked from the rest of the state machine (But is still stored in Vao)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Renderes the mesh when the window is open
void Mesh::RenderMesh() {
	glBindVertexArray(Vao);
	//Explicitly binding Ibo because some GPU have bug which require you to explicitly bind Ibo to Vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibo);
	//Params 1)How to consider vertices as,2) Number of elements to be rendered,3)Data type of index (byte-wise or int-wise) 4)Address of indices.0 because indices were already bound in CreateTr
	glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);
	//Unbind Ibo and Vao to prevent accidental data modification
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//Removes the mesh and resets all variables
void Mesh::RemoveMesh()
{
	if (Vao != 0)
	{
		glDeleteVertexArrays(1, &Vao);
		Vao = 0;
	}
	if (Vbo != 0)
	{
		glDeleteBuffers(1, &Vbo);
		Vbo = 0;
	}
	if (Ibo != 0)
	{
		glDeleteBuffers(1, &Ibo);
		Ibo = 0;
	}
}

//void Mesh::CalculateAverageNormals(unsigned int* Indices, unsigned int IndiceCount, GLfloat* Vertices, unsigned int VerticeCount, unsigned int vLength, unsigned int normalOffset)
//{
//	//For every face, we calculate the normal and add it to the nx,ny,nz of Vertices[]
//   //We then normalize nx,ny,nz to get average normal
//
//	unsigned int in0;
//	unsigned int in1;
//	unsigned int in2;
//	for (size_t i = 0; i < IndiceCount; i += 3)
//	{
//		//vLength is the number of elements between the start of one vertex and the start of its subsequent vertex
//		//In our case, it is 8
//		in0 = Indices[i + 0] * vLength;
//		in1 = Indices[i + 1] * vLength;
//		in2 = Indices[i + 2] * vLength;
//
//		glm::vec3 v1(Vertices[in1] - Vertices[in0], Vertices[in1 + 1] - Vertices[in0 + 1], Vertices[in1 + 2] - Vertices[in0 + 2]);
//
//		glm::vec3 v2(Vertices[in2] - Vertices[in0], Vertices[in2 + 1] - Vertices[in0 + 1], Vertices[in2 + 2] - Vertices[in0 + 2]);
//
//		glm::vec3 Normalvec = glm::cross(v1, v2);
//
//		Normalvec = glm::normalize(Normalvec);
//
//		in0 += normalOffset;
//		in1 += normalOffset;
//		in2 += normalOffset;
//
//		Vertices[in0] += Normalvec.x;
//		Vertices[in0 + 1] += Normalvec.y;
//		Vertices[in0 + 2] += Normalvec.z;
//
//		Vertices[in1] += Normalvec.x;
//		Vertices[in1 + 1] += Normalvec.y;
//		Vertices[in1 + 2] += Normalvec.z;
//
//		Vertices[in2] += Normalvec.x;
//		Vertices[in2 + 1] += Normalvec.y;
//		Vertices[in2 + 2] += Normalvec.z;
//	}
//
//	for (size_t i = 0; i < VerticeCount / vLength; i++)
//	{
//		unsigned int nOffset = i * vLength + normalOffset;
//		glm::vec3 vec(Vertices[nOffset], Vertices[nOffset + 1], Vertices[nOffset + 2]);
//
//		vec = glm::normalize(vec);
//
//		Vertices[nOffset] = vec.x;
//		Vertices[nOffset + 1] = vec.y;
//		Vertices[nOffset + 2] = vec.z;
//	}
//}

//Destructor
Mesh::~Mesh()
{
	RemoveMesh();
}