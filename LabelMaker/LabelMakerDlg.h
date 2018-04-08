
// LabelMakerDlg.h : header file
//

#pragma once
#include <vector>
#include "ImageCenterDib.h"
#include "afxwin.h"
#include "EnhanceDlg.h"
#include "ProcessDlg.h"
#include "OtherToolDlg.h"

// CLabelMakerDlg dialog
class CLabelMakerDlg : public CDialogEx
{
// Construction
public:
	CLabelMakerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LABELMAKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
private:
	BOOL IsDirectoryExists(CString const& path);
	void RecursiveFindFile(CString& strPath);
	void RecursiveFindFile(CString& strPath, CString ext);
	void ShowFileList();
	void ShowImage(UINT nID, ImgCenterDib* pDib,cv::Mat image);
	void ResetDlgItemPosition(int imageWidth, int imageHeight);
	CPoint GetMousePointOfPIC(CPoint pt);
	cv::Point ToCVPoint(CPoint pt);
	cv::Point PICControlToImage(cv::Point pt, cv::Mat image);
	void OpenImageOfList(int index);
	void SaveLabel(CString labelPath, CString imagePath, cv::Rect targetRect);
	void ShowInformation(CString strInfo);
//	cv::Rect getOuterRect(cv::Rect centerRect);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();

	afx_msg LRESULT ShowProgressDlg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	void StopGenSample();
private:
	std::vector<CString> filelist;
	ImgCenterDib m_dib;
	ImgCenterDib m_smallDib;
	cv::Mat frame;
	cv::Mat patch;
	CPoint pt1;
	CPoint pt2;
	BOOL bMouseDown;
	int offset_x;
	int offset_y;
	cv::Rect targetRect;
	BOOL bToSave;

	double fx;
	double fy;
	
public:
	BOOL bRunning;
	CEnhanceDlg enhanceDlg;
	CProcessDlg progressDlg;
	COtherToolDlg toolDlg;
public:
	afx_msg void OnBnClickedBtnOpen();
	CString m_strPath;
	CString m_filename;
	CListBox m_ctrList;
	afx_msg void OnLbnDblclkListFilename();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	int m_iType;
	afx_msg void OnBnClickedRadioStype1();
	afx_msg void OnBnClickedRadioStype2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedBtnPrevious();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnHelp();
	afx_msg void OnBnClickedBtnEnhance();
	afx_msg void OnBnClickedBtnTool();
};
