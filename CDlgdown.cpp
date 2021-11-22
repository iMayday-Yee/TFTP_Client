// CDlgdown.cpp: 实现文件
//
#include "pch.h"
#include "tftp.h"
#include "CDlgdown.h"
#include "afxdialogex.h"
#include "tftpDlg.h"


// CDlgdown 对话框

IMPLEMENT_DYNAMIC(CDlgdown, CDialogEx)

CDlgdown::CDlgdown(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DOWN, pParent)
{

}

CDlgdown::~CDlgdown()
{
}

void CDlgdown::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITFROM, downloadfrom);
	DDX_Control(pDX, IDC_EDITTO, downloadto);
}


BEGIN_MESSAGE_MAP(CDlgdown, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONDOWNLOAD, &CDlgdown::OnBnClickedButtondownload)
END_MESSAGE_MAP()


// CDlgdown 消息处理程序



void CDlgdown::OnBnClickedButtondownload()
{

	CString temp_from;
	downloadfrom.GetWindowTextW(temp_from);
	CString temp_to;
	downloadto.GetWindowTextW(temp_to);

	CtftpDlg* temp = (CtftpDlg*)AfxGetMainWnd();

	temp->filenamefroms = temp_from;
	temp->filenametos = temp_to;


	temp->DownLoad();
	// TODO: 在此添加控件通知处理程序代码
}
