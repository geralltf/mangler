#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Nullable.h"
#include "Camera.h"
#include "Matrix4.h"
#include <vector>

//namespace ath
//{
	class Transform;
	class Camera;

	typedef Vector2i&(Transform::*ProjectionFunct)(Vector3&);

	class Transform
	{
	public:
		static Nullable<Transform*>& rootTransform; //TEST:HACK:
		Nullable<Transform*>& ParentTransform;

		std::vector<Transform*>* Children;

		/// <summary>
		/// The combined set of local transformations to apply to the object.
		/// </summary>
		Matrix4& localMatrix = Matrix4::Identity;
		Matrix4& worldMatrix;// the new combined transform matrix

		Quaternion& QOrientationLocal;
		Quaternion& QOrientationWorld;

	private:

		Vector3& forwardVector;
		Vector3& upVector;
		float localYaw, localPitch, localRoll;
		float yaw, pitch, roll;
		Quaternion& qRotWorld;
		Matrix4& viewRotation;

	public:
		float _fovScale = 1;

	public:
		Transform();
		~Transform();

		const Vector3& GetLocalPosition();
		const Vector3& GetLocalScale();
		const Vector3& GetPosition();
		const Vector3& GetForwardVector();
		const Vector3& GetUpVector();
		const float GetLocalYaw();
		const float GetLocalPitch();
		const float GetLocalRoll();
		const float GetYaw();
		const float GetPitch();
		const float GetRoll();
		const Vector3& GetWorldPosition();

		void SetWorldPosition(Vector3& position);
		void SetLocalPosition(Vector3& value);
		void SetLocalScale(Vector3& value);
		void SetPosition(Vector3& value);
		void SetForwardVector(Vector3& value);
		void SetUpVector(Vector3& value);
		void SetLocalYaw(float yawValue);
		void SetLocalPitch(float pitchValue);
		void SetLocalRoll(float rollValue);
		void SetYaw(float yawValue);
		void SetPitch(float pitchValue);
		void SetRoll(float rollValue);

		
		void updatelocalVectorRotation();
		void updatelocalQuaternionRotation();
		void updateworldQuaternionRotation();


		Camera& GetCamera();

		ProjectionFunct worldToScreen();

		/// <summary>
		/// Scales a vector by the specified scale.
		/// </summary>
		void Scale(float scalar);
		void Scale(Vector3& scale);

		void RotateX(float angleInDegrees);
		void RotateY(float angleInDegrees);
		void RotateZ(float angleInDegrees);

		/// <summary>
		/// Translates a vector by the specified translation.
		/// </summary>
		/// <param name="vector"></param>
		/// <param name="translation"></param>
		/// <returns></returns>
		void Translate(float x, float y, float z);

		/// <summary>
		/// Translates a vector by the specified translation.
		/// </summary>
		/// <param name="vector"></param>
		/// <param name="translation"></param>
		/// <returns></returns>
		void Translate(Vector3& translation);

		Matrix4& LocalMatrix();

		void ScaleFieldOfView(float scaleFactor);

		Matrix4& ProjectionMatrix();

		Vector4& TransformSRT(Vector4& vector, Vector3& scale, Vector3& rotationYPR, Vector3& translation);

		Matrix4& RecalculateWorldTransformations();

		Matrix4& RecalculateLocalToWorldMatrix();

		Vector4& TransformLocalToWorldspace(Vector3& localPosition);

		/// <summary>
		/// Transforms the 3D world position to a 2D screen space coordinate 
		/// using 3D projection applying the state of the current Transform matricies.
		/// </summary>
		/// <param name="worldPosition"></param>
		/// <returns></returns>
		Vector4& TransformWorldToScreenspace(Vector3& worldPosition, Nullable<Vector4>& clippingSpace, Nullable<Transform>& objectTransform);

		Vector4& GetClipCoord(Vector3& worldPosition, Nullable<Transform>& objectTransform);

		Matrix4& GetClipMatrix(Nullable<Transform>& objectTransform);

		Matrix4& GetCameraMatrix();

		Matrix4& GetMVPMatrix(Nullable<Transform>& objectTransform);

		Vector4& ProjectVector(Vector3& worldPosition, Matrix4& modelViewProjection);

		/// <summary>
		/// Transforms the 3D world position to a 2D screen space coordinate 
		/// using 3D projection applying the state of the current Transform matricies.
		/// </summary>
		/// <param name="worldPosition"></param>
		/// <returns></returns>
		Vector2i& TransformWorldToScreen(Vector3& worldPosition);

		/// <summary>
		/// Transforms a normalised device coord in the range (-1, 1) to screen space. (Viewport.)
		/// </summary>
		/// <param name="normalisedDeviceCoord">
		/// Value in the range x: -1, 1, y: -1, 1, z: -1, 1
		/// </param>
		/// <returns></returns>
		Vector4& TransformNDCToScreenSpace(Vector4& coord);

		Vector2i& WorldToScreenSimple(Vector3& worldPosition); // HACKY projection!

		Vector2i& WorldToScreen(Vector3& worldPosition); // HACKY projection!

		/// <summary>
		/// Screen space to world space conversion
		/// </summary>
		Vector3& TransformToWorldSpace(Vector3& screenPoint);

		/// <summary>
		/// Screen space to world space conversion
		/// </summary>
		Vector4& TransformToWorldSpace(Vector2i& screenPoint, Vector2i& screenSize);

		/// <summary>
		/// Screen space to world space conversion
		/// </summary>
		Vector4& TransformToWorldSpace(Vector2& ndcScreenPoint);

		/// <summary>
		/// Screen space to world space conversion
		/// </summary>
		Vector4& TransformToWorldSpace(Vector2& ndcScreenPoint, Matrix4& projection, Matrix4& view);
	};

//}
#endif