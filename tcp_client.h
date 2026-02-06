//=========================================================
//
// TCP送受信管理処理 [ tcp_client.h ]
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
#include "main.h"

//*********************************************************
// TCP送受信を管理するクラスを定義
//*********************************************************
class CTcpclient
{
public:

	CTcpclient();
	~CTcpclient();

	bool Init(const char* pIPAddress, int nPortNum);
	int Send(char* pSendData, int nSendDataSize);
	int Recv(char* pRecvData, int nRecvDataSize);
	void Uninit(void);
	bool Init(SOCKET socket);
	SOCKET GetSock(void) { return m_sock; }

private:

	SOCKET m_sock;	// 通信用ソケット
};