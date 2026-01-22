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
#include <algorithm>
#include "main.h"
#include "time.h"
#include "tcplistener.h"
#include "ranking.h"

//*******************************************************************
// 定数宣言空間
//*******************************************************************
namespace SERVERINFO
{
	constexpr int MAX_WORD = 512;	// 最大文字列数
};

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
	CRanking* pRanking = CRanking::Create();
	if (pRanking == nullptr) return 0;

	// 初期ランキング読み込み
	pRanking->Load();

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
			pRanking->Update(recvScore);

			// 保存する
			pRanking->Save();

			// クライアントへランキング送信
			int sendSize = sizeof(int) * CRanking::MAX_RANKDATA;
			int sent = 0;
			SOCKET sock = pTcpclient->GetSock();

			while (sent < sendSize)
			{
				// ランキングのデータ送信
				int ret = pRanking->Send(sock);

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
	// ランキングクラス終了処理
	//=====================================
	if (pRanking != nullptr)
	{
		delete pRanking;
		pRanking = nullptr;
	}

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