// CDlgabout.cpp: 实现文件
//

#include "pch.h"
#include "tftp.h"
#include "CDlgabout.h"
#include "afxdialogex.h"


// CDlgabout 对话框

IMPLEMENT_DYNAMIC(CDlgabout, CDialogEx)

CDlgabout::CDlgabout(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{

}

CDlgabout::~CDlgabout()
{
}

void CDlgabout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgabout, CDialogEx)
END_MESSAGE_MAP()


// CDlgabout 消息处理程序
