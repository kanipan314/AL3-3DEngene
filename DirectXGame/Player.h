#pragma once
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "AABB.h"

/// <summary>
/// 自キャラ
/// </summary>

enum class LRDirection {
	kRigth,
	kLeft,
};

enum Corner {
	kRightBottom,   //右下
	kLeftBottom,	//左下
	kRightTom,		//右上
	kLeftTop,		//左上

	kNumCorner		//要素数
};

// マップとの当たり判定情報
struct CollisionMapInfo {

	bool TopFlag = false;
	bool BottomFlag = false;
	bool WallFlag = false;
	Vector3 velocity;
};

class MapChipField;

class Enemy;

class Player {

	public:
		/// 初期化
	    void Initialize(Model* model,ViewProjection* viewProjection,const Vector3& position);

	    /// 更新
	    void Update();

		///　描画
	    void Draw();

		//ワールドトランスフォーム
	    const WorldTransform& GetWorldTransform();

		const Vector3& GetVelocity() const { return velocity_; };

		void SetMapChipField(MapChipField* mapChipField) { 
			mapChipField_ = mapChipField; 
		};

		void CollisionFlag(CollisionMapInfo& info);

		static Vector3 CornerPosition(const Vector3& center, Corner corner);
		
		void TopCollision(CollisionMapInfo info);
	    void BottomCollision(CollisionMapInfo info);
	    void LeftCollision(CollisionMapInfo info);
	    void RightCollision(CollisionMapInfo info);

		void Move(const CollisionMapInfo& info);
	    void CeilingMove(const CollisionMapInfo& info);

		void ChangeGround(const CollisionMapInfo& info);

		//ワールド座標を取得
	    Vector3 GetWorldPosition();

		//AABBを取得
	    AABB GetAABB();

		//衝突応答
	    void OnCollision(const Enemy* enemy);

	private:
		///ワールド変換
	    WorldTransform worldTransform_;
		ViewProjection* viewProjection_ = nullptr;
		///モデル
	    Model* model_ = nullptr;
		///テクスチャハンドル
	    uint32_t textureHandle_ = 0;

		Vector3 velocity_ = {};
		//加速度
	    static inline const float kAcceleration = 0.10f;
		//減速率
	    static inline const float kAttenuation = 0.05f;
		//最大速度
	    static inline const float kLimitRunSpeed = 0.25f;
		//旋回時間＜秒＞
		static inline const float kTimeTurn = 0.3f;
		//旋回開始時の角度
	    float turnFirstRotationY_ = 0.0f;
		//旋回タイマー
	    float turnTimer_ = 0.0f;

		//接地状態フラグ
	    bool onGround_ = true;
		//重力加速度
	    static inline const float kGravityAcceleration = 0.2f;
		//最大落下速度
	    static inline const float kLimitFallSpeed = 1.5f;
		//ジャンプ初速
	    static inline const float kJumpAcceleration = 1.5f;

		LRDirection lrDirection_ = LRDirection::kRigth;

		//マップチップによるフィールド
	    MapChipField* mapChipField_ = nullptr;

		//キャラの当たり判定
	    static inline const float kWidth = 0.8f;
	    static inline const float kHeight = 0.8f;


		static inline const float kBlank = 0.8f;
	    static inline const float kAttenuationLanding = 0.8f;

		

};

