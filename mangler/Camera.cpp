#include "Camera.h"

Camera* Camera::MainCamera = nullptr;

Camera::Camera()
{
	transform = new Transform();

	MainCamera = this;
}

Vector3& Camera::GetPosition() const
{
	Vector3 worldPos = transform->GetWorldPosition();
	return worldPos;
}

void Camera::SetPosition(Vector3& value)
{
	transform->SetWorldPosition(value);
}

Quaternion& Camera::Orientation() const
{
	return transform->QOrientationLocal;
}

void Camera::Reset()
{
	if (transform != nullptr) delete transform;
	transform = new Transform();
}

Ray& Camera::ScreenToRay(Vector2& ndcScreenPoint) const
{
	Matrix4 projection = transform->ProjectionMatrix();

	Matrix4 view = transform->GetCameraMatrix();

	return ScreenToRay(ndcScreenPoint, projection, view);
}

Ray& Camera::ScreenToRay(Vector2& ndcScreenPoint, Matrix4& projection, Matrix4& view) const
{
	Vector4 near(ndcScreenPoint.x, ndcScreenPoint.y, nearDistance, 1.0f);
	Vector4 far(ndcScreenPoint.x, ndcScreenPoint.y, nearDistance + farDistance, 1.0f);

	Matrix4 inv_projection = projection.Inverse();
	Matrix4 projection_inv_view = (inv_projection * view);
	Matrix4 inv_projection_view = projection_inv_view.Inverse();

	Vector3 nearWorld = (inv_projection_view * near).xyz();

	Vector3 farWorld = (inv_projection_view * far).xyz();

	Vector3 direction = farWorld.Normalised();

	Ray rayFromScreen(nearWorld, direction);

	// This is spot on. Though it seems rays are projected the wrong way!

	return rayFromScreen;
}