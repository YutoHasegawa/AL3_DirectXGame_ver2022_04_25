#include "Enemy.h"
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

	 float eMoveSpeed = 2.0f;
	static float timer = 0;
	timer += 0.1;
	if (timer >= 2.4)eMoveSpeed = 0;

	Vector3 eMove = { 0.0f, 0.0f, eMoveSpeed };


	// ���W���ړ�������(1�t���[�����̈ړ��ʂ𑫂�����)
	worldTransform_.translation_ -= eMove;

	// �s��̍X�V
	worldTransform_.MatrixUpdate();
}


void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}