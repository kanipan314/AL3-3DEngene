#pragma once
#include "AABB.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

//enum EnemyCorner {
//	kEnemyRightBottom, // 右下
//	kEnemyLeftBottom,  // 左下
//	kEnemyRightTom,    // 右上
//	kEnemyLeftTop,     // 左上
//
//	kEnemyNumCorner // 要素数
//};
//
//// マップとの当たり判定情報
//struct EnemyCollisionMapInfo {
//
//	bool TopFlag = false;
//	bool BottomFlag = false;
//	bool WallFlag = false;
//	Vector3 velocity;
//};

class Player;

class Enemy {

public:
	// 初期化
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position_);

	// 更新
	void Update();

	// 描画
	void Draw();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();
	// 衝突応答
	void OnCollision(const Player* player);

private:
	/// ワールド変換
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	/// モデル
	Model* model_ = nullptr;

	Vector3 velocity_ = {};

	Vector3 position_ = {25.0f, 3.0f, 0.0f};

	//// マップチップによるフィールド
	//MapChipField* enemyMapChipField_ = nullptr;

	static inline const float kWalkSpeed = 0.125f;

	// 最初の角度
	static inline const float kWalkMotionAngleStart = 90.0f;

	// 最後の角度
	static inline const float kWalkMotionAngleEnd = 720.0f;

	// アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 2.0f;

	// 経過時間
	float walkTimer_ = 0.0f;

	// 敵の当たり判定
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 0.8f;
	static inline const float kAttenuationLanding = 0.8f;

	// 地面との当たり判定用
	static inline const float kCheckLanding = -0.5f;

	//ランダムな速度
	Vector3 randSpeed_ = 0.0f;

	
};
