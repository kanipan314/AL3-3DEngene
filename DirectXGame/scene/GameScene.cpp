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

	// 敵キャラの開放
	for (Enemy* enemyList : enemies_) {
		delete enemyList;
	}
	enemies_.clear();

	//	ブロックの解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	for (std::vector<WorldTransform*>& worldTransformJumpLine : worldTransformJumps_) {
		for (WorldTransform* worldTransformJumpBlock : worldTransformJumpLine) {
			delete worldTransformJumpBlock;
		}
	}
	worldTransformJumps_.clear();

	// パーティクルの開放
	delete deathParticles_;

	// 天球の解放
	delete SkydomeModel_;
	// カメラコントローラの開放
	delete cameraController_;
	// マップチップの開放
	delete mapChipField_;
}

void GameScene::Initialize() {

	// ゲームプレイフェーズから
	phase_ = Phase::kPlay;

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名の指定
	textureHandle_ = TextureManager::Load("/cube/cube.jpg");

	// 3dモデルの生成
	model_ = Model::CreateFromOBJ("Player", true);
	enemyModel_ = Model::CreateFromOBJ("Enemy", true);
	particleModel_ = Model::CreateFromOBJ("Paticle", true);
	ScopeModel_ = Model::CreateFromOBJ("Scope", true);
	goalModel_ = Model::CreateFromOBJ("Gaol", true);
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

	//// 敵キャラの生成
	// enemy_ = new Enemy();
	// enemy_->Initialize(enemyModel_, &viewProjection_,);

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(model_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	// 仮
	deathParticles_ = new DeatheParticle;
	deathParticles_->Initialize(particleModel_, &viewProjection_, playerPosition);

	// 天球の生成
	SkydomeModel_ = Model::CreateFromOBJ("skydome", true);

	// 天球の初期化
	skydome_ = new Skydome();

	skydome_->Initialize(SkydomeModel_, &viewProjection_);

	// カメラコントローラ初期化
	cameraController_ = new CaneraController();
	cameraController_->Initialize(&viewProjection_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	// 移動範囲の指定
	cameraController_->SetMovableArea(cameraArea);
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		// 1列の要素数を設定(横方向)
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
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kScope) {
				WorldTransform* JumpTransform = new WorldTransform();
				JumpTransform->Initialize();
				worldTransformJumps_[i][j] = JumpTransform;
				worldTransformJumps_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Update() {

	ChangePhase();

	switch (phase_) {
	case Phase::kPlay:

		if (EnemyTimer_ > 15.0f) {

			EnemyTimer_ = 0.0f;
			float randPosition = (float)(rand() % 20 + 1);

			Enemy* newEnemy = new Enemy();
			Vector3 enemyPosition = {400, randPosition, 0.0f};

			newEnemy->Initialize(enemyModel_, &viewProjection_, enemyPosition);

			enemies_.push_back(newEnemy);
		}

		// 自キャラの更新
		player_->Update();

		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				worldTransformBlock->UpdateMatrix();
			}
		}

		for (std::vector<WorldTransform*>& worldTransformJumpLine : worldTransformJumps_) {
			for (WorldTransform* worldTransformJumpBlock : worldTransformJumpLine) {
				if (!worldTransformJumpBlock)
					continue;

				worldTransformJumpBlock->UpdateMatrix();
			}
		}

		// 敵キャラの更新
		for (Enemy* enmeyList : enemies_) {

			enmeyList->Update();
		}

		// 天球
		skydome_->Update();

		// 当たり判定
		CheckAllCollisions();

#ifdef _DEBUG

		if (input_->TriggerKey(DIK_BACKSPACE)) {

			isDebugCameraActive_ = true;
		}

#endif // DEBUG

		// カメラ処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {
			// カメラコントローラ
			cameraController_->Update();

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}

		EnemyTimer_++;

		if (maxPosition.x < player_->GetWorldPosition().x) {

			maxPosition.x = LeadPosition();
		}

		

		break;

	case Phase::kDeath:

		// 天球
		skydome_->Update();
		// 敵キャラの更新
		for (Enemy* enmeyList : enemies_) {

			enmeyList->Update();
		}

		MaxPosition(player_->GetWorldPosition());

		// パーティクル更新
		if (deathParticles_) {
			deathParticles_->Update();
		}

#ifdef _DEBUG

		if (input_->TriggerKey(DIK_BACKSPACE)) {

			isDebugCameraActive_ = true;
		}

#endif // DEBUG

		// カメラ処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {
			// カメラコントローラ
			cameraController_->Update();

			// ビュープロジェクション行列の更新と転送
			viewProjection_.UpdateMatrix();
		}

		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				worldTransformBlock->UpdateMatrix();
			}
		}
		break;
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

	// 天球の描画
	skydome_->Drow();

	if (player_->ISDead() == false) {

		// 自キャラの描画
		player_->Draw();
	}

	// 敵キャラの描画
	for (Enemy* enmeyList : enemies_) {

		enmeyList->Draw();
	}

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			Blockmodel_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// パーティクルの描画
	if (deathParticles_) {

		deathParticles_->Draw();
	}

	if (goal_) {

		goalModel_->Draw(worldTransfrom_, viewProjection_);
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

#pragma region 自キャラと敵キャラの当たり判定

void GameScene::CheckAllCollisions() {

	// 判定対象1と2の座標
	AABB aabb1, aabb2;
	// じきゃらのざひょう
	aabb1 = player_->GetAABB();

	// 自キャラと敵の当たり判定
	for (Enemy* enemy : enemies_) {

		// 敵キャラの座標
		aabb2 = enemy->GetAABB();

		// ＡＡＢＢ同士の考査判定
		if (IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
			// 敵の衝突時コールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}
}

bool GameScene::IsCollision(AABB aabb1, AABB aabb2) {

	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		return true;
	}

	return false;
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:

		if (player_->ISDead() == true) {
			// 死亡演出へ切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPostion = player_->GetWorldPosition();

			deathParticles_ = new DeatheParticle();

			deathParticles_->Initialize(particleModel_, &viewProjection_, deathParticlesPostion);
		}

		break;
	case Phase::kDeath:

		if (deathParticles_ && deathParticles_->isFinished()) {

			finished_ = true;
		}

		break;
	default:
		break;
	}
}

void GameScene::MaxPosition(Vector3 positon) {
	std::string output_csv_file_path = "./Resources/HiScore.txt";

	// 書き込むCSVファイルを開く（std::ofstreamのコンストラクタで開く）
	std::ofstream ofs_csv_file(output_csv_file_path, std::ios::app); // `std::ios::app` でファイル末尾に追記

	// ファイルが正しく開けたか確認
	if (!ofs_csv_file.is_open()) {
		std::cerr << "ファイルを開くことができませんでした: " << output_csv_file_path << std::endl;
		return;
	}

	// `position` の x, y, z の値をCSV形式で書き込む
	ofs_csv_file << positon.x << ',' ;

	// ファイルを閉じる
	ofs_csv_file.close();
}

float GameScene::LeadPosition() { 

 	std::string str_buf;
	
	std::string input_csv_file_path = "./Resources/HiScore.txt";

	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream file(input_csv_file_path);

	// ファイルが正しく開けたか確認
	if (!file.is_open()) {
		std::cerr << "ファイルを開くことができませんでした: " << input_csv_file_path << std::endl;
		return 1;
	}

	 // 最大値を求めるために初期値を非常に小さな値で設定
	float maxValue = std::numeric_limits<float>::lowest();

	 // ファイルから1行ずつ読み込む
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line); // 文字列をストリームとして扱う
		float x;

		// 1つの浮動小数点数（xの値）を読み取る
		if (iss >> x) {
			// 最大値を更新
			if (x > maxValue)
				maxValue = x;
		} else {
			std::cerr << "データの解析に失敗しました: " << line << std::endl;
		}
	}
	return maxValue;
}

#pragma endregion
