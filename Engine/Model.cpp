#include "Model.h"
#include "Mesh.h"
#include "ResourceSystem.h"


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
	Mesh* mesh = resourceSys->AddMesh("../media/ninja.b3d");
	mesh->GenerateNormals();
	mesh->CalcBounds();
	//AddStaticModel(model);
}

