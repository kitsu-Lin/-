#pragma once


// CCDlgOne20ne �Ի���

class CCDlgOne20ne : public CDialogEx
{
	DECLARE_DYNAMIC(CCDlgOne20ne)

public:
	CCDlgOne20ne(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCDlgOne20ne();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����༭��
	CString m_strShow;
	// ���ͱ༭��
	CString m_strSend;
	afx_msg void OnBnClickedButton1();
};
