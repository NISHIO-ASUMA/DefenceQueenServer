//====================================================
//
// TCP送受信管理 [ tcp_client.h ]
// Author : Asuma Nishio
//
//====================================================

//**************************** 
// インクルードガード
//**************************** 
#pragma once

//**************************** 
// インクルードファイル宣言
//****************************
#include "main.h"

//****************************
// クラスを定義
//****************************
class CTcpclient
{
public:
	CTcpclient();
	~CTcpclient();

	bool Init(const char* pIPAddress, int nPortNum); // 初期化
	int Send(char* pSendData, int nSendDataSize);	 // データ送信
	int Recv(char* pRecvData, int nRecvDataSize);	 // データ受信
	void Uninit(void);								 // 終了(切断)処理
	bool Init(SOCKET socket);			// オーバーロード初期化
	SOCKET GetSock(void) { return m_sock; }

private:
	SOCKET m_sock;   // 通信用ソケットを一つだけ宣言
};