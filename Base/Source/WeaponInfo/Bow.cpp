#include "Bow.h"
#include "../Projectile/Arrow.h"
#include "MeshBuilder.h"
#include "../EntityManager.h"

Bow::Bow()
{
}


Bow::~Bow()
{
}

// Initialise this instance to default values
void Bow::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 5;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 5;
	// The current total number of rounds currently carried by this player
	totalRounds = 10;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 10;

	// The time between shots
	timeBetweenShots = 0.1667;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Discharge this weapon
void Bow::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			Vector3 _direction = (target - position).Normalized();
			// Create an arrow with a arrow mesh. The length is 10.0f, mesh is also set at 10.0f
			Arrow* sArrow = Create::arrow("laser", position, _direction, 10.0f, 2.0f, 100.0f, _source);
			sArrow->SetIsLaser(true);
			sArrow->SetCollider(true);
			//aLaser->SetCollider(true);
			//aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
	}
}
