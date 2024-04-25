#include "Player.h"
#include "cassert"


void Player::Initialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection) {

	///Nullチェック
	assert(model);
	///引き渡し
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	///ワールド変換の初期化
	worldTransform_.Initialize();
	
}

void Player::Update() {

	worldTransform_.matWorld_.m;
	worldTransform_.TransferMatrix();

}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }