#pragma once


// CCDlgOne20ne 对话框

class CCDlgOne20ne : public CDialogEx
{
	DECLARE_DYNAMIC(CCDlgOne20ne)

public:
	CCDlgOne20ne(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCDlgOne20ne();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 聊天编辑框
	CString m_strShow;
	// 发送编辑框
	CString m_strSend;
	afx_msg void OnBnClickedButton1();
};
