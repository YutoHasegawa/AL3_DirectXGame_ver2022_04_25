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

	for (WorldTransform& worldTransform : worldTransforms_)
	{
		// ワールドトランスフォームの初期化
		worldTransform.Initialize();
		// X, Y, Z方向のスケーリングを設定
		worldTransform.scale_ = { 1.0f, 1.0f, 1.0f };
		// 回転
		worldTransform.rotation_ = { rotDist(engine), rotDist(engine), rotDist(engine) };
		// 平行移動
		worldTransform.translation_ = { posDist(engine), posDist(engine), posDist(engine) };

		// 行列の合成
		worldTransform.matWorld_.MatrixUpdate(worldTransform.translation_, worldTransform.rotation_, worldTransform.scale_);

		// 行列の転送
		worldTransform.TransferMatrix();
	}
	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = XMConvertToRadians(10.0f);
	//アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;
	// ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	//debugCamera_->Update();
	//上キーで視野が広がる
	if (input_->PushKey(DIK_W))
	{
		viewProjection_.fovAngleY += 0.01f;
		viewProjection_.fovAngleY = MaxNum(viewProjection_.fovAngleY, 3.1415f);
	}
	//下キーで視野が狭まる
	else if (input_->PushKey(DIK_S))
	{
		viewProjection_.fovAngleY -= 0.01f;
		viewProjection_.fovAngleY = MinNum(viewProjection_.fovAngleY, 0.01f);
	}
	//上下キーでニアクリップ距離を増減
	if (input_->PushKey(DIK_UP))
	{
		viewProjection_.nearZ += 0.1f;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		viewProjection_.nearZ -= 0.1f;
	}

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 110);
	debugText_->Printf(
		"fovAngleY(Degree):%f", XMConvertToRadians(viewProjection_.fovAngleY)
		);
	//ニアクリップ
	debugText_->SetPos(50, 130);
	debugText_->Printf(
		"nearZ:%f", viewProjection_.nearZ
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
	for (WorldTransform& worldTransform : worldTransforms_)
	{
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
		//debugCamera_->GetViewProjection()
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
