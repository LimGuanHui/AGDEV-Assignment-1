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

    Vector3 startpos(20, 0, 20);
    for (int x = 0; x < 3; ++x)
    {
        for (int z = 0; z < 3; z++)
        {
            Vector3 temp = startpos + Vector3(x * 6.f, 0, z * 6.f);
            temp.y = a_b_c_groundEntity->GetTerrainHeight(temp);
            GenericEntity* plant = Create::Entity("Corn_plant");
            plant->SetPosition(temp);
            GenericEntity* corn = Create::Entity("Corn");
            corn->SetPosition(Vector3(temp.x, temp.y + 1.f, temp.z));
        }
    }
}