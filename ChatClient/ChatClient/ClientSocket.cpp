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

//连接服务器：ConnectServer
bool CClientSocket::ConnectServer(char* szIp, WORD port){
	//初始化套接字动态库
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}
	//检查返回的库版本是否2.2
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}
	//创建套接字
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}
	//设置服务器地址
	SOCKADDR_IN servAddr = {};//服务器地址
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(szIp);
	servAddr.sin_port = htons((short)port);
	//连接服务器
	//SOCKADDR sService;//跟服务端相连的套接字
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

//关闭套接字，结束连接；
bool CClientSocket::Close(){
	WSACleanup();
	closesocket(m_sClient);
	return true;
}

//发送消息  Send函数
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

//Recv函数 接收消息
char *CClientSocket::Recv(){
	//再接收消息
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

//给服务器发消息通知其他客户端
void CClientSocket::SendForAnonymous(char* bufSend, DWORD dwLen){
	CHATSEND ct = { ANONYMOUS };
	//匿名 长度+昵称
	strcpy_s(ct.m_content.chat.buf, bufSend);
	ct.m_content.chat.dwLen = dwLen;
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//客户端响应ANONYMOUS消息和UPDATEUSER消息
char *CClientSocket::RecvForAnonumous(){
	sprintf_s(m_bufRecv, BUFMSG, "%s加入聊天室!\n",
		m_pObjChatRecv->m_content.any.buf);
	return m_bufRecv;
}

char*CClientSocket::RecvForUpdateUserlist(){
	//新用户加入，更新到用户列表窗口
	m_pObjUpdate = new CHATUPDATEUSER;
	memcpy_s(m_pObjUpdate, sizeof(CHATUPDATEUSER), &m_pObjChatRecv->m_content.upd,
		sizeof(CHATUPDATEUSER));
	return nullptr;
}

//私聊Send函数SendForOne20ne的处理
void CClientSocket::SendForOne20ne(char* bufSend, DWORD dwLen){
	CHATSEND ct = { ONE20NE };
	//姓名 ：内容
	char* nextToken = nullptr;
	char* szContext = strtok_s(bufSend, ":", &nextToken);
	memcpy_s(ct.m_content.o2o.szName, nextToken - bufSend, bufSend, nextToken - bufSend);
	memcpy_s(ct.m_content.o2o.szContent, strlen(nextToken), nextToken, strlen(nextToken));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//私聊另一方对One20ne消息的响应：
char*CClientSocket::RecvForOne20ne(){
	//新用户加入，更新到用户列表窗口
	m_pObjOne20ne = new CHATONE20NE;
	memcpy_s(m_pObjOne20ne, sizeof(CHATONE20NE), &m_pObjChatRecv->m_content.o2o, sizeof(CHATONE20NE));
	return nullptr;
}

//通讯加密
void	CClientSocket::SendForChat(char * bufSend, DWORD dwLen){
	CHATSEND ct{ CHAT };
	//聊天 长度+姓名：内容
	strcpy_s(ct.m_content.chat.buf, m_szName);
	strcat_s(ct.m_content.chat.buf, ":");
	strcat_s(ct.m_content.chat.buf, bufSend);
	ct.m_content.chat.dwLen = strlen(ct.m_content.chat.buf) + 1;
	//加密
	for (int i = 0; i < ct.m_content.any.dwLen; i++)
	{
		ct.m_content.chat.buf[i] ^= 15;
	}
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}
//其他客户端收到该群聊消息需要解密才能正常显示：
char* CClientSocket::RecvForChat(){
	//解密
	for (int i = 0; i < m_pObjChatRecv->m_content.chat.dwLen; i++)
	{
		m_pObjChatRecv->m_content.chat.buf[i] ^= 15;
	}
	//正常代码
	strcpy_s(m_bufRecv, m_pObjChatRecv->m_content.chat.buf);
	return	m_bufRecv;
}

//注册
void CClientSocket::SendForRegister(char *bufSend, DWORD dwLen){
	CHATSEND ct = { REGISTER };
	char*  pwd = nullptr;
	strtok_s(bufSend, ":", &pwd);
	memcpy_s(ct.m_content.reg.szName, pwd - bufSend, bufSend, pwd - bufSend);
	memcpy_s(ct.m_content.reg.szPwd, strlen(pwd), pwd, strlen(pwd));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

char* CClientSocket::RecvForRegister(){
	if (!strcmp(m_pObjChatRecv->m_content.log.szName, "注册成功"))
	{
		return "注册成功";
	}
	else
	{
		return "注册失败";
	}
}
//登陆的执行流程和注册相似，
void CClientSocket::SendForLogin(char*bufSend, DWORD dwLen){
	CHATSEND ct = { LOGIN };
	char*pwd = nullptr;
	strtok_s(bufSend, ":", &pwd);
	memcpy_s(ct.m_content.log.szName, pwd - bufSend, bufSend, pwd - bufSend);
	memcpy_s(ct.m_content.log.szPwd, strlen(pwd),pwd,strlen(pwd));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//客户端对查询结果的处理
char*CClientSocket::RecvForLogin(){
	if (!strcmp(m_pObjChatRecv->m_content.log.szName, "登录成功"))
	{
		return "登录成功";
	}
	else{
		return "登录失败";
	}
}

//添加好友send函数的ADDRIEND函数
void CClientSocket::SendForAddFriend(char*bufSend, DWORD dwLen){
	CHATSEND ct = { ADDFRIEND };
	char *frd = nullptr;
	//构造内容 谁要添加：添加谁
	strtok_s(bufSend, ":", &frd);
	memcpy_s(ct.m_content.adf.szName, frd - bufSend, bufSend, frd - bufSend);
	memcpy_s(ct.m_content.adf.szFrd, strlen(frd), frd, strlen(frd));
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//添加好友结果处理
char*CClientSocket::RecvForAddFriend(){
	MessageBoxA(NULL, m_pObjChatRecv->m_content.adf.szName, "添加好友", MB_OK);
	return nullptr;
}

//查询函数
void CClientSocket::SendForSearchUser(char* bufSend, DWORD dwLen){
	CHATSEND ct = { SEARCHUSER };
	memcpy_s(ct.m_content.seu.szName, dwLen, bufSend, dwLen);
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}

//客户端查询结果处理
char*CClientSocket::RecvForSearchUser(){
	MessageBoxA(NULL, m_pObjChatRecv->m_content.seu.szName, "搜索用户", MB_OK);
	return nullptr;
}

//聊天记录
void CClientSocket::SendForGetMsgRecord(char *bufSend, DWORD dwLen){
	CHATSEND ct = { MSGRECORD };
	//服务器保存有当前客户端的名称，所以查询所有的聊天记录只需要发送个消息类型的就可以了
	send(m_sClient, (char*)&ct, sizeof(ct), NULL);
}


//客户端对聊天记录处理
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
















