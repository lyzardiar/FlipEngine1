#include "Model.h"
#include "Mesh.h"
#include "ResourceSystem.h"

Model::Model()
{

}

Model::~Model()
{

}

void Model::Init()
{

}

void Model::SetPosition(float x, float y, float z)
{
	_position.set(x, y, z);
	_drawSurf->matModel.buildTranslate(x, y, z);
}

vec3 Model::GetPosition()
{
	return _position;
}

void Model::SetFile( const char* filename )
{
	Mesh* mesh = resourceSys->AddMesh(filename);
	mesh->GenerateNormals();
	mesh->CalcBounds();
	//AddStaticModel(model);
	_drawSurf->geo = mesh->GetGeometries(0);
}

