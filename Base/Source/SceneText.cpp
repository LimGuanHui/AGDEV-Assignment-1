#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"
#include "SceneEditor.h"
#include <iostream>
using namespace std;

SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("Start", this);
}

SceneText::~SceneText()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::ShaderInit()
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");

	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");


}

void SceneText::LightInit()
{
	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(0, 20, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	currProg->UpdateInt("numLights", 1);
	currProg->UpdateInt("textEnabled", 0);
}

void SceneText::MeshInit()
{
	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 1.f, 1.f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//SkyBox//skybox_front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//SkyBox//skybox_back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//SkyBox//skybox_left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//SkyBox//skybox_right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//SkyBox//skybox_top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//SkyBox//skybox_bottom.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 10.f);

	// Bow and Arrow
	MeshBuilder::GetInstance()->GenerateOBJ("Bow", "OBJ//Bow.obj");
	MeshBuilder::GetInstance()->GetMesh("Bow")->textureID = LoadTGA("Image//Bow.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Arrow", "OBJ//Arrow.obj");
	MeshBuilder::GetInstance()->GetMesh("Arrow")->textureID = LoadTGA("Image//Arrow.tga");

	// Rifle
	MeshBuilder::GetInstance()->GenerateOBJ("Rifle", "OBJ//Rifle.obj");
	MeshBuilder::GetInstance()->GetMesh("Rifle")->textureID = LoadTGA("Image//Rifle.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Bullet", "OBJ//Bullet.obj");
	MeshBuilder::GetInstance()->GetMesh("Bullet")->textureID = LoadTGA("Image//Bullet.tga");

	// Tree
	MeshBuilder::GetInstance()->GenerateOBJ("Tree", "OBJ//Tree.obj");
	MeshBuilder::GetInstance()->GetMesh("Tree")->textureID = LoadTGA("Image//Tree.tga");

	// Fence
	MeshBuilder::GetInstance()->GenerateOBJ("Fence_High", "OBJ//Fence_High.obj");
	MeshBuilder::GetInstance()->GetMesh("Fence_High")->textureID = LoadTGA("Image//Fence.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Fence_Medium", "OBJ//Fence_Medium.obj");
	MeshBuilder::GetInstance()->GetMesh("Fence_Medium")->textureID = LoadTGA("Image//Fence.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Fence_Low", "OBJ//Fence_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("Fence_Low")->textureID = LoadTGA("Image//Fence.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Well_High", "OBJ//Well//Well_High.obj");

	// Well
	MeshBuilder::GetInstance()->GetMesh("Well_High")->textureID = LoadTGA("Image//Well//Well.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Well_Medium", "OBJ//Well//Well_Medium.obj");
	MeshBuilder::GetInstance()->GetMesh("Well_Medium")->textureID = LoadTGA("Image//Well//Well.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Well_Low", "OBJ//Well//Well_Low.obj");
	MeshBuilder::GetInstance()->GetMesh("Well_Low")->textureID = LoadTGA("Image//Well//Well.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Well_Stand", "OBJ//Well//Well_Stand.obj");
	MeshBuilder::GetInstance()->GetMesh("Well_Stand")->textureID = LoadTGA("Image//Well//Well_Stand.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Well_Lever", "OBJ//Well//Well_Lever.obj");
	MeshBuilder::GetInstance()->GetMesh("Well_Lever")->textureID = LoadTGA("Image//Well//Well_Lever.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Well_Handle", "OBJ//Well//Well_Handle.obj");
	MeshBuilder::GetInstance()->GetMesh("Well_Handle")->textureID = LoadTGA("Image//Well//Well_Lever.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Rope_Coil", "OBJ//Well//Rope_Coil.obj");
	MeshBuilder::GetInstance()->GetMesh("Rope_Coil")->textureID = LoadTGA("Image//Well//Rope.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Rope_Hang", "OBJ//Well//Rope_Hang.obj");
	MeshBuilder::GetInstance()->GetMesh("Rope_Hang")->textureID = LoadTGA("Image//Well//Rope.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Bucket_Empty", "OBJ//Well//Bucket_Empty.obj");
	MeshBuilder::GetInstance()->GetMesh("Bucket_Empty")->textureID = LoadTGA("Image//Well//Bucket_Empty.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Bucket_Filled", "OBJ//Well//Bucket_Filled.obj");
	MeshBuilder::GetInstance()->GetMesh("Bucket_Filled")->textureID = LoadTGA("Image//Well//Bucket_Filled.tga");

	// Chicken
	MeshBuilder::GetInstance()->GenerateOBJ("Chicken_Body", "OBJ//Chicken_Body.obj");
	MeshBuilder::GetInstance()->GetMesh("Chicken_Body")->textureID = LoadTGA("Image//Chicken_Body.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Chicken_Head", "OBJ//Chicken_Head.obj");
	MeshBuilder::GetInstance()->GetMesh("Chicken_Head")->textureID = LoadTGA("Image//Chicken_Head.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Chicken_Beak", "OBJ//Chicken_Beak.obj");
	MeshBuilder::GetInstance()->GetMesh("Chicken_Beak")->textureID = LoadTGA("Image//Chicken_Beak.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Chicken_Chin", "OBJ//Chicken_Chin.obj");
	MeshBuilder::GetInstance()->GetMesh("Chicken_Chin")->textureID = LoadTGA("Image//Chicken_Chin.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Chicken_LeftLeg", "OBJ//Chicken_LeftLeg.obj");
	MeshBuilder::GetInstance()->GetMesh("Chicken_LeftLeg")->textureID = LoadTGA("Image//Chicken_Leg.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Chicken_RightLeg", "OBJ//Chicken_RightLeg.obj");
	MeshBuilder::GetInstance()->GetMesh("Chicken_RightLeg")->textureID = LoadTGA("Image//Chicken_Leg.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Chicken_LeftWing", "OBJ//Chicken_LeftWing.obj");
	MeshBuilder::GetInstance()->GetMesh("Chicken_LeftWing")->textureID = LoadTGA("Image//Chicken_Body.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Chicken_RightWing", "OBJ//Chicken_RightWing.obj");
	MeshBuilder::GetInstance()->GetMesh("Chicken_RightWing")->textureID = LoadTGA("Image//Chicken_Body.tga");

	

	// Sheep
	/*MeshBuilder::GetInstance()->GenerateOBJ("Sheep_Body", "OBJ//Sheep_Body.obj");
	MeshBuilder::GetInstance()->GetMesh("Sheep_Body")->textureID = LoadTGA("Image//Sheep_Body.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Sheep_Head", "OBJ//Sheep_Head.obj");
	MeshBuilder::GetInstance()->GetMesh("Sheep_Head")->textureID = LoadTGA("Image//Sheep_Body.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Sheep_Face", "OBJ//Sheep_Face.obj");
	MeshBuilder::GetInstance()->GetMesh("Sheep_Face")->textureID = LoadTGA("Image//Sheep_Face.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Sheep_Legs", "OBJ//Sheep_Legs.obj");
	MeshBuilder::GetInstance()->GetMesh("Sheep_Legs")->textureID = LoadTGA("Image//Sheep_Leg.tga");*/

	/*MeshBuilder::GetInstance()->GenerateOBJ("Sheep_LeftLeg_Front", "OBJ//Sheep_LeftLeg_Front.obj");
	MeshBuilder::GetInstance()->GetMesh("Sheep_LeftLeg_Front")->textureID = LoadTGA("Image//Sheep_Leg.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Sheep_RightLeg_Front", "OBJ//Sheep_RightLeg_Front.obj");
	MeshBuilder::GetInstance()->GetMesh("Sheep_RightLeg_Front")->textureID = LoadTGA("Image//Sheep_Leg.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Sheep_LeftLeg_Back", "OBJ//Sheep_LeftLeg_Back.obj");
	MeshBuilder::GetInstance()->GetMesh("Sheep_LeftLeg_Back")->textureID = LoadTGA("Image//Sheep_Leg.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Sheep_RightLeg_Front", "OBJ//Sheep_RightLeg_Back.obj");
	MeshBuilder::GetInstance()->GetMesh("Sheep_RightLeg_Front")->textureID = LoadTGA("Image//Sheep_Leg.tga");*/

	// Wolf
	MeshBuilder::GetInstance()->GenerateOBJ("Wolf_Body", "OBJ//Wolf_Body.obj");
	MeshBuilder::GetInstance()->GetMesh("Wolf_Body")->textureID = LoadTGA("Image//Wolf_Body.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Wolf_Head", "OBJ//Wolf_Head.obj");
	MeshBuilder::GetInstance()->GetMesh("Wolf_Head")->textureID = LoadTGA("Image//Wolf_Head.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Wolf_Ears", "OBJ//Wolf_Ears.obj");
	MeshBuilder::GetInstance()->GetMesh("Wolf_Ears")->textureID = LoadTGA("Image//Wolf_Body.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Wolf_Mouth", "OBJ//Wolf_Mouth.obj");
	MeshBuilder::GetInstance()->GetMesh("Wolf_Mouth")->textureID = LoadTGA("Image//Wolf_Mouth.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Wolf_LeftLeg_Front", "OBJ//Wolf_LeftLeg_Front.obj");
	MeshBuilder::GetInstance()->GetMesh("Wolf_LeftLeg_Front")->textureID = LoadTGA("Image//Wolf_Body.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Wolf_RightLeg_Front", "OBJ//Wolf_RightLeg_Front.obj");
	MeshBuilder::GetInstance()->GetMesh("Wolf_RightLeg_Front")->textureID = LoadTGA("Image//Wolf_Body.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Wolf_LeftLeg_Back", "OBJ//Wolf_LeftLeg_Back.obj");
	MeshBuilder::GetInstance()->GetMesh("Wolf_LeftLeg_Back")->textureID = LoadTGA("Image//Wolf_Body.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("Wolf_RightLeg_Front", "OBJ//Wolf_RightLeg_Back.obj");
	MeshBuilder::GetInstance()->GetMesh("Wolf_RightLeg_Front")->textureID = LoadTGA("Image//Wolf_Body.tga");

	//terrain
	MeshBuilder::GetInstance()->GenerateTerrain("TEST_TERRAIN", "Image//Terrain.raw", m_heightMap);
	MeshBuilder::GetInstance()->GetMesh("TEST_TERRAIN")->textureID = LoadTGA("Image//moss1.tga");
    MeshBuilder::GetInstance()->GenerateTerrain("FARMLAND_TERRAIN", "Image//Terrain//farmlands_heightmap.raw", m_heightMap);
    MeshBuilder::GetInstance()->GetMesh("FARMLAND_TERRAIN")->textureID = LoadTGA("Image//moss1.tga");

    //Windmill
    MeshBuilder::GetInstance()->GenerateOBJ("Windmill_Base", "OBJ//Windmill//windmill_high.obj");
    //MeshBuilder::GetInstance()->GetMesh("Windmill_Base")->textureID = LoadTGA("Image//Wolf_Body.tga");
    MeshBuilder::GetInstance()->GenerateOBJ("Windmill_Fan", "OBJ//Windmill//windmill_fan.obj");
	MeshBuilder::GetInstance()->GetMesh("Windmill_Fan")->textureID = LoadTGA("Image//Fence.tga");
}

void SceneText::EntityInit()
{
	// Create entities into the scene
	Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	GenericEntity* testCube = Create::Entity("cube");
	CSceneNode* testNode = CSceneGraph::GetInstance()->AddNode(testCube);
	testCube->SetCollider(true);
	testCube->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-1.f, -1.f, -1.f));
	testCube->SetHP(3);
	testCube->InitLOD("cube", "sphere", "cubeSG");
	testNode->ApplyTranslate(10, 0, 0);

	GenericEntity* testChildCube = Create::Entity("cube");
	CSceneNode* testChildNode = testNode->AddChild(testChildCube);
	testChildCube->SetCollider(true);
	testChildCube->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-1.f, -1.f, -1.f));
	testChildCube->SetHP(10);
	testChildNode->ApplyTranslate(0, 5, 0);

	//GenericEntity* baseCube = Create::Asset("sphere");
	//CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);

	//CUpdateTransformation* baseMtx = new CUpdateTransformation();
	////baseMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	////baseMtx->SetSteps(-60, 60);
	//baseNode->ApplyTranslate(0.0f, 0.0f, 0.0f);
	////baseNode->SetUpdateTransformation(baseMtx);

	//GenericEntity* childCube = Create::Asset("cubeSG");
	//CSceneNode* childNode = baseNode->AddChild(childCube);
	//childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);
	////childNode->ApplyRotate(90.f, 0.f, 0.f, 1.f);
	//CUpdateTransformation* rotateChild = new CUpdateTransformation();
	//rotateChild->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	//rotateChild->SetSteps(-90, 90);
	//childNode->SetUpdateTransformation(rotateChild);

	//GenericEntity* grandchildCube = Create::Asset("cube");
	//CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	//grandchildNode->ApplyTranslate(0.0f, 0.5f, 0.0f);
	////grandchildNode->ApplyRotate(-90.f, 0.f, 0.f, 1.f);
	//CUpdateTransformation* rotateGChild = new CUpdateTransformation();
	//rotateGChild->ApplyUpdate(1.0f, 0.0f, 1.0f, 0.0f);
	//rotateGChild->SetSteps(0, 360);
	//grandchildNode->SetUpdateTransformation(rotateGChild);

	//GenericEntity* Fence = Create::Entity("Fence_High", Vector3(0.f, 0.f, 15.f), Vector3(3.f, 3.f, 3.f));
	////GenericEntity* Fence = Create::Asset("Fence_High");
	////CSceneNode* FenceNode = CSceneGraph::GetInstance()->AddNode(Fence);
	//Fence->SetCollider(true);
	//Fence->SetAABB(Vector3(5.f, 5.f, 2.f), Vector3(-5.f, -5.f, -2.f));
	////FenceNode->ApplyTranslate(0, 0, 15);
	//Fence->InitLOD("Fence_High", "Fence_Medium", "Fence_Low");

    //windmill
	GenericEntity* Windmill_base = Create::Asset("Windmill_Base", Vector3(10.f, 0.f, 15.f), Vector3(3.f, 3.f, 3.f));
    CSceneNode* Windmill_base_node = CSceneGraph::GetInstance()->AddNode(Windmill_base);
    /*CUpdateTransformation* ROATE = new CUpdateTransformation();
    ROATE->ApplyUpdate(1.f, 0.f, 0.f, 1.f);
    ROATE->SetSteps(0, 360);
    Windmill_base_node->SetUpdateTransformation(ROATE);*/

    GenericEntity* Windmill_fan = Create::Asset("Windmill_Fan", Vector3(0.f, 0.f, 0.f), Vector3(3.f, 3.f, 3.f));
    CSceneNode* Windmill_fan_node = Windmill_base_node->AddChild(Windmill_fan);
	Windmill_fan_node->ApplyTranslate(10.f, 22.17f, 6.9f);
    CUpdateTransformation* rotate_windmill_fan = new CUpdateTransformation();
    rotate_windmill_fan->ApplyUpdate(1.f, 0.f, 0.f, 1.f);
    rotate_windmill_fan->SetSteps(0, 360);
    Windmill_fan_node->SetUpdateTransformation(rotate_windmill_fan);

	GenericEntity* Well_Main = Create::Asset("Well_High");
	CSceneNode* Well_Main_Node = CSceneGraph::GetInstance()->AddNode(Well_Main);
	Well_Main->InitLOD("Well_High", "Well_Medium", "Well_Low");
	Well_Main_Node->ApplyTranslate(0.f, 0.f, -15.f);

	GenericEntity* Well_Stand = Create::Asset("Well_Stand");
	CSceneNode* Well_Stand_Node = Well_Main_Node->AddChild(Well_Stand);

	GenericEntity* Well_Lever = Create::Asset("Well_Lever");
	CSceneNode* Well_Lever_Node = Well_Stand_Node->AddChild(Well_Lever);

	GenericEntity* Well_Handle = Create::Asset("Well_Handle");
	CSceneNode* Well_Handle_Node = Well_Lever_Node->AddChild(Well_Handle);

	GenericEntity* Rope_Coil = Create::Asset("Rope_Coil");
	CSceneNode* Rope_Coil_Node = Well_Lever_Node->AddChild(Rope_Coil);

	GenericEntity* Rope_Hang = Create::Asset("Rope_Hang");
	CSceneNode* Rope_Hang_Node = Well_Stand_Node->AddChild(Rope_Hang);

	GenericEntity* Bucket = Create::Asset("Bucket_Empty");
	CSceneNode* Bucket_Node = Rope_Hang_Node->AddChild(Bucket);
	
	//sheep = new Sheep();
	//sheep->Init((0,0,0));
	
	pig = new Pig();
	pig->Init(Vector3(0, -10, 0));

	//Create a CEnemyinstance
	theEnemy = new CEnemy();
	theEnemy->Init();

	groundEntity = Create::Ground("FARMLAND_TERRAIN", "FARMLAND_TERRAIN", m_heightMap);
	//	Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(85.0f, 45.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(2.25f, 2.0f, 2.0f));


	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
		"SKYBOX_LEFT", "SKYBOX_RIGHT",
		"SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -30, 0));
	groundEntity->SetScale(Vector3(1000.0f, 40.f, 1000.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);
	theEnemy->SetTerrain(groundEntity);

	//SceneEditor
	SceneEditor::GetInstance()->Init();
	SceneEditor::GetInstance()->AttachCamera(&camera);

	// Setup the 2D entities
	halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 3.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 3; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(0, 0 + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
	}
}

void SceneText::Init()
{	
	ShaderInit();
	LightInit();

	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
    camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp(), Vector3(Application::GetInstance().GetWindowWidth() / 2, Application::GetInstance().GetWindowHeight() / 2, 0));
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshInit();

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(40000.0f, 160000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	EntityInit();
}

void SceneText::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);

	// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
	/*if(KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if(KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);*/
	if(KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(KeyboardController::GetInstance()->IsKeyDown('5'))
	{
		lights[0]->type = Light::LIGHT_POINT;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('6'))
	{
		lights[0]->type = Light::LIGHT_DIRECTIONAL;
	}
	else if(KeyboardController::GetInstance()->IsKeyDown('7'))
	{
		lights[0]->type = Light::LIGHT_SPOT;
	}

	if(KeyboardController::GetInstance()->IsKeyDown('I'))
		lights[0]->position.z -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('K'))
		lights[0]->position.z += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('J'))
		lights[0]->position.x -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('L'))
		lights[0]->position.x += (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('O'))
		lights[0]->position.y -= (float)(10.f * dt);
	if(KeyboardController::GetInstance()->IsKeyDown('P'))
		lights[0]->position.y += (float)(10.f * dt);

	if (KeyboardController::GetInstance()->IsKeyReleased('M'))
	{
		CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
		Vector3 pos = theNode->GetEntity()->GetPosition();
		theNode->GetEntity()->SetPosition(Vector3(pos.x + 50.0f, pos.y, pos.z + 50.0f));
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('N'))
	{
		CSpatialPartition::GetInstance()->PrintSelf();
	}

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
	// <THERE>

	// Update the player position and other details based on keyboard and mouse inputs
	playerInfo->Update(dt);

	//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

	GraphicsManager::GetInstance()->UpdateLights(dt);

    //SceneEditor update
    SceneEditor::GetInstance()->Update();

	// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
	// Eg. FPSRenderEntity or inside RenderUI for LightEntity
	std::ostringstream ss;
	ss.precision(5);
	float fps = (float)(1.f / dt);
	ss << "FPS: " << fps;
	textObj[0]->SetText(ss.str());

	std::ostringstream ss1;
	ss1.precision(4);
	ss1 << "Player:" << playerInfo->GetPos();
	textObj[1]->SetText(ss1.str());
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EntityManager::GetInstance()->Render();
    //SceneEditor render
    SceneEditor::GetInstance()->Render();


	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, 170, 0, 90, -70, 140);
	GraphicsManager::GetInstance()->DetachCamera();
	CPlayerInfo::GetInstance()->Render("Rifle", "Bow");
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	delete lights[0];
	delete lights[1];
}
