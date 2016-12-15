#include "Arrow.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GL\glew.h"
#include "../PlayerInfo/PlayerInfo.h"
#include "MyMath.h"
#include "../SpatialPartition/SpatialPartition.h"
#include "../SceneGraph/SceneGraph.h"

#include <iostream>
using namespace std;

#define Y_OFFSET 0.1f

Arrow::Arrow(void) : CProjectile(NULL), m_fGravity(-10.0f), m_fElapsedTime(0.0f), m_pTerrain(NULL)
{
}

Arrow::Arrow(Mesh* _modelMesh) : CProjectile(_modelMesh), m_fGravity(-10.0f), m_fElapsedTime(0.0f), m_pTerrain(NULL)
{
}

Arrow::~Arrow(void)
{
	m_pTerrain = NULL; // Don't delete this as the terrain is deleted in CPlayerInfo
	modelMesh = NULL;
	theSource = NULL;
}

// Update the status of this projectile
void Arrow::Update(double dt)
{
	if (m_bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	m_fLifetime -= (float)dt;
	if (m_fLifetime < 0.0f)
	{
		SetStatus(false);
		SetIsDone(true);	// This method informs EntityManager to remove this instance
		return;
	}

	// Check if the arrow is already on the ground
	if (position.y >= m_pTerrain->GetTerrainHeight(position) + Math::EPSILON + Y_OFFSET)
	{
		// Update Position
		m_fElapsedTime += dt;

		position.x += (float)(theDirection.x * m_fElapsedTime * m_fSpeed);
		position.y += (float)(theDirection.y * m_fElapsedTime * m_fSpeed) + (0.5 * m_fGravity * m_fElapsedTime * m_fElapsedTime);
		position.z += (float)(theDirection.z * m_fElapsedTime * m_fSpeed);

		if (position.y < m_pTerrain->GetTerrainHeight(position))
		{
			position.y = m_pTerrain->GetTerrainHeight(position) + Y_OFFSET;
			m_fSpeed = 0.0f;
			return;
		}
	}
}

// Set the terrain for the player info
void Arrow::SetTerrain(GroundEntity* m_pTerrain)
{
	this->m_pTerrain = m_pTerrain;
}


float Arrow::CalculateYrotation()
{
#define CAM_TARGET theSource->GetTarget()
#define CAM_POS position
    
    return Math::RadianToDegree(atan2(theDirection.x, theDirection.z));
}

float Arrow::CalculateXrotation()
{
    return 0.f;//Math::RadianToDegree(atan2(theDirection.y,0 ));
}

// Create a projectile and add it into EntityManager
Arrow* Create::arrow(const std::string& _meshName,
				     const Vector3& _position, 
					 const Vector3& _direction, 
					 const float m_fLifetime, 
					 float m_fSpeed,                      
                     CPlayerInfo* _source)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	Arrow* result = new Arrow(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetSource(_source);
	result->SetTerrain(_source->GetTerrain());
    
    result->SetRotation(Vector3(0, result->CalculateYrotation(), result->CalculateXrotation()));
	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}