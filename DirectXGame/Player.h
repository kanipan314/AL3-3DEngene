#pragma once
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 自キャラ
/// </summary>

enum class LRDirection {
	kRigth,
	kLeft,
};

class Player {

	public:
		/// 初期化
	    void Initialize(Model* model,ViewProjection* viewProjection,const Vector3& position);

	    /// 更新
	    void Update();

		///　描画
	    void Draw();

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
	    static inline const float kGravityAcceleration = 0.98f;
		//最大落下速度
	    static inline const float kLimitFallSpeed = 0.25f;
		//ジャンプ初速
	    static inline const float kJumpAcceleration = 1.50f;

		LRDirection lrDirection_ = LRDirection::kRigth;

};

