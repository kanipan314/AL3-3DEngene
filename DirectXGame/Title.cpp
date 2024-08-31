#include "Title.h"

Title::~Title() {

	// 天球の解放
	delete SkydomeModel_;

	delete model;

}

void Title::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	// カメラ系初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// 天球の生成
	SkydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// 天球の初期化
	skydome_ = new Skydome();

	skydome_->Initialize(SkydomeModel_, &viewProjection_);

	model = Model::CreateFromOBJ("Title",true);

}

void Title::Update() {

	// 天球
	skydome_->Update();

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

	// 天球の描画
	skydome_->Drow();
	
	model->Draw(worldTransform_, viewProjection_, textureHandle_);

		// 3Dオブジェクト描画後処理
	Model::PostDraw();

}
