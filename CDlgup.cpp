// CDlgup.cpp: 实现文件
//
#include "pch.h"
#include "tftp.h"
#include "CDlgup.h"
#include "afxdialogex.h"
#include "tftpDlg.h"

// CDlgup 对话框

IMPLEMENT_DYNAMIC(CDlgup, CDialogEx)

CDlgup::CDlgup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UP, pParent)
{

}

CDlgup::~CDlgup()
{
}

void CDlgup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, filename_up);
}


BEGIN_MESSAGE_MAP(CDlgup, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONUPLOAD, &CDlgup::OnBnClickedButtonupload)
END_MESSAGE_MAP()


// CDlgup 消息处理程序




void CDlgup::OnBnClickedButtonupload()
{
	CString temp_upfilenames;
	filename_up.GetWindowTextW(temp_upfilenames);


	CtftpDlg* temp = (CtftpDlg*)AfxGetMainWnd();
	
	temp->filenames = temp_upfilenames;

	temp->UpLoad();

	/*CtftpDlg* temp = (CtftpDlg*)AfxGetMainWnd();
	CString localip_up = temp->GetLocalIp();
	CString farip_up = temp->GetFarIp();
	CString filenameW;
	int mode = temp->octet_radio.GetCheck();
	filename_up.GetWindowTextW(filenameW);*/






	// TODO: 在此添加控件通知处理程序代码
}
