#pragma once
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {

public:
	//初期化
	void Initialize(Model* model,ViewProjection* viewProjection); 
	
	//更新
	void Update();

	//描画
	void Draw();

private:

	/// ワールド変換
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	/// モデル
	Model* model_ = nullptr;

	Vector3 velocity_ = {};

	Vector3 position_ = {25.0f,3.0f,0.0f};

	static inline const float kWalkSpeed = 0.125f;

	//最初の角度
	static inline const float kWalkMotionAngleStart = 90.0f;

	//最後の角度
	static inline const float kWalkMotionAngleEnd = 180.0f;

	//アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 2.0f;

	//経過時間
	float walkTimer_ = 0.0f;

};