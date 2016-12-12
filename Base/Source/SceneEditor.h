#ifndef SCENE_EDITOR_H
#define SCENE_EDITOR_H

#include "SingletonTemplate.h"
#include "Vector3.h"
#include <vector>
#include "FPSCamera.h"
#include "EntityBase.h"

class Mesh;

using namespace std;
class SceneEditor : public Singleton<SceneEditor>
{
public:
    SceneEditor();
    SceneEditor(FPSCamera* _cameraPtr);
    ~SceneEditor();

    void Init();
    void Update(double dt = 0.333f);
    void Render();
    void AddPoint(Vector3 point);
    void DeletePoint();
    void DeletePoint(Vector3 point);
    void ShiftObject();

	bool isPointOnLine(Vector3 Line, Vector3 point);

    enum Mode
    {
		Normal,
		FreeCam,
        Translate,
        Rotate,
        Scale
    };
	Mode ShiftMode;
    //camera
    void AttachCamera(FPSCamera* _cameraPtr);
    void DetachCamera(void);

private:
    vector<Vector3> ListOfPoints;
    Mesh* point_mesh;
    FPSCamera* attachedCamera;
    float inputDelay;
    
};



#endif