#ifndef MATRIX_H
#define MATRIX_H



//namespace ath
//{
#ifndef MAT_FWD_DECLARE_H
#define MAT_FWD_DECLARE_H
	class Matrix4;
	class Vector4;
	class Vector3;
	class Vector2;
	class Quaternion;
	class Matrix4;

#include "Vector2i.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

#endif

	class Matrix4
	{
	public:
		static Matrix4& Identity;

		// Matrix Values

		// Row 0
		float m00;
		float m01;
		float m02;
		float m03;

		// Row 1
		float m10;
		float m11;
		float m12;
		float m13;

		// Row 2
		float m20;
		float m21;
		float m22;
		float m23;

		// Row 3
		float m30;
		float m31;
		float m32;
		float m33;

		float _fovScale = 1; // FOR TESTING

		// Constructors
		Matrix4();

		Matrix4(
			const Vector4& row0, 
			const Vector4& row1, 
			const Vector4& row2, 
			const Vector4& row3
		);

		// Matrix Rows
		Vector4& GetRow0() const;
		Vector4& GetRow1() const;
		Vector4& GetRow2() const;
		Vector4& GetRow3() const;
		void SetRow0(Vector4& row0);
		void SetRow1(Vector4& row1);
		void SetRow2(Vector4& row2);
		void SetRow3(Vector4& row3);
		void SetRow0(const Vector4& row0);
		void SetRow1(const Vector4& row1);
		void SetRow2(const Vector4& row2);
		void SetRow3(const Vector4& row3);
		// Matrix Rows

		// Matrix Columns

		Vector4& GetColumn0() const;
		Vector4& GetColumn1() const;
		Vector4& GetColumn2() const;
		Vector4& GetColumn3() const;
		void SetColumn0(Vector4& column0);
		void SetColumn1(Vector4& column1);
		void SetColumn2(Vector4& column2);
		void SetColumn3(Vector4& column3);
		void SetColumn0(const Vector4& column0);
		void SetColumn1(const Vector4& column1);
		void SetColumn2(const Vector4& column2);
		void SetColumn3(const Vector4& column3);

		// Matrix Columns

		const float GetMagnitude() const;
		const float GetDeterminant() const;
		static Matrix4& GetIdentity();

		static float CrossProduct(const float left, const float right);
		static float CrossProductcfn(const float left, const float right);
		static float CrossProductcfp(const float left, const float right);

		static float Determinant2x2cn(const float m00, const float m01, const float m10, const float m11);
		static float Determinant2x2cp(const float m00, const float m01, const float m10, const float m11);
		const Matrix4& Cofactor() const;
		const Matrix4& Inverse();
		static const Matrix4& Invert(const Matrix4& m);

		void ScalarMultiply(Matrix4& input, const float scalar);

		static Matrix4& CreateSRTMatrix(const Vector3& scale, const Vector3& rotationYPR, const Vector3& translation);
		
		static Matrix4& CreateSQTMatrix(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
		
		void SetTRS(const Vector3& translation, const Quaternion& rotation, const Vector3& scale);

		void Scale(const Vector3& scale);

		void Scale(const Vector4& scale);

		const Vector3& GetScale();

		void RotateX(const float angleInDegrees);

		void RotateY(const float angleInDegrees);

		void RotateZ(const float angleInDegrees);

		static const Matrix4& RotationYawPitchRoll(const float rotationYYaw, const float rotationXPitch, const float rotationZRoll);

		void Rotate(const Vector2& angleXY);

		void Translate(const Vector3& translation);

		const Vector3& GetTranslation();

		const Vector4& GetTranslation4();

		void Translate(const Vector4& translation);

		void ShearX(const float y, const float z);

		const Matrix4& Transpose();

		const float* ToArray();

		void FromArray(const float* arr);

		// Const correct multipliers.
		void Multiply(const Matrix4& right);

		static const Matrix4& Multiply(const Matrix4& left, const Matrix4& right);

		static const Matrix4& Multiply(const Matrix4& left, const Vector4& right);

		static const Vector2& TransformVector(const Matrix4& left, const Vector2& right);

		static const Vector3& TransformVector(const Matrix4& left, const Vector3& right);

		static const Vector4& TransformVector(const Matrix4& left, const Vector4& right);

		const Vector3& Matrix4::TransformVector(const Vector3& vector);
		
		const Vector3& Matrix4::TransformNormal(const Vector3& normal);

		static const Vector3& Matrix4::TransformNormal(const Matrix4& transform, const Vector3& normal);

		// Affine Transformations.
		const Vector4& TransformSRT(const Vector4& vector, const Vector3& scale, const Vector3& rotationAxis, const Vector2& xyRotationDeg, const Vector3& translation);

		static const Matrix4& CreateScaleMatrix(const Vector3& scale);

		static const Matrix4& CreateRotationXMatrix(const float angle);

		static const Matrix4& CreateRotationYMatrix(const float angle);

		static const Matrix4& CreateRotationZMatrix(const float angle);

		static const Matrix4& CreateRotatationMatrix(const Quaternion& rotation);

		static const Matrix4& CreateRotatationMatrix(const Vector3& rotationAxis, const float angle);

		static const Matrix4& CreateTranslationMatrix(const Vector3& translation);

		static const Matrix4& CreateYZFlipMatrix();

		// Projections.
		void ScaleFieldOfView(const float scaleFactor);

		static const Matrix4& Orthographic(const float leftPlane, const float rightPlane, const float bottomPlane, const float topPlane, const float nearDistance, const float farDistance);

		static const Matrix4& Perspective(const float fieldOfView, const float aspectRatio, const float nearDistance, const float farDistance);

		static const Matrix4& CreateScreenSpaceMatrix(const Vector2i& halfSize);

		static const Matrix4& CreateFrustumMatrix(const float left, const float right, const float bottom, const float top, const float nearDepth, const float farDepth);

		static const Matrix4& LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

		/// Get value in the matrix given the specified index.
		float operator [] (const int index) const
		{
			return (&m00)[index];
		}
		/// Get value in the matrix given the specified index.
		float GetValue(const int index) const;

		/// Set a value in the matrix given the specified index.
		void SetValue(const int index, const float value);



		//const Matrix4& operator *(const Matrix4& right) const {
		//	Matrix4 result = static_cast<Matrix4>(Multiply(*this, right));
		//	return result;
		//}
		//const Vector4& operator *(const Vector4& right) const {
		//	Vector4 result = static_cast<Vector4>(TransformVector(*this, right));
		//	return result;
		//}
		//const Vector3& operator *(const Vector3& right) const {
		//	Vector3 result = static_cast<Vector3>(TransformVector(*this, right));
		//	return result;
		//}
		//const Vector2& operator *(const Vector2& right) const {
		//	Vector2 result = static_cast<Vector2>(TransformVector(*this, right));
		//	return result;
		//}
	};


	inline const Matrix4& operator *=(Matrix4& left, const Matrix4& right) { 
		const Matrix4 lhs = static_cast<const Matrix4>(left);
		return Matrix4::Multiply(lhs, right);
	}

	inline const Matrix4& operator *(const Matrix4& left, const Matrix4& right) { return Matrix4::Multiply(left, right); }
	inline const Vector4& operator *(const Matrix4& left, const Vector4& right) { return Matrix4::TransformVector(left, right); }
	inline const Vector3& operator *(const Matrix4& left, const Vector3& right) { return Matrix4::TransformVector(left, right); }
	inline const Vector2& operator *(const Matrix4& left, const Vector2& right) { return Matrix4::TransformVector(left, right); }

//}
#endif