#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetRotation(const Vector3& _value){ rotate = _value; };
	inline Vector3 GetRotation() { return rotate; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	inline void SetHP(const  int& _value) { HP = _value; };
	inline void ModifyHP(const  int& _value) { HP += _value; };
	inline int GetHP(){ return HP; };

	bool IsDone();
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);
	// Set the flag, bLaser
	virtual void SetIsLaser(const bool bLaser);
	// Get the flag, bLaser
	virtual bool GetIsLaser(void) const;

protected:
	Vector3 position;
	Vector3 rotate;
	Vector3 scale;

	bool isDone;
	bool m_bCollider;
	bool bLaser;

	int HP;
};

#endif // ENTITY_BASE_H