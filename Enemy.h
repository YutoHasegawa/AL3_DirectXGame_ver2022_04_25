#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include <EnemyBullet.h>
#include <memory>
#include <list>

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

	/// <summary>
	/// �G�ɋ߂Â�
	/// </summary>
	void ApproachUpdate();

	/// <summary>
	/// ��ʊO�ɔ�����
	/// </summary>
	void LeaveUpdate();

	/// <summary>
	/// �U��
	/// </summary>
	void Fire();

	// ���ˊԊu
	static const int kFireInterval = 60;

	/// <summary>
	/// �ڋ߃t�F�[�Y������
	/// </summary>
	void ApproachInitialize();
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
	// �ړ�
	Vector3 eMove = { 0, 0, 0 };
	// �ړ����鑬��
	const float eMoveSpeed = 0.1f;
	// ���˃^�C�}�[
	int32_t fireTimer_ = 0;
	// �e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
};

