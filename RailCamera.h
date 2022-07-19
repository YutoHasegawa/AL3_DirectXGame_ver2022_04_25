#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera
{
public:
	void Initialize(const Vector3& position, const Vector3& rotation);

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }
	WorldTransform* GetWorldTransForm() { return &worldTransform_; }
private:
	// ワールド返還データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
};