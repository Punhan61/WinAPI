#include<Windows.h>

CONST CHAR gsz_MY_WINDOW_CLASS[] = "MYWindowClass";

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) ����������� ������ ����
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszClassName = NULL;
	wc.lpszClassName = gsz_MY_WINDOW_CLASS;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) C������� ����
	HWND hwnd = CreateWindowEx
	(
		NULL, //ExStyle
		gsz_MY_WINDOW_CLASS,    //Class name
		"My First Window",    //Window name  
		WS_EX_OVERLAPPEDWINDOW, //dwStyle
		CW_USEDEFAULT, CW_USEDEFAULT,  //��������� ������� ����
		CW_USEDEFAULT, CW_USEDEFAULT,  //������ ����
		NULL,       //HWND ������������� ����. � �������� ���� ��� ������������� ����  
		NULL,       //Menu �����������
		hInstance,
		NULL
		);
	if (hwnd==NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) ������ ����� ���������
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:	break;
	case WM_COMMAND:break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:
		switch
			(
				MessageBox(hwnd,
					"�� ������������� ������ ������� ����?",
					"������ �� ������� ��������",
					MB_YESNO | MB_ICONQUESTION)
				)
		{
		case IDYES:
			MessageBox(hwnd, "����� �� ����� �������)", "�������� ����", MB_OK | MB_ICONINFORMATION);
			DestroyWindow(hwnd);
			break;
		}
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}