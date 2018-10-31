
// ChatClientDlg.h : ͷ�ļ�
//

#pragma once
#include "ClientSocket.h"

// CChatClientDlg �Ի���
class CChatClientDlg : public CDialogEx
{
// ����
public:
	CChatClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHATCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	CClientSocket m_client;
	afx_msg void OnBnClickedProclogin();
	afx_msg void OnBnClickedProcregister();
	afx_msg void OnBnClickedProcanony();
	CString m_strName;
	CString m_strPass;
};
