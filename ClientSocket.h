

#pragma once
struct pData
{
	pData()
	{
		R_img_x = 0;
		R_img_y = 0;
		R_img_z = 0;

		L_img_x = 0;
		L_img_y = 0;
		L_img_z = 0;


		xR = 0;
		yR = 0;
		zR = 0;

		xL = 0;
		yL = 0;
		zL = 0;

		distance_X = 0;
		distance_Y = 0;
		distance_Z = 0;

		rotation_x = 0;
		rotation_y = 0;
		rotation_z = 0;
	}

	float R_img_x,R_img_y,R_img_z;
	float L_img_x,L_img_y,L_img_z;
	float xR,yR,zR;
	float xL,yL,zL;
	//������
	float distance_X;
	float distance_Y;
	float distance_Z;
	
	//ȸ����
	float rotation_x;
	float rotation_y;
	float rotation_z;
};

class Data
{
public:
	Data()
	{
		x=0;
		y=0;
		z=0;
	}
	float x,y,z;
};

//�б� ������
DWORD WINAPI ReadThread(LPVOID arg);
//���� ������
DWORD WINAPI WriteThread(LPVOID arg);

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);

class CClientSocket 
{
public:
	CClientSocket();
	~CClientSocket();

	bool Connect(const char* ip,const char* port);
	void close();
	void Recv();

public:
	void SetControl(CClientSocket *Cam_input);

	CClientSocket *Cam_Control; 
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	Data right_pos;
	Data left_pos;
	Data center_pos; 
	Data img_right_pos;
	Data img_left_pos;
	Data distance_pos; 
	Data rotation_pos; 
	 
	pData Data;
	

	bool m_pFlg;

	//������ �Լ� �߰�
	HANDLE hThread[2];
	DWORD ThreadId[2];

	//std::vector<D3DXVECTOR3> distance_pos_Pack; 

	
};



