//===================================================================
//
// オンラインランキングサーバー用プロジェクト [ main.cpp ]
// Author : Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include <cstdio>
#include <stdlib.h>
#include <crtdbg.h>
#include <memory>
#include <functional>
#include <fstream>
#include <iostream>
#include <array>
#include <algorithm>
#include "main.h"
#include "time.h"
#include "tcplistener.h"

//*******************************************************************
// 定数宣言空間
//*******************************************************************
namespace SERVERINFO
{
	constexpr int MAX_WORD = 512;	// 最大文字列数
	constexpr int MAX_DATA = 5;		// 最大ランキングデータ数
	constexpr const char* FILE_NAME = "data/SCORE/Ranking.bin"; // データを保持しているファイルパス名
};

//*******************************************************************
// ランキング用構造体
//*******************************************************************
struct Ranking
{
	int nScores[SERVERINFO::MAX_DATA];	// スコア格納配列
};

//*******************************************************************
// 関数宣言空間
//*******************************************************************
void UpdateRanking(Ranking* ranking,const int& nScore);
void LoadRanking(Ranking* ranking);
void SaveRanking(Ranking* ranking);
void ResetRanking(Ranking* ranking);

//===================================================================
// メインエントリーポイント関数
//===================================================================
int main(void)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);	// メモリリーク検知用のフラグ
#endif // _DEBUG

	//=========================
	// Winsockの初期化
	//=========================
	WSADATA wsaData;

	// 例外メッセージ設定
	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0)
	{
		printf("\n初期化に失敗しました。終了します。\n");
		return 0;
	}

	// インスタンスを生成
	CTcplistener* pTcplisener = new CTcplistener;
	CTcpclient* pTcpclient = nullptr;

	// ランキングデータを初期化する
	Ranking ranking = {};
	LoadRanking(&ranking);

	// nullptrチェック
	if (pTcplisener != nullptr && pTcplisener->Init(PORT) == true)
	{
		while (1)
		{
			// 接続待ち
			pTcpclient = pTcplisener->Accept();
			if (pTcpclient == nullptr) continue;

			// 格納変数
			int recvScore = 0;

			// データをクライアントから受信
			int nRecvByte = recv(pTcpclient->GetSock(),(char*)&recvScore,sizeof(int),0);

			// エラー処理
			if (nRecvByte != sizeof(int))
			{
				pTcpclient->Uninit();
				delete pTcpclient;
				pTcpclient = nullptr;
				continue;
			}

			// サーバー終了コマンド
			if (recvScore == 0 || recvScore == 999)
			{
				printf("終了コマンドを受信\n");

				// クライアント切断処理
				pTcpclient->Uninit();
				delete pTcpclient;
				pTcpclient = nullptr;
				break;
			}

			// サーバー表示
			printf("受信スコア: %d\n", recvScore);

			// ランキング更新
			UpdateRanking(&ranking, recvScore);
			SaveRanking(&ranking);

			// クライアントへランキング送信	(20バイト)
			int sendSize = sizeof(int) * SERVERINFO::MAX_DATA;
			int sent = 0;
			SOCKET sock = pTcpclient->GetSock();

			while (sent < sendSize)
			{
				// データ送信
				int ret = send(sock,(char*)ranking.nScores + sent,sendSize - sent,0);

				if (ret <= 0)
				{
					printf("送信失敗\n");
					break;
				}

				sent += ret;
			}
		}
	}

	// 終了メッセージ表示
	printf("\nサーバーを閉じます\n");

	//=====================================
	// クライアントクラス終了処理
	//=====================================
	if (pTcpclient != nullptr)
	{
		// 破棄
		pTcpclient->Uninit();
		delete pTcpclient;
		pTcpclient = nullptr;
	}

	//=====================================
	// クライアントリスナークラス終了処理
	//=====================================
	if (pTcplisener != nullptr)
	{
		pTcplisener->Uninit();
		delete pTcplisener;
		pTcplisener = nullptr;
	}

	// WinSock終了
	WSACleanup();

	// 終了コード
	return 0;
}
//===================================================================
// ランキング初期化処理
//===================================================================
void ResetRanking(Ranking* ranking)
{
	for (int nCnt = 0; nCnt < SERVERINFO::MAX_DATA; nCnt++)
	{
		ranking->nScores[nCnt] = 0;
	}
}
//===================================================================
// ランキング更新処理
//===================================================================
void UpdateRanking(Ranking* ranking, const int& nScore)
{
	// 5位以下なら無視
	if (nScore <= ranking->nScores[SERVERINFO::MAX_DATA - 1])
		return;

	// 最下位と入れ替え
	ranking->nScores[SERVERINFO::MAX_DATA - 1] = nScore;

	// 降順ソート
	std::sort(ranking->nScores,ranking->nScores + SERVERINFO::MAX_DATA,std::greater<int>());
}
//===================================================================
// ランキング読み込み処理
//===================================================================
void LoadRanking(Ranking* ranking)
{
	// バイナリ数値ファイルを開く
	std::ifstream Openfile(SERVERINFO::FILE_NAME, std::ios::binary);

	if (!Openfile)
	{
		// ファイルが無ければ初期化
		ResetRanking(ranking);
		SaveRanking(ranking);
		return;
	}

	// 数値データを格納する
	Openfile.read((char*)ranking->nScores, sizeof(int) * SERVERINFO::MAX_DATA);
	
	// ファイルを閉じる
	Openfile.close();
}
//===================================================================
// ランキング書き出し処理
//===================================================================
void SaveRanking(Ranking* ranking)
{
	// 書き出すファイルを指定
	std::ofstream SaveFile(SERVERINFO::FILE_NAME, std::ios::binary);
	if (!SaveFile) return;

	// 配列データを書き出す
	SaveFile.write((const char*)ranking->nScores, sizeof(int) * SERVERINFO::MAX_DATA);

	// ファイルを閉じる
	SaveFile.close();
}

//// デバッグ表示
//printf("ランキングのスコア\n");

//// サーバー側に表示
//for (int nScoreData = 0; nScoreData < SERVERINFO::MAX_DATA; nScoreData++)
//{
//	printf("%d位: %d\n", nScoreData + 1, ranking.nScores[nScoreData]);
//}
