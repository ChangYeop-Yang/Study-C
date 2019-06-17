
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
	DDX_Control(pDX, IDC_EDIT4, IDC_INPUT_PORT_EDIT);

	DDX_Control(pDX, IDC_MFCBUTTON_1, IDC_SERVER_OPEN_BUTTON);
	DDX_Control(pDX, IDC_MFCBUTTON_2, IDC_SERVER_CLOSE_BUTTON);
	DDX_Control(pDX, IDC_LIST1, IDC_EVENT_MESSAGE_LIST);
	DDX_Control(pDX, IDC_MFCBUTTON2, IDC_MESSAGE_SEND_BUTTON);
	DDX_Control(pDX, IDC_EDIT1, IDC_INPUT_MESSAGE_EDIT);
	DDX_Control(pDX, IDC_RADIO_01, IDC_SERVER_MODE_RADIO);
	DDX_Control(pDX, IDC_COMBO2, IDC_PORT_DROP_BOX);
	DDX_Control(pDX, IDC_COMBO1, IDC_BANDWITH_DROP_BOX);
	DDX_Control(pDX, IDC_MFCBUTTON3, IDC_SERIAL_CONNNECT_BUTTON);
}

BEGIN_MESSAGE_MAP(CChatMFCApplicationDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(19, &CChatMFCApplicationDlg::OnDidReceiveSerialMessage)

	ON_COMMAND_RANGE( IDC_RADIO_01, IDC_RADIO_02, CChatMFCApplicationDlg::OnClickedRadioButtons )
	ON_COMMAND_RANGE( IDC_MFCBUTTON_2, IDC_MFCBUTTON_1, CChatMFCApplicationDlg::OnClickedTCPbuttons )

	ON_BN_CLICKED( IDC_MFCBUTTON2, &CChatMFCApplicationDlg::OnSendMessage )
	ON_EN_CHANGE(IDC_EDIT1, &CChatMFCApplicationDlg::OnChangeMessage)
	ON_BN_CLICKED(IDC_MFCBUTTON3, &CChatMFCApplicationDlg::OnConnectSerial)
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
	this->IDC_MESSAGE_SEND_BUTTON.EnableWindowsTheming(false);
	this->IDC_MESSAGE_SEND_BUTTON.SetFaceColor(RGB(254, 240, 27));

	this->IDC_SERIAL_CONNNECT_BUTTON.EnableWindowsTheming(false);
	this->IDC_SERIAL_CONNNECT_BUTTON.SetFaceColor(RGB(254, 240, 27));

	this->socket = unique_ptr<WinSocket>( new WinSocket() );
	this->socket->eventListBox = &this->IDC_EVENT_MESSAGE_LIST;

	this->IDC_INPUT_PORT_EDIT.SetLimitText(MAX_PORT_EDIT_DIGIT);
	this->IDC_SERVER_MODE_RADIO.SetCheck(true);

	// MARK: Setting Windows Form Title
	this->SetWindowTextW( TEXT("TCP/IP CHAT PROJECT") );

	SettingDropBoxMenu();

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

// MARK: - Action Methods

void CChatMFCApplicationDlg::OnClickedTCPbuttons(const UINT id) {

	switch (id) {
		case IDC_MFCBUTTON_1 : { // IDC_SERVER_OPEN_BUTTON
			this->socket_mode ? CChatMFCApplicationDlg::OpenTCPServer() : CChatMFCApplicationDlg::ConnectTCPClient();
			break; 
		}
		case IDC_MFCBUTTON_2 : { // IDC_SERVER_CLOSE_BUTTON
			this->socket_mode ? CChatMFCApplicationDlg::CloseTCPServer() : CChatMFCApplicationDlg::DisConnectTCPSocketClient();
			break; 
		}
	}

}

void CChatMFCApplicationDlg::OnClickedRadioButtons(const UINT id) {

	CString message;

	// MARK: Clear Edit Contril Message.
	this->IDC_INPUT_PORT_EDIT.SetSel(0, EOF);
	this->IDC_INPUT_PORT_EDIT.Clear();

	switch (id) {
		case IDC_RADIO_01: {
			this->socket_mode = true;
			this->IDC_INPUT_PORT_EDIT.SetLimitText(MAX_PORT_EDIT_DIGIT);
			message = "서버 모드 (CHANGE SERVER MODE)";
			break;
		}
		case IDC_RADIO_02: {
			this->socket_mode = false;
			this->IDC_INPUT_PORT_EDIT.SetLimitText(MAX_IP_EDIT_DIGIT);
			message = "클라이언트 모드 (CHANGE CLIENT MODE)";
			break;
		}
	}

	MessageBox(message);
}

void CChatMFCApplicationDlg::OnSendMessage()
{
	CString message;
	this->IDC_INPUT_MESSAGE_EDIT.GetWindowTextW(message);

	std::string send_message = std::string( ATL::CW2A(message.GetString()) );

	// MARK: TCP/IP Socket Open and TCP/IP Socket Connected.
	if (socket_mode) {
		this->socket->OnAllSendClientMessage(send_message);
	} else {
		this->socket->OnSendMessageServer(send_message);
	}

	// MARK: Serial Socket Open and Serial Socket Connected.
	if (this->serial != nullptr && this->serial->connected) {
		this->serial->WriteMessageSerial(send_message);
	}

	// MARK: Clear Edit Contril Message.
	this->IDC_INPUT_MESSAGE_EDIT.SetSel(0, EOF);
	this->IDC_INPUT_MESSAGE_EDIT.Clear();
}

void CChatMFCApplicationDlg::OnChangeMessage()
{
	if (const int length = this->IDC_INPUT_MESSAGE_EDIT.GetWindowTextLengthW()) {
		this->IDC_MESSAGE_SEND_BUTTON.EnableWindow(true);
	}
	else {
		this->IDC_MESSAGE_SEND_BUTTON.EnableWindow(false);
	}
}

void CChatMFCApplicationDlg::OnConnectSerial()
{
	std::pair<CString, CString> port;

	auto pos = this->IDC_BANDWITH_DROP_BOX.GetCurSel();
	this->IDC_BANDWITH_DROP_BOX.GetLBText(pos, port.first);

	pos = this->IDC_PORT_DROP_BOX.GetCurSel();
	this->IDC_PORT_DROP_BOX.GetLBText(pos, port.second);
	
	std::pair<std::string, std::string> pass = std::make_pair( std::string( ATL::CW2A(port.first.GetString()) ), std::string( ATL::CW2A(port.second.GetString()) ) );
	this->serial = std::unique_ptr<WinSerial>( new WinSerial(pass, this->m_hWnd) );
}

// MARK: - User Methods

void CChatMFCApplicationDlg::SettingDropBoxMenu() {

	// MARK: Setting IDC_PORT_DROP_BOX Add String.
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM1") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM2") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM3") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM4") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM5") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM6") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM7") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM8") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM9") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM10") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM11") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM12") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM13") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM14") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM15") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM16") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM17") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM18") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM19") );
	this->IDC_PORT_DROP_BOX.AddString( TEXT("COM20") );

	// MARK: Setting IDC_BANDWITH_DROP_BOX Add String.
	this->IDC_BANDWITH_DROP_BOX.AddString( TEXT("9600") );
	this->IDC_BANDWITH_DROP_BOX.AddString( TEXT("38400") );
	this->IDC_BANDWITH_DROP_BOX.AddString( TEXT("57600") );
	this->IDC_BANDWITH_DROP_BOX.AddString( TEXT("115200") );
}

void CChatMFCApplicationDlg::OpenTCPServer() {

	if (const int length = this->IDC_INPUT_PORT_EDIT.GetWindowTextLength()) {

		CString port_string;
		this->IDC_INPUT_PORT_EDIT.GetWindowText(port_string);
		
		const auto message = this->socket->GetCurrentTimeAndMessage( TEXT("TCP/IP SERVER OPEN") );
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

void CChatMFCApplicationDlg::ConnectTCPClient() {

	CString message;
	this->IDC_INPUT_PORT_EDIT.GetWindowTextW(message);

	std::string convert_str = std::string( ATL::CW2A(message.GetString()) );
	if (convert_str.find(' ') == EOF) {
		MessageBox( TEXT("[IP PORT]의 형식으로 서버정보를 입력주세요.") );
		return;
	}

	auto server_info = SplitIPAddressAndPort(convert_str);
	
	this->socket->ConnectTCPClient(server_info.first, std::stoi(server_info.second), this->m_hWnd);

	// MARK: IDC_SERVER_OPEN_BUTTON, IDC_SERVER_CLOSE_BUTTON Enable (버튼 활성화)
	this->IDC_SERVER_CLOSE_BUTTON.EnableWindow(true);
	this->IDC_SERVER_OPEN_BUTTON.EnableWindow(false);
}

void CChatMFCApplicationDlg::DisConnectTCPSocketClient() {

	this->socket->DisConnectTCPSocketClient();

	// MARK: IDC_SERVER_OPEN_BUTTON, IDC_SERVER_CLOSE_BUTTON Enable (버튼 활성화)
	this->IDC_SERVER_CLOSE_BUTTON.EnableWindow(false);
	this->IDC_SERVER_OPEN_BUTTON.EnableWindow(true);
}

std::pair<std::string, std::string> CChatMFCApplicationDlg::SplitIPAddressAndPort(const std::string stub) {

	auto index = 0;
	while (stub[index++] != ' ');

	auto result = std::make_pair( std::string(stub.begin(), stub.begin() + index), std::string(stub.begin() + index, stub.end()) );
	return result;
}

// MARK: - System Call Methods

LRESULT CChatMFCApplicationDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	const SOCKET socket = (SOCKET) wParam;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (message) {
		case MWM_SERVER_EVENT_SOCK:

			char message[BUFSIZ];
			std::memset(&message, 0, sizeof(message));

			if ( const int length = recv(socket, message, BUFSIZ, 0) ) {
				const std::string message_str = std::string(message);
				this->socket->OnSocketServerEventHandler(this->m_hWnd, socket, lParam, lParam, message_str);
				
				// MARK: Only Socket Connected Client.
				if (this->serial != nullptr && this->serial->connected && socket_mode) {
					this->socket->OnAllSendClientMessage(message_str);
				}
			}
			break;

		case MWM_CLIENT_EVENT_SOCK:
			this->socket->OnSocketClientEventHandler(this->m_hWnd, socket, lParam, lParam);
			break;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

LRESULT CChatMFCApplicationDlg::OnDidReceiveSerialMessage(WPARAM wParam, LPARAM lParam) {
	
	CString * receive = (CString *) lParam;
	
	CString message = CString();
	message.Format( _T("※ [SERIAL - MESSAGE] %s"), *receive);
	
	this->IDC_EVENT_MESSAGE_LIST.AddString(message);

	auto convert = std::string(ATL::CW2A(receive->GetString()));

	delete(receive);

	return 0;
}