//=========================================================
//
// ランキングクラス [ ranking.cpp ]
// Author : Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "ranking.h"
#include <fstream>
#include <iostream>
#include <algorithm>

//=========================================================
// コンストラクタ
//=========================================================
CRanking::CRanking() : m_ScoreInfo{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CRanking::~CRanking()
{

}
//=========================================================
// ポインタ生成関数
//=========================================================
CRanking* CRanking::Create(void)
{
	// インスタンス生成
	CRanking* pRanking = new CRanking;
	if (pRanking == nullptr) return nullptr;

	return pRanking;
}
//=========================================================
// 更新関数
//=========================================================
void CRanking::Update(const int& nScore)
{
	// 引数の値がランキングの5位以下の値なら無視
	if (nScore <= m_ScoreInfo.nScores[MAX_RANKDATA - 1])
		return;

	// 最下位と入れ替え
	m_ScoreInfo.nScores[MAX_RANKDATA - 1] = nScore;

	// 降順ソート
	std::sort(m_ScoreInfo.nScores, m_ScoreInfo.nScores + MAX_RANKDATA, std::greater<int>());
}
//=========================================================
// 読み込み関数
//=========================================================
void CRanking::Load(void)
{
	// バイナリ数値ファイルを開く
	std::ifstream Openfile(FILE_NAME, std::ios::binary);

	if (!Openfile)
	{
		// ファイルが無ければ初期化
		Reset();
		Save();
		return;
	}

	// 数値データを格納する
	Openfile.read((char*)m_ScoreInfo.nScores, sizeof(int) * MAX_RANKDATA);

	// ファイルを閉じる
	Openfile.close();
}
//=========================================================
// 書き出し関数
//=========================================================
void CRanking::Save(void)
{
	// 書き出すファイルを指定
	std::ofstream SaveFile(FILE_NAME, std::ios::binary);
	if (!SaveFile) return;

	// 配列データを書き出す
	SaveFile.write((const char*)m_ScoreInfo.nScores, sizeof(int) *MAX_RANKDATA);

	// ファイルを閉じる
	SaveFile.close();
}
//=========================================================
// リセット関数
//=========================================================
void CRanking::Reset(void)
{
	// 構造体の要素をクリアする
	for (int nCnt = 0; nCnt < MAX_RANKDATA; nCnt++)
	{
		m_ScoreInfo.nScores[nCnt] = 0;
	}
}
//=========================================================
// データ送信関数
//=========================================================
int CRanking::Send(SOCKET sock) const
{
	return send(sock,(const char*)m_ScoreInfo.nScores,sizeof(int) * MAX_RANKDATA,0);
}