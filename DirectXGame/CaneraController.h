#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

class Player;

struct Rect {

	float left = 0.0f;   // 左
	float right = 1.0f;  // 右
	float bottom = 0.0f; // 下
	float top = 1.0f;    // 上
};



//カメラコントローラ
class CaneraController {

	public:
		//初期化
	    void Initialize(ViewProjection* viewProjection);

		//更新
	    void Update();

		void SetTarget(Player* target) { target_ = target; }

		void SetMovableArea(Rect area) { movableArea_ = area; }

		void Reset();

	private:

		//ビュープロジェクション
	    ViewProjection* viewProjection_ = nullptr;
	    Player* target_ = nullptr;
	    Vector3 targeOffset_ = {0, 0, -30.0f};
	    // カメラ移動範囲
	    Rect movableArea_ = {25, 75, 25, 75};
		//追従対象の各方向へのカメラ移動範囲
	    static inline const Rect margin = {-25, 25, -25, 25};
		//カメラの目標座標
	    Vector3 cameraTargetPosition = {0, 0, -30.0f};
		//座標補間割合
	    static inline const float kInterpolationRate = 0.8f;
		//速度掛け算
	    static inline const float kVelocityBias = 1.5f;
		
};
