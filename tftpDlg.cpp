
// tftpDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "tftp.h"
#include "tftpDlg.h"
#include "afxdialogex.h"
#include "CDlgabout.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CtftpDlg 对话框



CtftpDlg::CtftpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TFTP_DIALOG, pParent)
	, printout(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtftpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABYYY, tabyyy);
	DDX_Control(pDX, IDC_EDIT1, editlocalip);
	DDX_Control(pDX, IDC_EDIT2, editfarip);
	DDX_Control(pDX, IDC_RADIO2, netascii_radio);
	DDX_Control(pDX, IDC_RADIO3, octet_radio);
	DDX_Control(pDX, IDC_EDIT3, print);
	DDX_Text(pDX, IDC_EDIT3, printout);
}

BEGIN_MESSAGE_MAP(CtftpDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABYYY, &CtftpDlg::OnTcnSelchangeTabyyy)
	ON_BN_CLICKED(IDC_EXIT, &CtftpDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_ABOUT, &CtftpDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_LINK, &CtftpDlg::OnBnClickedLink)
END_MESSAGE_MAP()


// CtftpDlg 消息处理程序

BOOL CtftpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码
	
	//"关于"初始化
	aboutyyy.Create(IDD_ABOUTBOX);

	//标签页
	CRect tabRect;
	tabyyy.InsertItem(0, L"上传");
	tabyyy.InsertItem(1, L"下载");
	upyyy.Create(IDD_UP, &tabyyy);
	downyyy.Create(IDD_DOWN, &tabyyy);
	tabyyy.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	upyyy.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	downyyy.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	upyyy.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	downyyy.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	
	//radio初始化
	((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE); //选上
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);//不选上

	reupnum = 0;
	redownnum = 0;

	printout = ("");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtftpDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CDlgabout dlgAbout;
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

void CtftpDlg::OnPaint()
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
HCURSOR CtftpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtftpDlg::OnTcnSelchangeTabyyy(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	CRect tabRect;    // 标签控件客户区的Rect
	// 获取标签控件客户区Rect，并对其调整，以适合放置标签页
	tabyyy.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	switch (tabyyy.GetCurSel())
	{
	case 0:
		upyyy.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		downyyy.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 1:
		upyyy.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		downyyy.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}

}



void CtftpDlg::OnBnClickedExit()
{
	exit(0);
	// TODO: 在此添加控件通知处理程序代码
}


void CtftpDlg::OnBnClickedAbout()
{
	
	aboutyyy.ShowWindow(SW_SHOWNORMAL);
	// TODO: 在此添加控件通知处理程序代码
}



void CtftpDlg::OnBnClickedLink()
{
	CString localipW = GetLocalIp();
	CString faripW = GetFarIp();

	USES_CONVERSION;

	char* localipc = T2A(localipW);
	char* faripc = T2A(faripW);

	LinkIp(localipc, faripc);
	// TODO: 在此添加控件通知处理程序代码
}
