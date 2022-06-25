// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <gdiplus.h>
#include <fstream>
#include <windows.h>

#define MAX_LOADSTRING 100
#define TMR_1 1

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
TCHAR text1[10], text2[10];

INT value;

// buttons
HWND hwndButton;
HWND TextBox, TextBox2;

// sent data
bool is_box = 0;
bool attached = 0;
int kierunek, weight = 100, max_weight = 1000, acceleration;
int col = 0, box_x = 350, box_y = 565, hook_x = 350, hook_y = 500;
std::vector<Point> data;
RECT drawArea1 = { 350, 130, 800, 619 };
RECT drawArea2 = { 50, 400, 650, 422 };

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void tch_waga(int temp)
{
	int i = 0;
	int num = 0;
	while (text1[i] != 0)
	{
		num = (text1[i] - '0') + (num * 10);
		i++;
	}
	if (temp == 0)
		weight = num;
	else if (temp == 0)
		max_weight = num;
}

void inputData()
{
	data.push_back(Point(0, 0));
	for (int i = 1; i < 100; i++) {
		data.push_back(Point(2 * i + 1, 200 * rand() / RAND_MAX));
	}
}


int OnCreate(HWND window)
{
	inputData();
	return 0;
}

void drawBox(HDC hdc) {
	Gdiplus::Graphics gf2(hdc);
	Pen pen(Color(255, 0, 0, 0));
	Gdiplus::Bitmap box(L"box.png");
	gf2.DrawImage(&box, box_x, box_y);
	gf2.DrawLine(&pen, box_x + 22, 124, box_x + 22, box_y);
}

void drawHook(HDC hdc) {
	Gdiplus::Graphics gf2(hdc);
	Pen pen(Color(255, 0, 0, 0));
	Gdiplus::Bitmap box(L"hak.png");
	gf2.DrawImage(&box, hook_x, hook_y);
	gf2.DrawLine(&pen, hook_x + 6, 124, hook_x + 6, hook_y);
	if (is_box == true)
	{
		Gdiplus::Bitmap box(L"box.png");
		gf2.DrawImage(&box, box_x, box_y);
	}

}

void repaintWindow(HWND hWnd, HDC& hdc, PAINTSTRUCT& ps, RECT* drawArea)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	if (attached == 1)
	{
		drawBox(hdc);
	}
	else if (attached == 0)
	{
		drawHook(hdc);
	}

	EndPaint(hWnd, &ps);
}

// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	value = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// create button and store the handle                                                       
	hwndButton = CreateWindow(TEXT("button"), TEXT("UP"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		900, 400, 100, 50, hWnd, (HMENU)ID_RBUTTON1, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("DOWN"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		900, 500, 100, 50, hWnd, (HMENU)ID_RBUTTON2, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("RIGHT"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		1000, 450, 100, 50, hWnd, (HMENU)ID_RBUTTON3, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("LEFT"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		800, 450, 100, 50, hWnd, (HMENU)ID_RBUTTON4, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("STOP"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		900, 450, 100, 50, hWnd, (HMENU)ID_RBUTTON5, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("ADD BOX"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		900, 100,                                  // the left and top co-ordinates
		100, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Weight"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		900, 10,                                  // the left and top co-ordinates
		100, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON6,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("MAX Weight"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		700, 10,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON7,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("ATTACH"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		900, 200,                                  // the left and top co-ordinates
		100, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON8,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("DROP"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		900, 300,                                  // the left and top co-ordinates
		100, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON9,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	OnCreate(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps, psBOX;
	HDC hdc, hdcBOX;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON1:
		{
			is_box = 1;
			repaintWindow(hWnd, hdc, ps, &drawArea1);
		}
		break;
		case ID_RBUTTON1:
			if (weight <= max_weight)
			{
				kierunek = 1;
				SetTimer(hWnd, TMR_1, 25, 0);
			}
			else
			{
				KillTimer(hWnd, TMR_1);
			}
			break;
		case ID_RBUTTON2:
			if (weight <= max_weight)
			{
				kierunek = 2;
				SetTimer(hWnd, TMR_1, 25, 0);
			}
			else
			{
				KillTimer(hWnd, TMR_1);
			}
			break;
		case ID_RBUTTON3:
			if (weight <= max_weight)
			{
				kierunek = 3;
				SetTimer(hWnd, TMR_1, 25, 0);
			}
			else
			{
				KillTimer(hWnd, TMR_1);
			}
			break;
		case ID_RBUTTON4:
			if (weight <= max_weight)
			{
				kierunek = 4;
				SetTimer(hWnd, TMR_1, 25, 0);
			}
			else
			{
				KillTimer(hWnd, TMR_1);
			}
			break;
		case ID_RBUTTON5:
			KillTimer(hWnd, TMR_1);
			break;
		case ID_BUTTON6:
			GetWindowText(TextBox, text1, 10);
			tch_waga(0);
			break;
		case ID_BUTTON7:
			GetWindowText(TextBox2, text2, 10);
			tch_waga(1);
			break;
		case ID_BUTTON8:
			if (is_box == 1)
			{
				kierunek = 5;
				SetTimer(hWnd, TMR_1, 25, 0);
			}
			else
			{
				KillTimer(hWnd, TMR_1);
			}
			break;
		case ID_BUTTON9:
		{
			kierunek = 6;
			SetTimer(hWnd, TMR_1, 25, 0);
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
	{
		TextBox = CreateWindow(TEXT("EDIT"), TEXT("100"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			840, 25, 60, 20,
			hWnd, (HMENU)NULL, NULL, NULL);

		TextBox2 = CreateWindow(TEXT("EDIT"), TEXT("1000"),
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			640, 25, 60, 20,
			hWnd, (HMENU)NULL, NULL, NULL);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Tutaj dodaj kod rysuj¹cy u¿ywaj¹cy elementu hdc...
		Gdiplus::Graphics gf(hdc);
		Gdiplus::Bitmap bmp(L"dzwig.png");
		gf.DrawImage(&bmp, 0, 0);
		EndPaint(hWnd, &ps);
		repaintWindow(hWnd, hdc, ps, &drawArea1);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			if (((box_x < 700 && attached == 1) || (hook_x < 700 && attached == 0)) && kierunek == 3)//b³¹d
			{
				if (attached == 1)
				{
					box_x++;
				}
				hook_x++;
			}
			else if (((box_x > 350 && attached == 1) || (hook_x > 350 && attached == 0)) && kierunek == 4)//b³¹d
			{
				if (attached == 1)
				{
					box_x--;
				}
				hook_x--;
			}
			else if (((box_y > 150 && attached == 1) || (hook_y > 150 && attached == 0)) && kierunek == 1)//b³¹d
			{
				if (attached == 1)
				{
					box_y--;
				}
				hook_y--;
			}
			else if (((box_y < 565 && attached == 1) || (hook_y < 565 && attached == 0)) && kierunek == 2)//b³¹d
			{
				if (attached == 1)
				{
					box_y++;
				}
				hook_y++;
			}
			else if (kierunek == 5 && attached == 0)
			{
				if (box_x > hook_x - 18)
				{
					hook_x++;
				}
				else if (box_x < hook_x - 18)
				{
					hook_x--;
				}
				else if (box_y < hook_y + 10)
				{
					hook_y--;
				}
				else if (box_y > hook_y + 10)
				{
					hook_y++;
				}
				if (hook_x - 18 == box_x && hook_y + 10 == box_y)
				{
					attached = 1;
				}
			}
			else if (kierunek == 6 && box_y<=565)
			{
				attached = 0;
				box_y=box_y+acceleration;
				acceleration++;
				if (box_y >= 565)
				{
					box_y = 565;
					acceleration = 0;
				}
			}
			else
			{
				KillTimer(hWnd, TMR_1);
			}
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}