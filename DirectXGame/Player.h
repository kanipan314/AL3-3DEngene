#pragma once
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 自キャラ
/// </summary>

class Player {

	public:
		/// 初期化
	    void Initialize(Model* model,uint32_t textureHandle,ViewProjection* viewProjection);

	    /// 更新
	    void Update();

		///　描画
	    void Draw();

	private:
		///ワールド変換
	    WorldTransform worldTransform_;
		ViewProjection* viewProjection_ = nullptr;
		///モデル
	    Model* model_ = nullptr;
		///テクスチャハンドル
	    uint32_t textureHandle_ = 0;


};
