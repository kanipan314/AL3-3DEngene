#define NOMINMAX
#include "CaneraController.h"
#include "Player.h"
#include <algorithm>

Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {

	Vector3 answer;
	answer.x = t * a.x + (1.0f - t) * b.x;
	answer.y = t * a.y + (1.0f - t) * b.y;
	answer.z = t * a.z + (1.0f - t) * b.z;

	return answer;
}

void CaneraController::Initialize(ViewProjection* viewProjection) {
	viewProjection_ = viewProjection;
}

void CaneraController::Update() {

	// 追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	Vector3 targerVelocity = target_->GetVelocity();

	cameraTargetPosition.x = targetWorldTransform.translation_.x + targeOffset_.x + targerVelocity.x * kVelocityBias;
	cameraTargetPosition.y = targetWorldTransform.translation_.y + targeOffset_.y + targerVelocity.y * kVelocityBias;
	
	//移動範囲制限
	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, movableArea_.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, movableArea_.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, movableArea_.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, movableArea_.top);

	//座標補間によりゆったり追従
	viewProjection_->translation_ = Lerp(viewProjection_->translation_, cameraTargetPosition, kInterpolationRate);

	//追従対象が画面外に出ないように補正
	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, targetWorldTransform.translation_.x + margin.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, targetWorldTransform.translation_.x + margin.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, targetWorldTransform.translation_.y + margin.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, targetWorldTransform.translation_.y + margin.top);


	// 行列計算
	viewProjection_->UpdateMatrix();
}

void CaneraController::Reset() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対象とオフセットからカメラの座標を計算
	viewProjection_->translation_.x = targetWorldTransform.translation_.x + targeOffset_.x;
	viewProjection_->translation_.y = targetWorldTransform.translation_.y + targeOffset_.y;
	viewProjection_->translation_.z = targetWorldTransform.translation_.z + targeOffset_.z;

	
}
