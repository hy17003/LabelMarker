// ProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LabelMaker.h"
#include "ProcessDlg.h"
#include "afxdialogex.h"
#include "LabelMakerDlg.h"


// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSDLG, pParent),
	minVal(0),
	maxVal(0),
	FatherWnd(NULL)
{

}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, ctrProgress);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CProcessDlg::OnBnClickedOk)
	//更新进度
	ON_MESSAGE(WM_PROGRESSUPDATE, UpdateProgress)
	ON_MESSAGE(WM_CLOSEPROGRESS, CloseProgressDlg)
	ON_BN_CLICKED(IDC_BTN_STOP, &CProcessDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序


void CProcessDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ctrProgress.SetRange(minVal, maxVal);
	ctrProgress.SetStep(1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


LRESULT CProcessDlg::UpdateProgress(WPARAM wParam, LPARAM lParam)
{
	ctrProgress.StepIt();
	if (ctrProgress.GetPos() > maxVal)
	{
		CDialogEx::OnOK();
	}
	return 0;
}

void CProcessDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

//	CDialogEx::OnCancel();
}



void CProcessDlg::OnBnClickedBtnStop()
{
	// TODO: Add your control notification handler code here
	((CLabelMakerDlg*)FatherWnd)->StopGenSample();
}

LRESULT CProcessDlg::CloseProgressDlg(WPARAM wParam, LPARAM lParam)
{
	CDialogEx::OnOK();
	return 0;
}