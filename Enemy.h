#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include "EnemyBullet.h"
#include <memory>
#include "Vector3.h"


// 自機クラスの前方宣言
class Player;

// GameSceneの前方宣言
class GameScene;

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	//Enemy();
	/// <summary>
	/// 初期化
	/// </summary>
	void initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 敵に近づく
	/// </summary>
	void ApproachUpdate();

	/// <summary>
	/// 画面外に避ける
	/// </summary>
	void LeaveUpdate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Fire();

	// 発射間隔
	static const int kFireInterval = 120;

	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void ApproachInitialize();

	//セッター
	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	/// <summary>
	/// 衝突したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	//// 弾リストを取得
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }


	bool IsDead() const { return isDead_; }
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	enum class Phase
	{
		Approach,		//接近する
		Leave,			//離脱する
	};
	// フェーズ
	Phase phase_ = Phase::Approach;
	// 移動
	Vector3 eMove = { 0, 0, 0 };
	// 移動する速さ
	const float eMoveSpeed = 0.0f;
	// 発射タイマー
	int32_t fireTimer_ = 120;
	//// 弾
	//std::list<std::unique_ptr<EnemyBullet>> bullets_;
	// 自キャラ
	Player* player_ = nullptr;
	// ゲームシーン
	GameScene* gameScene_ = nullptr;
	// デスフラグ
	bool isDead_ = false;
};

