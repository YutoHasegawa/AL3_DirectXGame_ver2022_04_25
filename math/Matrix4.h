#pragma once
#include <cmath>
/// <summary>
/// 行列
/// </summary>

class Matrix4 {
public:
	Matrix4 ToMatIdentity()
	{
		Matrix4 matIdentity;

		matIdentity.m[0][0] = 1;
		matIdentity.m[1][1] = 1;
		matIdentity.m[2][2] = 1;
		matIdentity.m[3][3] = 1;

		return matIdentity;
	}

	Matrix4 ToMatScale(Vector3 scale)
	{
		Matrix4 matScale = ToMatIdentity();

		matScale.m[0][0] = scale.x;
		matScale.m[1][1] = scale.y;
		matScale.m[2][2] = scale.z;

		return matScale;
	}

	Matrix4 ToMatRotZ(float radian)
	{
		Matrix4 matRotZ = ToMatIdentity();

		matRotZ.m[0][0] = cos(radian);
		matRotZ.m[0][1] = sin(radian);
		matRotZ.m[1][0] = -sin(radian);
		matRotZ.m[1][1] = cos(radian);

		return matRotZ;
	}

	Matrix4 ToMatRotX(float radian)
	{
		Matrix4 matRotX = ToMatIdentity();

		matRotX.m[1][1] = cos(radian);
		matRotX.m[1][2] = sin(radian);
		matRotX.m[2][1] = -sin(radian);
		matRotX.m[2][2] = cos(radian);

		return matRotX;
	}

	Matrix4 ToMatRotY(float radian)
	{
		Matrix4 matRotY = ToMatIdentity();

		matRotY.m[0][0] = cos(radian);
		matRotY.m[0][2] = sin(radian);
		matRotY.m[2][0] = -sin(radian);
		matRotY.m[2][2] = cos(radian);

		return matRotY;
	}

	Matrix4 ToMatRot(Vector3 rotation)
	{
		Matrix4 matRot = ToMatIdentity();

		matRot *= ToMatRotZ(rotation.z);
		matRot *= ToMatRotX(rotation.x);
		matRot *= ToMatRotY(rotation.y);

		return matRot;

	}

	Matrix4 ToMatTrans(Vector3 translation)
	{
		Matrix4 matTrans = ToMatIdentity();

		matTrans.m[3][0] = translation.x;
		matTrans.m[3][1] = translation.y;
		matTrans.m[3][2] = translation.z;

		return matTrans;
	}


public:
	// 行x列
	float m[4][4];

	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);



	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);

	void MatrixUpdate(Vector3 scale, Vector3 rotation, Vector3 translation)
	{
		*this *= ToMatIdentity();
		*this *= ToMatScale(scale);
		*this *= ToMatRot(rotation);
		*this *= ToMatTrans(translation);
	}
};