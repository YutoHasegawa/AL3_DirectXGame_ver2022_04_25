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
	Move();
	ScreenOut();

	// �s��X�V
	MatrixUpdate();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"transform:(%f, %f, %f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z
	);
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
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

void Player::MatrixUpdate()
{
	// �s��̍X�V
	worldTransform_.matWorld_.MatrixUpdate
	(
		worldTransform_.scale_,
		worldTransform_.rotation_,
		worldTransform_.translation_
	);

	// ���[���h�̍X�V
	worldTransform_.TransferMatrix();
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