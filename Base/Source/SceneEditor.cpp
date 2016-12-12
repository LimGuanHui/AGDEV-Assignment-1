#include "SceneEditor.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"
#include "./SpatialPartition/SpatialPartition.h"
#define TIME_BETWEEN_INPUT 0.3f
#define POINTER_DISTANCE 5.f
SceneEditor::SceneEditor()
{
}

SceneEditor::SceneEditor(FPSCamera* _cameraPtr)
{
    AttachCamera(_cameraPtr);
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::Init()
{
    point_mesh = MeshBuilder::GetInstance()->GenerateCube("Point", Color(1.0f, 0.0f, 0.0f), 1.0f);
    MeshBuilder::GetInstance()->GenerateCube("Pointer", Color(0.0f, 0.0f, 1.0f), 1.0f);
    inputDelay = 0;
    ShiftMode = Mode::Normal;
    pointer_position = Vector3(0, 0, 0);
    Rotation = Vector3(0, 0, 0);
    selectedEntity = NULL;
}
void SceneEditor::Update(double dt)
{
    if (inputDelay < TIME_BETWEEN_INPUT)
        inputDelay += dt;
    if (KeyboardController::GetInstance()->IsKeyDown(VK_TAB)
        && inputDelay > TIME_BETWEEN_INPUT)
    {
		inputDelay = 0;
        AddPoint(attachedCamera->GetCameraPos());
    }
    if (KeyboardController::GetInstance()->IsKeyDown(VK_CONTROL)
        && KeyboardController::GetInstance()->IsKeyDown('Z')
        && inputDelay > TIME_BETWEEN_INPUT)
    {
        inputDelay = 0;
        DeletePoint();
    }
    Shift_Mode();
    CalculatePositionOfPointer(POINTER_DISTANCE);
}
void SceneEditor::Render()
{
    MS& ms = GraphicsManager::GetInstance()->GetModelStack();
    
    vector<Vector3>::iterator it;
    for (it = ListOfPoints.begin(); it != ListOfPoints.end(); it++)
    {
        ms.PushMatrix();
        ms.Translate((*it).x, (*it).y, (*it).z);
        RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Point"));
        string text = to_string((*it).x);
        text += ", ";
        text += to_string((*it).y);
        text += +", ";
        text += to_string((*it).z);
        ms.Translate((*it).x, (*it).y + 5.0f, (*it).z);
        RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), text, Color(1.f, 1.f, 1.f));
        ms.PopMatrix();
    }
    //Render pointer
    ms.PushMatrix();
    ms.Translate(pointer_position.x, pointer_position.y, pointer_position.z);
    RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("Pointer"));
    ms.PopMatrix();
}

void SceneEditor::AddPoint(Vector3 point)
{
    ListOfPoints.push_back(point);
}

void SceneEditor::DeletePoint()
{
    ListOfPoints.pop_back();
}

void SceneEditor::DeletePoint(Vector3 point)
{
    vector<Vector3>::iterator it;
    for (it = ListOfPoints.begin(); it != ListOfPoints.end(); it++)
    {
        if (*it == point)
        {
            ListOfPoints.erase(it);
            return;
        }
    }
}

void SceneEditor::Shift_Mode()
{
	if (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT)
		&& KeyboardController::GetInstance()->IsKeyDown('1')
		&& inputDelay > TIME_BETWEEN_INPUT)
	{
		inputDelay = 0;
		ShiftMode = Mode::Normal;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT)
		&& KeyboardController::GetInstance()->IsKeyDown('2')
		&& inputDelay > TIME_BETWEEN_INPUT)
	{
		inputDelay = 0;
		ShiftMode = Mode::FreeCam;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT)
		&& KeyboardController::GetInstance()->IsKeyDown('3')
		&& inputDelay > TIME_BETWEEN_INPUT)
	{
		inputDelay = 0;
		ShiftMode = Mode::Translate;
	}

	else if (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT)
		&& KeyboardController::GetInstance()->IsKeyDown('4')
		&& inputDelay > TIME_BETWEEN_INPUT)
    {
		inputDelay = 0;
        ShiftMode = Mode::Rotate;
    }
	else if (KeyboardController::GetInstance()->IsKeyDown(VK_SHIFT)
		&& KeyboardController::GetInstance()->IsKeyDown('5')
		&& inputDelay > TIME_BETWEEN_INPUT)
    {
		inputDelay = 0;
        ShiftMode = Mode::Scale;
    }	
}

void SceneEditor::ModeAction()
{
    switch (ShiftMode)
    {
    case SceneEditor::Normal:
        break;
    case SceneEditor::FreeCam:
        break;
    case SceneEditor::Translate:
        if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
        {

        }
        if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
        {

        }
        if (KeyboardController::GetInstance()->IsKeyDown(VK_UP))
        {

        }
        if (KeyboardController::GetInstance()->IsKeyDown(VK_DOWN))
        {

        }
        break;
    case SceneEditor::Rotate:
        break;
    case SceneEditor::Scale:
        break;
    default:
        break;
    }
}

void SceneEditor::CalculatePositionOfPointer(float dist)
{
    pointer_position = attachedCamera->GetCameraPos() + (attachedCamera->GetCameraTarget() * dist);
}

void SceneEditor::SelectObject()
{
    //vector<EntityBase*> ExportList = CSpatialPartition::GetInstance()->GetObjects(position, 1.0f);
}

void SceneEditor::AttachCamera(FPSCamera* _cameraPtr)
{
    attachedCamera = _cameraPtr;
}

void SceneEditor::DetachCamera()
{
    attachedCamera = nullptr;
}

