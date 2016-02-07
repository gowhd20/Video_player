#include "stdafx.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
	//Data = NULL;
	m_pFlg = false;
	/*right_pos = D3DXVECTOR3(0,0,0);
	left_pos  = D3DXVECTOR3(0,0,0);
	img_left_pos = D3DXVECTOR3(0,0,0);
	img_right_pos = D3DXVECTOR3(0,0,0);
	Center_pos    = D3DXVECTOR3(0,0,0);*/
}

CClientSocket::~CClientSocket()
{
}

bool CClientSocket::Connect(const char* ip,const char* port)
{
	bool flag = false;
	char message[30];
	int strLen  = 0;
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) !=0)
		return flag;

	hSocket=socket(PF_INET, SOCK_STREAM, 0);
	if(hSocket==INVALID_SOCKET)
		return flag;

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=inet_addr(ip);
	servAddr.sin_port=htons(atoi(port));

	
	if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
		return false;


	// 읽기 스레드 생성
	hThread[0] = CreateThread(NULL, 0, ReadThread, (LPVOID)this, 0, &ThreadId[0]);
	

	if(hThread[0] == NULL ){
		MessageBox(NULL,L"스레드를 시작할 수 없습니다."
			L"\r\n프로그램을 종료합니다.", 
			L"실패", MB_ICONERROR);
		exit(-1);
	}
	return true;
}


void CClientSocket::Recv()
{
	//int strLen = 0;
	
	pData Data;
	recv(hSocket, (char*)&Data, sizeof(pData), 0);
	//if(strLen == -1) return;
		//키넥트 사용
	/*right_pos.x	=   -Data.xR;
	right_pos.y	=   -Data.yR;
	right_pos.z =	-4;

	left_pos.x =	-Data.xL;
	left_pos.y =	-Data.yL;
	left_pos.z =	 -4;
			
	//토비 사용flal
	img_left_pos.x = Data.L_img_x;
	img_left_pos.y = Data.L_img_y;
	img_left_pos.z = Data.L_img_z;


	img_right_pos.x = Data.R_img_x;
	img_right_pos.y = Data.R_img_y;
	img_right_pos.z = Data.R_img_z;
	Center_pos = (left_pos + right_pos) / 2.0f;
			
	
		*/
}
	


//쓰레드 종류
void CClientSocket::close()
{
	TerminateThread(hThread[0], -1);
	CloseHandle(hThread[0]);
	MessageBox(NULL,L"서버가 접속을 끊었습니다",L"알림", MB_ICONINFORMATION);
	closesocket(hSocket);
	WSACleanup();
}


void CClientSocket::SetControl(CClientSocket* Cam_input)
{
	
	Cam_Control = Cam_input;
}


// 데이터 받기
DWORD WINAPI ReadThread(LPVOID arg)
{
	CClientSocket* sock = (CClientSocket*)arg;
	//CHATMSG chatmsg;
	//DRAWMSG drawmsg;
	int type;
	int retval;
	pData Data;
	while(1)
	{
		retval = recvn(sock->hSocket, (char *)&Data, sizeof(pData), 0);
		if(retval == 0 || retval == SOCKET_ERROR){
			break;
		}
		
		//키넥트 사용
		sock->right_pos.x	=   -Data.xR;
		sock->right_pos.y	=   -Data.yR;
		sock->right_pos.z =	-4;

		sock->left_pos.x =	-Data.xL;
		sock->left_pos.y =	-Data.yL;
		sock->left_pos.z =	 -4;
			
		//토비 사용
		sock->img_left_pos.x = Data.L_img_x;
		sock->img_left_pos.y = Data.L_img_y;
		sock->img_left_pos.z = Data.L_img_z;


		sock->img_right_pos.x = Data.R_img_x;
		sock->img_right_pos.y = Data.R_img_y;
		sock->img_right_pos.z = Data.R_img_z;


		sock->distance_pos.x =  Data.distance_X;
		sock->distance_pos.y =  Data.distance_Y;
		sock->distance_pos.z =  Data.distance_Z;

		sock->rotation_pos.x = Data.rotation_x;
		sock->rotation_pos.y = Data.rotation_y;
		sock->rotation_pos.z = Data.rotation_z;

		
		//sock->distance_pos_Pack.push_back(sock->distance_pos);
		//sock->center_pos = (sock->left_pos + sock->right_pos) / 2.0f;
		
		//distance_pos_Pack
	}
	// closesocket()
	//closesocket(sock->hSocket);

	return 0;
}


// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR) 
			return SOCKET_ERROR;
		else if(received == 0) 
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}