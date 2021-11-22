#pragma once


// CDlgdown 对话框

class CDlgdown : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgdown)

public:
	CDlgdown(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgdown();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit downloadfrom;
	CEdit downloadto;
	afx_msg void OnBnClickedButtondownload();
};
