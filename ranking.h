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
#include <WinSock2.h>

//*********************************************************
// ランキングのデータを扱うクラスを定義
//*********************************************************
class CRanking
{
public:

	CRanking();
	~CRanking();

	void Update(const int& nScore);
	void Load(void);
	void Save(void);
	void Reset(void);
	int Send (SOCKET sock) const;

	const int* GetScore(void) { return m_ScoreInfo.nScores; } // 先頭アドレスを返す

	static CRanking* Create(void);

	static constexpr int MAX_RANKDATA = 5; // 最大配列

private:

	static constexpr const char* FILE_NAME = "data/SCORE/Ranking.bin"; // 対象ファイル名

	//******************************
	// 変数構造体
	//******************************
	struct DataInfo
	{
		int nScores[MAX_RANKDATA];	// スコア格納配列
	};

	DataInfo m_ScoreInfo; // 構造体変数
};