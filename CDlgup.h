#pragma once


// CDlgup 对话框

class CDlgup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgup)

public:
	CDlgup(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgup();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic test;
	afx_msg void OnBnClickedButtonupload();
	CEdit filename_up;
	char* filename_upc;
};
