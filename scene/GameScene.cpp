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
	//カメラ視点座標を設定
	//viewProjection_.eye = { 0, 0, -10 };
	//カメラ注視点座標を設定
	viewProjection_.target = { 10, 0, 0 };
	//カメラ上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = { cosf(3.14 / 4.0f), sinf(3.14 / 4.0f), 0.0f };
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {
	//debugCamera_->Update();
	//視点の移動ベクトル
	Vector3 move = { 0, 0, 0 };
	//注視点のベクトル
	Vector3 targetMove = { 0, 0, 0 };
	//視点の移動深さ
	const float kEyeSpeed = 0.2f;
	//注視点の移動深さ
	const float kTargetSpeed = 0.2f;
	//上方向の回転速さ(ラジアン / frame)
	const float kUpRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W))
	{
		move = { 0, 0, kEyeSpeed };
	}
	else if (input_->PushKey(DIK_S)) 
	{
		move = { 0, 0, -kEyeSpeed };
	}
	//押した方向で移動ベクトルを変更(注視点)
	if (input_->PushKey(DIK_LEFT))
	{
		targetMove = { kTargetSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHT))
	{
		targetMove = { -kTargetSpeed, 0, 0 };
	}
	//押した方向で移動ベクトルを変更(回転処理)
	if (input_->PushKey(DIK_SPACE))
	{
		viewAngle += kUpRotSpeed;
		//2πを超えたら0に戻す
		viewAngle = fmodf(viewAngle, 3.14 * 2.0f);
	}

	//視点移動(ベクトルの加算)
	viewProjection_.eye += move;
	//視点移動(注視点)
	viewProjection_.target += targetMove;
	//上方向ベクトルを計算(半径1の円周上の座標)
	viewProjection_.up = { cosf(viewAngle), sinf(viewAngle), 0.0f };
	//行列の再計算
	viewProjection_.UpdateMatrix();
	
	//デバック用表示
	//視点移動
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"eye:(%f, %f, %f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z
	);
	//注視点
	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"target:(%f, %f, %f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z
	);
	//回転
	debugText_->SetPos(50, 90);
	debugText_->Printf(
		"up:(%f, %f, %f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z
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
