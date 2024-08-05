#pragma once
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <array>

class DeatheParticle {

public:
	void Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position);

	void Update();

	void Draw();

private:

	Model* model_ = nullptr;
	ViewProjection* viewProjection_ = nullptr;

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	std::array<WorldTransform, kNumParticles> worldTransforms_;

};
