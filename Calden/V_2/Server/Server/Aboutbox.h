#pragma once
// Aboutbox dialog

class Aboutbox : public CDialogEx
{
	DECLARE_DYNAMIC(Aboutbox)

public:
	Aboutbox(CWnd* pParent = NULL);   // standard constructor
	virtual ~Aboutbox();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
