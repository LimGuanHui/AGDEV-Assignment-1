#include "Crop.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "SceneGraph/SceneGraph.h"

#include "MeshBuilder.h"
#include "LoadTGA.h"
Crop::Crop()
{
}

Crop::~Crop()
{
}

void Crop::Init(GroundEntity* a_b_c_groundEntity)
{
    MeshBuilder::GetInstance()->GenerateCube("Corn_plant", Color(1.0f, 0.f, 0.f), 1.5f);
    MeshBuilder::GetInstance()->GenerateCube("Corn", Color(0.f, 1.f, 0.f), 1.f);

    Vector3 startpos(30, 0, 100);
    for (int x = 0; x < 3; ++x)
    {
        for (int z = 0; z < 3; z++)
        {
            Vector3 temp = startpos + Vector3(x * 6.f, 0, z * 6.f);
            temp.y = a_b_c_groundEntity->GetTerrainHeight(temp);
            GenericEntity* plant = Create::Entity("Corn_plant", temp);
			plant->SetCollider(true);
			plant->SetAABB(Vector3(1.5f, 1.5f, 1.5f), Vector3(-1.5f, -1.5f, -1.5f));
			CSceneNode* plant_node = CSceneGraph::GetInstance()->AddNode(plant);
            //plant->SetPosition(temp);
			GenericEntity* corn = Create::Entity("Corn", Vector3(temp.x, temp.y + 1.f, temp.z));
			CSceneNode* corn_node = CSceneGraph::GetInstance()->AddNode(corn);
            //corn->SetPosition(Vector3(temp.x, temp.y + 1.f, temp.z));
        }
    }
}