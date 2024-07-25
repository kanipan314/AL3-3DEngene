#define NOMINMAX
#include "Player.h"
#include "Input.h"
#include "MapChipField.h"
#include "cassert"
#include "numbers"
#include <algorithm>

// float TurnRotation(float timer) { return timer; }

float Lerp(float a, float b, float t) {

	float answer;
	answer = t * a + (1.0f - t) * b;

	return answer;
}

void Player::CollisionFlag(CollisionMapInfo& info) {
	TopCollision(info);
	BottomCollision(info);
	RightCollision(info);
	LeftCollision(info);
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offSetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	Vector3 answer;
	answer.x = center.x + offSetTable[static_cast<uint32_t>(corner)].x;
	answer.y = center.y + offSetTable[static_cast<uint32_t>(corner)].y;
	answer.z = center.z + offSetTable[static_cast<uint32_t>(corner)].z;

	return answer;
}

void Player::TopCollision(CollisionMapInfo info) {
	// 上昇あり？
	if (info.velocity.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = Player::CornerPosition(
		    {worldTransform_.translation_.x + info.velocity.x, worldTransform_.translation_.y + info.velocity.y, worldTransform_.translation_.z + info.velocity.z}, static_cast<Corner>(i));
	}
	info.TopFlag = false;

	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定

	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kLeftTop]);

	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定

	indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kRightTom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
}
void Player::BottomCollision(CollisionMapInfo info) {
	info.BottomFlag = false;
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = Player::CornerPosition(
		    {worldTransform_.translation_.x + info.velocity.x, worldTransform_.translation_.y + info.velocity.y, worldTransform_.translation_.z + info.velocity.x}, static_cast<Corner>(i));
	}
	info.BottomFlag = false;
}
void Player::RightCollision(CollisionMapInfo info) {
	info.BottomFlag = false;
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = Player::CornerPosition(
		    {worldTransform_.translation_.x + info.velocity.x, worldTransform_.translation_.y + info.velocity.y, worldTransform_.translation_.z + info.velocity.x}, static_cast<Corner>(i));
	}
	info.WallFlag = false;
}
void Player::LeftCollision(CollisionMapInfo info) {
	info.BottomFlag = false;
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = Player::CornerPosition(
		    {worldTransform_.translation_.x + info.velocity.x, worldTransform_.translation_.y + info.velocity.y, worldTransform_.translation_.z + info.velocity.x}, static_cast<Corner>(i));
	}
	info.WallFlag = false;
}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	/// Nullチェック
	assert(model);
	/// 引き渡し
	model_ = model;
	// textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	/// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	// 移動入力

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度を値をコピー
	collisionMapInfo.velocity = velocity_;
	// マップ衝突チェック
	CollisionFlag(collisionMapInfo);

	// 接地状態
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {0, 0, 0};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に急ブレーキ
					velocity_.x *= (1.0f - kAcceleration);
				}
				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRigth) {
					lrDirection_ = LRDirection::kRigth;
					turnFirstRotationY_ = std::numbers::pi_v<float> * 3.0f / 2.0f;
					turnTimer_ = 1.0f;
				}

				if (turnTimer_ > 0.0f) {

					turnTimer_ -= 0.1f;

					// 左右の自キャラ角度テーブル
					float destinationRotationYTable[2] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

					// 状態に応じた角度を取得する
					float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
					// 自キャラの角度を設定

					worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, destinationRotationY, turnTimer_);

					/*if (turnTimer_ <= 0.0f) {
					,
					    worldTransform_.rotation_.y = destinationRotationY;
					}*/
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に急ブレーキ
					velocity_.x *= (1.0f - kAcceleration);
				}

				acceleration.x -= kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = std::numbers::pi_v<float> / 2.0f;
					turnTimer_ = 1.0f;
				}
				if (turnTimer_ > 0.0f) {

					turnTimer_ -= 0.1f;

					float destinationRotationYTable[2] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

					// 状態に応じた角度を取得する
					float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
					// 自キャラの角度を設定
					worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, destinationRotationY, turnTimer_);
				}
			}
			// 加速/減速
			velocity_.x += acceleration.x;

			// 最大速度制限

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		} else {
			// 入力されていないとき減速
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_.y += kJumpAcceleration;
			// 空中状態に移行
			onGround_ = false;
		}
		// 空中
	} else {
		// 落下速度
		velocity_.y += -kGravityAcceleration;
		// 落下速度
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

		bool landing = false;

		// 地面との当たり判定
		// 降下中？

		if (velocity_.y < 0) {
			// y座標が地面以下になったら着地
			if (worldTransform_.translation_.y <= 1.0f) {
				landing = true;
			}
		}

		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度の原則
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 行列計算
	worldTransform_.UpdateMatrix();

	worldTransform_.matWorld_.m;
	worldTransform_.TransferMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }

const WorldTransform& Player::GetWorldTransform() { return worldTransform_; }
