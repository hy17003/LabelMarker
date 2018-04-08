// EnhanceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LabelMaker.h"
#include "EnhanceDlg.h"
#include "afxdialogex.h"


// CEnhanceDlg dialog

IMPLEMENT_DYNAMIC(CEnhanceDlg, CDialogEx)

CEnhanceDlg::CEnhanceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnhanceDlg::IDD, pParent)
	, bAngle(FALSE)
	, bXPos(FALSE)
	, bYPos(FALSE)
	, bScale(FALSE)
	, dStartAngle(0)
	, dEndAngle(360)
	, dAngleInterval(45)
	, bUnion(FALSE)
	, dStartPos(-0.2)
	, dEndPos(0.2)
	, dPosInterval(0.1)
	, dStartScale(0.8)
	, dEndScale(1.2)
	, dScaleInterval(0.2)
	, mImagesPath(_T(""))
	, FatherWnd(NULL)
	, bResize(FALSE)
	, iResizeHeight(30)
	, iResizeWidth(30)
{

}

CEnhanceDlg::~CEnhanceDlg()
{
}

void CEnhanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ANGLE, bAngle);
	DDX_Check(pDX, IDC_CHECK_X, bXPos);
	DDX_Check(pDX, IDC_CHECK_Y, bYPos);
	DDX_Check(pDX, IDC_CHECK_SCALE, bScale);
	DDX_Text(pDX, IDC_EDIT_ANGLE_START, dStartAngle);
	DDV_MinMaxDouble(pDX, dStartAngle, 0, 356);
	DDX_Text(pDX, IDC_EDIT_ANGLE_END, dEndAngle);
	DDV_MinMaxDouble(pDX, dEndAngle, 0, 360);
	DDX_Text(pDX, IDC_EDIT_ANGLE_INTER, dAngleInterval);
	DDV_MinMaxDouble(pDX, dAngleInterval, 0, 360);
	DDX_Check(pDX, IDC_CHECK_UNION, bUnion);
	DDX_Text(pDX, IDC_EDIT_POS_START, dStartPos);
	DDV_MinMaxDouble(pDX, dStartPos, -0.5, 0.5);
	DDX_Text(pDX, IDC_EDIT_POS_END, dEndPos);
	DDV_MinMaxDouble(pDX, dEndPos, -0.5, 0.5);
	DDX_Text(pDX, IDC_EDIT_POS_INTER, dPosInterval);
	DDV_MinMaxDouble(pDX, dPosInterval, 0, 0.5);
	DDX_Text(pDX, IDC_EDIT_SCALE_START, dStartScale);
	DDV_MinMaxDouble(pDX, dStartScale, 0.1, 10);
	DDX_Text(pDX, IDC_EDIT_SCALE_END, dEndScale);
	DDV_MinMaxDouble(pDX, dEndScale, 0.1, 10);
	DDX_Text(pDX, IDC_EDIT_SCALE_INTER, dScaleInterval);
	DDV_MinMaxDouble(pDX, dScaleInterval, 0.01, 10);
	DDX_Text(pDX, IDC_EDIT_IMAGE_FOLDER, mImagesPath);
	DDX_Text(pDX, IDC_EDIT_SAMPLE_FOLDER, mSamplePath);
	DDX_Check(pDX, IDC_CHECK_RESIZE, bResize);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, iResizeHeight);
	DDV_MinMaxInt(pDX, iResizeHeight, 5, 200);
	DDX_Text(pDX, IDC_EDIT_WIDTH, iResizeWidth);
	DDV_MinMaxInt(pDX, iResizeWidth, 5, 200);
}


BEGIN_MESSAGE_MAP(CEnhanceDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BROUSE1, &CEnhanceDlg::OnBnClickedBtnBrouse1)
	ON_BN_CLICKED(IDC_BTN_BROUSE2, &CEnhanceDlg::OnBnClickedBtnBrouse2)
	ON_BN_CLICKED(IDC_CHECK_ANGLE, &CEnhanceDlg::OnBnClickedCheckAngle)
	ON_BN_CLICKED(IDC_CHECK_X, &CEnhanceDlg::OnBnClickedCheckX)
	ON_BN_CLICKED(IDC_CHECK_Y, &CEnhanceDlg::OnBnClickedCheckY)
	ON_BN_CLICKED(IDC_CHECK_SCALE, &CEnhanceDlg::OnBnClickedCheckScale)
	ON_BN_CLICKED(IDOK, &CEnhanceDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_RESIZE, &CEnhanceDlg::OnBnClickedCheckResize)
END_MESSAGE_MAP()


// CEnhanceDlg message handlers


void CEnhanceDlg::OnBnClickedBtnBrouse1()
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("请选择目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))
	{
		mImagesPath.Format(_T("%s"), szPath);
	}
	UpdateData(FALSE);
}


void CEnhanceDlg::OnBnClickedBtnBrouse2()
{
	// TODO: Add your control notification handler code here
	char szPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("请选择目录：");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if (lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))
	{
		mSamplePath.Format(_T("%s"), szPath);
	}
	UpdateData(FALSE);
}

void CEnhanceDlg::DisplayItem()
{
	UpdateData(TRUE);
	if (bAngle)
	{
		GetDlgItem(IDC_EDIT_ANGLE_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ANGLE_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ANGLE_INTER)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_ANGLE_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_INTER)->EnableWindow(FALSE);
	}
	if (bXPos || bYPos)
	{
		GetDlgItem(IDC_EDIT_POS_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POS_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_POS_INTER)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_POS_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POS_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_POS_INTER)->EnableWindow(FALSE);
	}
	if (bScale)
	{
		GetDlgItem(IDC_EDIT_SCALE_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SCALE_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SCALE_INTER)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_SCALE_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCALE_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCALE_INTER)->EnableWindow(FALSE);
	}
	if (bResize)
	{
		GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(FALSE);
	}
}

void CEnhanceDlg::OnBnClickedCheckAngle()
{
	// TODO: 在此添加控件通知处理程序代码
	DisplayItem();
}


BOOL CEnhanceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DisplayItem();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CEnhanceDlg::OnBnClickedCheckX()
{
	// TODO: 在此添加控件通知处理程序代码
	DisplayItem();
}


void CEnhanceDlg::OnBnClickedCheckY()
{
	// TODO: 在此添加控件通知处理程序代码
	DisplayItem();
}


void CEnhanceDlg::OnBnClickedCheckScale()
{
	// TODO: 在此添加控件通知处理程序代码
	DisplayItem();
}

BOOL IsDirectoryExists(CString const& path)
{
	//判断是否存在
	if (!PathFileExists(path))
		return false;
	//判断是否为目录
	DWORD attributes = ::GetFileAttributes(path);
	attributes &= FILE_ATTRIBUTE_DIRECTORY;
	return attributes == FILE_ATTRIBUTE_DIRECTORY;
}

void CEnhanceDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!IsDirectoryExists(mImagesPath))
	{
		MessageBox(_T("请选择正确的图像目录"), _T("提示"));
		return;
	}
	if (!IsDirectoryExists(mSamplePath))
	{
		MessageBox(_T("请选择正确的样本目录"), _T("提示"));
		return;
	}
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


void CEnhanceDlg::OnBnClickedCheckResize()
{
	// TODO: Add your control notification handler code here
	DisplayItem();
}
