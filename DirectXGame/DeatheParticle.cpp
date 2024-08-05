#define NOMINMAX
#include "DeatheParticle.h"
#include <algorithm>


Matrix4x4 MakeRotate(float angle) {
	Matrix4x4 roteta_Z = {};

	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			roteta_Z.m[i][j] = 0;
			if (i == j) {
				roteta_Z.m[i][j] = 1;
			}
		}
	}

	roteta_Z.m[0][0] = cosf(angle);
	roteta_Z.m[0][1] = sinf(angle);
	roteta_Z.m[1][1] = cosf(angle);
	roteta_Z.m[1][0] = -sinf(angle);

	return roteta_Z;
}

Vector3 TransfomMatrix(Vector3 point, Matrix4x4 transform) {

	Vector3 answer;

	answer.x = point.x * transform.m[0][0] + point.y * transform.m[1][0] + point.z * transform.m[2][0] + 1.0f * transform.m[3][0];
	answer.y = point.x * transform.m[0][1] + point.y * transform.m[1][1] + point.z * transform.m[2][1] + 1.0f * transform.m[3][1];
	answer.z = point.x * transform.m[0][2] + point.y * transform.m[1][2] + point.z * transform.m[2][2] + 1.0f * transform.m[3][2];

	float w = point.x * transform.m[0][3] + point.y * transform.m[1][3] + point.z * transform.m[2][3] + 1.0f * transform.m[3][3];

	assert(w != 0.0f);

	answer.x /= w;
	answer.y /= w;
	answer.z /= w;

	return answer;
}

void DeatheParticle::Initialize(Model* model, ViewProjection* viewProjection,const Vector3& position) {

	model_ = model;
	//ワールドの初期化
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	viewProjection_ = viewProjection;

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeatheParticle::Update() {

	//終了したら何もしない
	if (isFinished_) {
		return;
	}

	//ワールド変換の更新
	for (auto& worldTransform : worldTransforms_) {

		// 行列計算
		worldTransform.UpdateMatrix();

		worldTransform.matWorld_.m;
		worldTransform.TransferMatrix();

	}

	for (uint32_t i = 0; i < 8; i++) {
	//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};

		//回転角を計算する
		float angle = kAngleUnit * i;

		//Z軸周り回転
		Matrix4x4 matrixRotation = MakeRotate(angle);
		//ベクトルを回転させて速度を得る
		velocity = TransfomMatrix(velocity,matrixRotation);
		//移動処理
		worldTransforms_[i].translation_ += velocity;
	}

	//カウンターを１フレーム分進める
	counter_ += 1.0f / 60.0f;

	//存続時間に達したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		//終了
		isFinished_ = true;
	}

	color_.w = std::max(0.0f, 1.0f - counter_ / kDuration);
	objectColor_.SetColor(color_);
	objectColor_.TransferMatrix();
	
}

void DeatheParticle::Draw() {

	
	// 終了したら何もしない
	if (isFinished_) {
		return;
	}


	for (auto& worldTransform : worldTransforms_) {
	
		model_->Draw(worldTransform, *viewProjection_,&objectColor_);

	}

}
