#include "CameraController.h"
#include "Player.h"

void CameraController::Initialize(ViewProjection* viewProjection) {
	viewProjection_ = viewProjection;
}

void CameraController::Update() {

}

void CameraController::Reset() {
	///*追従対象のワールドトランスフォームを参照
	//const WorldTransform& targetWorldTransform = target_;*/
}
