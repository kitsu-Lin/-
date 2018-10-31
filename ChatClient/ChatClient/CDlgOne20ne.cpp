// CDlgOne20ne.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "CDlgOne20ne.h"
#include "afxdialogex.h"
#include "CDlgChatMain.h"

// CCDlgOne20ne �Ի���

IMPLEMENT_DYNAMIC(CCDlgOne20ne, CDialogEx)

CCDlgOne20ne::CCDlgOne20ne(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCDlgOne20ne::IDD, pParent)
	, m_strShow(_T(""))
	, m_strSend(_T(""))
{

}

CCDlgOne20ne::~CCDlgOne20ne()
{
}

void CCDlgOne20ne::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strShow);
	DDX_Text(pDX, IDC_EDIT2, m_strSend);
}


BEGIN_MESSAGE_MAP(CCDlgOne20ne, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCDlgOne20ne::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCDlgOne20ne ��Ϣ�������


void CCDlgOne20ne::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
	{
		return;
	}
	CString strOne20ne;
	GetWindowText(strOne20ne);
	strOne20ne += L":" + m_strSend;
	CStringA str = CW2A(strOne20ne.GetBuffer(), CP_THREAD_ACP);
	CCDlgChatMain * pParent = (CCDlgChatMain*)GetParent();
	pParent->m_pClient->Send(ONE20NE, str.GetBuffer(), str.GetLength() + 1);
	//��ʾ���Լ��ĶԻ�����
	m_strShow += "��˵:";
	m_strShow += m_strSend;
	m_strShow += "\r\n";
	//�ѷ��������ÿ�
	m_strSend.Empty();
	UpdateData(FALSE);
}
