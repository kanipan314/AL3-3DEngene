#pragma once

#include "WorldTransform.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"


/// <summary>
/// 天球
/// </summary>
class skydome {

	public:

	/// <summary>
	/// 初期化
	/// </summary>

	void Initialize(ViewProjection* viewProjection);

	///更新
	void Update();

	///描画
	void Drow();

	private:
		///ワールド変換データ
	    WorldTransform worldTransform_;
	    ViewProjection* viewProjection_ = nullptr;
		///モデル
	    Model* model_ = nullptr;
	
};
