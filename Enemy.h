#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DebugText.h"
#include <EnemyBullet.h>
#include <memory>
#include <list>

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
	static const int kFireInterval = 60;

	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void ApproachInitialize();
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
	const float eMoveSpeed = 0.1f;
	// 発射タイマー
	int32_t fireTimer_ = 0;
	// 弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
};

