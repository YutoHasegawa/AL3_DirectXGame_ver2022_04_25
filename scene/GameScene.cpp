#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <math/Matrix4.h>
#include <random>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete modelSkydome_;
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
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// 初期座標
	Vector3 position = { 10.0f, 2.0f, 50.0f };
	// カメラの初期座標
	Vector3 cameraPosition = { 0, 0, -50.0f };
	Vector3 cameraRotation = { 0, 0, 0 };

	// 敵キャラの生成
	LoadEnemyPopData();
	// 関数
	// 敵キャラの初期化
	std::unique_ptr<Enemy> enemy_ = std::make_unique<Enemy>();
	enemy_->initialize(model_, position);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_.get());
	enemy_->SetGameScene(this);
	enemys_.push_back(std::move(enemy_));
	//

	// 天球の作成
	skydome_ = std::make_unique<Skydome>();
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("sky", true);
	// 天球の初期化
	skydome_->Initialize(modelSkydome_);
	// レールカメラの初期化
	railCamera_ = std::make_unique<RailCamera>();
	railCamera_->Initialize(cameraPosition, cameraRotation);
	//viewProjection_ = railCamera_->GetViewProjection();
	// 親子関係
	player_->SetPlayer(railCamera_->GetWorldTransForm());
}

void GameScene::Update() {
	//天球の更新
	skydome_->Update();
	// レールカメラの更新
	// railcameraをゲームシーンのカメラに適応する
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	// ビュープロジェクションの転送
	viewProjection_.TransferMatrix();
	railCamera_->Update();
	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update();
		UpdateEnemyPopCommands();
	}
	// デスフラグの立った敵を削除
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy)
		{
			return enemy->IsDead();
		});
	// デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	// 弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	//enemys_.push_back(std::move(enemy_));

	for (std::unique_ptr<Enemy>& enemy_ : enemys_)
	{
		// 当たり判定の更新
		CheckAllCollisions();
	}

	//// 弾の管理
	//int count = 0;
	//const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
	//for (const std::unique_ptr<EnemyBullet>& eBullet : enemyBullets)
	//{
	//	count++;
	//}
	//debugText_->SetPos(0, 0);
	//debugText_->Printf("%d", count);

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
	skydome_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	for (std::unique_ptr<Enemy>& enemy_ : enemys_)
	{
		if (enemy_ != nullptr)
		{
			enemy_->Draw(viewProjection_);
		}
	}
	// 弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
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

void GameScene::CheckAllCollisions()
{
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bullets_;

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 弾と弾の交差判定
		if (Collisions(posA, posB))
		{
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	for (std::unique_ptr<Enemy>& enemy_ : enemys_)
	{
		if (enemy_ != nullptr)
		{
			posA = enemy_->GetWorldPosition();
		}

		// 敵キャラと自弾すべての当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
		{
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			// 弾と弾の交差判定
			if (Collisions(posA, posB))
			{

				// 敵キャラの衝突時コールバックを呼び出す
				enemy_->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& ebullet : enemyBullets)
	{
		// 敵弾の座標
		posA = ebullet->GetWorldPosition();

		for (const std::unique_ptr<PlayerBullet>& pbullet : playerBullets)
		{
			// 自弾の座標
			posB = pbullet->GetWorldPosition();

			// 弾と弾の交差判定
			if (Collisions(posA, posB))
			{
				// 自弾の衝突時コールバックを呼び出す
				pbullet->OnCollision();

				// 敵弾の衝突時コールバックを呼び出す
				ebullet->OnCollision();
			}
		}
	}
#pragma endregion
}

bool GameScene::Collisions(Vector3 posA, Vector3 posB)
{
	return (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z) < 0.5f;
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	// リストに登録する
	bullets_.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemyPopData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	// 待機処理
	if (waitFlag_)
	{
		waitTimer_--;
		if (waitTimer_ <= 0)
		{
			// 待機完了
			waitFlag_ = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (std::getline(enemyPopCommands, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		std::getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0)
		{
			// コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0)
		{
			// x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			enemyPop(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			std::getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			waitFlag_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::enemyPop(Vector3 position)
{
	// 敵キャラの初期化
	std::unique_ptr<Enemy> enemy_ = std::make_unique<Enemy>();
	enemy_->initialize(model_, position);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_.get());
	enemy_->SetGameScene(this);
	enemys_.push_back(std::move(enemy_));
}
