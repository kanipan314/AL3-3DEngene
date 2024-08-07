#include "Enemy.h"
#include "numbers"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

float DegreesToRadians(float degrees) { return degrees * (M_PI / 180.0f); }

void Enemy::Initialize(Model* model, ViewProjection* viewProjection ,const Vector3& position) {

	model_ = model;

	viewProjection_ = viewProjection;

	//速度の設定
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;

	/// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
}

void Enemy::Update() {

	// 移動
	position_ += velocity_;

	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	/*worldTransform_.translation_.z += velocity_.z;*/
	

	//タイム―を加算
	walkTimer_ += 1.0f / 60.0f;

	//回転アニメーション
	float param = std::sinf(2 * M_PI * walkTimer_);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 20.0f;
	worldTransform_.rotation_.x = DegreesToRadians(radian);

	// 行列計算
	worldTransform_.UpdateMatrix();

	worldTransform_.matWorld_.m;
	worldTransform_.TransferMatrix();

}

void Enemy::Draw() {

	 model_->Draw(worldTransform_, *viewProjection_);

}

Vector3 Enemy::GetWorldPosition() {
	
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;

	}

AABB Enemy::GetAABB() { 
	
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - 4.0f / 2.0f, worldPos.y - 4.0f / 2.0f, worldPos.z - 4.0f / 2.0f};
	aabb.max = {worldPos.x + 4.0f / 2.0f, worldPos.y + 4.0f / 2.0f, worldPos.z + 4.0f / 2.0f};

	return aabb;
	
 }

void Enemy::OnCollision(const Player* player) {

	(void)player;

}
