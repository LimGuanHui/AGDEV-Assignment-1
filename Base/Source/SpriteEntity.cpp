#include "SpriteEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

SpriteEntity::SpriteEntity(Mesh* _modelMesh) :
modelMesh(_modelMesh),
position(0.0f, 0.0f, 0.0f),
rotate(0.0f, 0.0f, 0.0f),
scale(1.0f, 1.0f, 1.0f),
mode(MODE_2D)
{
}

SpriteEntity::~SpriteEntity()
{
}

void SpriteEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void SpriteEntity::Render()
{
	if (mode == MODE_2D)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotate.x, 1, 0, 0);
	modelStack.Rotate(rotate.y, 0, 1, 0);
	modelStack.Rotate(rotate.z, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void SpriteEntity::RenderUI()
{
	if (mode == MODE_3D)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotate.x, 1, 0, 0);
	modelStack.Rotate(rotate.y, 0, 1, 0);
	modelStack.Rotate(rotate.z, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

SpriteEntity* Create::Sprite2DObject(const std::string& _meshName, const Vector3& _position, const Vector3& _rotate, const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	SpriteEntity* result = new SpriteEntity(modelMesh);
	result->SetPosition(_position);
	result->SetRotation(_rotate);
	result->SetScale(_scale);
	result->SetTextRenderMode(SpriteEntity::MODE_2D);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}

SpriteEntity* Create::Sprite3DObject(const std::string& _meshName, const Vector3& _position, const Vector3& _rotate, const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	SpriteEntity* result = new SpriteEntity(modelMesh);
	result->SetPosition(_position);
	result->SetRotation(_rotate);
	result->SetScale(_scale);
	result->SetTextRenderMode(SpriteEntity::MODE_3D);
	EntityManager::GetInstance()->AddEntity(result);
	return result;
}