﻿
// ChatMFCApplicationDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ChatMFCApplication.h"
#include "ChatMFCApplicationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CChatMFCApplicationDlg 대화 상자

CChatMFCApplicationDlg::CChatMFCApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATMFCAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatMFCApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON2, sendMessageButton);
	DDX_Control(pDX, IDC_EDIT4, IDC_INPUT_PORT_EDIT);

	DDX_Control(pDX, IDC_MFCBUTTON_1, IDC_SERVER_OPEN_BUTTON);
	DDX_Control(pDX, IDC_MFCBUTTON_2, IDC_SERVER_CLOSE_BUTTON);
	DDX_Control(pDX, IDC_LIST1, IDC_EVENT_MESSAGE_LIST);
}

BEGIN_MESSAGE_MAP(CChatMFCApplicationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND_RANGE( IDC_RADIO_01, IDC_RADIO_02, CChatMFCApplicationDlg::OnClickedRadioButtons )
	ON_COMMAND_RANGE( IDC_MFCBUTTON_2, IDC_MFCBUTTON_1, CChatMFCApplicationDlg::OnClickedTCPbuttons )
END_MESSAGE_MAP()


// CChatMFCApplicationDlg 메시지 처리기

BOOL CChatMFCApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	sendMessageButton.EnableWindowsTheming(false);
	sendMessageButton.SetFaceColor(RGB(254, 240, 27));

	this->socket = unique_ptr<WinSocket>(new WinSocket());
	this->socket->eventListBox = &this->IDC_EVENT_MESSAGE_LIST;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChatMFCApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CChatMFCApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChatMFCApplicationDlg::OnClickedTCPbuttons(const UINT id) {

	switch (id) {
		case IDC_MFCBUTTON_1 : { // IDC_SERVER_OPEN_BUTTON
			CChatMFCApplicationDlg::OpenTCPServer();
			break; 
		}
		case IDC_MFCBUTTON_2 : { // IDC_SERVER_CLOSE_BUTTON
			CChatMFCApplicationDlg::CloseTCPServer();
			break; 
		}
	}

}

void CChatMFCApplicationDlg::OnClickedRadioButtons(const UINT id) {

	switch (id) {
		case IDC_RADIO_01: {
			break;
		}
		case IDC_RADIO_02: {
			break;
		}
	}

}

void CChatMFCApplicationDlg::OpenTCPServer() {

	if (const int length = this->IDC_INPUT_PORT_EDIT.GetWindowTextLength()) {

		CString port_string;
		this->IDC_INPUT_PORT_EDIT.GetWindowText(port_string);
		
		const auto message = this->socket->GetCurrentTimeAndMessage(TEXT("TCP/IP SERVER OPEN"));
		this->IDC_EVENT_MESSAGE_LIST.AddString(message);

		const int port = _ttoi(port_string);
		this->socket->openTCPSocketServer(port, this->m_hWnd);

		// MARK: IDC_SERVER_OPEN_BUTTON, IDC_SERVER_CLOSE_BUTTON Enable (버튼 활성화)
		this->IDC_SERVER_CLOSE_BUTTON.EnableWindow(true);
		this->IDC_SERVER_OPEN_BUTTON.EnableWindow(false);
	}
	else {
		auto message = TEXT("※ Please, Input Port Number.");
		MessageBox(message);
	}
	
}

void CChatMFCApplicationDlg::CloseTCPServer() {

	const auto message = this->socket->GetCurrentTimeAndMessage(TEXT("TCP/IP SERVER CLOSE"));
	this->IDC_EVENT_MESSAGE_LIST.AddString(message);

	this->socket->closeTCPSocketServer();

	// MARK: IDC_SERVER_OPEN_BUTTON, IDC_SERVER_CLOSE_BUTTON Enable (버튼 활성화)
	this->IDC_SERVER_CLOSE_BUTTON.EnableWindow(false);
	this->IDC_SERVER_OPEN_BUTTON.EnableWindow(true);
}

LRESULT CChatMFCApplicationDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	const SOCKET socket = (SOCKET) wParam;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (MWM_SOCK == message) {
		this->socket->OnSocketEvent(this->m_hWnd, socket, lParam, lParam);
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}