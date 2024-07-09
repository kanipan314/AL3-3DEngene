#pragma once

#include "GameScene.h"

enum class MapChipType {

	kBlank,
	kBlock,

};

struct MapChipData {

	std::vector<std::vector<MapChipType>> data;
};


/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {

	//1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;

	//ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;


public:

	MapChipData mapChipData_;

	void ResetMapChipData();

	void LoadMapChipCSV(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical();

	uint32_t GetNumBlockHorizontal();
};



