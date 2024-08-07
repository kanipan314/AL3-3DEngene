#include "GameScene.h"
#include "MapChipField.h"
#include <cassert>
#include <map>
#include <fstream>
#include <sstream>



//namespace {
    std::map<std::string, MapChipType> mapChipTable = {

        {"0", MapChipType::kBlank},
        {"1", MapChipType::kBlock},
    };
//}

void MapChipField::ResetMapChipData() {

    mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);

    for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
	
        mapChipDataLine.resize(kNumBlockHorizontal);

    }

}

void MapChipField::LoadMapChipCSV(const std::string& filePath) {

    //マップチップデータをリセット
	ResetMapChipData();

    //ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

    //マップチップcsv
	std::stringstream mapChipCsv;
    //ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
    //ファイルを閉じる
	file.close();

    //csvからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
	
        std::string line;
		getline(mapChipCsv, line);

        //１行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

        for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
		
            std::string word;
			getline(line_stream, word, ',');

            if (mapChipTable.contains(word)) {
			
                mapChipData_.data[i][j] = mapChipTable[word];

            }

        }

    }

}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) { 
    
    if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
    }
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
    }
    
    return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
    return Vector3(kBlockWidth * xIndex,kBlockHeight * (kNumBlockVirtical - 1 - yIndex),0); }

uint32_t MapChipField::GetNumBlockVirtical() { return 20; }

uint32_t MapChipField::GetNumBlockHorizontal() { return 100; }

IndexSet MapChipField::GetMapChipIndexSetPosition(const Vector3& position) {
    
    IndexSet indexSet = {};
	indexSet.xIndex = (uint32_t)((position.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = (uint32_t)(kNumBlockVirtical - 1 - (position.y + kBlockHeight / 2) / kBlockHeight + 1);

    return indexSet;
}

BlockRect MapChipField::GetRectByIndex(uint32_t xindex, uint32_t yindex) { 
    
    //指定ブロックの中心座標を取得
	Vector3 center = GetMapChipPositionByIndex(xindex, yindex);

    BlockRect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
    
    return rect; 

}
