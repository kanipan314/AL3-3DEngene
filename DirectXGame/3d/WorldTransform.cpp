#include "WorldTransform.h"
#include "GameScene.h"
#include "TextureManager.h"

Matrix4x4 Multi(Matrix4x4 m1, Matrix4x4 m2) {
	Matrix4x4 answer;

	answer.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	answer.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	answer.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	answer.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	answer.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	answer.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	answer.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	answer.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	answer.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	answer.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	answer.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	answer.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	answer.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	answer.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	answer.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	answer.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return answer;
}

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {

	Matrix4x4 rotetaMatrix_X;

	Matrix4x4 rotetaMatrix_Y;

	Matrix4x4 rotetaMatrix_Z;

	Matrix4x4 rotate_answer;

	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {

			rotetaMatrix_X.m[i][j] = 0;
			rotetaMatrix_Y.m[i][j] = 0;
			rotetaMatrix_Z.m[i][j] = 0;
			if (i == j) {
				rotetaMatrix_X.m[i][j] = 1;
				rotetaMatrix_Y.m[i][j] = 1;
				rotetaMatrix_Z.m[i][j] = 1;
			}
		}
	}

	rotetaMatrix_X.m[1][1] = cosf(rotate.x);
	rotetaMatrix_X.m[1][2] = sinf(rotate.x);
	rotetaMatrix_X.m[2][1] = -sinf(rotate.x);
	rotetaMatrix_X.m[2][2] = cosf(rotate.x);

	rotetaMatrix_Y.m[0][0] = cosf(rotate.y);
	rotetaMatrix_Y.m[0][2] = -sinf(rotate.y);
	rotetaMatrix_Y.m[2][0] = sinf(rotate.y);
	rotetaMatrix_Y.m[2][2] = cosf(rotate.y);

	rotetaMatrix_Z.m[0][0] = cosf(rotate.z);
	rotetaMatrix_Z.m[0][1] = sinf(rotate.z);
	rotetaMatrix_Z.m[1][1] = cosf(rotate.z);
	rotetaMatrix_Z.m[1][0] = -sinf(rotate.x);

	rotate_answer = Multi(rotetaMatrix_X, Multi(rotetaMatrix_Y, rotetaMatrix_Z));

	Matrix4x4 worldMatrix;

	worldMatrix.m[0][0] = rotate_answer.m[0][0] * scale.x;
	worldMatrix.m[0][1] = rotate_answer.m[0][1] * scale.x;
	worldMatrix.m[0][2] = rotate_answer.m[0][2] * scale.x;
	worldMatrix.m[0][3] = 0;

	worldMatrix.m[1][0] = rotate_answer.m[1][0] * scale.y;
	worldMatrix.m[1][1] = rotate_answer.m[1][1] * scale.y;
	worldMatrix.m[1][2] = rotate_answer.m[1][2] * scale.y;
	worldMatrix.m[1][0] = 0;

	worldMatrix.m[2][0] = rotate_answer.m[2][0] * scale.z;
	worldMatrix.m[2][1] = rotate_answer.m[2][1] * scale.z;
	worldMatrix.m[2][2] = rotate_answer.m[2][2] * scale.z;
	worldMatrix.m[2][3] = 0;

	worldMatrix.m[3][0] = translate.x;
	worldMatrix.m[3][1] = translate.y;
	worldMatrix.m[3][2] = translate.z;
	worldMatrix.m[3][3] = 1;

	return worldMatrix;
}


void WorldTransform::UpdateMatrix() {

	//スケール、回転、平行移動を合成
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	//定数バッファに転送する
	TransferMatrix();

}