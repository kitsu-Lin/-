#include "stdafx.h"
#include "ClientSocket.h"
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

//���ӷ�������ConnectServer
bool CClientSocket::ConnectServer(char* szIp, WORD port){
	//��ʼ���׽��ֶ�̬��
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}
	//��鷵�صĿ�汾�Ƿ�2.2
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}
	//�����׽���
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}
	//���÷�������ַ
	SOCKADDR_IN servAddr = {};//��������ַ
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(szIp);
	servAddr.sin_port = htons((short)port);
	//���ӷ�����
	//SOCKADDR sService;//��������������׽���
	int nRetValue = connect(m_sClient, (sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
	if (nRetValue == SOCKET_ERROR)
	{
		closesocket(m_sClient);
		m_sClient = NULL;
		WSACleanup();
		return false;
	}
	return true;
}

//�ر��׽��֣��������ӣ�
bool CClientSocket::Close(){
	WSACleanup();
	closesocket(m_sClient);
	return true;
}

//������Ϣ  Send����
bool CClientSocket::Send(CHATPURPOSE purpose, char* bufSend, DWORD dwLen){
	//	Send(CHATPURPOSE purpose, char* bufSend, DWORD dwLen);
	switch (purpose)
	{
	case ANONYMOUS:
		SendForAnonymous(bufSend, dwLen);
		break;
	case CHAT:
		SendForChat(bufSend, dwLen);
		break;
	case ONE20NE:
		SendForOne20ne(bufSend, dwLen);
		break;
	case REGISTER:
		SendForRegister(bufSend, dwLen);
		break;
	case LOGIN:
		SendForLogin(bufSend, dwLen);
		break;
	case ADDFRIEND:
		SendForAddFriend(bufSend, dwLen);
		break;
	case SEARCHUSER:
		SendForSearchUser(bufSend, dwLen);
		break;
	case FILETRANS:
		break;
	case MSGRECORD:
		SendForGetMsgRecord(bufSend, dwLen);
		break;
	case UPDATEUSER:
		break;
	default:
		break;
	}
	return true;
}

//Recv���� ������Ϣ
char *CClientSocket::Recv(){
	//�ٽ�����Ϣ
	CHATSEND ct = {};
	if (SOCKET_ERROR == recv(m_sClient, (char*)&ct, sizeof(CHATSEND), NULL))
	{
		printf("recv error!\n");
		return nullptr;
	}
	m_pObjChatRecv = &ct;
	switch (ct.m_type)
	{
	case ANONYMOUS:
		return RecvForAnonumous();
		break;
	case CHAT:
		return RecvForChat();
		break;
	case ONE20NE:
		return RecvForOne20ne();
		break;
	case REGISTER:
		return RecvForRegister();
		break;
	case LOGIN:
		return RecvForLogin();
		break;
	case ADDFRIEND:
		return RecvForAddFriend();
		break;
	case SEARCHUSER:
		return RecvForSearchUser();
		break;
	case FILETRANS:
		break;
	case MSGRECORD:
		return RecvForGetMsgRecord();
		break;
	case UPDATEUSER:
		return RecvForUpdateUserlist();
		break;
	default:
		break;
	}
	return nullptr;
}

//������������Ϣ֪ͨ�����ͻ���
void CClientSocket::SendForAnonymous(char* bufSend, DWORD dwLen){
	CHATSEND ct = { ANONYMOUS };
	//���� ����+�ǳ�
	strcpy_s(ct.m_content.chat.buf, bufSend);
	ct.m_content.chat.dwLen = dwLen;
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//�ͻ�����ӦANONYMOUS��Ϣ��UPDATEUSER��Ϣ
char *CClientSocket::RecvForAnonumous(){
	sprintf_s(m_bufRecv, BUFMSG, "%s����������!\n",
		m_pObjChatRecv->m_content.any.buf);
	return m_bufRecv;
}

char*CClientSocket::RecvForUpdateUserlist(){
	//���û����룬���µ��û��б���
	m_pObjUpdate = new CHATUPDATEUSER;
	memcpy_s(m_pObjUpdate, sizeof(CHATUPDATEUSER), &m_pObjChatRecv->m_content.upd,
		sizeof(CHATUPDATEUSER));
	return nullptr;
}

//˽��Send����SendForOne20ne�Ĵ���
void CClientSocket::SendForOne20ne(char* bufSend, DWORD dwLen){
	CHATSEND ct = { ONE20NE };
	//���� ������
	char* nextToken = nullptr;
	char* szContext = strtok_s(bufSend, ":", &nextToken);
	memcpy_s(ct.m_content.o2o.szName, nextToken - bufSend, bufSend, nextToken - bufSend);
	memcpy_s(ct.m_content.o2o.szContent, strlen(nextToken), nextToken, strlen(nextToken));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//˽����һ����One20ne��Ϣ����Ӧ��
char*CClientSocket::RecvForOne20ne(){
	//���û����룬���µ��û��б���
	m_pObjOne20ne = new CHATONE20NE;
	memcpy_s(m_pObjOne20ne, sizeof(CHATONE20NE), &m_pObjChatRecv->m_content.o2o, sizeof(CHATONE20NE));
	return nullptr;
}

//ͨѶ����
void	CClientSocket::SendForChat(char * bufSend, DWORD dwLen){
	CHATSEND ct{ CHAT };
	//���� ����+����������
	strcpy_s(ct.m_content.chat.buf, m_szName);
	strcat_s(ct.m_content.chat.buf, ":");
	strcat_s(ct.m_content.chat.buf, bufSend);
	ct.m_content.chat.dwLen = strlen(ct.m_content.chat.buf) + 1;
	//����
	for (int i = 0; i < ct.m_content.any.dwLen; i++)
	{
		ct.m_content.chat.buf[i] ^= 15;
	}
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}
//�����ͻ����յ���Ⱥ����Ϣ��Ҫ���ܲ���������ʾ��
char* CClientSocket::RecvForChat(){
	//����
	for (int i = 0; i < m_pObjChatRecv->m_content.chat.dwLen; i++)
	{
		m_pObjChatRecv->m_content.chat.buf[i] ^= 15;
	}
	//��������
	strcpy_s(m_bufRecv, m_pObjChatRecv->m_content.chat.buf);
	return	m_bufRecv;
}

//ע��
void CClientSocket::SendForRegister(char *bufSend, DWORD dwLen){
	CHATSEND ct = { REGISTER };
	char*  pwd = nullptr;
	strtok_s(bufSend, ":", &pwd);
	memcpy_s(ct.m_content.reg.szName, pwd - bufSend, bufSend, pwd - bufSend);
	memcpy_s(ct.m_content.reg.szPwd, strlen(pwd), pwd, strlen(pwd));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

char* CClientSocket::RecvForRegister(){
	if (!strcmp(m_pObjChatRecv->m_content.log.szName, "ע��ɹ�"))
	{
		return "ע��ɹ�";
	}
	else
	{
		return "ע��ʧ��";
	}
}
//��½��ִ�����̺�ע�����ƣ�
void CClientSocket::SendForLogin(char*bufSend, DWORD dwLen){
	CHATSEND ct = { LOGIN };
	char*pwd = nullptr;
	strtok_s(bufSend, ":", &pwd);
	memcpy_s(ct.m_content.log.szName, pwd - bufSend, bufSend, pwd - bufSend);
	memcpy_s(ct.m_content.log.szPwd, strlen(pwd),pwd,strlen(pwd));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//�ͻ��˶Բ�ѯ����Ĵ���
char*CClientSocket::RecvForLogin(){
	if (!strcmp(m_pObjChatRecv->m_content.log.szName, "��¼�ɹ�"))
	{
		return "��¼�ɹ�";
	}
	else{
		return "��¼ʧ��";
	}
}

//��Ӻ���send������ADDRIEND����
void CClientSocket::SendForAddFriend(char*bufSend, DWORD dwLen){
	CHATSEND ct = { ADDFRIEND };
	char *frd = nullptr;
	//�������� ˭Ҫ��ӣ����˭
	strtok_s(bufSend, ":", &frd);
	memcpy_s(ct.m_content.adf.szName, frd - bufSend, bufSend, frd - bufSend);
	memcpy_s(ct.m_content.adf.szFrd, strlen(frd), frd, strlen(frd));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//��Ӻ��ѽ������
char*CClientSocket::RecvForAddFriend(){
	MessageBoxA(NULL, m_pObjChatRecv->m_content.adf.szName, "��Ӻ���", MB_OK);
	return nullptr;
}

//��ѯ����
void CClientSocket::SendForSearchUser(char* bufSend, DWORD dwLen){
	CHATSEND ct = { SEARCHUSER };
	memcpy_s(ct.m_content.seu.szName, dwLen, bufSend, dwLen);
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//�ͻ��˲�ѯ�������
char*CClientSocket::RecvForSearchUser(){
	MessageBoxA(NULL, m_pObjChatRecv->m_content.seu.szName, "�����û�", MB_OK);
	return nullptr;
}

//�����¼
void CClientSocket::SendForGetMsgRecord(char *bufSend, DWORD dwLen){
	CHATSEND ct = { MSGRECORD };
	//�����������е�ǰ�ͻ��˵����ƣ����Բ�ѯ���е������¼ֻ��Ҫ���͸���Ϣ���͵ľͿ�����
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}


//�ͻ��˶������¼����
char* CClientSocket::RecvForGetMsgRecord(){
	if (!strcmp(m_pObjChatRecv->m_content.rec.szFrom, "~~~end~~~"))
	{
		SetEvent(m_hEvent);
	}
	else{
		m_vecMsgRecord.push_back(m_pObjChatRecv->m_content.rec);
	}
	return nullptr;
}
















