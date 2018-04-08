// HelpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LabelMaker.h"
#include "HelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg 对话框

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLGHELP, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
END_MESSAGE_MAP()


// CHelpDlg 消息处理程序


BOOL CHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strHelpTex = _T("\t\t\t  使用说明\r\n \
1.标记样本\r\n \
  1）点击打开按钮，选择样本所在目录\r\n \
  2）使用正常模式时，窗口为矩形，使用固定模式时，窗口为矩形\r\n \
  3）点击上一张、下一张或按下键盘A、D键，可以上下切换图像\r\n \
  4）一张图像每次只能标记一个标签，若有多个目标，可以标记保存后，再切换回来，继续标记\r\n \
  5）样本标志后，会在图像目录下生成label.txt文件\r\n \
  6）按下空格键保存，每标记一个目标后，都要按空格键\r\n \
2.样本增强\r\n \
  1）选择图像所在目录，该目录下必须有label.txt文件\r\n \
  2）选择存放样本的目录\r\n \
  3）勾选相当的增加方式\r\n \
  4）可以选择尺寸归一化，并设置尺寸\r\n \
  5）点击确定 \r\n \
  6）在生成过程中，点击停止按钮可以停止样本生成程序\r\n \
	\t\t\t\t\t2018.03.20");
	SetDlgItemText(IDC_HELP_TEXT, strHelpTex);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
