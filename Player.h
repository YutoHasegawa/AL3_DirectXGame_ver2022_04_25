#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include <PlayerBullet.h>
#include <memory>
#include <list>

/// <summary>
/// ���L����
/// </summary>
class Player
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �s��̍���
	/// </summary>
	//void MatrixUpdate();

	/// <summary>
	/// ��ʊO����
	/// </summary>
	void ScreenOut();
	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();
	/// <summary>
	/// �U��(�e�̔���)
	/// </summary>
	void Attack();
	/// <summary>
	/// �x�N�g���ƍs��̌v�Z
	/// </summary>
	/// <param name="velocity">���x</param>
	/// <param name="matWorld">�X�V��̃��[���h���W</param>
	/// <returns></returns>
	Vector3 direction(const Vector3& velocity, const Matrix4& matWorld);

	// ���[���h���W���擾
	Vector3 GetWorldPosition();

	/// <summary>
	/// �Փ˂�����Ăяo�����R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	// �e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	// ���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	// ���f��
	Model* model_ = nullptr;
	// �e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	// ���͏������邽��
	Input* input_ = nullptr;
	// �f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;
	// �e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};