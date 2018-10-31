#pragma once
#include "ClientSocket.h"
#include <vector>
#include <map>
#include "afxcmn.h"
#include "DlgRecord.h"
using namespace std;

// CCDlgChatMain �Ի���

class CCDlgChatMain : public CDialogEx
{
	DECLARE_DYNAMIC(CCDlgChatMain)

public:
	CCDlgChatMain(CWnd* pParent = NULL);   // ��׼���캯��
	CCDlgChatMain::CCDlgChatMain(CClientSocket *pClient, CWnd* pParent = NULL);
	virtual ~CCDlgChatMain();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//��¼���컹����������
	BOOL m_bLogin = FALSE;
	//�߼������ͨ��������Send
	//���¼��������¼�������
	CClientSocket * m_pClient;
	//1V1����map��
	map<CString, CDialogEx*>m_map;
	//����������û��б�ʱ������ֵ
	DWORD m_dwNameIndex = 0;
	//�����¼����ָ��
	CDlgRecord * m_pDlgRecord = nullptr;
	// 	//list�ؼ��������û��б�
	CListCtrl m_listName;
	// 	Ⱥ�ı༭��
	CString m_strShow;
	// 	//������Ϣ��
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
