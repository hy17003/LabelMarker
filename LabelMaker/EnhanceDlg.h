#pragma once
#include "afxwin.h"


// CEnhanceDlg dialog

class CEnhanceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnhanceDlg)

public:
	CEnhanceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnhanceDlg();

// Dialog Data
	enum { IDD = IDD_DLGENHANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL bAngle;
	BOOL bXPos;
	BOOL bYPos;
	BOOL bScale;
	double dStartAngle;
	double dEndAngle;
	double dAngleInterval;
	BOOL bUnion;
	double dStartPos;
	double dEndPos;
	double dPosInterval;
	double dStartScale;
	double dEndScale;
	double dScaleInterval;
	afx_msg void OnBnClickedBtnBrouse1();
	CString mImagesPath;
	CString mSamplePath;
	afx_msg void OnBnClickedBtnBrouse2();
private:
	void DisplayItem();
public:
	afx_msg void OnBnClickedCheckAngle();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckX();
	afx_msg void OnBnClickedCheckY();
	afx_msg void OnBnClickedCheckScale();
	afx_msg void OnBnClickedOk();
public:
	CWnd* FatherWnd;
	BOOL bResize;
	int iResizeHeight;
	int iResizeWidth;
	afx_msg void OnBnClickedCheckResize();
};
