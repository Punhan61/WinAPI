#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>

#define IDC_BUTTON_0		1000
#define IDC_BUTTON_1		1001
#define IDC_BUTTON_2		1002
#define IDC_BUTTON_3		1003
#define IDC_BUTTON_4		1004
#define IDC_BUTTON_5		1005
#define IDC_BUTTON_6		1006
#define IDC_BUTTON_7		1007
#define IDC_BUTTON_8		1008
#define IDC_BUTTON_9		1009

#define IDC_BUTTON_PLUS		1010
#define IDC_BUTTON_MINUS	1011
#define IDC_BUTTON_ASTER	1012
#define IDC_BUTTON_SLASH	1013
#define IDC_BUTTON_POINT	1014
#define IDC_BUTTON_EQUAL	1015
#define IDC_BUTTON_CLEAR	1016
#define IDC_EDIT			1017

CONST INT g_INTERVAL = 5;
CONST INT g_BUTTON_SIZE = 50;
CONST INT g_DISPLAY_WIDTH = 400;
CONST INT g_DISPLAY_HEIGHT = 20;
CONST INT g_START_X = 10;
CONST INT g_START_Y = 10;

CONST CHAR gsz_MY_WINDOW_CLASS[] = "MyWindowClass";
CONST CHAR gsz_WINDOW_NAME[] = "My Firs Window";

double a, b;
int operation;
bool stored = TRUE;

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна:
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, "Palm.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "Star.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hCursor = (HICON)LoadCursorFromFile("StarBackground.ani");
	//wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = gsz_MY_WINDOW_CLASS;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна:
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	//int window_width = screen_width * 3 / 4;
	//int window_height = screen_width * 3 / 4;
	int window_width = g_START_X * 3.5 + (g_BUTTON_SIZE + g_INTERVAL) * 5;
	int window_height = g_START_Y + (g_BUTTON_SIZE + g_INTERVAL) * 4 + g_INTERVAL * 2 + 55;
	int start_x = screen_width / 8;
	int start_y = screen_height / 8;

	HWND hwnd = CreateWindowEx
	(
		NULL,	//ExStyle
		gsz_MY_WINDOW_CLASS,	//Class name
		gsz_WINDOW_NAME,		//Window name
		//WS_OVERLAPPEDWINDOW,	//dwStyle
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	//dwStyle
		start_x, start_y,	//Начальная позиция окна
		window_width, window_height,	//Размер окна
		NULL,		//HWND родительского окна. У главного окна НЕТ родительского окна
		NULL,		//Menu отсутствует
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);	//Задает режим отображения окна (свернуто в окно, развернуто на весь экран, свернуто на панель задач.....)
	UpdateWindow(hwnd);	//Прорисовывает содержимое окна

	//3) Запуск цикла сообщений:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		//Создаем экран клькулятора:
		CreateWindowEx(NULL, "Edit", "",
			WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_RIGHT,
			g_START_X, g_START_Y,
			(g_BUTTON_SIZE + g_INTERVAL) * 5, 20,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		//Создаем кнопки:
		int digit = 1;
		char sz_digit[] = "0";
		char sz_icon[256] = {};
		sprintf(sz_icon, "ico_numbers\\100%i.ico", digit);
		HICON hIcon = (HICON)LoadImage(GetModuleHandle(NULL), sz_icon, IMAGE_ICON, 48, 48, LR_LOADFROMFILE);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = '0' + digit;
				HWND hDigitButton = CreateWindowEx(
					NULL, "BUTTON", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					g_START_X + (g_BUTTON_SIZE + g_INTERVAL) * j,
					g_START_Y + g_DISPLAY_HEIGHT + g_INTERVAL + (g_BUTTON_SIZE + g_INTERVAL) * (2 - i),
					g_BUTTON_SIZE, g_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_0 + digit),
					GetModuleHandle(NULL),
					NULL
				);
				//SendMessage(hDigitButton, WM_SETICON, 0, (LPARAM)hIcon);
				digit++;
			}
		}
		CONST CHAR* sz_operation[] = { "+", "-", "*", "/" };
		for (int i = 0; i < 4; i++)
		{
			CreateWindowEx(NULL, "BUTTON", sz_operation[i],
				WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
				g_START_X + (g_BUTTON_SIZE + g_INTERVAL) * 3,
				g_START_Y + g_DISPLAY_HEIGHT + g_INTERVAL + (g_BUTTON_SIZE + g_INTERVAL) * i,
				g_BUTTON_SIZE, g_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		CreateWindowEx
		(
			NULL, "BUTTON", "0",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			g_START_X, g_START_Y + g_DISPLAY_HEIGHT + g_INTERVAL + (g_BUTTON_SIZE + g_INTERVAL) * 3,
			g_BUTTON_SIZE * 2 + g_INTERVAL, g_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx(NULL, "BUTTON", ".",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			g_START_X + (g_BUTTON_SIZE + g_INTERVAL) * 2,
			g_START_Y + g_DISPLAY_HEIGHT + g_INTERVAL + (g_BUTTON_SIZE + g_INTERVAL) * 3,
			g_BUTTON_SIZE, g_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);

		CreateWindowEx(NULL, "BUTTON", "C",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			g_START_X + (g_BUTTON_SIZE + g_INTERVAL) * 4,
			g_START_Y + 20 + g_INTERVAL,
			g_BUTTON_SIZE, g_BUTTON_SIZE * 2 + g_INTERVAL,
			hwnd,
			(HMENU)IDC_BUTTON_CLEAR,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx(NULL, "BUTTON", "=",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			g_START_X + (g_BUTTON_SIZE + g_INTERVAL) * 4,
			g_START_Y + 20 + g_INTERVAL * 3 + g_BUTTON_SIZE * 2,
			g_BUTTON_SIZE, g_BUTTON_SIZE * 2 + g_INTERVAL,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
	}
	break;
	case WM_COMMAND:
	{
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE] = {};
		CHAR sz_digit[2] = {};
		//sz_ - String Zero (NULL-Terminated Line - C-строка)
		//https://ru.wikipedia.org/wiki/%D0%92%D0%B5%D0%BD%D0%B3%D0%B5%D1%80%D1%81%D0%BA%D0%B0%D1%8F_%D0%BD%D0%BE%D1%82%D0%B0%D1%86%D0%B8%D1%8F
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			if (stored == TRUE)
			{
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
				//ZeroMemory(sz_buffer, SIZE);
				for (int i = 0; i < SIZE; i++)sz_buffer[i] = 0;
				stored = FALSE;
			}
			sz_digit[0] = LOWORD(wParam) - 952;
			strcat(sz_buffer, sz_digit);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			if (strchr(sz_buffer, '.'))break;
			strcat(sz_buffer, ".");
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLEAR)
		{
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
			a = b = 0;
			operation = 0;
			stored = TRUE;
		}

		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			a = strtod(sz_buffer, NULL);	//strtod() - String to double
			operation = LOWORD(wParam);
			stored = true;
		}

		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			b = strtod(sz_buffer, NULL);
			switch (operation)
			{
			case IDC_BUTTON_PLUS:  a += b;	break;
			case IDC_BUTTON_MINUS: a -= b;	break;
			case IDC_BUTTON_ASTER: a *= b;	break;
			case IDC_BUTTON_SLASH: a /= b;	break;
			default: a = b;
			}
			sprintf(sz_buffer, "%f", a);
			SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			stored = TRUE;
		}
	}
	break;
	case WM_KEYDOWN:
	{
		if (LOWORD(wParam) >= '0' && LOWORD(wParam) <= '9')	SendMessage(hwnd, WM_COMMAND, LOWORD(wParam) + 1000 - '0', 0);
		//if (LOWORD(wParam) == VK_OEM_PERIOD) SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_POINT, 0);
		switch (LOWORD(wParam))
		{
		case VK_OEM_PERIOD:	SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_POINT, 0); break;
		case VK_ESCAPE:		SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_CLEAR, 0); break;
		}
	}
	case WM_SIZE:
	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE] = {};
		sprintf(sz_buffer, "%s, Position:%dx%d, Size:%dx%d", gsz_WINDOW_NAME,
			rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
	}
	break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:
		/*switch
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
		}*/
		DestroyWindow(hwnd);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}