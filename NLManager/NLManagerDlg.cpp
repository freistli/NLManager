
// NLManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NLManager.h"
#include "NLManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNLManagerDlg dialog



CNLManagerDlg::CNLManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNLManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nIndex = 0;
}

void CNLManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edConType);
	DDX_Control(pDX, IDC_EDIT3, m_edCategory);
	DDX_Control(pDX, IDC_EDIT2, m_edDomain);
	DDX_Control(pDX, IDC_EDIT4, m_edNID);
	DDX_Control(pDX, IDC_EDIT5, m_edName);
	DDX_Control(pDX, IDC_EDIT7, m_edCreation);
	DDX_Control(pDX, IDC_EDIT6, m_edConTime);
	DDX_Control(pDX, IDC_EDIT8, m_edInternet);
	DDX_Control(pDX, IDC_EDIT9, m_edHeader);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
}

BEGIN_MESSAGE_MAP(CNLManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_NW, &CNLManagerDlg::OnBnClickedBtnNw)
	ON_MESSAGE(UM_NETWORK_NOTIFY,OnNetworkChanged)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CNLManagerDlg::OnBnClickedBtnNext)
END_MESSAGE_MAP()



// CNLManagerDlg message handlers

BOOL CNLManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_btnNext.EnableWindow(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNLManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNLManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNLManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNLManagerDlg:: Init_Controls()
{
	m_edCategory.SetWindowTextW(L"");
	m_edConTime.SetWindowTextW(L"");
	m_edCreation.SetWindowTextW(L"");
	m_edConType.SetWindowTextW(L"");
	m_edDomain.SetWindowTextW(L"");
	m_edInternet.SetWindowTextW(L"");
	m_edName.SetWindowTextW (L"");
	m_edNID.SetWindowTextW(L"");
}

void CNLManagerDlg:: Fill_NW_Info()
{
	Init_Controls();
	Networks networks = m_helper.Get_NW_Info();
	int nTotalNetworks = networks.size(); 
	nTotalNetworks > 1 ? m_btnNext.EnableWindow(true) : m_btnNext.EnableWindow(false) ;

	WCHAR str[MAX_PATH] = {0};
	wsprintf (str, L"%d - Network Detected", nTotalNetworks);
	m_edHeader.SetWindowTextW(str);
	
	if (m_nIndex >= nTotalNetworks) m_nIndex = 0;

	if(nTotalNetworks > 0 )
	{
		m_edCategory.SetWindowTextW(networks[m_nIndex].GetNWCategory().c_str());
		m_edConTime.SetWindowTextW(networks[m_nIndex].GetConnectionTime().c_str());
		m_edCreation.SetWindowTextW(networks[m_nIndex].GetCreationTime().c_str());
		m_edConType.SetWindowTextW(networks[m_nIndex].GetConnectivityType().c_str());
		m_edDomain.SetWindowTextW(networks[m_nIndex].GetDomain().c_str());
		m_edInternet.SetWindowTextW(networks[m_nIndex].GetInternetState() ? L"Available" : L"Not Available");
		m_edName.SetWindowTextW (networks[m_nIndex].GetNetworkName().c_str());
		m_edNID.SetWindowTextW(networks[m_nIndex].GetNetworkID().c_str());
	}
}


void CNLManagerDlg::OnBnClickedBtnNw()
{
	Fill_NW_Info();
}

LRESULT CNLManagerDlg::OnNetworkChanged(WPARAM wParam, LPARAM lParam)
{
	Fill_NW_Info();
	return 0L;
}


void CNLManagerDlg::OnBnClickedBtnNext()
{
	m_nIndex++;
	Fill_NW_Info();
}
