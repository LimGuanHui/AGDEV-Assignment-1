#include "Pig.h"

#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../SceneGraph/SceneGraph.h"
#include "../SceneGraph/SceneNode.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
Pig::Pig() : GenericEntity(NULL)
, target(Vector3(0.0f, 0.0f, 0.0f))
, up(Vector3(0.0f, 0.0f, 0.0f))
, maxBoundary(Vector3(10.0f, 10.0f, 10.0f))
, minBoundary(Vector3(-10.0f, -10.0f, -10.0f))
, m_pTerrain(NULL)
{
}

Pig::~Pig()
{
}

void Pig::Init(Vector3 pos)
{
    //mesh
    // Pig
    MeshBuilder::GetInstance()->GenerateOBJ("Pig_Body", "OBJ//Pig_Body.obj");
    MeshBuilder::GetInstance()->GetMesh("Pig_Body")->textureID = LoadTGA("Image//Pig_Body.tga");

    MeshBuilder::GetInstance()->GenerateOBJ("Pig_Head", "OBJ//Pig_Head.obj");
    MeshBuilder::GetInstance()->GetMesh("Pig_Head")->textureID = LoadTGA("Image//Pig_Head.tga");

    MeshBuilder::GetInstance()->GenerateOBJ("Pig_Nose", "OBJ//Pig_Nose.obj");
    MeshBuilder::GetInstance()->GetMesh("Pig_Nose")->textureID = LoadTGA("Image//Pig_Nose.tga");
    //leg
    MeshBuilder::GetInstance()->GenerateOBJ("Pig_LeftLeg_Front", "OBJ//Pig_LeftLeg_Front.obj");
    MeshBuilder::GetInstance()->GetMesh("Pig_LeftLeg_Front")->textureID = LoadTGA("Image//Pig_Leg.tga");

    MeshBuilder::GetInstance()->GenerateOBJ("Pig_RightLeg_Front", "OBJ//Pig_RightLeg_Front.obj");
    MeshBuilder::GetInstance()->GetMesh("Pig_RightLeg_Front")->textureID = LoadTGA("Image//Pig_Leg.tga");

    MeshBuilder::GetInstance()->GenerateOBJ("Pig_LeftLeg_Back", "OBJ//Pig_LeftLeg_Back.obj");
    MeshBuilder::GetInstance()->GetMesh("Pig_LeftLeg_Back")->textureID = LoadTGA("Image//Pig_Leg.tga");

    MeshBuilder::GetInstance()->GenerateOBJ("Pig_RightLeg_Front", "OBJ//Pig_RightLeg_Back.obj");
    MeshBuilder::GetInstance()->GetMesh("Pig_RightLeg_Front")->textureID = LoadTGA("Image//Pig_Leg.tga");


    SetPosition(pos);

    Body = Create::Entity("Pig");
    leg1 = Create::Entity("Pig_LeftLeg_Front");
    leg2 = Create::Entity("Pig_RightLeg_Front");
    leg3 = Create::Entity("Pig_LeftLeg_Back");
    leg4 = Create::Entity("Pig_RightLeg_Front");
    //CSceneNode* BodyNode = CSceneGraph::GetInstance()->AddNode(Body);
    //Body->SetCollider(true);
    //Body->SetAABB(Vector3(1.434, 1.434, 1.29), Vector3(-1.434, -1.434, -1.29));
}

void Pig::SetTarget(const Vector3& target)
{
    this->target = target;
}

void Pig::SetUp(const Vector3& up)
{
    this->up = up;
}

void Pig::SetBoundary(Vector3 max, Vector3 min)
{

}

void Pig::SetTerrain(GroundEntity* m_pTerrain)
{
    this->m_pTerrain = m_pTerrain;
}

Vector3 Pig::GetPos(void) const
{
    return position;
}

Vector3 Pig::GetTarget(void) const
{
    return target;
}

Vector3 Pig::GetUp(void) const
{
    return up;
}

GroundEntity* Pig::GetTerrain(void)
{
    return m_pTerrain;
}

void Pig::Update(double dt)
{

}

void Pig::Constrain(void)
{

}

void Pig::Render(void)
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Scale(scale.x, scale.y, scale.z);
    //if (GetLODStatus() == true)
    //{
    //    if (theDetailLevel != NO_DETAILS)
    //    {
    //        //cout << theDetailLevel << endl;
    //        RenderHelper::RenderMesh(GetLODMesh());
    //    }
    //}
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Pig_LeftLeg_Front"));
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Pig_RightLeg_Front"));
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Pig_LeftLeg_Back"));
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Pig_RightLeg_Front"));
    modelStack.PopMatrix();
}