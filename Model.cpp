#include "Model.h"

Model::Model()
{
	std::cout<<"MeshCreated\n";
}

void Model::LoadModel(const std::string& FileName)
{
	Assimp::Importer AImporter = Assimp::Importer();
	const aiScene* Scene = AImporter.ReadFile(FileName, aiProcess_Triangulate| aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices|aiProcess_FlipUVs);
	if (!Scene)
	{
		std::cout << "Model " << FileName << " Failed to load : " << AImporter.GetErrorString() << std::endl;
		return;
	}
	LoadNode(Scene->mRootNode, Scene);
	LoadMaterials(Scene);
}

void Model::RenderModel()
{
	for (size_t i = 0; i < MeshList.size(); i++)
	{
		unsigned int MaterialIndex = MeshToTex[i];
		if (MaterialIndex < TextureList.size() && (TextureList[MaterialIndex]!=nullptr))
		{
			TextureList[MaterialIndex]->UseTexture();
		}
		else {
			std::cout << "none\n";
		}
		MeshList[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < MeshList.size(); i++)
	{
		if (MeshList[i]!=nullptr)
		{
			delete MeshList[i];
			MeshList[i] =nullptr;
		}
		if (TextureList[i]!=nullptr)
		{
			delete TextureList[i];
			TextureList[i] = nullptr;
		}
	}
}

Model::~Model()
{
	for (Mesh* i :MeshList)
		delete i;
	for (Texture* i : TextureList)
		delete i;
	MeshList.clear();
	TextureList.clear();
	MeshToTex.clear();
}

void Model::LoadNode(aiNode* NodeToLoad, const aiScene* SceneToLoad)
{
	for (size_t i = 0; i < NodeToLoad->mNumMeshes; i++)
	{
		LoadMesh(SceneToLoad->mMeshes[NodeToLoad->mMeshes[i]], SceneToLoad);
	}

	for (size_t i = 0; i < NodeToLoad->mNumChildren; i++)
	{
		LoadNode(NodeToLoad->mChildren[i], SceneToLoad);
	}
}

void Model::LoadMesh(aiMesh* MeshToLoad, const aiScene* Scene)
{
	std::vector<GLfloat> Vertices;
	std::vector<GLuint> Indices;
	for (size_t i = 0; i < MeshToLoad->mNumVertices; i++)
	{
		Vertices.push_back(MeshToLoad->mVertices[i].x);
		Vertices.push_back(MeshToLoad->mVertices[i].y);
		Vertices.push_back(MeshToLoad->mVertices[i].z);
		if (MeshToLoad->mTextureCoords[0])
		{
			Vertices.push_back(MeshToLoad->mTextureCoords[0][i].x);
			Vertices.push_back(MeshToLoad->mTextureCoords[0][i].y);
		}
		else
		{
			Vertices.push_back(0.f);
			Vertices.push_back(0.f);
		}
		Vertices.push_back(-MeshToLoad->mNormals[i].x);
		Vertices.push_back(-MeshToLoad->mNormals[i].y);
		Vertices.push_back(-MeshToLoad->mNormals[i].z);
	}

	for (size_t i = 0; i < MeshToLoad->mNumFaces; i++)
	{
		aiFace Face = MeshToLoad->mFaces[i];
		for (size_t j = 0; j < Face.mNumIndices; j++)
		{
			Indices.push_back(Face.mIndices[j]);
		}
	}
	Mesh* NewMesh = new Mesh();
	NewMesh->CreateMesh(&Vertices.front(), &Indices.front(), Vertices.size(), Indices.size());
	MeshList.push_back(NewMesh);
	MeshToTex.push_back(MeshToLoad->mMaterialIndex);

}

void Model::LoadMaterials(const aiScene* Scene)
{
	TextureList.resize(Scene->mNumMaterials);
	for (size_t i = 0; i < Scene->mNumMaterials; i++)
	{
		aiMaterial* Material = Scene->mMaterials[i];
		TextureList[i] = nullptr;
		if (Material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString StringPath;
			if (Material->GetTexture(aiTextureType_DIFFUSE, 0, &StringPath) == AI_SUCCESS)
			{
				int Index = std::string(StringPath.data).rfind("\\");
				//std::cout << Index << std::endl;
				std::string FileName = std::string(StringPath.data).substr(Index + 1);
				std::string TexturePath = std::string("Textures/")+ FileName;
				std::cout << "Tex path is" << TexturePath<< std::endl;
				TextureList[i] = new Texture((char*)TexturePath.c_str());
				if (!TextureList[i]->LoadTexture())
				{
					std::cout << "Failed to load texture at " << TexturePath << std::endl;
					delete TextureList[i];
					//TextureList[i] = new Texture((char*)"Textures/plain.png");
					TextureList[i]->LoadTexture();
				}
			}
		}
		if (!TextureList[i])
		{
			TextureList[i] = new Texture((char*)"Textures/plain.png");
		}
	}
}
