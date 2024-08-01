#pragma once

#include "iostream"
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Enemy.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include "DebugCamera.h"
#include "skydome.h"
#include "MapChipField.h"
#include "CaneraController.h"
#include "ImGuiManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	std::list<Enemy*> enemies_;

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

    //全ての当たり判定を行う
	void CheckAllCollisions();

	//交差判定
	bool IsCollision(AABB aabb1, AABB aabb2);

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	
private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	Model* model_ = nullptr;
	Model* enemyModel_ = nullptr;
	Model* Blockmodel_ = nullptr;
	Model* SkydomeModel_ = nullptr;

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

	//敵キャラ
	Enemy* enemy_ = nullptr;
	
	//天球
	Skydome* skydome_ = nullptr;

	//カメラコントローラ
	CaneraController* cameraController_ = nullptr;

	Rect cameraArea = {0,100,0,100}; 

	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	void GenerateBlocks();

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
