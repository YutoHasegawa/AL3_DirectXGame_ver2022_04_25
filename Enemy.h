#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "EnemyBullet.h"
#include <memory>
#include "Vector3.h"


// ���@�N���X�̑O���錾
class Player;

// GameScene�̑O���錾
class GameScene;

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
	static const int kFireInterval = 120;

	/// <summary>
	/// �ڋ߃t�F�[�Y������
	/// </summary>
	void ApproachInitialize();

	//�Z�b�^�[
	void SetPlayer(Player* player) { player_ = player; }

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	/// <summary>
	/// �Փ˂�����Ăяo�����R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	//// �e���X�g���擾
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }


	bool IsDead() const { return isDead_; }
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
	const float eMoveSpeed = 0.0f;
	// ���˃^�C�}�[
	int32_t fireTimer_ = 120;
	//// �e
	//std::list<std::unique_ptr<EnemyBullet>> bullets_;
	// ���L����
	Player* player_ = nullptr;
	// �Q�[���V�[��
	GameScene* gameScene_ = nullptr;
	// �f�X�t���O
	bool isDead_ = false;
};

