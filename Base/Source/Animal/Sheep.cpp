#include "Sheep.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

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


}

void Sheep::SetTarget(const Vector3& target)
{

}

void Sheep::SetUp(const Vector3& up)
{

}

void Sheep::SetBoundary(Vector3 max, Vector3 min)
{

}

void Sheep::SetTerrain(GroundEntity* m_pTerrain)
{

}

Vector3 Sheep::GetPos(void) const
{

}

Vector3 Sheep::GetTarget(void) const
{

}

Vector3 Sheep::GetUp(void) const
{

}

GroundEntity* Sheep::GetTerrain(void)
{

}

void Sheep::Update(double dt)
{

}

void Sheep::Constrain(void)
{

}

void Sheep::Render(void)
{

}