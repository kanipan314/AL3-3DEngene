#include "skydome.h"
#include "cassert"

void Skydome::Initialize(Model* model,ViewProjection* viewProjeciton) { 
	
	assert(model);
	//引き渡し
	SkydomeModel_ = model;
	viewProjection_ = viewProjeciton;

	//ワールド変換初期化
	worldTransform_.Initialize();

}
void Skydome::Update() {

	worldTransform_.matWorld_.m;
	worldTransform_.TransferMatrix();

}

void Skydome::Drow() { 
	SkydomeModel_->Draw(worldTransform_, *viewProjection_);
}