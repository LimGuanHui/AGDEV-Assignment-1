#ifndef BOW_H
#define BOW_H

#include "WeaponInfo.h"
#include "../FPSCamera.h"
class Bow :	public CWeaponInfo
{
public:
    Bow(FPSCamera* _cameraPtr);
	virtual ~Bow();

	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void Discharge(float force, 
				   Vector3 position, 
				   Vector3 target, 
				   CPlayerInfo* _source = NULL);
    FPSCamera* camera;
};


#endif