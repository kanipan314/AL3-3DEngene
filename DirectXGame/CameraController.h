#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// カメラコントローラー
/// </summary>
class Player;

class CameraController {

	public:
		//初期化
	    void Initialize(ViewProjection* viewProjection);

		//更新
	    void Update();

		void SetTarget(Player* target) { 
			target_ = target;
		}

		void Reset();

	private:
	    ViewProjection* viewProjection_ = nullptr;
	    Player* target_ = nullptr;
		//追従対象とカメラの座標の差（オフセット）
	    Vector3 targetOffset_ = {0, 0, -15.0f};


};
