//Windows API
//API - Application Programming Interface
//		(Прикладной интерфейс программиста)
//это набор функций и других готовых инструментов для разработки приложений.
#include<Windows.h>
#include"resource.h"

CONST CHAR gsz_LOGIN_INVITATION[] = "Введите имя пользователя";
CONST CHAR gsz_PASSWORD_INVITATION[] = "Введите пароль";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
/*
---------------------------
1. hwnd - окно;
2. uMsg - cообщение;
Параметры сообщения:
3. wParam;
4. lParam;
---------------------------
*/

//#define MSG_BOX

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
//INT	 - тип вовращаемого значения
//WINAPI - соглашение о вызове функции (Calling convention)
//Calling convention определяет какая функция очищает стек, вызывающая, или вызываемая,
//а так же определяет, как параметры передаются в функцию.
//для обычных C/C++ приложений, по умолчанию исползуется конвенция __cdecl,
//в WinAPI всегда исползуется конвенция __stdcall
//https://learn.microsoft.com/en-us/cpp/cpp/argument-passing-and-naming-conventions?view=msvc-170
//caller - вызывающая функция (функция, КОТОРАЯ вызывает другую функцию)
//callee - вызываемая функция (функция, КОТОРУЮ вызывают)
{
	/*
	-------------------------------------------------
		hInstance - *.exe-файл программы
		hPrevInst - предыдущий экземпляр программы
		lpCmdLine - командная строка, которую приложение получает при запуске.
					Если это текстовый редактор, то в него можно передать открываемый файл,
					если это браузер, то  него можно передать загружаемую страницу, и т.д.
					Командная строка представляет собой массив строк, нулевым элементом которого
					всегда является имя *.exe-файла.
					lp - LongPointer (Венгерская нотация)
		nCmdShow -  режим отображения окна при запуске
					(развернуто на весь экран, свернуто в окно, свернуто на панель задач.....)
					n - number (Венгерская нтация)
	-------------------------------------------------
	*/
#ifdef MSG_BOX
	MessageBox(
		NULL,	//Родительское окно
		"Привет! Это наше первое сообщение",
		"Заголовок окна сообщения!",
		MB_YESNOCANCEL | MB_HELP //Набор кнопок
		| MB_ICONASTERISK	//Значек
		| MB_DEFBUTTON3		//Кнопка по умолчанию
		| MB_SYSTEMMODAL	//WS_EX_TOPMOST - поверх всех окон
		| MB_SETFOREGROUND
	);
#endif // MSG_BOX

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:	//Создание элементов окна
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), (LPSTR)IDI_ICON1);
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);

		//Получаем обработчики окна тектовых полей
		//HWND - Handler to Window (Обработчик окна)
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);

		//Для того чтобы установить текст в окна hEditLogin и hEditPassword, нужно отправить сообщение этим окнам:
		SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)gsz_LOGIN_INVITATION);
		SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)gsz_PASSWORD_INVITATION);
	}
	break;
	case WM_COMMAND:	//Обработка команд нажатия кнопок
		switch (LOWORD(wParam))
		{
		case IDC_EDIT_LOGIN:
		{
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS)
			{
				if (strcmp(sz_buffer, gsz_LOGIN_INVITATION) == 0)
					SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)"");
			}
			if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				if (strlen(sz_buffer) == 0)
					SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)gsz_LOGIN_INVITATION);
			}
		}
		break;
		case IDC_BUTTON_COPY:
		{
			//Создаем буфер, через который будет производиться копирование:
			CONST INT SIZE = 256;
			CHAR buffer[SIZE] = {};
			//Получаем окна тактовых полей, для того, чтобы им можно было отправлять сообщения
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			//Читаем содержимое тектового поля LOGIN:
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)buffer);
			//Загружаем текст из буфера в поле PASSWORD:
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)buffer);
		}
		break;
		case IDOK:MessageBox(NULL, "Была нажата кнопка OK", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL:EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:		//Закрытие окна
		EndDialog(hwnd, 0);
	}
	return FALSE;
}