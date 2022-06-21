#include "EnemyBullet.h"
#include "assert.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("black.png");

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update()
{
	// ���[���h�̏�����
	worldTransform_.Initialize();

	// ���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ += velocity_;

	// �s��̍X�V
	worldTransform_.MatrixUpdate();

	// ���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}