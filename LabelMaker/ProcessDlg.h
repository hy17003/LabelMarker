#pragma once
#include "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\atlmfc\include\afxcmn.h"



// CProcessDlg �Ի���

class CProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcessDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT UpdateProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT CloseProgressDlg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CProgressCtrl ctrProgress;
	CWnd* FatherWnd;
	int minVal;
	int maxVal;
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnBnClickedBtnStop();
};
