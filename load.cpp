//=========================================================
//
// 各種ロード処理クラス [ load.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "load.h"
#include <fstream>
#include <iostream>
#include <algorithm>

//=========================================================
// コンストラクタ
//=========================================================
CLoad::CLoad()
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CLoad::~CLoad()
{
	// 無し
}
//=========================================================
// 一つのバイナリ数値データを読み込む
//=========================================================
int CLoad::LoadInt(const char* loadfilename)
{
	// 格納用変数
	int nReturnScore = 0;

	// ファイルを開く
	std::ifstream Openfile(loadfilename, std::ios::binary);

	if (!Openfile)
	{
		// 例外処理
		return -1;
	}

	// 読み取ったスコアを入れる
	Openfile.read(reinterpret_cast<char*>(&nReturnScore), sizeof(int));

	// 読み込み失敗チェック
	if (Openfile.fail())
	{
		return -1;
	}

	// ファイルを閉じる
	Openfile.close();

	// 格納スコアを返す
	return nReturnScore;
}
//=========================================================
// int型のバイナリデータ配列を返す(5個)
//=========================================================
std::vector<int> CLoad::LoadIntToArray(const char* loadfilename)
{
	// 配列
	std::vector<int> ReturnScore;

	// ファイルを開く
	std::ifstream Openfile(loadfilename, std::ios::binary);

	if (!Openfile)
	{
		return ReturnScore;
	}

	// まず要素数を読む
	int nDataCount = 0;

	// ファイルから情報をセットする
	Openfile.read(reinterpret_cast<char*>(&nDataCount), sizeof(int));

	// サイズを確定する
	ReturnScore.resize(nDataCount);

	// 配列本体を読み込む
	Openfile.read(reinterpret_cast<char*>(ReturnScore.data()), sizeof(int) * nDataCount);

	// ファイルを閉じる
	Openfile.close();

	// 配列そのものを返す
	return ReturnScore;
}
//=============================================================
// 固定長5件分のint型バイナリデータを返す
//=============================================================
std::array<int,CLoad::Config::SAVE_NUMARRAY> CLoad::LoadIntToFixedArray(const char* loadfilename)
{
	// 読み込み用配列を宣言する
	std::array<int, Config::SAVE_NUMARRAY> LoadData = { NULL };

	// ファイルを開く
	std::ifstream Openfile(loadfilename, std::ios::binary);

	// 開けたら
	if (Openfile)
	{
		// データを格納する
		Openfile.read((char*)LoadData.data(),sizeof(int) * Config::SAVE_NUMARRAY);
	}

	// ファイルを閉じる
	Openfile.close();

	// 配列データを返す
	return LoadData;
}
//=================================================================
// 一つのバイナリ数値データを書き出す
//=================================================================
void CLoad::SaveInt(const char* savefilename, const int OutputData)
{
	// 書き出すファイルを開く
	std::ofstream Openfile(savefilename, std::ios::binary);
	if (!Openfile)
	{
		return;
	}

	// 数値を書き出す
	Openfile.write((const char*)&OutputData, sizeof(int));

	// ファイルを閉じる
	Openfile.close();
}
//=============================================================
// 配列のバイナリデータを書き出す
//=============================================================
void CLoad::SaveIntToArray(const char* savefilename, const std::vector<int>& ArrayData)
{
	// 書き出すファイルを開く
	std::ofstream Openfile(savefilename, std::ios::binary);
	if (!Openfile)
	{
		return;
	}

	// ソート用に一時保存
	std::vector<int>SortData = ArrayData;

	// 降順にソート
	std::sort(SortData.begin(), SortData.end(), std::greater<int>());

	// 要素数を書き出す
	int nNumData = static_cast<int>(ArrayData.size());
	Openfile.write((const char*)(&nNumData), sizeof(int));

	// 配列データをバイナリ形式で書き出す
	Openfile.write((const char*)(ArrayData.data()), sizeof(int) * nNumData);
	
	// ファイルを閉じる
	Openfile.close();
}
//=============================================================
// 固定長5件分のint型バイナリデータを書き出す
//=============================================================
void CLoad::SaveIntToFixedArray(const char* savefilename, const std::array<int, Config::SAVE_NUMARRAY>& ArrayData)
{
	// 書き出すファイル
	std::ofstream SaveFile(savefilename, std::ios::binary);

	if (!SaveFile)
	{
		return;
	}

	// 配列データを書き出す
	SaveFile.write((const char*)ArrayData.data(), sizeof(int) * Config::SAVE_NUMARRAY);

	// ファイルを閉じる
	SaveFile.close();
}