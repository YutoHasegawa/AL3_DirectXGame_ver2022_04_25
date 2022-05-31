#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <math/Matrix4.h>
#include <random>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの作成
	model_ = Model::Create();
	// デバッグカメラの作成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//debugCamera_->GetViewProjection()
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定(回転角用)
	std::uniform_real_distribution<float> rotDist(0.0f, (3.1415 * 2));
	//乱数範囲の指定(座標用)
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	// ワールドトランスフォームの初期化
	// 親
	worldTransforms_[PartId::kRoot].Initialize();
	// 子
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].translation_ = { 0, 4.5f, 0 };
	// 行列の合成
	worldTransforms_[PartId::kSpine].matWorld_.ToMatTrans(worldTransforms_[PartId::kSpine].translation_);
	// 行列の転送
	worldTransforms_[PartId::kSpine].TransferMatrix();


	//上半身
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kChest].translation_ = { 0, -4.5f, 0 };
	worldTransforms_[PartId::kChest].matWorld_.ToMatTrans(worldTransforms_[PartId::kChest].translation_);
	worldTransforms_[PartId::kChest].TransferMatrix();

	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kHead].translation_ = { 0, 4.5f, 0 };
	worldTransforms_[PartId::kHead].matWorld_.ToMatTrans(worldTransforms_[PartId::kHead].translation_);
	worldTransforms_[PartId::kHead].TransferMatrix();

	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmL].translation_ = { -4.5f, 0, 0 };
	worldTransforms_[PartId::kArmL].matWorld_.ToMatTrans(worldTransforms_[PartId::kArmL].translation_);
	worldTransforms_[PartId::kArmL].TransferMatrix();

	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmR].translation_ = { 4.5f, 0, 0 };
	worldTransforms_[PartId::kArmR].matWorld_.ToMatTrans(worldTransforms_[PartId::kArmR].translation_);
	worldTransforms_[PartId::kArmR].TransferMatrix();

	//下半身
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kHip].translation_ = { 0, -9.0f, 0 };
	worldTransforms_[PartId::kHip].matWorld_.ToMatTrans(worldTransforms_[PartId::kHip].translation_);
	worldTransforms_[PartId::kHip].TransferMatrix();

	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegL].translation_ = { -4.5f, -4.5f, 0 };
	worldTransforms_[PartId::kLegL].matWorld_.ToMatTrans(worldTransforms_[PartId::kLegL].translation_);
	worldTransforms_[PartId::kLegL].TransferMatrix();

	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegR].translation_ = { 4.5f, -4.5f, 0 };
	worldTransforms_[PartId::kLegR].matWorld_.ToMatTrans(worldTransforms_[PartId::kLegR].translation_);
	worldTransforms_[PartId::kLegR].TransferMatrix();


	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	//debugCamera_->Update();
	//キャラクターの移動ベクトル
	for (int i = 0; i < PartId::kNumPartId; i++)
	{
		worldTransforms_[i].Initialize();
	}

	Vector3 move = { 0, 0, 0 };

	const float kMoveSpeed = 0.2f;
	const float kChestSpeed = 0.05f;
	const float kHipSpeed = 0.05f;

	//キーを押したら移動量を変化
	if (input_->PushKey(DIK_LEFT))
	{
		move = { -kMoveSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		move = { kMoveSpeed, 0, 0 };
	}

	//上半身の移動
	if (input_->PushKey(DIK_U))
	{
		worldTransforms_[PartId::kChest].rotation_.y -= kChestSpeed;
	}
	if (input_->PushKey(DIK_I))
	{
		worldTransforms_[PartId::kChest].rotation_.y += kChestSpeed;
	}

	//下半身の移動
	if (input_->PushKey(DIK_J))
	{
		worldTransforms_[PartId::kHip].rotation_.y -= kHipSpeed;
	}
	if (input_->PushKey(DIK_K))
	{
		worldTransforms_[PartId::kHip].rotation_.y += kHipSpeed;
	}

	//translationにmoveを加算
	worldTransforms_[PartId::kRoot].translation_ += move;

	for (int i = 0; i < PartId::kNumPartId; i++)
	{
		worldTransforms_[i].matWorld_.MatrixUpdate
		(
			worldTransforms_[i].scale_,
			worldTransforms_[i].rotation_,
			worldTransforms_[i].translation_
		);
		if (1 <= i)
		{
			worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
		}
			worldTransforms_[i].TransferMatrix();
	}

	//デバッグ用表示
	debugText_->SetPos(50, 110);
	debugText_->Printf(
		"translation:(%f, %f, %f)",
		worldTransforms_[PartId::kRoot].translation_.x,
		worldTransforms_[PartId::kRoot].translation_.y,
		worldTransforms_[PartId::kRoot].translation_.z
	);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// デバッグカメラの描画
	//debugCamera_->GetViewProjection()
	for (int i = 2; i < PartId::kNumPartId; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>


	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

float GameScene::XMConvertToRadians(float angle)
{
	return angle * 3.1415 / 180;
}

float GameScene::MinNum(float num, float num2)
{
	if (num < num2)
	{
		return num2;
	}
	return num;
}

float GameScene::MaxNum(float num, float num2)
{
	if (num > num2)
	{
		return num2;
	}
	return num;
}

float GameScene::Clamp(float min, float max, float num)
{
	if (max < num)
	{
		return max;
	}
	else if (num < min)
	{
		return min;
	}
	return num;
}
