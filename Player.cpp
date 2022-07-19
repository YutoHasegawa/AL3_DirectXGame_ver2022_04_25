#include "Player.h"
#include "assert.h"
#include "MathUtility.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	worldTransform_.translation_ = { 0, 0, 30.0f };
	textureHandle_ = textureHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update()
{
	// �f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});
	// ���[���h�̏�����
	worldTransform_.Initialize();
	// �L�����N�^�[���񏈗�
	Rotate();
	// �L�����N�^�[�ړ�����
	Move();
	// �L�����N�^�[��ʊO����
	ScreenOut();
	// �s��X�V
	worldTransform_.MatrixUpdate();
	// �L�����N�^�[�U������
	Attack();
	// �e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}


	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"transform:(%f, %f, %f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z
	);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"rotation:(%f, %f, %f)", worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z
	);
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// �e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Player::Move()
{
	// �L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0, 0, 0 };

	// �ړ���
	const float kCharacter = 0.2f;

	// �������L�[�ňړ�
	if (input_->PushKey(DIK_W))
	{
		move.y = kCharacter;
	}
	if (input_->PushKey(DIK_S))
	{
		move.y = -kCharacter;
	}
	if (input_->PushKey(DIK_A))
	{
		move.x = -kCharacter;
	}
	if (input_->PushKey(DIK_D))
	{
		move.x = kCharacter;
	}

	worldTransform_.translation_ += move;
}

void Player::ScreenOut()
{
	// �ړ����E���W
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// �͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

}

void Player::Rotate()
{
	const float rotationSpeed = 0.01f;

	if (input_->PushKey(DIK_U))
	{
		worldTransform_.rotation_.y -= rotationSpeed;
	}
	else if (input_->PushKey(DIK_I))
	{
		worldTransform_.rotation_.y += rotationSpeed;
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		// �e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		Vector3 PlayerWorldTransform_ = MathUtility::Vector3TransformCoord(worldTransform_.translation_, worldTransform_.matWorld_);

		// ���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = direction(velocity, worldTransform_.matWorld_);

		// �e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, PlayerWorldTransform_, velocity);

		// �e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
}

Vector3 Player::direction(const Vector3& velocity, const Matrix4& matWorld)
{
	Vector3 puts;

	puts.x = velocity.x * matWorld.m[0][0] + velocity.y * matWorld.m[1][0] + velocity.z * matWorld.m[2][0];
	puts.y = velocity.x * matWorld.m[0][1] + velocity.y * matWorld.m[1][1] + velocity.z * matWorld.m[2][1];
	puts.z = velocity.x * matWorld.m[0][2] + velocity.y * matWorld.m[1][2] + velocity.z * matWorld.m[2][2];

	return puts;
}

Vector3 Player::GetWorldPosition()
{
	// ���[���h���W������ϐ�
	Vector3 worldPos;
	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Player::OnCollision()
{
}