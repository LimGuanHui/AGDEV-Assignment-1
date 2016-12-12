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
	magRounds = 6;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 6;
	// The current total number of rounds currently carried by this player
	totalRounds = 12;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 12;

	// The time between shots
	timeBetweenShots = 0.1;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Discharge this weapon
void Bow::Discharge(float force, Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			CProjectile* aProjectile = Create::arrow("Arrow", position,	(target - position).Normalized(), 7.0f, 8.0f * force, _source);
			aProjectile->SetCollider(true);
			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
	}
}