#pragma once

#include <vector>
#include "afxwin.h"
// COtherToolDlg dialog

class COtherToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COtherToolDlg)

public:
	COtherToolDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COtherToolDlg();

// Dialog Data
	enum { IDD = IDD_DLGTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBrouse1();
	afx_msg void OnBnClickedBtnBrouse2();
	CString strSrcPath;
	CString strDstPath;
	int iNormalWidth;
	int iNormalHeight;
	int iChoice;

	CWnd *FatherWnd;
	std::vector<CString> srcList;
	std::vector<CString> dstList;
private:
	void RecursiveFindFile(std::vector<CString>& filelist, CString& strPath);
	void RecursiveFindFile(std::vector<CString>& filelist, CString& strPath, CString ext);
public:
	afx_msg void OnBnClickedOk();
	CString strSrcPath2;
	CString strDstPath2;
	afx_msg void OnBnClickedBtnBrouse3();
	afx_msg void OnBnClickedBtnBrouse4();
	void DisplayItems();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChoiceResize();
	afx_msg void OnBnClickedChoiceCut();
	int iMaxSize;
	int iMinSize;
	int iNormalSize;
	BOOL bNormalSize;
	int iCutNum;
	afx_msg void OnBnClickedChoiceVideo2image();
	CString strVideoFilePath;
	CString strImageOutPutFolder;
	afx_msg void OnBnClickedBtnBrouse5();
	int iFormatIdx;
	afx_msg void OnBnClickedBtnOpenVideo();
	int iSampleInterval;
};
