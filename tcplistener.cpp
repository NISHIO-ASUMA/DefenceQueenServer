//=========================================================
//
// サーバー用クラス処理 [ tcplistener.cpp ]
// Author : Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "tcplistener.h"
#include <cstdio>

//=========================================================
// コンストラクタ
//=========================================================
CTcplistener::CTcplistener()
{
	// 値のクリア
	m_sockServer = INVALID_SOCKET;
	m_pClient = nullptr;
}
//=========================================================
// デストラクタ
//=========================================================
CTcplistener::~CTcplistener()
{
	// 無し
}
//=========================================================
// 初期化処理
//=========================================================
bool CTcplistener::Init(int nPortNum)
{
	//=========================
	// サーバー用ソケット作成
	//=========================
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);

	// 例外処理
	if (m_sockServer == INVALID_SOCKET)
	{
		printf("\nソケットの作成に失敗しました。終了します。\n");
		WSACleanup();
		return 0;
	}

	//=========================
	// ソケットのアドレス設定
	//=========================
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPortNum);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(m_sockServer, (struct sockaddr*)&addr, sizeof(addr));
	listen(m_sockServer, 999);

	// メッセージ表示
	printf("接続を待機中...\n");

	// 結果を返す
	return true;
}
//=========================================================
// 終了処理
//=========================================================
void CTcplistener::Uninit(void)
{
	// 接続済みかチェック
	if (m_sockServer == INVALID_SOCKET)
	{
		// 接続していない
		return;
	}

	// 切断する
	closesocket(m_sockServer);

	// 初期化状態にする
	m_sockServer = INVALID_SOCKET;
}
//=========================================================
// 接続待ち受け関数
//=========================================================
CTcpclient* CTcplistener::Accept(void)
{
	// ソケット生成済みかチェック
	if (m_sockServer == INVALID_SOCKET) return nullptr;

	struct sockaddr_in clientAddr;
	int nLength = sizeof(clientAddr);

	// 接続待ち受け
	SOCKET socket = accept(m_sockServer, (struct sockaddr*)&clientAddr, &nLength);

	// 接続失敗時
	if (socket == INVALID_SOCKET)
	{
		printf("クライアントの接続に失敗しました。\n");
		return nullptr;
	}

	// 接続後,インスタンス生成
	if (m_pClient == nullptr)
	{
		m_pClient = new CTcpclient;
	}

	// nullptrチェック
	if (m_pClient != nullptr)
	{
		// 初期化実行
		m_pClient->Init(socket); // 接続済みソケットで初期化
	}
	
	// クライアント情報の表示
	const char* pClientIP = inet_ntoa(clientAddr.sin_addr);

	printf("\n--- クライアント接続 ---\nIP: %s, Port: %d\n", pClientIP, PORT);

	// ポインタを返す
	return m_pClient;
}
