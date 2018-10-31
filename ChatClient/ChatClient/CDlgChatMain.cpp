// CDlgChatMain.cpp : 实现文件
//

#include "stdafx.h"
#include "ChatClient.h"
#include "CDlgChatMain.h"
#include "afxdialogex.h"
#include "CDlgOne20ne.h"
#include "DlgSearch.h"


int m_Count = 0;
#define WM_MYSOCKET WM_USER+100

// CCDlgChatMain 对话框

void CCDlgChatMain::InserOrDeleteUser(CHATUPDATEUSER &objUpdate)
{
  	if (objUpdate.bAdd == 1)//加入
  	{
  		int n = m_listName.GetItemCount();
  		m_listName.InsertItem(n + 1, CA2W(objUpdate.buf));
  	}
  	else
  	{
  		for (int n = 0; n < m_listName.GetItemCount(); ++n)
  		{
  			if (!m_listName.GetItemText(n, 0).Compare((CString)objUpdate.buf))
  			{
  				m_listName.DeleteItem(n);
  				return;
  			}
  		}
  	}

}



IMPLEMENT_DYNAMIC(CCDlgChatMain, CDialogEx)

CCDlgChatMain::CCDlgChatMain(CWnd* pParent /*=NULL*/)
: CDialogEx(CCDlgChatMain::IDD, pParent)
, m_strShow(_T(""))
, m_strSend(_T(""))
{

}

CCDlgChatMain::CCDlgChatMain(CClientSocket *pClient, CWnd* pParent /*= NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent), m_pClient(pClient)
{
}

CCDlgChatMain::~CCDlgChatMain()
{
}

void CCDlgChatMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listName);
	DDX_Text(pDX, IDC_EDIT3, m_strShow);
	DDX_Text(pDX, IDC_EDIT1, m_strSend);
}


BEGIN_MESSAGE_MAP(CCDlgChatMain, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCDlgChatMain::OnBnClickedButton1)
	ON_MESSAGE(WM_MYSOCKET, &CCDlgChatMain::OnMysocket)
	ON_EN_CHANGE(IDC_EDIT1, &CCDlgChatMain::OnEnChangeEdit1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CCDlgChatMain::OnLvnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CCDlgChatMain::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CCDlgChatMain::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CCDlgChatMain::OnAddFriend)
	ON_COMMAND(ID_32772, &CCDlgChatMain::OnSearch)
	ON_COMMAND(ID_32773, &CCDlgChatMain::OnGetChatRecord)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCDlgChatMain 消息处理程序


BOOL CCDlgChatMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置窗口标题为用户昵称
	//匿名登录时m_pClient->m_szName是个随机值，构造函数内初始化
	//登录的时候是登录用户名，点登录按钮的时候初始化
	CString strTitle(m_pClient->m_szName);
	SetWindowText(strTitle.GetBuffer());
	//设置窗口任务栏可见
	ModifyStyleEx(0, WS_EX_APPWINDOW);
	//在线用户列表
	m_listName.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listName.InsertColumn(0, L"在线列表", 0, 100);

// 	CString k(m_pClient->m_szName);
// 	m_listName.InsertItem(Num, NULL);
// 
// 	m_listName.SetItemText(Num, 0, k);
// 	Num += 1;

	//使用消息选择函数模型绑定socket和当前对话框
	//1.define自定义消息WM_SOCKET
	//2.类向导添加自定义消息（WM_SCOKET）响应函数OnMysocket
	//WSAAsyncSelect绑定socket、窗口和消息
	WSAAsyncSelect(m_pClient->m_sClient, m_hWnd, WM_MYSOCKET, FD_READ | FD_CLOSE);
	//告诉服务器自己的昵称
	m_pClient->Send(ANONYMOUS, m_pClient->m_szName, strlen(m_pClient->m_szName) + 1);
	m_pClient->m_hEvent = CreateEvent(0, FALSE, TRUE, (LPCWSTR)"m_Event");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CCDlgChatMain::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
	{
		return;
	}
	CStringA str = CW2A(m_strSend.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(CHAT, str.GetBuffer(), str.GetLength() + 1);
	m_strShow += "你说:";
	m_strShow += m_strSend;
	m_strShow += "\r\n";
	m_strSend.Empty();
	UpdateData(FALSE);
}


afx_msg LRESULT CCDlgChatMain::OnMysocket(WPARAM wParam, LPARAM lParam)
{
	SOCKET s = wParam;
	WORD wEvent = WSAGETSELECTEVENT(lParam);
	WORD wErrorCode = WSAGETSELECTERROR(lParam);

	if (wErrorCode)
	{
		exit(0);
		MessageBox(L"网络错误！");
		CDialogEx::OnCancel();
	}
	switch (wEvent)
	{
	case FD_READ:
	{
		char *szRecv = m_pClient->Recv();
		if (szRecv == nullptr)
		{
			if (m_pClient->m_pObjUpdate)
			{
				InserOrDeleteUser(*m_pClient->m_pObjUpdate);
				delete m_pClient->m_pObjUpdate;
				m_pClient->m_pObjUpdate = nullptr;
			}
			else if (m_pClient->m_pObjOne20ne)
			{
				//1v1聊天
				ChatForOne20ne(*m_pClient->m_pObjOne20ne);
				delete m_pClient->m_pObjOne20ne;
				m_pClient->m_pObjOne20ne = nullptr;
			}
			return 0;
		}
		UpdateData(TRUE);
		m_strShow += szRecv;
		m_strShow += "\r\n";
		UpdateData(FALSE);
	}
	default:
		break;
	}
	return 0;
}




void CCDlgChatMain::ChatForOne20ne(CHATONE20NE objOne20ne)
{
	CString strName(objOne20ne.szName);
	CString strContent(objOne20ne.szContent);
	if (m_map.find(strName)==m_map.end())
	{
		//创建私聊窗口
		CCDlgOne20ne * pDlg = new CCDlgOne20ne;
		pDlg->Create(IDD_DIALOG2, this);
		pDlg->SetWindowTextW(strName.GetBuffer());
		m_map[strName] = pDlg;
		pDlg->m_strShow += strName + L":" + strContent;
		pDlg->m_strShow += "\r\n";
		pDlg->UpdateData(FALSE);
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		CCDlgOne20ne * pDlg = (CCDlgOne20ne*)m_map[strName];
		pDlg->UpdateData(TRUE);
		pDlg->m_strShow += strName + L":" + strContent;
		pDlg->m_strShow += "\r\n";
		pDlg->UpdateData(FALSE);
		pDlg->ShowWindow(SW_SHOW);
	}
}


void CCDlgChatMain::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CCDlgChatMain::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CCDlgChatMain::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	//双击空白区域
	if (pNMItemActivate->iItem == -1)return;
	//新建1V1聊天对话框
	CCDlgOne20ne *pDlg = new CCDlgOne20ne;
	pDlg->Create(IDD_DIALOG2, this);//把当前窗口指定为私聊窗口的父窗口
	//设置窗口标题为聊天的目标用户名称
	CString strTitle = m_listName.GetItemText(pNMItemActivate->iItem, 0);
	pDlg->SetWindowTextW(strTitle.GetBuffer());
	//把该私聊窗口添加到自己的私聊MAP表里
	m_map[strTitle] = pDlg;
	pDlg->ShowWindow(SW_SHOW);
}


void CCDlgChatMain::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	//匿名聊天右键失效
	if (!m_bLogin) return;
	//当前点击的项，用于获得添加的是哪个好友
	m_dwNameIndex = pNMItemActivate->iItem;
	//弹出右键菜单
	CMenu mu;
	mu.LoadMenuW(IDR_MENU1);
	CMenu*pSub = mu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	pSub->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this, NULL);
	
	
}


void CCDlgChatMain::OnAddFriend()
{
	// TODO:  在此添加命令处理程序代码
	CString strFriend = m_listName.GetItemText(m_dwNameIndex, 0);
	//当前用户：要添加用户
	CString strUserFrd;
	GetWindowText(strUserFrd);
	strUserFrd += L":";
	strUserFrd += strFriend;
	CStringA strSend = CW2A(strUserFrd.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(ADDFRIEND, strSend.GetBuffer(), strSend.GetLength() + 1);
}


void CCDlgChatMain::OnSearch()
{
	// TODO:  在此添加命令处理程序代码
	CDlgSearch dlgSearch;
	dlgSearch.DoModal();
	if (dlgSearch.m_strSearch.IsEmpty())return;//要搜索对话框的搜索编辑框的内容
	CStringA strSearch = CW2A(dlgSearch.m_strSearch.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(SEARCHUSER, strSearch.GetBuffer(), strSearch.GetLength() + 1);
	
	
}


void CCDlgChatMain::OnGetChatRecord()
{
	// TODO:  在此添加命令处理程序代码
	DWORD dwRet = WaitForSingleObject(m_pClient->m_hEvent, 100);
	if (dwRet == WAIT_FAILED || dwRet == WAIT_TIMEOUT) return;
	m_pClient->Send(MSGRECORD, NULL, NULL);
	m_pClient->m_vecMsgRecord.clear();
	//设置定时器等待消息的返回
	SetTimer(0x1001, 1000, NULL);
}


void CCDlgChatMain::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
 	if (nIDEvent==0x1001)
 	{
 		//等待消息记录返回，消息记录会把事件对象置为true
 		DWORD dwRet = WaitForSingleObject(m_pClient->m_hEvent, 100);
 		if (dwRet==WAIT_OBJECT_0)
 		{
 			KillTimer(nIDEvent);
 			SetEvent(m_pClient->m_hEvent);
 			//显示新窗口
 			if (m_pDlgRecord)
 			{
 				//更新数据
 				m_pDlgRecord->UpdateList();
 				//显示窗口
 				m_pDlgRecord->ShowWindow(SW_SHOW);
 			}
 			else{
 				m_pDlgRecord = new CDlgRecord;
				m_pDlgRecord->Create(IDD_DIALOG4, this);
 				m_pDlgRecord->ShowWindow(SW_SHOW);
 			}
 		}
 	}

	CDialogEx::OnTimer(nIDEvent);
}
