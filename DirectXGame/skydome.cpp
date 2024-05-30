#include "skydome.h"
#include "cassert"

void skydome::Initialize(ViewProjection* viewProjeciton) { 
	
	viewProjection_ = viewProjeciton;

}
void skydome::Update() {

}

void skydome::Drow() { model_->Draw(worldTransform_,*viewProjection_); }