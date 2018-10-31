// CDlgChatMain.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "CDlgChatMain.h"
#include "afxdialogex.h"
#include "CDlgOne20ne.h"
#include "DlgSearch.h"


int m_Count = 0;
#define WM_MYSOCKET WM_USER+100

// CCDlgChatMain �Ի���

void CCDlgChatMain::InserOrDeleteUser(CHATUPDATEUSER &objUpdate)
{
  	if (objUpdate.bAdd == 1)//����
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


// CCDlgChatMain ��Ϣ�������


BOOL CCDlgChatMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���ô��ڱ���Ϊ�û��ǳ�
	//������¼ʱm_pClient->m_szName�Ǹ����ֵ�����캯���ڳ�ʼ��
	//��¼��ʱ���ǵ�¼�û��������¼��ť��ʱ���ʼ��
	CString strTitle(m_pClient->m_szName);
	SetWindowText(strTitle.GetBuffer());
	//���ô����������ɼ�
	ModifyStyleEx(0, WS_EX_APPWINDOW);
	//�����û��б�
	m_listName.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listName.InsertColumn(0, L"�����б�", 0, 100);

// 	CString k(m_pClient->m_szName);
// 	m_listName.InsertItem(Num, NULL);
// 
// 	m_listName.SetItemText(Num, 0, k);
// 	Num += 1;

	//ʹ����Ϣѡ����ģ�Ͱ�socket�͵�ǰ�Ի���
	//1.define�Զ�����ϢWM_SOCKET
	//2.��������Զ�����Ϣ��WM_SCOKET����Ӧ����OnMysocket
	//WSAAsyncSelect��socket�����ں���Ϣ
	WSAAsyncSelect(m_pClient->m_sClient, m_hWnd, WM_MYSOCKET, FD_READ | FD_CLOSE);
	//���߷������Լ����ǳ�
	m_pClient->Send(ANONYMOUS, m_pClient->m_szName, strlen(m_pClient->m_szName) + 1);
	m_pClient->m_hEvent = CreateEvent(0, FALSE, TRUE, (LPCWSTR)"m_Event");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CCDlgChatMain::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
	{
		return;
	}
	CStringA str = CW2A(m_strSend.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(CHAT, str.GetBuffer(), str.GetLength() + 1);
	m_strShow += "��˵:";
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
		MessageBox(L"�������");
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
				//1v1����
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
		//����˽�Ĵ���
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CCDlgChatMain::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CCDlgChatMain::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//˫���հ�����
	if (pNMItemActivate->iItem == -1)return;
	//�½�1V1����Ի���
	CCDlgOne20ne *pDlg = new CCDlgOne20ne;
	pDlg->Create(IDD_DIALOG2, this);//�ѵ�ǰ����ָ��Ϊ˽�Ĵ��ڵĸ�����
	//���ô��ڱ���Ϊ�����Ŀ���û�����
	CString strTitle = m_listName.GetItemText(pNMItemActivate->iItem, 0);
	pDlg->SetWindowTextW(strTitle.GetBuffer());
	//�Ѹ�˽�Ĵ�����ӵ��Լ���˽��MAP����
	m_map[strTitle] = pDlg;
	pDlg->ShowWindow(SW_SHOW);
}


void CCDlgChatMain::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	//���������Ҽ�ʧЧ
	if (!m_bLogin) return;
	//��ǰ���������ڻ����ӵ����ĸ�����
	m_dwNameIndex = pNMItemActivate->iItem;
	//�����Ҽ��˵�
	CMenu mu;
	mu.LoadMenuW(IDR_MENU1);
	CMenu*pSub = mu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	pSub->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this, NULL);
	
	
}


void CCDlgChatMain::OnAddFriend()
{
	// TODO:  �ڴ���������������
	CString strFriend = m_listName.GetItemText(m_dwNameIndex, 0);
	//��ǰ�û���Ҫ����û�
	CString strUserFrd;
	GetWindowText(strUserFrd);
	strUserFrd += L":";
	strUserFrd += strFriend;
	CStringA strSend = CW2A(strUserFrd.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(ADDFRIEND, strSend.GetBuffer(), strSend.GetLength() + 1);
}


void CCDlgChatMain::OnSearch()
{
	// TODO:  �ڴ���������������
	CDlgSearch dlgSearch;
	dlgSearch.DoModal();
	if (dlgSearch.m_strSearch.IsEmpty())return;//Ҫ�����Ի���������༭�������
	CStringA strSearch = CW2A(dlgSearch.m_strSearch.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(SEARCHUSER, strSearch.GetBuffer(), strSearch.GetLength() + 1);
	
	
}


void CCDlgChatMain::OnGetChatRecord()
{
	// TODO:  �ڴ���������������
	DWORD dwRet = WaitForSingleObject(m_pClient->m_hEvent, 100);
	if (dwRet == WAIT_FAILED || dwRet == WAIT_TIMEOUT) return;
	m_pClient->Send(MSGRECORD, NULL, NULL);
	m_pClient->m_vecMsgRecord.clear();
	//���ö�ʱ���ȴ���Ϣ�ķ���
	SetTimer(0x1001, 1000, NULL);
}


void CCDlgChatMain::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
 	if (nIDEvent==0x1001)
 	{
 		//�ȴ���Ϣ��¼���أ���Ϣ��¼����¼�������Ϊtrue
 		DWORD dwRet = WaitForSingleObject(m_pClient->m_hEvent, 100);
 		if (dwRet==WAIT_OBJECT_0)
 		{
 			KillTimer(nIDEvent);
 			SetEvent(m_pClient->m_hEvent);
 			//��ʾ�´���
 			if (m_pDlgRecord)
 			{
 				//��������
 				m_pDlgRecord->UpdateList();
 				//��ʾ����
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
