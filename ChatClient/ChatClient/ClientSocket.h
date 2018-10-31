#pragma once
#include <vector>
using std::vector;
#define BUFNAME 50 //姓名

//结构体说明
//协议设计
#define BUFMSG	2048//一次接受消息的最大字节数
//头部：通讯目的，包括匿名登录，聊天，1V1聊天，注册，登录，添加好友，搜索好友，文件传送，查看聊天记录
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
//聊天内容
//匿名聊天
typedef struct _CHATANONYMOUS{
	DWORD	dwLen;//匿名名称长度
	char buf[50];
}CHATANONYMOUS;

//聊天结构体
typedef	struct _CHATCHAT{
	DWORD dwLen;//聊天内容长度
	char buf[1024];
}CHATCHAT;

//1V1聊天结构体
typedef	struct _CHATONE20NE{
	CHAR szName[50];
	CHAR szContent[1024];
}CHATONE20NE;
//注册结构体
typedef struct _CHATREGISTER{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATREGISTER;
//登录结构体，服务器返回结果放在szName里
typedef struct _CHATLOGIN{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATLOGIN;
//添加好友结构体
typedef struct _CHATADDFRIEND{
	CHAR szName[50];//自己名字
	CHAR szFrd[50];//要添加好友的名字
}CHATADDFRIEND;
//搜索用户结构体
typedef struct _CHATSEARCHUSER{
	CHAR szName[50];//要搜索的用户名
}CHATSEARCHUSER;
//文件传送结构体
typedef struct _CHATFILETRANS{

}CHATFILETRANS;
//查看聊天记录结构体
typedef struct _CHATMSGRECORD{
	CHAR szFrom[50];
	CHAR szTo[50];
	CHAR szContent[1024];
}CHATMSGRECORD;
//更新在线用户列表
typedef struct _CHATUPDATEUSER{
	DWORD bAdd;//是加入还是退出
	DWORD dwLen;//一次只接收一个用户名
	CHAR buf[50];
}CHATUPDATEUSER;
//发送内容
typedef struct _CHATSEND{
	CHATPURPOSE m_type;
	union
	{
		char buf[BUFMSG];
		CHATANONYMOUS any;	//匿名
		CHATCHAT chat;		//聊天
		CHATONE20NE o2o;	//1v1
		CHATREGISTER reg;	//注册
		CHATLOGIN log;		//登录
		CHATADDFRIEND adf;	//添加好友
		CHATSEARCHUSER seu;	//搜索好友
		CHATFILETRANS trs;	//文件攒送
		CHATMSGRECORD rec;	//聊天记录
		CHATUPDATEUSER upd;	//更新用户列表
	}m_content;
}CHATSEND;


class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();
	//初始化客户端套接字，并参数连接服务器
	bool ConnectServer(char* szIp, WORD port);
	//负责创建线程用来接收服务器的信息
	char* Recv();
	bool Send(CHATPURPOSE purpose, char* bufSend, DWORD dwLen);
	bool Close();
private:
	//recv function
	char*RecvForAnonumous();		//接收匿名
	char*RecvForChat();				//接收聊天
	char*RecvForUpdateUserlist();	//接收更新用户列表
	char*RecvForOne20ne();			//接收1V1私聊
	char*RecvForRegister();			//接收注册
	char*RecvForLogin();			//接收登录
	char*RecvForAddFriend();		//接收添加好友
	char*RecvForSearchUser();		//接收搜索用户
	char*RecvForGetMsgRecord();		//接收聊天记录
	//send function
	void SendForAnonymous(char* bufSend, DWORD dwLen);			//发送匿名
	void SendForChat(char * bufSend, DWORD dwLen);				//发送聊天
	void SendForOne20ne(char* bufSend, DWORD dwLen);			//发送1V1私聊
	void SendForRegister(char *bufSend, DWORD dwLen);			//发送注册
	void SendForLogin(char*bufSend, DWORD dwLen);				//发送登录
	void SendForAddFriend(char*bufSend, DWORD dwLen);			//发送添加好友
	void SendForSearchUser(char* bufSend, DWORD dwLen);			//发送搜索用户
	void SendForGetMsgRecord(char *bufSend, DWORD dwLen);		//发送聊天记录
public:
	//保存聊天记录用的
	HANDLE m_hEvent = 0;
	vector<CHATMSGRECORD>m_vecMsgRecord;
	//客户端套接字
	SOCKET m_sClient = NULL;
	//新用户加入或者退出聊天室用的
	CHATUPDATEUSER * m_pObjUpdate = nullptr;
	//1V1
	CHATONE20NE *  m_pObjOne20ne = nullptr;
	//当前客户端的用户名
	char m_szName[BUFNAME] = {};
	//收到消息的时候用
	CHATSEND *m_pObjChatRecv;

	char m_bufRecv[BUFMSG] = {};
private:
};


