#pragma once

#include "WorldTransform.h"
#include "TextureManager.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"


/// <summary>
/// 天球
/// </summary>
class Skydome {

	public:

	/// <summary>
	/// 初期化
	/// </summary>

	void Initialize(Model* model,ViewProjection* viewProjection);

	///更新
	void Update();

	///描画
	void Drow();

	private:
		///ワールド変換データ
	    WorldTransform worldTransform_;
	    ViewProjection* viewProjection_ = nullptr;
		///モデル
	    Model* SkydomeModel_ = nullptr;

		//テクスチャハンドル
	    uint32_t textureHandle_;
	
};
