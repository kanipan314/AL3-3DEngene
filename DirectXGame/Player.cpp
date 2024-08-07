#define NOMINMAX
#include "Player.h"
#include "ImGuiManager.h"
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

void Player::TopCollision(CollisionMapInfo& info) {
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

	if (hit) {

		// めり込みを排除する
		indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kRightTom]);
		// めり込み先のブロック範囲
		BlockRect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kCheckLanding + kHeight / 2.0f));
		// 天井に当たった子を記録する
		info.TopFlag = true;
	}

	Move(info);
}
void Player::BottomCollision(CollisionMapInfo& info) {

	// 下降？
	if (info.velocity.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = Player::CornerPosition(
		    {worldTransform_.translation_.x + info.velocity.x, worldTransform_.translation_.y + info.velocity.y, worldTransform_.translation_.z + info.velocity.x}, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真下の当たり判定を行う
	bool hit = false;

	// 左下点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kLeftBottom]);

	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		// めり込みを排除する
		indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kRightBottom]);
		// めり込み先のブロック範囲
		BlockRect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		info.velocity.y = std::max(0.0f, (rect.bottom - worldTransform_.translation_.y) - (kCheckLanding + kHeight / 2.0f)); // ｙ移動量5-5-1.8/2//ムーブがゼロ//

		// 地面に当たった子を記録する
		info.BottomFlag = true;
	}
}
void Player::RightCollision(CollisionMapInfo& info) {

	// 右移動あり
	if (info.velocity.x <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = Player::CornerPosition(
		    {worldTransform_.translation_.x + info.velocity.x, worldTransform_.translation_.y + info.velocity.y, worldTransform_.translation_.z + info.velocity.x}, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真下の当たり判定を行う
	bool hit = false;

	// 右上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kRightTom]);

	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		// めり込みを排除する
		indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kRightBottom]);
		// めり込み先のブロック範囲
		BlockRect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		info.velocity.x = std::max(0.0f, (rect.right - worldTransform_.translation_.x) + (kCheckLanding + kWidth / 2.0f));
		info.WallFlag = true;
	}

	WallMove(info);
}

void Player::LeftCollision(CollisionMapInfo& info) {

	// 右移動あり
	if (info.velocity.x >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); i++) {
		positionsNew[i] = Player::CornerPosition(
		    {worldTransform_.translation_.x + info.velocity.x, worldTransform_.translation_.y + info.velocity.y, worldTransform_.translation_.z + info.velocity.x}, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真下の当たり判定を行う
	bool hit = false;

	// 右上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kLeftTop]);

	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {

		// めり込みを排除する
		indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kLeftBottom]);
		// めり込み先のブロック範囲
		BlockRect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);

		info.velocity.x = std::max(0.0f, (rect.left - worldTransform_.translation_.x) + (kCheckLanding + kWidth / 2.0f));
		// 地面に当たった子を記録する
		info.WallFlag = true;
	}
	WallMove(info);
}

void Player::Move(const CollisionMapInfo& info) {

	CeilingMove(info);

	// 移動
	worldTransform_.translation_ += info.velocity;
}
void Player::CeilingMove(const CollisionMapInfo& info) {

	// 天井に当たった？
	if (info.TopFlag) {

		velocity_.y = 0;
	}
}
void Player::ChangeGround(const CollisionMapInfo& info) {

	// 自キャラが接地状態
	if (onGround_) {

		// 移動後の4つの角の座標
		std::array<Vector3, kNumCorner> positionsNew;

		for (uint32_t i = 0; i < positionsNew.size(); i++) {
			positionsNew[i] = Player::CornerPosition(
			    {worldTransform_.translation_.x + info.velocity.x, worldTransform_.translation_.y + info.velocity.y, worldTransform_.translation_.z + info.velocity.x}, static_cast<Corner>(i));
		}

		MapChipType mapChipType;

		// 真下の当たり判定を行う
		bool hit = false;

		// 左下点の判定
		IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kLeftBottom] + Vector3{0, kCheckLanding, 0});

		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);

		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		// 左下点の判定
		indexSet = mapChipField_->GetMapChipIndexSetPosition(positionsNew[kRightBottom] + Vector3{0, kCheckLanding, 0});
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}

		if (!hit) {

			onGround_ = false;
		}
	} else {

		// 落下速度
		velocity_.y += -kGravityAcceleration;
		// 落下速度
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

		// 着地フラグ
		if (info.BottomFlag) {

			// 着地状態に切り替える(落下を止める
			onGround_ = true;
			// 着地にX速度を減衰
			velocity_.x *= (1.0f - kAttenuation);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}

void Player::WallMove(const CollisionMapInfo& info) {

	if (info.WallFlag) {

		velocity_.x *= (1.0f - kAttenuationWall);
	}
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

	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.velocity = velocity_;

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

				// if (turnTimer_ > 0.0f) {

				//	// 左右の自キャラ角度テーブル
				//	float destinationRotationYTable[2] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

				//	// 状態に応じた角度を取得する
				//	float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				//	// 自キャラの角度を設定

				//	worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, destinationRotationY, turnTimer_);

				//	/*if (turnTimer_ <= 0.0f) {
				//	,
				//	    worldTransform_.rotation_.y = destinationRotationY;
				//	}*/
				//}

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
	}

	if (turnTimer_ > 0.0f) {

		float destinationRotationYTable[2] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定
		worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, destinationRotationY, turnTimer_);
		turnTimer_ -= 0.1f;
	}

	//// 衝突情報を初期化
	// CollisionMapInfo collisionMapInfo;
	//  移動量に速度を値をコピー
	collisionMapInfo.velocity = velocity_;
	// マップ衝突チェック
	CollisionFlag(collisionMapInfo);
	// 床衝突
	ChangeGround(collisionMapInfo);

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
