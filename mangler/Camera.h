#ifndef CAMERA_H
#define CAMERA_H

class Transform;
class RenderingContext;

#include "Vector3.h"
#include "Transform.h"
#include "Ray.h"
#include "RenderingContext.h"

enum ProjectionMode 
{
	PM_Orthographic,
	PM_Projection3D,
	PM_ProjectionSimple,
	PM_ProjectionSimpleTest2
};
	
class Camera
{
public:
	static Camera* MainCamera;//HACK: Static main camera

	//TODO: test lighting paramters dont belong in camera
	float Brightness = 10.01f;; // Test lighting
	float BrightnessInv = 1; // Test lighting
	float ExpCuttof= 0.4f; // Test lighting
	float ExpMultiplier= 1; // Test 
	ProjectionMode ProjectionMode = ProjectionMode::PM_Projection3D;
	float fieldOfViewHoriz = 60;
	float fieldOfViewVert = 45;
	float FieldOfView = 90; // TODO: shift from single fov to controllable vertical, and horizontal 
	float nearDistance = 0.01f;
	float farDistance = 6;
	Transform* transform;
	RenderingContext* context;

	Camera();

	Vector3& GetPosition() const;
	Quaternion& Orientation() const;

	void SetPosition(Vector3& value);
	void Reset();

	Ray& ScreenToRay(Vector2& ndcScreenPoint) const;
	Ray& ScreenToRay(Vector2& ndcScreenPoint, Matrix4& projection, Matrix4& view) const;
};

#endif