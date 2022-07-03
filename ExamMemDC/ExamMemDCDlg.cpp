
// ExamMemDCDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ExamMemDC.h"
#include "ExamMemDCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExamMemDCDlg 대화 상자



CExamMemDCDlg::CExamMemDCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXAMMEMDC_DIALOG, pParent), m_ini_filepath(L"C:\\ELP\\docs\\MemDC\\ExamMemDC\\Bin\\DataWrite.ini")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExamMemDCDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_H_SCROLLBAR, this->m_h_scrollbar);
	DDX_Control(pDX, IDC_V_SCROLLBAR, this->m_v_scrollbar);
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExamMemDCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OPEN_BTN, &CExamMemDCDlg::OnBnClickedOpenBtn)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CExamMemDCDlg 메시지 처리기

BOOL CExamMemDCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CRect r;
	GetClientRect(r);
	CClientDC dc(this);
	m_mem_dc.CreateCompatibleDC(&dc);
	m_mem_bmp.CreateCompatibleBitmap(&dc, r.Width(), r.Height());
	m_mem_dc.SelectObject(&m_mem_bmp);
	
	CString default_path = L"bmp2.bmp";
	SetBitmap(default_path);

	HBITMAP pass_hbmp = GetBitmap();
	CSize size_src(this->m_rect_pic_client.Width(), this->m_rect_pic_client.Height());
	POINT src_pt = GetPicCtrlPt();
	for (int i = 0; i < 5; i++)
	{
		HWND ah_wnd_pic = ::GetDlgItem(m_hWnd, IDC_PIC1 + i);
		HDC ah_dc_pic = ::GetDC(ah_wnd_pic);
		GetFracImgCBrushVers(ah_dc_pic, src_pt.x, src_pt.y, size_src.cx, size_src.cy);

	}
	
	pass_hbmp = GetBitmap();
	CopyBitmap(pass_hbmp);

	CRect rect_mem_view;
	CStatic *p_static = (CStatic*)GetDlgItem(IDC_MY_PICTURE);
	p_static->GetClientRect(&rect_mem_view);
	rect_mem_view.NormalizeRect();
	p_static->ClientToScreen(&rect_mem_view);
	p_static->ScreenToClient(&rect_mem_view);
	m_mem_view.Create(rect_mem_view.Width(), rect_mem_view.Height(),24,0);

	m_h_scrollbar.ShowWindow(false);
	m_v_scrollbar.ShowWindow(false);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CExamMemDCDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		HWND hWnd_pic = ::GetDlgItem(m_hWnd, IDC_MY_PICTURE);
		HDC hdc_pic = ::GetDC(hWnd_pic);
		::GetWindowRect(hWnd_pic, rect);

		DrawImage(hdc_pic);
		CSize size_rect(0, 0);
		for (int i = 0; i < 5; i++) {
			hWnd_pic = ::GetDlgItem(m_hWnd, IDC_PIC1 + i);
			hdc_pic = ::GetDC(hWnd_pic);
			CRect frac_rect_pic_client;
			CStatic*p_static = (CStatic*)GetDlgItem(IDC_PIC1 + i);
			p_static->GetClientRect(&frac_rect_pic_client);
			frac_rect_pic_client.NormalizeRect();
			p_static->ClientToScreen(&frac_rect_pic_client);
			p_static->ScreenToClient(&frac_rect_pic_client);
			CSize size_frac_rect;
			size_frac_rect.cx = frac_rect_pic_client.Width();
			size_frac_rect.cy = frac_rect_pic_client.Height();

			DrawClipImagesCBrushVers(hdc_pic, size_frac_rect.cx, size_frac_rect.cy);

			DrawScrollBar();
		}
		// CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CExamMemDCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CExamMemDCDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_mem_dc.m_hDC != NULL) {
		m_mem_bmp.DeleteObject();
		m_mem_dc.DeleteDC();
	}

	if (!m_mem_view.IsNull())
	{
		m_mem_view.ReleaseDC();
	}
}

void CExamMemDCDlg::SetBitmap(CString a_file_path)
{
	if (mh_mem_bmp!= NULL) {
		::SelectObject(m_mem_dc.m_hDC,p_old_bmp);
		::DeleteObject(mh_mem_bmp);
	}
	if (!a_file_path.IsEmpty()) {
		mh_mem_bmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), a_file_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		HDC h_screen_dc = ::GetDC(NULL);
		HGDIOBJ h_screen_bitmap = ::GetCurrentObject(h_screen_dc, OBJ_BITMAP);
		BITMAP bmp_info;
		GetObject(h_screen_bitmap, sizeof(BITMAP), &bmp_info);
		if (mh_mem_bmp != NULL) {
			p_old_bmp = (HBITMAP)SelectObject(m_mem_dc.m_hDC, mh_mem_bmp);
			GetObject(mh_mem_bmp, sizeof(BITMAP), &m_bmp_info);
		}
		::ReleaseDC(NULL, h_screen_dc);
	}
}
HBITMAP CExamMemDCDlg::GetBitmap()
{
	return mh_mem_bmp;
}
void CExamMemDCDlg::CopyBitmap(HBITMAP ah_bitmap)
{
	int w = m_bmp_info.bmWidth, h = m_bmp_info.bmHeight, clr = m_bmp_info.bmBitsPixel;
	BITMAP dest_bmp_info;
	
	if (!dest_image.IsNull()) {
		dest_image.Destroy();
		dest_image.Create(w, h, clr);
	}
	else {
		dest_image.Create(w, h, clr);
	}
	// CImage의 HDC가 dest_bmp_info를 가리키고
	GetObject((HBITMAP)dest_image, sizeof(BITMAP), &dest_bmp_info);
	// src_bmp을 dest_bmp_info.bmBits에 복사한다
	GetObject(ah_bitmap, w*h*clr/8, dest_bmp_info.bmBits);
	// char*이 src_bmp을 가리키게 하고
	int image_size = w * h*clr / 8, copy_size = image_size;
	int line_size = w * clr / 8;
	char *p_copied = new char[image_size];
	GetBitmapBits(ah_bitmap, copy_size, p_copied);
	// 새로운 char*이 dest_bmp_info.bmBits를 가리키게 하고
	char *p_bmp_pattern = (char*)dest_bmp_info.bmBits;
	// 한 줄씩 복사한다
	for (int y = h - 1; y >= 0; y--) {
		memcpy(p_copied, p_bmp_pattern + y * line_size, line_size);
		p_bmp_pattern += line_size;
	}
	delete[] p_copied;

}
//
//BITMAPINFO* CExamMemDCDlg::CreateBitmapInfo()
//{
//	CFile file;
//	BITMAPINFOHEADER bmfh;
//	DWORD dwFileSize, dwDibSize;
//
//	file.Open(m_file_path, CFile::modeRead | CFile::shareDenyWrite, NULL);
//	dwFileSize = (DWORD)file.GetLength();
//	dwDibSize = dwFileSize - sizeof(BITMAPINFOHEADER);
//	BYTE *pDib = new BYTE[dwDibSize];
//	
//	file.Read(&bmfh, sizeof(BITMAPINFOHEADER));
//	file.Read(pDib, dwDibSize);
//	file.Close();
//
//	BITMAPINFOHEADER *pBmh = (BITMAPINFOHEADER*)pDib;
//	int nWidth = pBmh->biWidth;
//	int nHeight = pBmh->biHeight;
//	int nBit = pBmh->biBitCount;
//	
//	return (BITMAPINFO*)pDib;
//}

void CExamMemDCDlg::DrawImage(HDC ah_dc_pic)
{
	/*BITMAPINFO* pDib = CreateBitmapInfo();
	BITMAPINFOHEADER* pBmh = (BITMAPINFOHEADER*)pDib;
	int nBit = pBmh->biBitCount;
	BYTE* lpBits = NULL;
	if (nBit > 8)
	{
		lpBits = (BYTE*)pDib + sizeof(BITMAPINFOHEADER);
	}
	else
	{
		lpBits = (BYTE*)pDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*(1 << nBit);
	}*/

	int w = m_bmp_info.bmWidth, h = m_bmp_info.bmHeight;
	int dst_w = this->m_rect_pic_client.Width();
	int dst_h = this->m_rect_pic_client.Height();
	int scroll_pos_x = m_h_scrollbar.GetScrollPos();
	int scroll_pos_y = m_v_scrollbar.GetScrollPos();

	WORD cClrBits=0;
	cClrBits = (WORD)(m_bmp_info.bmPlanes*m_bmp_info.bmBitsPixel);
	
	// ::BitBlt(ah_dc_pic, 0, 0, w, h, m_mem_dc, 0, 0, SRCCOPY);
	// StretchBlt(ah_dc_pic, 0, 0, w, h, m_mem_dc, scroll_pos_x, scroll_pos_y,w-this->m_rect_pic_client.Width(), h - m_rect_pic_client.Height(), SRCCOPY);
	// 이게 됨 ::BitBlt(ah_dc_pic, 0, 0, w-this->m_rect_pic_client.Width(), h-this->m_rect_pic_client.Height(), m_mem_dc, scroll_pos_x, scroll_pos_y, SRCCOPY);
	//StretchBlt(ah_dc_pic, 0, 0, dst_w, dst_h, m_mem_dc, scroll_pos_x, scroll_pos_y,  - this->m_rect_pic_client.Width(), h - this->m_rect_pic_client.Height(), SRCCOPY);
	HBITMAP brush_frac_bmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), frac_filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	CDC brush_frac_memDC;
	BITMAP brush_frac_bmp_info;
	if (brush_frac_bmp != NULL)
	{
		GetObject(brush_frac_bmp, sizeof(BITMAP), &brush_frac_bmp_info);
		::SelectObject(brush_frac_memDC.m_hDC, brush_frac_bmp);
	}

	/* CImage를 HBITMAP정보를 받아와 Create한다 */
	CImage brush_frac_dest_image;

	tagBITMAP bi = brush_frac_bmp_info;

	/* 1. CImage를 Create */
	brush_frac_dest_image.Create(bi.bmWidth, bi.bmHeight, bi.bmBitsPixel);

	/* 2. dest_bmp_info에 생성한 CImage의 비트패턴 시작주소를 받아온다 */
	BITMAP brush_frac_dest_bmp_info;

	GetObject((HBITMAP)brush_frac_dest_image, sizeof(BITMAP), &brush_frac_dest_bmp_info);

	/* 3. HBITMAP에 저장된 비트 패턴 구성 정보를 CImgae 비트패턴 시작주소에 복사 */
	int copy_size = bi.bmWidth * bi.bmHeight * bi.bmBitsPixel / 8;

	GetBitmapBits(brush_frac_bmp, copy_size, brush_frac_dest_bmp_info.bmBits);

	PBITMAPINFO pbmi = CreateBitmapInfoNewVers();

	SetStretchBltMode(ah_dc_pic, COLORONCOLOR);

	StretchDIBits(ah_dc_pic, 0, 0, dst_w, dst_h, scroll_pos_x, scroll_pos_y, w - this->m_rect_pic_client.Width(), h - this->m_rect_pic_client.Height(), brush_frac_dest_bmp_info.bmBits, pbmi, DIB_RGB_COLORS, SRCCOPY);
}

PBITMAPINFO CExamMemDCDlg::CreateBitmapInfoNewVers()
{
	WORD cClrBits;

	WORD bm_planes = (WORD)(1);

	cClrBits = (WORD)(m_bmp_info.bmPlanes*m_bmp_info.bmBitsPixel);

	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	if (cClrBits != 24)
	{
		p_bmp_info = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + (1 << cClrBits) * sizeof(RGBQUAD));
	}
	else
	{
		p_bmp_info = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));
	}

	p_bmp_info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	p_bmp_info->bmiHeader.biPlanes = 1;

	p_bmp_info->bmiHeader.biBitCount = m_bmp_info.bmBitsPixel;

	p_bmp_info->bmiHeader.biCompression = BI_RGB;

	p_bmp_info->bmiHeader.biSizeImage = 0;

	p_bmp_info->bmiHeader.biSizeImage = ((p_bmp_info->bmiHeader.biWidth * cClrBits
		+ 31) & ~31) / 8;

	p_bmp_info->bmiHeader.biXPelsPerMeter = 0;

	p_bmp_info->bmiHeader.biYPelsPerMeter = 0;

	if (cClrBits < 24)
		p_bmp_info->bmiHeader.biClrUsed = (1 << cClrBits);

	p_bmp_info->bmiHeader.biClrImportant = 0;

	if (m_bmp_info.bmBitsPixel == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			p_bmp_info->bmiColors[i].rgbBlue = (BYTE)i;

			p_bmp_info->bmiColors[i].rgbGreen = (BYTE)i;

			p_bmp_info->bmiColors[i].rgbRed = (BYTE)i;

			p_bmp_info->bmiColors[i].rgbReserved = 0;
		}
	}

	p_bmp_info->bmiHeader.biWidth = m_bmp_info.bmWidth;

	p_bmp_info->bmiHeader.biHeight = m_bmp_info.bmHeight;

	return p_bmp_info;
}

void CExamMemDCDlg::OnBnClickedOpenBtn()
{
	FILE* p_file = NULL;

	CString name_filter = L"모든 파일(*.*) | *.* | 이미지 파일(*.BMP, *.GIF, *.JPG, *.PNG) | (*.BMP; *.GIF; *.JPG; *.PNG; *.bmp; *.gif; *.jpg; *.png) ||";

	CFileDialog ins_dlg(TRUE, L"BMP; GIF; JPG; PNG; bmp; gif; jpg; png", L"*.BMP; *.GIF; *.JPG; *.PNG; *.bmp; *.gif; *.jpg; *.png", OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, name_filter);

	ins_dlg.m_ofn.nFilterIndex = 0;

	if (ins_dlg.DoModal() == IDOK)
	{

		if (0 == _wfopen_s(&p_file, ins_dlg.GetPathName(), L"rb"))
		{
			m_file_path = ins_dlg.GetPathName();

			frac_filePath = m_file_path;

			SetBitmap(m_file_path);

			CSize a_size = GetTotalSize();

			WriteDefaultData(a_size.cx, a_size.cy);

			fclose(p_file);
		}
		
		RenewalDisplay();
	}
}

void CExamMemDCDlg::WriteDefaultData(int cx, int cy)
{
	WriteNullSectionIni();
	CString strLX, strLY, strRX, strRY;

	CString sectionName = GetSectionName(IDC_PIC1 + 0 - IDC_PIC1);
	strLX.Format(L"%d", 0);
	strLY.Format(L"%d", 0);
	strRX.Format(L"%d", cx / 3);
	strRY.Format(L"%d", cy / 3);
	WriteStringIni(sectionName, L"LX", strLX, m_ini_filepath);
	WriteStringIni(sectionName, L"LY", strLY, m_ini_filepath);
	WriteStringIni(sectionName, L"RX", strRX, m_ini_filepath);
	WriteStringIni(sectionName, L"RY", strRY, m_ini_filepath);

	sectionName = GetSectionName(IDC_PIC1 + 1 - IDC_PIC1);
	strLX.Format(L"%d", cx*2/ 3);
	strLY.Format(L"%d", 0);
	strRX.Format(L"%d", cx);
	strRY.Format(L"%d", cy / 3);
	WriteStringIni(sectionName, L"LX", strLX, m_ini_filepath);
	WriteStringIni(sectionName, L"LY", strLY, m_ini_filepath);
	WriteStringIni(sectionName, L"RX", strRX, m_ini_filepath);
	WriteStringIni(sectionName, L"RY", strRY, m_ini_filepath);

	sectionName = GetSectionName(IDC_PIC1 + 2 - IDC_PIC1);
	strLX.Format(L"%d", cx / 3);
	strLY.Format(L"%d", cy / 3);
	strRX.Format(L"%d", cx * 2 / 3);
	strRY.Format(L"%d", cy * 2 / 3);
	WriteStringIni(sectionName, L"LX", strLX, m_ini_filepath);
	WriteStringIni(sectionName, L"LY", strLY, m_ini_filepath);
	WriteStringIni(sectionName, L"RX", strRX, m_ini_filepath);
	WriteStringIni(sectionName, L"RY", strRY, m_ini_filepath);

	sectionName = GetSectionName(IDC_PIC1 + 3 - IDC_PIC1);
	strLX.Format(L"%d", 0);
	strLY.Format(L"%d", cy *2/ 3);
	strRX.Format(L"%d", cx / 3);
	strRY.Format(L"%d", cy);
	WriteStringIni(sectionName, L"LX", strLX, m_ini_filepath);
	WriteStringIni(sectionName, L"LY", strLY, m_ini_filepath);
	WriteStringIni(sectionName, L"RX", strRX, m_ini_filepath);
	WriteStringIni(sectionName, L"RY", strRY, m_ini_filepath);

	sectionName = GetSectionName(IDC_PIC1 + 4 - IDC_PIC1);
	strLX.Format(L"%d", cx * 2 / 3);
	strLY.Format(L"%d", cy * 2 / 3);
	strRX.Format(L"%d", cx);
	strRY.Format(L"%d", cy);
	WriteStringIni(sectionName, L"LX", strLX, m_ini_filepath);
	WriteStringIni(sectionName, L"LY", strLY, m_ini_filepath);
	WriteStringIni(sectionName, L"RX", strRX, m_ini_filepath);
	WriteStringIni(sectionName, L"RY", strRY, m_ini_filepath);
}

void CExamMemDCDlg::WriteNullSectionIni()
{
	CString sectionName;
	for (int i = 0; i < 5; i++)
	{
		sectionName = GetSectionName(IDC_BUTTON1+i-IDC_BUTTON1);

		WritePrivateProfileString(sectionName, NULL, NULL, m_ini_filepath);
	}
}

CString CExamMemDCDlg::GetSectionName(UINT a_ctrl_id)
{
	CString strSectionName, strSectionNum;

	strSectionName = L"ROI_";

	strSectionNum.Format(L"%d", a_ctrl_id);

	strSectionName += strSectionNum;

	return strSectionName;
}

void CExamMemDCDlg::RenewalDisplay()
{
	CStatic *p_static = (CStatic*)GetDlgItem(IDC_MY_PICTURE);

	p_static->GetClientRect(&m_rect_pic_client);

	m_rect_pic_client.NormalizeRect();

	p_static->ClientToScreen(&m_rect_pic_client);

	p_static->ScreenToClient(&m_rect_pic_client);

	InvalidateRect(&m_rect_pic_client);
}

CSize CExamMemDCDlg::GetTotalSize()
{
	HBITMAP img_bmp = GetBitmap();

	BITMAP bi;

	GetObject(img_bmp, sizeof(BITMAP), &bi);

	CSize a_size(bi.bmWidth, bi.bmHeight);

	return a_size;
}

BOOL CExamMemDCDlg::WriteStringIni(CString sectionName, CString keyName, CString strDefault, CString strFilePath)
{
	WritePrivateProfileString(sectionName, keyName, strDefault, m_ini_filepath);

	return TRUE;
}

void CExamMemDCDlg::GetFracImgCBrushVers(HDC ah_dc, int a_src_x, int a_src_y, int a_src_width, int a_src_height)
{
	CSize size_img(0,0);
	if (!m_mem_view.IsNull())
	{
		m_mem_view.ReleaseDC();
		size_img = GetTotalSize();
		m_mem_view.Create(size_img.cx / 3, size_img.cy / 3, 32, 0);
	}
	else {
		m_mem_view.Create(size_img.cx / 3, size_img.cy / 3,32,0);
	}
	/*HDC hdc_mmem_view = m_mem_view.GetDC();

	DWORD cx = size_img.cx / 3;
	DWORD cy = size_img.cy / 3;
	BitBlt(hdc_mmem_view, 0, 0, cx, cy, ah_dc,0,0,SRCCOPY);
	::DeleteDC(hdc_mmem_view);*/

	/* 한줄씩 복사하는 코드사용 */
	HBITMAP brush_frac_bmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), frac_filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	CDC brush_frac_memDC;
	BITMAP brush_frac_bmp_info;
	
	/* 제대로 이미지가 로드된 경우에만 SetBitmap For Fractal Image*/
	if (brush_frac_bmp != NULL)
	{
		GetObject(brush_frac_bmp, sizeof(BITMAP), &brush_frac_bmp_info);
		::SelectObject(brush_frac_memDC.m_hDC, brush_frac_bmp);
	}

	/* CImage를 HBITMAP정보를 받아와 Create한다 */
	CImage brush_frac_dest_image;

	tagBITMAP bi = brush_frac_bmp_info;

	/* 1. CImage를 Create */
	brush_frac_dest_image.Create(bi.bmWidth, bi.bmHeight, bi.bmBitsPixel);

	/* 2. dest_bmp_info에 생성한 CImage의 비트패턴 시작주소를 받아온다 */
	BITMAP brush_frac_dest_bmp_info;

	GetObject((HBITMAP)brush_frac_dest_image, sizeof(BITMAP), &brush_frac_dest_bmp_info);

	/* 3. HBITMAP에 저장된 비트 패턴 구성 정보를 CImgae 비트패턴 시작주소에 복사 */
	int copy_size = bi.bmWidth * bi.bmHeight * bi.bmBitsPixel / 8;

	GetBitmapBits(brush_frac_bmp, copy_size, brush_frac_dest_bmp_info.bmBits);

	/* 4. char 포인터형 변수에 HBITMAP정보를 저장해둔다 */
	int image_size = copy_size;

	char *p_copied = new char[image_size];

	GetBitmapBits(brush_frac_bmp, image_size, p_copied);

	int line_size = bi.bmWidth*bi.bmBitsPixel / 8;

	/* 5. 새로운 char 포인터형 변수에 brush_frac_dest_image의 비트패턴 시작 주소를 가리키게 */
	char *p_bmp_pattern = (char*)brush_frac_dest_bmp_info.bmBits;

	/* 6. 거꾸로 LoadImage된 비트 패턴을 한 줄씩 메모리에서 CImage객체로 복사한다 */
	for (int y = bi.bmHeight - 1; y >= 0; y--)
	{
		memcpy(p_copied, p_bmp_pattern + y * line_size, line_size);
		p_copied += line_size;
	}
	delete[] p_copied;

}

POINT CExamMemDCDlg::GetPicCtrlPt()
{
	POINT ret_pt;
	ret_pt.x = this->m_rect_pic_client.left;
	ret_pt.y = this->m_rect_pic_client.top;

	return ret_pt;
}

DWORD CExamMemDCDlg::ReadIntIni(CString strSection, CString strKey, CString strDefault, CString strFilePath)
{
	TCHAR szTemp[1024];
	memset(szTemp, 0x00, sizeof(szTemp));
	GetPrivateProfileString(strSection, strKey, strDefault, szTemp, sizeof(szTemp), strFilePath);
	DWORD ret_val = _tstof(szTemp);
	return ret_val;
}
void CExamMemDCDlg::DrawClipImagesCBrushVers(HDC ah_dc, int cx, int cy)
{
	m_mem_view.Draw(ah_dc, 0, 0, cx, cy);
}

void CExamMemDCDlg::DrawScrollBar()
{
	BITMAP bi;
	GetObject(mh_mem_bmp, sizeof(BITMAP), &bi);
	CSize size_rect_pic_client(this->m_rect_pic_client.Width(), this->m_rect_pic_client.Height());
	POINT pt;
	pt.x = m_rect_pic_client.left;
	pt.y = m_rect_pic_client.top;
	this->offsetx = pt.x;
	this->offsety = pt.y;
	
	if (bi.bmHeight > size_rect_pic_client.cy&&bi.bmWidth > size_rect_pic_client.cx)
	{
		
		m_v_scrollbar.SetScrollRange(0, bi.bmHeight - size_rect_pic_client.cy,0);
		m_h_scrollbar.SetScrollRange(0, bi.bmWidth - size_rect_pic_client.cx, 0);
		m_v_scrollbar.ShowWindow(true);
		m_h_scrollbar.ShowWindow(true);
	}
	else if (bi.bmWidth > size_rect_pic_client.cx &&bi.bmHeight <= size_rect_pic_client.cy)
	{
		m_h_scrollbar.SetScrollRange(0, bi.bmWidth - size_rect_pic_client.cx, 0);
		this->offsety = pt.y + ((size_rect_pic_client.cy - bi.bmHeight) / 2);
		m_h_scrollbar.MoveWindow(offsetx, offsety + bi.bmHeight, size_rect_pic_client.cx, 18);
		m_h_scrollbar.ShowWindow(true);
		m_v_scrollbar.ShowWindow(false);
	}
	else if (bi.bmHeight > size_rect_pic_client.cy && bi.bmWidth <= size_rect_pic_client.cx)
	{
		m_v_scrollbar.SetScrollRange(0, bi.bmHeight - size_rect_pic_client.cy, 0);
		this->offsetx = pt.x + ((size_rect_pic_client.cx - bi.bmWidth) / 2);
		m_v_scrollbar.MoveWindow(offsetx + bi.bmWidth, offsety, 18, size_rect_pic_client.cy);
		m_v_scrollbar.ShowWindow(false);
		m_h_scrollbar.ShowWindow(true);
	}
	else if (bi.bmWidth <= size_rect_pic_client.cx && bi.bmHeight <= size_rect_pic_client.cy)
	{
		offsetx = pt.x + ((size_rect_pic_client.cx-bi.bmWidth) / 2);
		offsety = pt.y + ((size_rect_pic_client.cy-bi.bmHeight) / 2);
		m_h_scrollbar.ShowWindow(false);
		m_v_scrollbar.ShowWindow(false);
	}
	else
	{
	}
	InvalidateRect(&m_rect_pic_client);
}

void CExamMemDCDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetSafeHwnd() == m_h_scrollbar.GetSafeHwnd())
	{
		if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBPOSITION)
		{
			m_h_scrollbar.SetScrollPos(nPos);
		}
		else
		{
			int scroll_pos = m_h_scrollbar.GetScrollPos();
			int min = 0;
			int max = 0;

			m_h_scrollbar.GetScrollRange(&min, &max);

			switch (nSBCode)
			{
			case SB_LINEDOWN:
				if (scroll_pos < max)	m_h_scrollbar.SetScrollPos(SB_VERT, ++scroll_pos);
				break;
			case SB_LINEUP:
				if (scroll_pos > min)	m_h_scrollbar.SetScrollPos(SB_VERT, --scroll_pos);
				break;
			case SB_PAGEDOWN:
				if (scroll_pos + 24 < max)	m_h_scrollbar.SetScrollPos(SB_VERT, scroll_pos + 24);
				break;
			case SB_PAGEUP:
				if (scroll_pos - 24 > min)	m_h_scrollbar.SetScrollPos(SB_VERT, scroll_pos - 24);
				break;
			default:
				break;
			}
		}
		Invalidate(FALSE);
	}


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CExamMemDCDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetSafeHwnd() == m_v_scrollbar.GetSafeHwnd())
	{
		if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBPOSITION)
		{
			m_v_scrollbar.SetScrollPos(nPos);
		}
		else
		{
			int scroll_pos = m_v_scrollbar.GetScrollPos();
			int min = 0;
			int max = 0;

			m_v_scrollbar.GetScrollRange(&min, &max);

			switch (nSBCode)
			{
			case SB_LINEDOWN:
				if (scroll_pos < max)	m_v_scrollbar.SetScrollPos(SB_HORZ, ++scroll_pos);
				break;
			case SB_LINEUP:
				if (scroll_pos > min)	m_v_scrollbar.SetScrollPos(SB_HORZ, --scroll_pos);
				break;
			case SB_PAGEDOWN:
				if (scroll_pos + 24 < max)	m_v_scrollbar.SetScrollPos(SB_HORZ, scroll_pos + 24);
				break;
			case SB_PAGEUP:
				if (scroll_pos - 24 > min)	m_v_scrollbar.SetScrollPos(SB_HORZ, scroll_pos - 24);
				break;
			default:
				break;
			}
		}
		Invalidate(FALSE);
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}
