#ifndef ARROW_H
#define ARROW_H

#include "Projectile.h"
#include "EntityBase.h"
#include "Vector3.h"
#include "Collider/Collider.h"
#include "../GroundEntity.h"

class Mesh;
class CPlayerInfo;

class Arrow : public CProjectile
{
protected:
	float m_fGravity;
	float m_fElapsedTime;
	GroundEntity* m_pTerrain;

public:
	Arrow(void);
	Arrow(Mesh* _modelMesh);
	~Arrow(void);
public:
	void SetLength(const float m_fLength);
	float GetLength(void) const;
	void CalculateAngles(void);

	// Update the status of this projectile
	void Update(double dt = 0.0333f);

	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);
};

namespace Create
{
	Arrow* arrow(const std::string& _meshName, 
			     const Vector3& _position, 
				 const Vector3& _direction, 
				 const float m_fLifetime, 
				 float m_fSpeed, 
				 CPlayerInfo* _source = NULL);
};

#endif