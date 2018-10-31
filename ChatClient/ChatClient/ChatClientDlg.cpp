
// ChatClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "afxdialogex.h"
#include "CDlgChatMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CChatClientDlg �Ի���



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


// CChatClientDlg ��Ϣ�������

BOOL CChatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatClientDlg::OnBnClickedProclogin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ȡ�û���������
	UpdateData(TRUE);
	if (m_strName.IsEmpty() || m_strPass.IsEmpty())
	{

		MessageBox(L"�û���/���벻��Ϊ�գ�");
		return;
	}
	if (!m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"���ӷ�����ʧ�ܣ�", L"Error!", MB_OK || MB_ICONWARNING);
		return;
	}
	//��¼�û���������
	CString strSend = m_strName;
	strSend += L":" + m_strPass;
	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_client.Send(LOGIN, str.GetBuffer(), str.GetLength() + 1);
	char*ret = m_client.Recv();
	if (ret == nullptr)
	{
		MessageBox(L"��¼ʧ�ܣ�");
		return;
	}
	//��¼�ɹ�
	//���õ�ǰ��¼�û�����������ʾ���Ǹ����������
	CStringA strShowName = CW2A(m_strName.GetBuffer(), CP_THREAD_ACP);
	strcpy_s(m_client.m_szName, strShowName.GetBuffer());
	//���ص�¼����
	ShowWindow(SW_HIDE);
	//��ʾ���촰��
	CCDlgChatMain dlgChat(&m_client);
	dlgChat.m_bLogin = TRUE;
	dlgChat.DoModal();
	m_client.Close();
	CChatClientDlg::OnCancel();
}


void CChatClientDlg::OnBnClickedProcregister()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_strName.IsEmpty() || m_strPass.IsEmpty())
	{
		MessageBox(L"�û���/���벻��Ϊ�գ�");
		return;
	}
	if (!m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"���ӷ�����ʧ�ܣ�", L"Error��", MB_OK || MB_ICONWARNING);
		return;
	}
	//ע���û���������
	CString strSend = m_strName;
	strSend += L":" + m_strPass;
	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_client.Send(REGISTER, str.GetBuffer(), str.GetLength() + 1);
	//ֱ�ӵȴ�ע����
	char*ret = m_client.Recv();
	if (ret == nullptr)
	{
		MessageBox(L"ע��ʧ�ܣ�");
	}
	else{
		MessageBox(L"ע��ɹ���");
	}
	m_client.Close();
	return;
}


void CChatClientDlg::OnBnClickedProcanony()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"���ӷ�����ʧ�ܣ�", L"Error!", MB_OK | MB_ICONWARNING);
		return;
	}
	//���ص�¼����
	ShowWindow(SW_HIDE);
	//��ʾ�������촰��
	CCDlgChatMain dlgChat(&m_client);//�����Զ���Ĺ��캯��
	dlgChat.DoModal();
	m_client.Close();
	//�˳�
	 CChatClientDlg::OnCancel();
}
