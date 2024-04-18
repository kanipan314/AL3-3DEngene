#include "Player.h"
#include "cassert"


void Player::Initialize(Model* model, uint32_t textureHandle_) {

	///Nullチェック
	assert(model);
	///引き渡し
	playermodel_ = model;
	playertextureHandle = textureHandle_;
	///ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {

	/*worldTransform_.TransferMatrix();*/

}

void Player::Draw() {

}