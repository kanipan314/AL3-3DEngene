#include "Title.h"

Title::~Title() {

	delete model;

}

void Title::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	// カメラ系初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	model = Model::CreateFromOBJ("Title",true);

}

void Title::Update() {

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
	
		finished_ = true;
	
	}

	worldTransform_.matWorld_.m;
	worldTransform_.TransferMatrix();

}

void Title::Draw() { 

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	
	model->Draw(worldTransform_, viewProjection_, textureHandle_);

		// 3Dオブジェクト描画後処理
	Model::PostDraw();

}
