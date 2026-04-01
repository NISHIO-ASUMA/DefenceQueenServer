//=========================================================
//
// ランキングクラス [ ranking.h ]
// Author : Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include <WinSock2.h>

//*********************************************************
// ランキングのデータを扱うクラスを定義
//*********************************************************
class CRanking
{
public:

	//******************************
	// 変数構造体
	//******************************
	struct Infomation
	{
		static constexpr const char* FILE_NAME = "data/SCORE/Ranking.bin";	// 書き出すファイル名
		static constexpr int MAX_RANKDATA = 5;								// 最大配列
		int nScores[MAX_RANKDATA];											// スコア格納配列
	};

	CRanking();
	~CRanking();

	void Update(const int& nScore);
	void Load(void);
	void Save(void);
	void Reset(void);
	int Send (SOCKET sock) const;
	const int* GetScore(void) { return m_ScoreInfo.nScores; } // 先頭アドレスを返す

	static CRanking* Create(void);

private:

	Infomation m_ScoreInfo; // 構造体変数
};