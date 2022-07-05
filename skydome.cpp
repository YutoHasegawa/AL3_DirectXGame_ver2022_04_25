#include "skydome.h"
#include "assert.h"

void Skydome::Initialize(Model* model)
{
	// NULLポインタチェック
	assert(model);

	// テクスチャの読み込み
	model_ = model;

	// ワールド変換の初期化
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
