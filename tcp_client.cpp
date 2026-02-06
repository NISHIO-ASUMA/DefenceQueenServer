//=========================================================
//
// TCP送受信管理 [ tcp_client.h ]
// Author : Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル宣言
//*********************************************************
#include "tcp_client.h"
#include <cstdio>

//=========================================================
// コンストラクタ
//=========================================================
CTcpclient::CTcpclient()
{
	// 値のクリア
	m_sock = INVALID_SOCKET;
}
//=========================================================
// デストラクタ
//=========================================================
CTcpclient::~CTcpclient()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
bool CTcpclient::Init(const char* pIPAddress, int nPortNum)
{
	// ソケット作成
	m_sock = socket(AF_INET, SOCK_STREAM, 0);

	// 例外処理
	if (m_sock == INVALID_SOCKET)
	{
		// 例外メッセージ
		printf("\nソケットの作成に失敗しました 終了します\n");

		// falseを返す
		return false;
	}

	// サーバー接続情報設定
	pIPAddress = "127.0.0.1"; // IPアドレスは引数設定
	struct sockaddr_in serverAddr = { 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(nPortNum);	// ポート番号
	serverAddr.sin_addr.S_un.S_addr = inet_addr(pIPAddress);

	// サーバーへ接続
	if (connect(m_sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
	{// 失敗時
		printf("\n接続失敗 終了します\n");

		// falseを返す
		return false;
	}

	// 成功結果を返す
	return true;
}
//=========================================================
// データ送信処理
//=========================================================
int CTcpclient::Send(char* pSendData, int nSendDataSize)
{
	// NULLチェック
	if (m_sock == INVALID_SOCKET)
	{
		// 接続されていない
		return 0;
	}

	// バイト数格納用変数
	int nSendByte = 0;

	// ここでエンディアン変換する
	nSendByte = htonl(nSendByte);

	// データを送信
	nSendByte = send(m_sock, pSendData, nSendDataSize, 0);

	// 送信データのサイズを返す
	return nSendByte;
}
//=========================================================
// データ受信処理
//=========================================================
int CTcpclient::Recv(char* pRecvData, int nRecvDataSize)
{
	// NULLチェック
	if (m_sock == INVALID_SOCKET)
	{
		// 接続されていない
		return 0;
	}

	// 受信用変数
	int nRecvByte = 0;

	// データ受信
	nRecvByte = recv(m_sock, pRecvData, nRecvDataSize, 0);

	// 例外処理
	if (nRecvByte <= 0)
	{
		// 終了処理
		Uninit();
	}

	// エンディアン変換する
	nRecvByte = ntohl(nRecvByte);

	// 受信データを返す
	return nRecvByte;
}
//=========================================================
// 終了処理
//=========================================================
void CTcpclient::Uninit(void)
{
	// NULLチェック
	if (m_sock == INVALID_SOCKET)
	{
		// 接続されていない時
		return;
	}

	// クライアント切断
	closesocket(m_sock);

	// ソケットをNULL化する
	m_sock = INVALID_SOCKET;
}
//=========================================================
// オーバーロード初期化
//=========================================================
bool CTcpclient::Init(SOCKET socket)
{
	// 代入
	m_sock = socket;

	return true;
}