#pragma once


// CDlgSearch �Ի���

class CDlgSearch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSearch)

public:
	CDlgSearch(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSearch();
// �Ի�������
	enum { IDD = IDD_DIALOG3 };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedButton1();
	CString m_strSearch;
};
