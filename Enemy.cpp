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
	textureHandle_ = TextureManager::Load("mario.jpg");

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	// �ڋ߃t�F�[�Y������
	ApproachInitialize();
}


void Enemy::Update()
{
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
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

	// �e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}


void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// �e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachUpdate()
{
	eMove = { 0.0f, 0.0f, -eMoveSpeed };
	// �ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += eMove;
	// �K��Ɉʒu�ɒB�����痣�E
	if (worldTransform_.translation_.z < -0.0f)
	{
		phase_ = Phase::Leave;
	}

	// ���˃^�C�}�[�J�E���g�_�E��
	fireTimer_--;
	// �w�莞�ԂɒB����
	if (fireTimer_ < 0)
	{
		// �e�𔭎�
		Fire();
		// ���˃^�C�}�[��������
		fireTimer_ = kFireInterval;
	}
}

void Enemy::LeaveUpdate()
{
	eMove = { eMoveSpeed, eMoveSpeed, 0.0f };
	// �ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += eMove;
}

void Enemy::Fire()
{
	// �e�̑��x
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	//// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	//velocity = direction(velocity, worldTransform_.matWorld_);

	// �e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// �e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize()
{
	// ���˃^�C�}�[��������
	fireTimer_ = kFireInterval;
}
