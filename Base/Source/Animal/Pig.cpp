#include "Pig.h"

#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../SceneGraph/SceneGraph.h"

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
    delete updateTrans;
}

void Pig::Init(Vector3 pos)
{
    //mesh

	SetPosition(pos);

    Body = Create::Entity("Pig_Body");
    Body_Node = CSceneGraph::GetInstance()->AddNode(Body);

    leg1 = Create::Entity("Pig_LeftLeg_Front");
	CSceneNode* leg1_Node = Body_Node->AddChild(leg1);
    leg2 = Create::Entity("Pig_RightLeg_Front");
	CSceneNode* leg2_Node = Body_Node->AddChild(leg2);
    leg3 = Create::Entity("Pig_LeftLeg_Back");
	CSceneNode* leg3_Node = Body_Node->AddChild(leg3);
    leg4 = Create::Entity("Pig_RightLeg_Back");
	CSceneNode* leg4_Node = Body_Node->AddChild(leg4);

	Head = Create::Entity("Pig_Head");
	CSceneNode* Head_Node = Body_Node->AddChild(Head);
	Head_Node->ApplyTranslate(0.f, 2.32f * scale.y, 2.04f * scale.z);
    
	Nose = Create::Entity("Pig_Nose");
	CSceneNode* Nose_Node = Body_Node->AddChild(Nose);
	Nose_Node->ApplyTranslate(0.f, 2.07f * scale.y, 2.59f * scale.z);
    //CSceneNode* BodyNode = CSceneGraph::GetInstance()->AddNode(Body);
    //Body->SetCollider(true);
    //Body->SetAABB(Vector3(1.434, 1.434, 1.29), Vector3(-1.434, -1.434, -1.29));
    /*pig head
        translate(0, 2.32, 2.04)

        pig nose
        translate(0, 2.07, 2.59)*/
    SetScale(Vector3(1, 1, 1));

    updateTrans = new CUpdateTransformation();
    
    Body_Node->SetUpdateTransformation(updateTrans);
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
    //updateTrans->Reset();
    updateTrans->ApplyUpdate(position.x,position.y,position.z);
    //Body_Node->SetUpdateTransformation(updateTrans);
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
        modelStack.PushMatrix();
            modelStack.Translate(0, 2.32 * scale.y, 2.04 * scale.z);
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Pig_Head"));
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        modelStack.Translate(0, 2.07 * scale.y, 2.59 * scale.z);
            RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Pig_Nose"));
        modelStack.PopMatrix();

    modelStack.PopMatrix();
}