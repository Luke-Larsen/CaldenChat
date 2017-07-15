#include "stdafx.h"
#include "Server.h"
#include "Aboutbox.h"
#include "afxdialogex.h"


// Aboutbox dialog

IMPLEMENT_DYNAMIC(Aboutbox, CDialogEx)

Aboutbox::Aboutbox(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{

}

Aboutbox::~Aboutbox()
{
}

void Aboutbox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Aboutbox, CDialogEx)
END_MESSAGE_MAP()