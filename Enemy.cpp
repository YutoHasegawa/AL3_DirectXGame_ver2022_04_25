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

	switch (phase_)
	{
	case Phase::Approach:
	default:
		ApproachUpdate();
		break;
	case Phase::Leave:
		LeaveUpdate();
		break;
	}
	// 行列の更新
	worldTransform_.MatrixUpdate();
}


void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachUpdate()
{
	eMove = { 0.0f, 0.0f, -eMoveSpeed };
	// 移動(ベクトルを加算)
	worldTransform_.translation_ += eMove;
	// 規定に位置に達したら離脱
	if (worldTransform_.translation_.z < -30.0f)
	{
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate()
{
	eMove = { -eMoveSpeed, eMoveSpeed, 0.0f };
	// 移動(ベクトルを加算)
	worldTransform_.translation_ += eMove;
}