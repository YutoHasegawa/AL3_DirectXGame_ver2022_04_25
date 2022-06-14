#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"


/// <summary>
/// �G
/// </summary>
class Enemy 
{
public:
	//Enemy();
	/// <summary>
	/// ������
	/// </summary>
	void initialize(Model* model, const Vector3& position);
	
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	void ApproachUpdate();

	void LeaveUpdate();

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	enum class Phase
	{
		Approach,		//�ڋ߂���
		Leave,			//���E����
	};
	// �t�F�[�Y
	Phase phase_ = Phase::Approach;
	//
	Vector3 eMove = { 0, 0, 0 };
	//
	const float eMoveSpeed = 0.03f;

};
