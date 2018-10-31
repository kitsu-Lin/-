#pragma once
#include "ClientSocket.h"
#include <vector>
#include <map>
#include "afxcmn.h"
#include "DlgRecord.h"
using namespace std;

// CCDlgChatMain 对话框

class CCDlgChatMain : public CDialogEx
{
	DECLARE_DYNAMIC(CCDlgChatMain)

public:
	CCDlgChatMain(CWnd* pParent = NULL);   // 标准构造函数
	CCDlgChatMain::CCDlgChatMain(CClientSocket *pClient, CWnd* pParent = NULL);
	virtual ~CCDlgChatMain();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//登录聊天还是匿名聊天
	BOOL m_bLogin = FALSE;
	//逻辑类对象，通过它调用Send
	//点登录或匿名登录界面进来
	CClientSocket * m_pClient;
	//1V1聊天map表
	map<CString, CDialogEx*>m_map;
	//鼠标点击在线用户列表时的索引值
	DWORD m_dwNameIndex = 0;
	//聊天记录窗口指针
	CDlgRecord * m_pDlgRecord = nullptr;
	// 	//list控件，在线用户列表
	CListCtrl m_listName;
	// 	群聊编辑框
	CString m_strShow;
	// 	//发送消息框
	CString m_strSend;
	afx_msg void OnBnClickedButton1();
protected:
	afx_msg LRESULT OnMysocket(WPARAM wParam, LPARAM lParam);
protected:
	void InserOrDeleteUser(CHATUPDATEUSER &objUpdate);
protected:
	void ChatForOne20ne(CHATONE20NE objOne20ne);
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddFriend();
	afx_msg void OnSearch();
	afx_msg void OnGetChatRecord();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
