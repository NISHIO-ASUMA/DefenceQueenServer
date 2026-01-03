//=========================================================
//
// 各種ロード処理クラス [ load.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include <vector>
#include <array>

//*********************************************************
// ロード処理クラスを定義
//*********************************************************
class CLoad
{
public:

	//****************************
	// 定数構造体
	//****************************
	struct Config
	{
		static constexpr int SAVE_NUMARRAY = 5; // セーブする配列の数
	};

	CLoad();
	~CLoad();

	int LoadInt(const char * loadfilename);
	std::vector<int> LoadIntToArray(const char* loadfilename);
	std::array<int,Config::SAVE_NUMARRAY> LoadIntToFixedArray(const char* loadfilename);

	void SaveInt(const char* savefilename, const int OutputData);
	void SaveIntToArray(const char* savefilename, const std::vector<int>& ArrayData);
	void SaveIntToFixedArray(const char* savefilename, const std::array<int, Config::SAVE_NUMARRAY>& ArrayData);

private:
};