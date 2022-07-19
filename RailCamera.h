#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// ���[���J����
/// </summary>
class RailCamera
{
public:
	void Initialize(const Vector3& position, const Vector3& rotation);

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	WorldTransform* GetWorldTransForm() { return &worldTransform_; }
private:
	// ���[���h�Ԋ҃f�[�^
	WorldTransform worldTransform_;
	// �r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
};