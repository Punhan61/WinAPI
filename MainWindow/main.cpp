#include<Windows.h>
#include<stdio.h>

CONST CHAR gsz_MY_WINDOW_CLASS[] = "MYWindowClass";
CONST CHAR gsz_MY_WINDOW_NAME[] = "My First Window";


INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, "Folder_Gear.ico",IMAGE_ICON,LR_DEFAULTSIZE,LR_DEFAULTSIZE,LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "settings.ico",IMAGE_ICON,LR_DEFAULTSIZE,LR_DEFAULTSIZE,LR_LOADFROMFILE);
	wc.hCursor = (HICON)LoadCursorFromFile("Starcraft.ani");
	
	//wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
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

	//2) Cоздание окна

	int scren_width = GetSystemMetrics(SM_CXSCREEN);
	int scren_height = GetSystemMetrics(SM_CYSCREEN);
	int window_widht = scren_width * 3 / 4;
	int window_height = scren_height * 3 / 4;
	int start_x = scren_width / 8;
	int start_y = scren_height / 8;

	HWND hwnd = CreateWindowEx
	(
		NULL, //ExStyle
		gsz_MY_WINDOW_CLASS,    //Class name
		"My First Window",    //Window name  
		WS_OVERLAPPEDWINDOW, //dwStyle
		start_x, start_y,  //Начальная позиция окна
		window_widht, window_height,  //Размер окна
		NULL,       //HWND родительского окна. У главного окна НЕТ родительского окна  
		NULL,       //Menu отсутстыует
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

	//3) Запуск цикла сообщений
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
	case WM_SIZE:
	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE]={};
		sprintf(sz_buffer, "%S, Position %i:%i Size: %i:%i",gsz_MY_WINDOW_NAME,
			rect.left.rect.top,
			rect.right - rect.left, rect.botton - rect.top);
		SendMessage(hwnd,WM_SETTEXT,)
	}
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:
		switch
			(
				MessageBox(hwnd,
					"Вы действительно хотите закрыть окно?",
					"Вопрос на миллион долларов",
					MB_YESNO | MB_ICONQUESTION)
				)
		{
		case IDYES:
			MessageBox(hwnd, "Лучше бы двери закрыли)", "Полезная инфа", MB_OK | MB_ICONINFORMATION);
			DestroyWindow(hwnd);
			break;
		}
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}