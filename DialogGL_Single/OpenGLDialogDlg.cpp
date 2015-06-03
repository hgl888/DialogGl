// OpenGLDialogDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "OpenGLDialog.h"
#include "OpenGLDialogDlg.h"
//#include "cctypes.h"
#include "set"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLDialogDlg Dialogfeld

COpenGLDialogDlg::COpenGLDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenGLDialogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenGLDialogDlg)
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf ben�tigt
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenGLDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenGLDialogDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COpenGLDialogDlg, CDialog)
	//{{AFX_MSG_MAP(COpenGLDialogDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenGLDialogDlg Nachrichten-Handler

BOOL COpenGLDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Symbol f�r dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Gro�es Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	
	// ZU ERLEDIGEN: Hier zus�tzliche Initialisierung einf�gen
	
	CRect rect;
	this->GetWindowRect( rect );
	ScreenToClient(rect);

	eglView = EGLView::sharedOpenGLView();
	//eglView->CreateGL( this->m_hWnd );
	//eglView->Create( this->m_hWnd );
	//eglView->setFrameSize( 800, 600 );
	eglView->CreateView( m_hWnd, 0, 0, rect.Width(), rect.Height() );
	eglView->setGLDefaultValues();
	pctx = EgretFilter::create( rect.Width(), rect.Height() );
	fillRectWithPattern();

	return TRUE;  
} 


void COpenGLDialogDlg::fillRectWithPattern()
{
	FILE *pFile = fopen( "c:/test.png", "rb" );
	fseek( pFile, 0, SEEK_END );
	int ilen = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );
	unsigned char *pData = new unsigned char[ilen ];
	fread( pData, ilen, 1, pFile );
	fclose( pFile );

	int width = 0;
	int height = 0;

	unsigned char *pImgData = pctx->DecodePngData( pData, ilen, width, height );
	pctx->mPattern.texId = pctx->initTexData(pImgData, width, height);
	pctx->mPattern.width = width;
	pctx->mPattern.height = height;

	delete []pData;
	//free( pImgData );
	
}


void COpenGLDialogDlg::OnPaint() 
{
	//glEnable( GL_SCISSOR_TEST );
	//glClearColor(1.0f, 0.0f, 0.0f, 0.0f); 
	//glScissor( 50, 200, 100, 500 );
	//glClear(GL_COLOR_BUFFER_BIT);  
	pctx->clear();
	pctx->DrawTexture(20, 20, 256, 256);

	pctx->dropShadowFilter();
	//glDisable( GL_SCISSOR_TEST );
	eglView->swapBuffers();

	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, w�hrend der Benutzer
//  das zum Symbol verkleinerte Fenster mit der Maus zieht.
HCURSOR COpenGLDialogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void COpenGLDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//eglView->setFrameSize( cx, cy );
	//mBwGLView.OnSize(nType, cx, cy );
	return;
}


BOOL COpenGLDialogDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//char pchTmp[ 256 ] = {0};
	//sprintf( pchTmp, "%s, %d", __FUNCTION__, __LINE__ );
	//OutputDebugStringA( pchTmp );
	//mBwGLView.OnEraseBkgnd(pDC );
	return CDialog::OnEraseBkgnd(pDC);
}


void COpenGLDialogDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
	//CCEGLView::sharedOpenGLView()->MsgLBtnDown( nFlags, point.x, point.y );
	return;
}


void COpenGLDialogDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonUp(nFlags, point);
	//CCEGLView::sharedOpenGLView()->MsgLBtnUp( nFlags, point.x, point.y );
	return;
}


void COpenGLDialogDlg::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMButtonDblClk(nFlags, point);
}


void COpenGLDialogDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMButtonDown(nFlags, point);
}


void COpenGLDialogDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMButtonUp(nFlags, point);
}


void COpenGLDialogDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonDblClk(nFlags, point);
}
