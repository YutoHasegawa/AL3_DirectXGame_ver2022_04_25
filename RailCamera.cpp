#include "RailCamera.h"
#include "GameScene.h"
#include "DebugText.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	// ワールドトランスフォームの初期設定
	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	// ビュープロジェクションの初期設定
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	const Vector3 tMove = { 0, 0, 0.1f };
	const Vector3 rMove = { 0, 0, 0 };
	worldTransform_.translation_ += tMove;
	worldTransform_.rotation_ += rMove;
	// 行列の更新
	worldTransform_.MatrixUpdate();

	viewProjection_.eye = worldTransform_.translation_;
	// ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	// レールカメラの回転を反映
	forward = worldTransform_.matWorld_.direction(forward, worldTransform_.matWorld_);
	// 視点から前方に適当な距離進んだ位置が注視点
	viewProjection_.target = viewProjection_.eye + forward;
	// ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	// レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.up = worldTransform_.matWorld_.direction(up, worldTransform_.matWorld_);
	// ビュープロジェクションを更新
	viewProjection_.UpdateMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	// デバッグ用表示
	debugText_->SetPos(20, 100);
	debugText_->Printf("RailCamera Pos:(%f, %f, %f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

}
