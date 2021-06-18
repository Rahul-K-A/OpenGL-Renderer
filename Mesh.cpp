#include "Mesh.h"

//Constructor
Mesh::Mesh()
    :Vao{ 0 }, Vbo{ 0 }, Ibo{ 0 }, IndexCount{ 0 }
{

}

//Creates Mesh from indices and vertices
void Mesh::CreateMesh(GLfloat* Vertices, unsigned int* Indices, unsigned int NumberOfVertices, unsigned int NumberOfIndices)
{
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,sizeof(Vertices[0])*5,0);
    //glEnableVertexAttribArray enables the generic vertex attribute array specified by index. 0 is the default index but you can have many
    glEnableVertexAttribArray(0);


    //Set texture data
    //Texture only needs UV data to wrap the image around the mesh so 2cd param is 2
    //first UV value
    //We set the texture info in index 1 of the attribute array
    //Since we need the 4th and 5th points in a index of vertices the 5th param offset is Vectrices[0]*3
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 5, (void*)(sizeof(Vertices[0]) * 3));
    glEnableVertexAttribArray(1);

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
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
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


//Destructor
Mesh::~Mesh()
{
    RemoveMesh();
}
