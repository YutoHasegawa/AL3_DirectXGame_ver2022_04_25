#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 度数法での表記
	/// </summary>
	/// <param name="angle">出したい角度</param>
	/// <returns>その角度を返す</returns>
	float XMConvertToRadians(float angle);

	/// <summary>
	/// 値が小さい方を返す
	/// </summary>
	/// <param name="num">比較したい値</param>
	/// <param name="num2">最小値</param>
	/// <returns></returns>
	float MinNum(float num, float num2);

	/// <summary>
	/// 値が大きい方を返す
	/// </summary>
	/// <param name="num">比較したい値</param>
	/// <param name="num2">最大値</param>
	/// <returns></returns>
	float MaxNum(float num, float num2);

	/// <summary>
	/// 最大値最小値を超えないようにする
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <param name="num">比較したい値</param>
	/// <returns>min, maxを超えない値を返す</returns>
	float Clamp(float min, float max, float num);

	enum PartId {
		kRoot,	//大元
		kSpine,	//脊髄
		kChest,	//胸
		kHead,	//頭
		kArmL,	//左腕
		kArmR,	//右腕
		kHip,	//尻
		kLegL,	//左足
		kLegR,	//右足

		kNumPartId
	};


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransforms_[100];
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//カメラ上方向の角度
	float viewAngle = 0.0f;
};
