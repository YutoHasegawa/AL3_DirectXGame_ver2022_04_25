#include "Enemy.h"
#include "Enemy.h"
#include "assert.h"

//Enemy::Enemy()
//{
//}

void Enemy::initialize(Model* model, const Vector3& position)
{
	// NULLポインタチェック
	assert(model);

	// テクスチャの読み込み
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}


void Enemy::Update()
{
	// ワールドの初期化
	worldTransform_.Initialize();

	 float eMoveSpeed = 2.0f;
	static float timer = 0;
	timer += 0.1;
	if (timer >= 2.4)eMoveSpeed = 0;

	Vector3 eMove = { 0.0f, 0.0f, eMoveSpeed };


	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ -= eMove;

	// 行列の更新
	worldTransform_.MatrixUpdate();
}


void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}