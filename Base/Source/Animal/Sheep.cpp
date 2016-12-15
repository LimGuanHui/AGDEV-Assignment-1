#include "Sheep.h"

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

void Sheep::Init()
{

}

void SetTarget(const Vector3& target)
{

}

void SetUp(const Vector3& up)
{

}

void SetBoundary(Vector3 max, Vector3 min)
{

}

void SetTerrain(GroundEntity* m_pTerrain)
{

}

Vector3 GetPos(void) const;
Vector3 GetTarget(void) const;
Vector3 GetUp(void) const;
GroundEntity* GetTerrain(void);

void Update(double dt = 0.0333f);

void Constrain(void);
void Render(void);