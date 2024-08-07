#pragma once
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <array>
#include <assert.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


class DeatheParticle {

public:
	void Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position);

	void Update();

	void Draw();

	bool isFinished() const { return isFinished_; }

private:

	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> worldTransforms_;

	//存続時間
	static inline const float kDuration = 2.0f;
	//移動の速さ
	static inline const float kSpeed = 0.1f;
	//分割角度
	static inline const float kAngleUnit = 2 * M_PI / 8.0f;

	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;

	// 色変更オブジェクト
	ObjectColor objectColor_;
	// 色の数値
	Vector4 color_;
};
