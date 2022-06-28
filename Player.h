#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include <PlayerBullet.h>
#include <memory>
#include <list>

/// <summary>
/// 自キャラ
/// </summary>
class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 行列の合成
	/// </summary>
	//void MatrixUpdate();

	/// <summary>
	/// 画面外処理
	/// </summary>
	void ScreenOut();
	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();
	/// <summary>
	/// 攻撃(弾の発射)
	/// </summary>
	void Attack();
	/// <summary>
	/// ベクトルと行列の計算
	/// </summary>
	/// <param name="velocity">速度</param>
	/// <param name="matWorld">更新後のワールド座標</param>
	/// <returns></returns>
	Vector3 direction(const Vector3& velocity, const Matrix4& matWorld);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	/// <summary>
	/// 衝突したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	// 弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 入力処理するため
	Input* input_ = nullptr;
	// デバッグテキスト
	DebugText* debugText_ = nullptr;
	// 弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};