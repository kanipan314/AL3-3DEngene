#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {

	delete model_;
	delete Blockmodel_;
	delete debugCamera_;

	// 自キャラの解放
	delete player_;

	//	ブロックの解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	//天球の解放
	delete Skydomemodel_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名の指定
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");

	// 3dモデルの生成
	model_ = Model::Create();
	Blockmodel_ = Model::Create();

	// カメラ系初期化
	worldTransfrom_.Initialize();
	viewProjection_.Initialize();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	// 自キャラの生成
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &viewProjection_);

	// ブロック
	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockwidth = 2.0f;
	const float kBlockHeigt = 2.0f;

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// キューブの作成
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			if (i % 2 == 0 && j % 2 == 1) {

				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockwidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeigt * i;
			}
		}
	}

	//天球の生成
	Skydomemodel_ = Model::CreateFromOBJ("skydome", true);

	////天球の初期化
	//skydome_->Initialize(&viewProjection_);
}

void GameScene::Update() {

	// 自キャラの更新
	player_->Update();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			worldTransformBlock->UpdateMatrix();
		}
	}

	////天球
	//skydome_->Update();

	#ifdef _DEBUG

	if (input_->TriggerKey(DIK_BACKSPACE)) {
	
		isDebugCameraActive_ = true;

	}

	#endif // DEBUG

	////カメラ処理
	//if (isDebugCameraActive_) {
	//	debugCamera_->Update();
	//	viewProjection_.matView = debugCamera_->GetViewProjection();
	//	viewProjection_.matProjection = debugCamera_->GetViewProjection();
	//}
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

	////天球の描画
	//skydome_->Drow();

	// 自キャラの描画
	player_->Draw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			Blockmodel_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
