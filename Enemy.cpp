#include "Enemy.h"
#include "assert.h"

//Enemy::Enemy()
//{
//}

void Enemy::initialize(Model* model, const Vector3& position)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �e�N�X�`���̓ǂݍ���
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
}


void Enemy::Update()
{
	// ���[���h�̏�����
	worldTransform_.Initialize();

	switch (phase_)
	{
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	}
	// �s��̍X�V
	worldTransform_.MatrixUpdate();
}


void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachUpdate()
{
	eMove = { 0.0f, 0.0f, -eMoveSpeed };
	// �ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += eMove;
	// �K��Ɉʒu�ɒB�����痣�E
	if (worldTransform_.translation_.z < -30.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate()
{
	eMove = { -eMoveSpeed, eMoveSpeed, 0.0f };
	// �ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += eMove;
}