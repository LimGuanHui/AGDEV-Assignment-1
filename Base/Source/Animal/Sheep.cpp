#include "Sheep.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../SceneGraph/SceneGraph.h"
#include "../SceneGraph/SceneNode.h"

Sheep::Sheep() : GenericEntity(NULL)
, target(Vector3(0.0f, 0.0f, 0.0f))
, up(Vector3(0.0f, 0.0f, 0.0f))
, maxBoundary(Vector3(10.0f, 10.0f, 10.0f))
, minBoundary(Vector3(-10.0f, -10.0f, -10.0f))
, m_pTerrain(NULL)
{
}

Sheep::~Sheep()
{
}

void Sheep::Init(Vector3 pos)
{
    SetPosition(pos);

	Body = Create::Entity("Sheep_Body");
	CSceneNode* BodyNode = CSceneGraph::GetInstance()->AddNode(Body);
	Body->SetCollider(true);
	Body->SetAABB(Vector3(1.434, 1.434, 1.29), Vector3(-1.434, -1.434, -1.29));
}

void Sheep::SetTarget(const Vector3& target)
{
	this->target = target;
}

void Sheep::SetUp(const Vector3& up)
{
	this->up = up;
}

void Sheep::SetBoundary(Vector3 max, Vector3 min)
{

}

void Sheep::SetTerrain(GroundEntity* m_pTerrain)
{
	this->m_pTerrain = m_pTerrain;
}

Vector3 Sheep::GetPos(void) const
{
	return position;
}

Vector3 Sheep::GetTarget(void) const
{
	return target;
}

Vector3 Sheep::GetUp(void) const
{
	return up;
}

GroundEntity* Sheep::GetTerrain(void)
{
	return m_pTerrain;
}

void Sheep::Update(double dt)
{

}

void Sheep::Constrain(void)
{

}

void Sheep::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel << endl;
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}