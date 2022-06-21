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
	textureHandle_ = TextureManager::Load("mario.jpg");

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 接近フェーズ初期化
	ApproachInitialize();
}


void Enemy::Update()
{
	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});
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

	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}
}


void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachUpdate()
{
	eMove = { 0.0f, 0.0f, -eMoveSpeed };
	// 移動(ベクトルを加算)
	worldTransform_.translation_ += eMove;
	// 規定に位置に達したら離脱
	if (worldTransform_.translation_.z < -0.0f)
	{
		phase_ = Phase::Leave;
	}

	// 発射タイマーカウントダウン
	fireTimer_--;
	// 指定時間に達した
	if (fireTimer_ < 0)
	{
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireInterval;
	}
}

void Enemy::LeaveUpdate()
{
	eMove = { eMoveSpeed, eMoveSpeed, 0.0f };
	// 移動(ベクトルを加算)
	worldTransform_.translation_ += eMove;
}

void Enemy::Fire()
{
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	//// 速度ベクトルを自機の向きに合わせて回転させる
	//velocity = direction(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachInitialize()
{
	// 発射タイマーを初期化
	fireTimer_ = kFireInterval;
}
