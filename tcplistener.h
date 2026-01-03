//=========================================================
//
// サーバー用クラス処理 [ tcplistener.h ]
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
#include "tcp_client.h"

class CTcpclient;

//*********************************************************
// サーバークラスを定義
//*********************************************************
class CTcplistener
{
public:

	CTcplistener();
	~CTcplistener();

	bool Init(int nPortNum);
	void Uninit(void);		

	CTcpclient* Accept(void);
	SOCKET GetSock(void) { return m_sockServer; }

private:

	CTcpclient* m_pClient;

	SOCKET m_sockServer;	// サーバー用ソケット
};

