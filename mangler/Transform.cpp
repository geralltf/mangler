#include "Transform.h"
#include "MathHelpers.h"
//#include <mmsystem.h>

//namespace ath
//{


Nullable<Transform*>& Transform::rootTransform = Nullable<Transform*>(); //TEST:HACK:

Transform::Transform() :
	ParentTransform(Nullable<Transform*>()),
	localMatrix(Matrix4::Identity),
	worldMatrix(Matrix4::Identity),
	QOrientationLocal(Quaternion::Identity),
	QOrientationWorld(Quaternion::Identity),
	upVector(Vector3::Up),
	forwardVector(Vector3::Forward),
	viewRotation(Matrix4::Identity),
	qRotWorld(Quaternion::Identity)
{

	Children = new std::vector<Transform*>();
	rootTransform = Nullable<Transform*>(); //TEST:HACK:
}

Transform::~Transform()
{
	delete Children;
}

const Vector3& Transform::GetLocalPosition()
{
	return localMatrix.GetTranslation();
}

const Vector3& Transform::GetLocalScale()
{
	return localMatrix.GetScale();
}

const Vector3& Transform::GetPosition()
{
	return (localMatrix * GetLocalPosition());
}

const Vector3& Transform::GetForwardVector()
{
	Quaternion::DeconstructQuaternion(QOrientationLocal, forwardVector, upVector);

	return forwardVector;
}

const Vector3& Transform::GetUpVector()
{
	Quaternion::DeconstructQuaternion(QOrientationLocal, forwardVector, upVector);

	return upVector;
}

const float Transform::GetLocalYaw()
{
	return localYaw;
}

const float Transform::GetLocalPitch()
{
	return localPitch;
}

const float Transform::GetLocalRoll()
{
	return localRoll;
}

const float Transform::GetYaw()
{
	return yaw;
}
const float Transform::GetPitch()
{
	return pitch;
}
const float Transform::GetRoll()
{
	return roll;
}

const Vector3& Transform::GetWorldPosition()
{
	return GetPosition();
}

void Transform::SetWorldPosition(Vector3& position)
{
	SetPosition(position);
}

void Transform::SetLocalPosition(Vector3& value)
{
	Translate(value);
}

void Transform::SetLocalScale(Vector3& value)
{
	Scale(value);
}

void Transform::SetPosition(Vector3& value)
{
	// TODO: TEST: World position to local position translation
	Vector3 local = (worldMatrix * value);

	SetLocalPosition(local);
}

void Transform::SetForwardVector(Vector3& value)
{
	forwardVector = value;

	updatelocalVectorRotation();
}

void Transform::SetUpVector(Vector3& value)
{
	upVector = value;

	updatelocalVectorRotation();
}

void Transform::SetLocalYaw(float yawValue)
{
	localYaw = yawValue;
	updatelocalQuaternionRotation();
}

void Transform::SetLocalPitch(float pitchValue)
{
	localPitch = pitchValue;
	updatelocalQuaternionRotation();
}

void Transform::SetLocalRoll(float rollValue)
{
	localRoll = rollValue;
	updatelocalQuaternionRotation();
}

void Transform::SetYaw(float yawValue)
{
	yaw = yawValue;
	updateworldQuaternionRotation();
}

void Transform::SetPitch(float pitchValue)
{
	pitch = pitchValue;
	updateworldQuaternionRotation();
}

void Transform::SetRoll(float rollValue)
{
	roll = rollValue;
	updateworldQuaternionRotation();
}


void Transform::updatelocalVectorRotation()
{
	QOrientationLocal = Quaternion::LookRotation(forwardVector, upVector);
}
void Transform::updatelocalQuaternionRotation()
{
	QOrientationLocal = Quaternion::Euler(GetLocalYaw(), GetLocalPitch(), GetLocalRoll());
}
void Transform::updateworldQuaternionRotation()
{
	QOrientationWorld = Quaternion::Euler(GetYaw(), GetPitch(), GetRoll());
}


Camera& Transform::GetCamera()
{
	return *Camera::MainCamera;
}

ProjectionFunct Transform::worldToScreen()
{
	switch (GetCamera().ProjectionMode)
	{
	case ProjectionMode::PM_ProjectionSimple:
		return &Transform::WorldToScreenSimple;
	case ProjectionMode::PM_ProjectionSimpleTest2:
		return &Transform::WorldToScreen;
	case ProjectionMode::PM_Projection3D:
		return &Transform::TransformWorldToScreen;
	case ProjectionMode::PM_Orthographic:
		return &Transform::TransformWorldToScreen;
	default:
		throw "ArgumentException";
	}
}

/// <summary>
/// Scales a vector by the specified scale.
/// </summary>
void Transform::Scale(float scalar)
{
	localMatrix.Scale(Vector3::One * scalar);
}
void Transform::Scale(Vector3& scale)
{
	localMatrix.Scale(scale);
}

void Transform::RotateX(float angleInDegrees)
{
	localMatrix.RotateX(angleInDegrees);
}
void Transform::RotateY(float angleInDegrees)
{
	localMatrix.RotateY(angleInDegrees);
}
void Transform::RotateZ(float angleInDegrees)
{
	localMatrix.RotateZ(angleInDegrees);
}

/// <summary>
/// Translates a vector by the specified translation.
/// </summary>
/// <param name="vector"></param>
/// <param name="translation"></param>
/// <returns></returns>
void Transform::Translate(float x, float y, float z)
{
	Vector3 transl = Vector3(x, y, z);
	Translate(transl);
}

/// <summary>
/// Translates a vector by the specified translation.
/// </summary>
/// <param name="vector"></param>
/// <param name="translation"></param>
/// <returns></returns>
void Transform::Translate(Vector3& translation)
{
	localMatrix.Translate(translation);
}

Matrix4& Transform::LocalMatrix() // LocalToWorldMatrix
{
	return localMatrix;
}

void Transform::ScaleFieldOfView(float scaleFactor)
{
	if (scaleFactor > 1)
	{
		_fovScale = (1 / _fovScale) * (1 / scaleFactor);
	}
	else
	{
		_fovScale = scaleFactor;
	}
}

Matrix4& Transform::ProjectionMatrix()
{
	Camera& camera = GetCamera();
	Matrix4 Projection;
	float aspectRatio = (float)camera.context->RenderingSize.y / (float)camera.context->RenderingSize.x;

	if (camera.ProjectionMode == ProjectionMode::PM_Projection3D)
	{
		Projection = Matrix4::Perspective(camera.FieldOfView * camera.transform->_fovScale, aspectRatio, camera.nearDistance, camera.farDistance);
	}
	else if (camera.ProjectionMode == ProjectionMode::PM_Orthographic)
	{
		Projection = Matrix4::Orthographic(-1 * _fovScale, 1 * _fovScale, -1 * _fovScale, 1 * _fovScale, -camera.farDistance, camera.farDistance);
	}
	else
	{
		Projection = Matrix4::Identity;
		// Use WorldToScreenSimple() or WorldToScreen

		//throw new NotSupportedException("Projection3D or Orthographic are only supported!");
	}

	return Projection;
}

Vector4& Transform::TransformSRT(Vector4& vector, Vector3& scale, Vector3& rotationYPR, Vector3& translation)
{
	// Manual SRT 4x4 Matrix

	Vector4 result;


	Matrix4 matrixSRT = Matrix4::Identity;

	matrixSRT.Scale(scale);
	matrixSRT *= Matrix4::RotationYawPitchRoll(rotationYPR.y, rotationYPR.x, rotationYPR.z);
	matrixSRT.Translate(translation);

	result = matrixSRT * vector;

	return result;
}

Matrix4& Transform::RecalculateWorldTransformations()
{
	Matrix4 combinedTransformationMatrix = Matrix4::CreateSQTMatrix(GetLocalScale(), QOrientationLocal, GetLocalPosition());

	if (ParentTransform.is_set())
	{
		// Recalculate prior transformations
		Transform* p = ParentTransform.get();

		while (p != nullptr)
		{
			combinedTransformationMatrix *= Matrix4::CreateSQTMatrix(p->GetLocalScale(), p->QOrientationLocal, p->GetLocalPosition());

			p->worldMatrix = combinedTransformationMatrix;

			if (p->ParentTransform.is_set())
			{
				p = p->ParentTransform.get();
			}
			else
			{
				p = nullptr;
			}
		}
	}

	worldMatrix = combinedTransformationMatrix;

	return combinedTransformationMatrix;
}

Matrix4& Transform::RecalculateLocalToWorldMatrix()
{
	Matrix4 localToWorld = localMatrix;

	return localToWorld;
}

Vector4& Transform::TransformLocalToWorldspace(Vector3& localPosition)
{
	// Transform the local position to world space
	// applying transformations from the object and its parents to derive a world position

	Vector4 inputLocal(localPosition, 1.0f);

	//Vector4 result = RecalculateLocalToWorldMatrix() * inputLocal;
	RecalculateWorldTransformations();

	Vector4 result = worldMatrix.TransformVector(inputLocal);

	result.w = 1.0f;

	return result;
}

/// <summary>
/// Transforms the 3D world position to a 2D screen space coordinate 
/// using 3D projection applying the state of the current Transform matricies.
/// </summary>
/// <param name="worldPosition"></param>
/// <returns></returns>
Vector4& Transform::TransformWorldToScreenspace(Vector3& worldPosition, Nullable<Vector4>& clippingSpace, Nullable<Transform>& objectTransform)
{
	Vector4 result;

	Camera& camera = GetCamera();

	if (camera.ProjectionMode == ProjectionMode::PM_ProjectionSimple)
	{
		result = (WorldToScreen(worldPosition).xy() + camera.transform->GetLocalPosition().xy()).xyzw();
		return result;
	}
	if (camera.ProjectionMode == ProjectionMode::PM_ProjectionSimpleTest2)
	{
		result = (WorldToScreenSimple(worldPosition).xy() + camera.transform->GetLocalPosition().xy()).xyzw();
		return result;
	}

	Matrix4 MVP = GetClipMatrix(objectTransform);

	Vector4 clipCoord = MVP * worldPosition;

	Nullable<Vector4> clipping = clipCoord;

	clippingSpace = clipping;

	result = ProjectVector(worldPosition, MVP);

	return result;
}

Vector4& Transform::GetClipCoord(Vector3& worldPosition, Nullable<Transform>& objectTransform)
{
	Matrix4 clipMatrix = GetClipMatrix(objectTransform); // Alsao the model view projection matrix.

	Vector4 world(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);

	Vector4 clipCoord = clipMatrix * world;

	return clipCoord;
}

Matrix4& Transform::GetClipMatrix(Nullable<Transform>& objectTransform)
{
	Matrix4 MVP;

	if (objectTransform.is_set() == false)
	{
		Nullable<Transform> _emptyTransform;

		MVP = GetMVPMatrix(_emptyTransform);
	}
	else
	{
		MVP = objectTransform.get().GetMVPMatrix(objectTransform);
	}

	return MVP;
}

Matrix4& Transform::GetCameraMatrix()
{
	Matrix4 View;
	Matrix4 ViewRot;
	Camera camera = GetCamera();

	if (camera.context->IgnoreCameraTranslation)
	{
		View = Matrix4::CreateSQTMatrix(Vector3::One, Quaternion::Identity, Vector3::Zero);
	}
	else
	{
		View = Matrix4::CreateSQTMatrix(Vector3::One, Quaternion::Identity, camera.transform->GetLocalPosition());
	}

	ViewRot = Matrix4::CreateRotatationMatrix(camera.transform->QOrientationLocal);

	Matrix4 CameraMatrix = View * ViewRot;

	return CameraMatrix;
}

Matrix4& Transform::GetMVPMatrix(Nullable<Transform>& objectTransform)
{
	Matrix4 World;
	Matrix4 View;
	Matrix4 Projection = ProjectionMatrix();

	View = GetCameraMatrix();

	if (objectTransform.is_set())
	{
		World = objectTransform.get().RecalculateWorldTransformations();
	}
	else
	{
		World = RecalculateWorldTransformations();
	}

	Matrix4 MVP = World * View * Projection;

	return MVP;
}

Vector4& Transform::ProjectVector(Vector3& worldPosition, Matrix4& modelViewProjection)
{
	Vector4 normalisedDeviceCoord;
	Vector4 screenPoint;
	Vector4 world(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);

	Vector4 clipCoord = modelViewProjection * world;

	// Preserve the value of what z was before the perspective divide to use later for depth testing
	float z = clipCoord.z;

	// Transform to Normalised Device Coordinates.
	// PERSPECTIVE DIVIDE
	normalisedDeviceCoord = clipCoord.PerspectiveDivide();

	// NDC-SPACE
	screenPoint = TransformNDCToScreenSpace(normalisedDeviceCoord);
	screenPoint.w = z;

	// SCREEN-SPACE
	return screenPoint;
}

/// <summary>
/// Transforms the 3D world position to a 2D screen space coordinate 
/// using 3D projection applying the state of the current Transform matricies.
/// </summary>
/// <param name="worldPosition"></param>
/// <returns></returns>
Vector2i& Transform::TransformWorldToScreen(Vector3& worldPosition)
{
	Nullable<Vector4> clippingCoord;
	Nullable<Transform> emptyTransform;
	Vector2i result = TransformWorldToScreenspace(worldPosition, clippingCoord, emptyTransform).xy().xyi();
	return result;
}

/// <summary>
/// Transforms a normalised device coord in the range (-1, 1) to screen space. (Viewport.)
/// </summary>
/// <param name="normalisedDeviceCoord">
/// Value in the range x: -1, 1, y: -1, 1, z: -1, 1
/// </param>
/// <returns></returns>
Vector4& Transform::TransformNDCToScreenSpace(Vector4& coord)
{
	Camera camera = GetCamera();
	Vector4 normalisedDeviceCoord = coord;

	normalisedDeviceCoord.x = (coord.x * camera.context->RenderingSize.x) + camera.context->HalfSize.x;
	normalisedDeviceCoord.y = ((coord.y * camera.context->RenderingSize.y) + camera.context->HalfSize.y);

	Vector4 screenPoint = normalisedDeviceCoord;

	// HACK Clip within screen:
	//screenPoint = MathHelpers.ClampVector(screenPoint.xyz, 0, Camera.Rasteriser.RenderingSize); // Clip against screen bounds

	return screenPoint; // technically we only need a 2 float vector though we will use z later for depth testing
}

Vector2i& Transform::WorldToScreenSimple(Vector3& worldPosition) // HACKY projection!
{
	Camera camera = GetCamera();
	Vector2i screen = Vector2i::Zero;
	Vector3 vectorToCamera = (worldPosition - camera.transform->GetPosition());

	// Scale and decrease distance from center.
	//float z = vectorToCamera.z == 0 ? 1 : vectorToCamera.z;
	float z = vectorToCamera.z;

	float viewDistance = tan((camera.FieldOfView / 2.0f) * Deg2Rad) * z;

	screen.x = (int)(vectorToCamera.x * viewDistance);
	screen.y = (int)(vectorToCamera.y * viewDistance);

	// Transform to screen space.
	screen += camera.context->HalfSize;

	// Clip within screen:
	sf::Vector2i sz(camera.context->RenderingSize.x, camera.context->RenderingSize.y);
	sf::Vector2i pt(screen.x, screen.y);
	sf::Vector2i sc = MathHelpers::ClampVector(pt, 0, sz);
	screen = Vector2i(sc.x, sc.y);
	//screen = MathHelpers::ClampVector(screen, 0, camera.context->RenderingSize);

	return screen;
}

Vector2i& Transform::WorldToScreen(Vector3& worldPosition) // HACKY projection!
{
	Camera camera = GetCamera();
	Vector2i screen = Vector2i::Zero;

	Vector3 vectorToCamera = (worldPosition - camera.transform->GetPosition());

	float aspectRatio = camera.fieldOfViewVert / camera.fieldOfViewHoriz;

	// Scale and decrease distance from center.
	float z = vectorToCamera.z == 0 ? 1 : vectorToCamera.z;
	screen.x = (int)(vectorToCamera.x / (z / camera.fieldOfViewHoriz));
	screen.y = (int)(vectorToCamera.y / (z / camera.fieldOfViewVert));

	screen += camera.context->HalfSize;

	sf::Vector2i sz(camera.context->RenderingSize.x, camera.context->RenderingSize.y);
	sf::Vector2i pt(screen.x, screen.y);
	sf::Vector2i sc = MathHelpers::ClampVector(pt, 0, sz);
	screen = Vector2i(sc.x, sc.y);
	//screen = MathHelpers::ClampVector(screen, 0, camera.context->RenderingSize);

	return screen;
}

/// <summary>
/// Screen space to world space conversion
/// </summary>
Vector3& Transform::TransformToWorldSpace(Vector3& screenPoint)
{
	return TransformToWorldSpace(screenPoint);
}

/// <summary>
/// Screen space to world space conversion
/// </summary>
Vector4& Transform::TransformToWorldSpace(Vector2i& screenPoint, Vector2i& screenSize)
{
	float w = (float)screenSize.x;
	float h = (float)screenSize.y;
	Vector2 ndc(screenPoint.x / w, screenPoint.y / h);
	return TransformToWorldSpace(ndc);
}

/// <summary>
/// Screen space to world space conversion
/// </summary>
Vector4& Transform::TransformToWorldSpace(Vector2& ndcScreenPoint)
{
	Matrix4 projection = ProjectionMatrix();
	Matrix4 view = GetCameraMatrix();

	return TransformToWorldSpace(ndcScreenPoint, projection, view);
}

/// <summary>
/// Screen space to world space conversion
/// </summary>
Vector4& Transform::TransformToWorldSpace(Vector2& ndcScreenPoint, Matrix4& projection, Matrix4& view)
{
	Vector4 screenPoint(ndcScreenPoint.x, ndcScreenPoint.y, 0.0f, 1.0f);

	Matrix4 projection_view = projection * view;

	Matrix4 inv_projection_view = projection_view.Inverse();
	Vector4 world = inv_projection_view * screenPoint;

	return world;
}

//}