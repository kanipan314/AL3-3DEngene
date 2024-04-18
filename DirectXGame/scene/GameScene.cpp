#include "GameScene.h"
#include "TextureManager.h"
#include"ImGuiManager.h"
#include"PrimitiveDrawer.h"
#include "AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

///デストラクタ
GameScene::~GameScene() {
	delete sprite_; 
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	modelHandele = TextureManager::Load("/cube/cube.jpg");
	sprite_ = Sprite::Create(modelHandele, {100, 50});

	model_ = Model::Create();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	voiceDataHandle_ = audio_->LoadWave("mokugyo.wav");
	audio_->PlayWave(voiceDataHandle_,true);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	debugCamera_ = new DebugCamera(1920, 720);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {

	Vector2 postion = sprite_->GetPosition();
	postion.x += 2.0f;
	postion.y += 1.0f;

	sprite_->SetPosition(postion);

	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voiceDataHandle_);
	}

	debugCamera_->Update();

	ImGui::Begin("Debug1");
	ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();

	ImGui::Begin("Debug2");
	ImGui::ShowDebugLogWindow();
	ImGui::End();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	PrimitiveDrawer::GetInstance()->DrawLine3d(
	    {0, 0, 0}, {0, 10, 0},
	    {
	        1.0f,
	        0.0f,
	        0.0f,
	        1.0f,
	    });

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), modelHandele);



	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
