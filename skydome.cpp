#include "skydome.h"
#include "assert.h"

void Skydome::Initialize(Model* model)
{
	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �e�N�X�`���̓ǂݍ���
	model_ = model;

	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
}

void Skydome::Update()
{
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 100.0f, 100.0f, 100.0f };
	worldTransform_.MatrixUpdate();
}

void Skydome::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
