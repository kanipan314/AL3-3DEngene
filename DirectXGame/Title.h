#pragma once

#include "iostream"
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "TextureManager.h"

class Title {

public:

	~Title();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	//デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:

	DirectXCommon* dxCommon_ = nullptr;

	/// ワールド変換
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	Model* model = nullptr;

	//テクスチャ
	uint32_t textureHandle_ = 0;

	//終了フラグ
	bool finished_ = false;


};
