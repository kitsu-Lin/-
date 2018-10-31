
// ChatClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "afxdialogex.h"
#include "CDlgChatMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatClientDlg 对话框



CChatClientDlg::CChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatClientDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strPass(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
	DDX_Text(pDX, IDC_EDIT1, m_strPass);
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ProcLogin, &CChatClientDlg::OnBnClickedProclogin)
	ON_BN_CLICKED(IDC_ProcRegister, &CChatClientDlg::OnBnClickedProcregister)
	ON_BN_CLICKED(IDC_ProcAnony, &CChatClientDlg::OnBnClickedProcanony)
END_MESSAGE_MAP()


// CChatClientDlg 消息处理程序

BOOL CChatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatClientDlg::OnBnClickedProclogin()
{
	// TODO:  在此添加控件通知处理程序代码
	//获取用户名和密码
	UpdateData(TRUE);
	if (m_strName.IsEmpty() || m_strPass.IsEmpty())
	{

		MessageBox(L"用户名/密码不能为空！");
		return;
	}
	if (!m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"连接服务器失败！", L"Error!", MB_OK || MB_ICONWARNING);
		return;
	}
	//登录用户名：密码
	CString strSend = m_strName;
	strSend += L":" + m_strPass;
	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_client.Send(LOGIN, str.GetBuffer(), str.GetLength() + 1);
	char*ret = m_client.Recv();
	if (ret == nullptr)
	{
		MessageBox(L"登录失败！");
		return;
	}
	//登录成功
	//设置当前登录用户名，否则显示的是个随机的名称
	CStringA strShowName = CW2A(m_strName.GetBuffer(), CP_THREAD_ACP);
	strcpy_s(m_client.m_szName, strShowName.GetBuffer());
	//隐藏登录窗口
	ShowWindow(SW_HIDE);
	//显示聊天窗口
	CCDlgChatMain dlgChat(&m_client);
	dlgChat.m_bLogin = TRUE;
	dlgChat.DoModal();
	m_client.Close();
	CChatClientDlg::OnCancel();
}


void CChatClientDlg::OnBnClickedProcregister()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strName.IsEmpty() || m_strPass.IsEmpty())
	{
		MessageBox(L"用户名/密码不能为空！");
		return;
	}
	if (!m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"连接服务器失败！", L"Error！", MB_OK || MB_ICONWARNING);
		return;
	}
	//注册用户名：密码
	CString strSend = m_strName;
	strSend += L":" + m_strPass;
	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_client.Send(REGISTER, str.GetBuffer(), str.GetLength() + 1);
	//直接等待注册结果
	char*ret = m_client.Recv();
	if (ret == nullptr)
	{
		MessageBox(L"注册失败！");
	}
	else{
		MessageBox(L"注册成功！");
	}
	m_client.Close();
	return;
}


void CChatClientDlg::OnBnClickedProcanony()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"连接服务器失败！", L"Error!", MB_OK | MB_ICONWARNING);
		return;
	}
	//隐藏登录窗口
	ShowWindow(SW_HIDE);
	//显示匿名聊天窗口
	CCDlgChatMain dlgChat(&m_client);//调用自定义的构造函数
	dlgChat.DoModal();
	m_client.Close();
	//退出
	 CChatClientDlg::OnCancel();
}
