#pragma once
#include <vector>
using std::vector;
#define BUFNAME 50 //����

//�ṹ��˵��
//Э�����
#define BUFMSG	2048//һ�ν�����Ϣ������ֽ���
//ͷ����ͨѶĿ�ģ�����������¼�����죬1V1���죬ע�ᣬ��¼����Ӻ��ѣ��������ѣ��ļ����ͣ��鿴�����¼
enum CHATPURPOSE{
	ANONYMOUS = 1,
	CHAT,
	ONE20NE,
	REGISTER,
	LOGIN,
	ADDFRIEND,
	SEARCHUSER,
	FILETRANS,
	MSGRECORD,
	UPDATEUSER
};
//��������
//��������
typedef struct _CHATANONYMOUS{
	DWORD	dwLen;//�������Ƴ���
	char buf[50];
}CHATANONYMOUS;

//����ṹ��
typedef	struct _CHATCHAT{
	DWORD dwLen;//�������ݳ���
	char buf[1024];
}CHATCHAT;

//1V1����ṹ��
typedef	struct _CHATONE20NE{
	CHAR szName[50];
	CHAR szContent[1024];
}CHATONE20NE;
//ע��ṹ��
typedef struct _CHATREGISTER{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATREGISTER;
//��¼�ṹ�壬���������ؽ������szName��
typedef struct _CHATLOGIN{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATLOGIN;
//��Ӻ��ѽṹ��
typedef struct _CHATADDFRIEND{
	CHAR szName[50];//�Լ�����
	CHAR szFrd[50];//Ҫ��Ӻ��ѵ�����
}CHATADDFRIEND;
//�����û��ṹ��
typedef struct _CHATSEARCHUSER{
	CHAR szName[50];//Ҫ�������û���
}CHATSEARCHUSER;
//�ļ����ͽṹ��
typedef struct _CHATFILETRANS{

}CHATFILETRANS;
//�鿴�����¼�ṹ��
typedef struct _CHATMSGRECORD{
	CHAR szFrom[50];
	CHAR szTo[50];
	CHAR szContent[1024];
}CHATMSGRECORD;
//���������û��б�
typedef struct _CHATUPDATEUSER{
	DWORD bAdd;//�Ǽ��뻹���˳�
	DWORD dwLen;//һ��ֻ����һ���û���
	CHAR buf[50];
}CHATUPDATEUSER;
//��������
typedef struct _CHATSEND{
	CHATPURPOSE m_type;
	union
	{
		char buf[BUFMSG];
		CHATANONYMOUS any;	//����
		CHATCHAT chat;		//����
		CHATONE20NE o2o;	//1v1
		CHATREGISTER reg;	//ע��
		CHATLOGIN log;		//��¼
		CHATADDFRIEND adf;	//��Ӻ���
		CHATSEARCHUSER seu;	//��������
		CHATFILETRANS trs;	//�ļ�����
		CHATMSGRECORD rec;	//�����¼
		CHATUPDATEUSER upd;	//�����û��б�
	}m_content;
}CHATSEND;


class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();
	//��ʼ���ͻ����׽��֣����������ӷ�����
	bool ConnectServer(char* szIp, WORD port);
	//���𴴽��߳��������շ���������Ϣ
	char* Recv();
	bool Send(CHATPURPOSE purpose, char* bufSend, DWORD dwLen);
	bool Close();
private:
	//recv function
	char*RecvForAnonumous();		//��������
	char*RecvForChat();				//��������
	char*RecvForUpdateUserlist();	//���ո����û��б�
	char*RecvForOne20ne();			//����1V1˽��
	char*RecvForRegister();			//����ע��
	char*RecvForLogin();			//���յ�¼
	char*RecvForAddFriend();		//������Ӻ���
	char*RecvForSearchUser();		//���������û�
	char*RecvForGetMsgRecord();		//���������¼
	//send function
	void SendForAnonymous(char* bufSend, DWORD dwLen);			//��������
	void SendForChat(char * bufSend, DWORD dwLen);				//��������
	void SendForOne20ne(char* bufSend, DWORD dwLen);			//����1V1˽��
	void SendForRegister(char *bufSend, DWORD dwLen);			//����ע��
	void SendForLogin(char*bufSend, DWORD dwLen);				//���͵�¼
	void SendForAddFriend(char*bufSend, DWORD dwLen);			//������Ӻ���
	void SendForSearchUser(char* bufSend, DWORD dwLen);			//���������û�
	void SendForGetMsgRecord(char *bufSend, DWORD dwLen);		//���������¼
public:
	//���������¼�õ�
	HANDLE m_hEvent = 0;
	vector<CHATMSGRECORD>m_vecMsgRecord;
	//�ͻ����׽���
	SOCKET m_sClient = NULL;
	//���û���������˳��������õ�
	CHATUPDATEUSER * m_pObjUpdate = nullptr;
	//1V1
	CHATONE20NE *  m_pObjOne20ne = nullptr;
	//��ǰ�ͻ��˵��û���
	char m_szName[BUFNAME] = {};
	//�յ���Ϣ��ʱ����
	CHATSEND *m_pObjChatRecv;

	char m_bufRecv[BUFMSG] = {};
private:
};


