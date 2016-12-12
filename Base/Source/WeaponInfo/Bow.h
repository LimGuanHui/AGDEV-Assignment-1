#ifndef BOW_H
#define BOW_H

#include "WeaponInfo.h"

class Bow :	public CWeaponInfo
{
public:
	Bow();
	virtual ~Bow();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(float force, 
				   Vector3 position, 
				   Vector3 target, 
				   CPlayerInfo* _source = NULL);
};


#endif