#include "PlayerBullet.h"
#include "assert.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update()
{
	worldTransform_.MatrixUpdate();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
