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

	//敵キャラの開放
	delete enemy_;

	//	ブロックの解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	//天球の解放
	delete SkydomeModel_;
	//カメラコントローラの開放
	delete cameraController_;
	//マップチップの開放
	delete mapChipField_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名の指定
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");

	// 3dモデルの生成
	model_ = Model::CreateFromOBJ("Player", true);
	enemyModel_ = Model::CreateFromOBJ("Enemy", true);
	Blockmodel_ = Model::Create();

	// カメラ系初期化
	worldTransfrom_.Initialize();
	viewProjection_.Initialize();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// マップチップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCSV("./Resources/map.csv");
	GenerateBlocks();

	// 敵キャラの生成
	enemy_ = new Enemy();
	enemy_->Initialize(enemyModel_, &viewProjection_);

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(model_, &viewProjection_,playerPosition);
	player_->SetMapChipField(mapChipField_);
	

	// 天球の生成
	SkydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// 天球の初期化
	skydome_ = new Skydome();

	skydome_->Initialize(SkydomeModel_, &viewProjection_);

	//カメラコントローラ初期化
	cameraController_ = new CaneraController();
	cameraController_->Initialize(&viewProjection_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	//移動範囲の指定
	cameraController_->SetMovableArea(cameraArea);

}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	//要素数を変更する
	//列数を設定（縦方向）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		//1列の要素数を設定(横方向)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// キューブの作成
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockHorizontal; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);

			}
		}
	}
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

	//敵キャラの更新
	enemy_->Update();

	//天球
	skydome_->Update();

	
	#ifdef _DEBUG

	if (input_->TriggerKey(DIK_BACKSPACE)) {
	
		isDebugCameraActive_ = true;

	}

	#endif // DEBUG

	//カメラ処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// カメラコントローラ
		cameraController_->Update();

		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
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

	//天球の描画
	skydome_->Drow();

	// 自キャラの描画
	player_->Draw();

	//敵キャラの描画
	enemy_->Draw();

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


