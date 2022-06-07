#include "Player.h"
#include "assert.h"

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	// NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ワールド変換の初期化
	worldTransform_.Initialize();

}

void Player::Update()
{
	// キャラクター旋回処理
	Rotate();
	// キャラクター移動処理
	Move();
	// キャラクター攻撃処理
	Attack();
	// 弾更新
	if (bullet_) { bullet_->Update(); }

	// キャラクター画面外処理
	ScreenOut();

	// 行列更新
	worldTransform_.MatrixUpdate();

	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"transform:(%f, %f, %f)", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z
	);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"rotation:(%f, %f, %f)", worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z
	);
}

void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾描画
	if (bullet_) { bullet_->Draw(viewProjection); }
}

void Player::Move()
{
	// ワールドの初期化
	worldTransform_.Initialize();

	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	
	// 移動量
	const float kCharacter = 0.2f;

	// 押したキーで移動
	if (input_->PushKey(DIK_W))
	{
		move.y = kCharacter;
	}
	if (input_->PushKey(DIK_S))
	{
		move.y = -kCharacter;
	}
	if (input_->PushKey(DIK_A))
	{
		move.x = -kCharacter;
	}
	if (input_->PushKey(DIK_D))
	{
		move.x = kCharacter;
	}

	worldTransform_.translation_ += move;
}

void Player::ScreenOut()
{
	// 移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

}

//void Player::MatrixUpdate()
//{
//	// 行列の更新
//	worldTransform_.matWorld_.MatrixUpdate
//	(
//		worldTransform_.scale_,
//		worldTransform_.rotation_,
//		worldTransform_.translation_
//	);
//
//	// ワールドの更新
//	worldTransform_.TransferMatrix();
//}

void Player::Rotate()
{
	const float rotationSpeed = 0.01f;

	if (input_->PushKey(DIK_U))
	{
		worldTransform_.rotation_.y -= rotationSpeed;
	}
	else if (input_->PushKey(DIK_I))
	{
		worldTransform_.rotation_.y += rotationSpeed;
	}
}

void Player::Attack()
{
	if (input_->PushKey(DIK_SPACE))
	{
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		// 弾を登録する
		bullet_ = newBullet;
	}
}
