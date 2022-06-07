#include "Player.h"
#include "assert.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();

}

void Player::Update()
{
	// �L�����N�^�[���񏈗�
	Rotate();
	// �L�����N�^�[�ړ�����
	Move();
	// �L�����N�^�[�U������
	Attack();
	// �e�X�V
	if (bullet_) { bullet_->Update(); }

	// �L�����N�^�[��ʊO����
	ScreenOut();

	// �s��X�V
	worldTransform_.MatrixUpdate();

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
	if (bullet_) { bullet_->Draw(viewProjection); }
}

void Player::Move()
{
	// ���[���h�̏�����
	worldTransform_.Initialize();

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

//void Player::MatrixUpdate()
//{
//	// �s��̍X�V
//	worldTransform_.matWorld_.MatrixUpdate
//	(
//		worldTransform_.scale_,
//		worldTransform_.rotation_,
//		worldTransform_.translation_
//	);
//
//	// ���[���h�̍X�V
//	worldTransform_.TransferMatrix();
//}

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
	if (input_->PushKey(DIK_SPACE))
	{
		// �e�𐶐����A������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		// �e��o�^����
		bullet_ = newBullet;
	}
}
