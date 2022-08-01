#include "RailCamera.h"
#include "GameScene.h"
#include "DebugText.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	// ���[���h�g�����X�t�H�[���̏����ݒ�
	// ���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	// �����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	// �r���[�v���W�F�N�V�����̏����ݒ�
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	const Vector3 tMove = { 0, 0, 0.1f };
	const Vector3 rMove = { 0, 0, 0 };
	worldTransform_.translation_ += tMove;
	worldTransform_.rotation_ += rMove;
	// �s��̍X�V
	worldTransform_.MatrixUpdate();

	viewProjection_.eye = worldTransform_.translation_;
	// ���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	// ���[���J�����̉�]�𔽉f
	forward = worldTransform_.matWorld_.direction(forward, worldTransform_.matWorld_);
	// ���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;
	// ���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	// ���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	viewProjection_.up = worldTransform_.matWorld_.direction(up, worldTransform_.matWorld_);
	// �r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	// �f�o�b�O�p�\��
	debugText_->SetPos(20, 100);
	debugText_->Printf("RailCamera Pos:(%f, %f, %f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

}
