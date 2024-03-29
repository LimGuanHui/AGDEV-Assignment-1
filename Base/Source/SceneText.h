#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include "Enemy\Enemy.h"
#include "Animal\Sheep.h"
#include "Animal\Pig.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public Scene
{	
public:
    SceneText(void);
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void ShaderInit();
	void LightInit();
	void MeshInit();
	void EntityInit();

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[3];
	Light* lights[2];

	GenericEntity* theCube;
    CEnemy* theEnemy;
	Sheep* sheep;
    Pig* pig;

	static SceneText* sInstance; // The pointer to the object that gets registered

	float halfWindowWidth;
	float halfWindowHeight;

    //Terrain
    std::vector<unsigned char> m_heightMap;
};

#endif