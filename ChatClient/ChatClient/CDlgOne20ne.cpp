// CDlgOne20ne.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "CDlgOne20ne.h"
#include "afxdialogex.h"
#include "CDlgChatMain.h"

// CCDlgOne20ne 对话框

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


// CCDlgOne20ne 消息处理程序


void CCDlgOne20ne::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
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
	//显示在自己的对话框上
	m_strShow += "你说:";
	m_strShow += m_strSend;
	m_strShow += "\r\n";
	//把发送内容置空
	m_strSend.Empty();
	UpdateData(FALSE);
}
