#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include "DebugCamera.h"
#include "skydome.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

    

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Model* model_ = nullptr;
	Model* Blockmodel_ = nullptr;
	Model* Skydomemodel_ = nullptr;

	// でバックカメラ有効
	bool isDebugCameraActive_ = false;

	uint32_t textureHandle_ = 0;

	// ワールドトランスフォーム
	WorldTransform worldTransfrom_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	/// <summary>
	/// 自キャラ　
	/// </summary>

	Player* player_ = nullptr;
	
	////天球
	//skydome* skydome_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
