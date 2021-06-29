#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include "Mesh.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model
{
public:
	Model();

	void LoadModel(const std::string& FileName);
	void RenderModel();
	void ClearModel();
	~Model();
private:
	void LoadNode(aiNode* NodeToLoad, const aiScene* SceneToLoad);
	void LoadMesh(aiMesh* Mesh, const aiScene* Scene);
	void LoadMaterials(const aiScene* Scene);
	std::vector<Mesh*> MeshList;
	std::vector<Texture*> TextureList;
	std::vector <unsigned int> MeshToTex;

};

