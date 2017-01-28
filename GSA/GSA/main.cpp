#include"test_func.h"
#include"GSA.h"
#include <iostream>
#include <sstream>
#include <string>
#include<stdlib.h>
#include <cmath>
#include <Windows.h>
#include "resource.h"



BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
		int ret = DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1),
			NULL, (DLGPROC)DlgProc);
		return ret;
}


HWND hCombo1;
TCHAR str_Him[] = {"Функция Химмельблау"};
TCHAR str_Simp1[] = { "x + y" };
TCHAR str_Simp2[] = { "x^2" };

HWND hCombo2;
TCHAR str_min[] = { "min" };
TCHAR str_max[] = { "max" };

HWND hCombo3;
TCHAR str_res1[] = { "restrict1" };
TCHAR str_res2[] = { "restrict2" };

BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
	{

		hCombo1 = GetDlgItem(hwnd, IDC_FUNC);
		SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)str_Him);
		SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)str_Simp1);
		SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)str_Simp2);

		hCombo2 = GetDlgItem(hwnd, IDC_EXS);
		SendMessage(hCombo2, CB_ADDSTRING, 0, (LPARAM)str_max);
		SendMessage(hCombo2, CB_ADDSTRING, 0, (LPARAM)str_min);

		hCombo3 = GetDlgItem(hwnd, IDC_REST);
		SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str_res1);
		SendMessage(hCombo3, CB_ADDSTRING, 0, (LPARAM)str_res2);
		return FALSE;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_OK:
		{
			TCHAR SN[10];
			int n;
			//получаем данные из поля IDC_EDIT1
			GetDlgItemText(hwnd, IDC_N, SN, 8);
			int N = atoi(SN);
			GetDlgItemText(hwnd, IDC_IT, SN, 8);
			int it = atoi(SN);
			GetDlgItemText(hwnd, IDC_G, SN, 8);
			double G = atof(SN);
			n = SendMessage(hCombo2, CB_GETCURSEL, 0, 0L);
			bool mode = n == 0 ? true : false;
			n = SendMessage(hCombo1, CB_GETCURSEL, 0, 0L);
			double(*func)(vector<double>);
			switch (n)
			{
			case 0:
				func = func2;
				break;
			case 1:
				func = func3;
				break;
			case 2:
				func = func1;
				break;
			default:
				break;
			}
			bool(*restrict) (vector<double>);
			n = SendMessage(hCombo3, CB_GETCURSEL, 0, 0L);
			switch (n)
			{
			case 0:
			{
				restrict = restrict1;
				break;
			}
			case 1:
				restrict = restrict2;
				break;
			}
			if ((!func) || (!restrict)||(!G)||(!N)||(!it)||(n<0))
			{
				MessageBox(NULL, (LPCTSTR)"ОШИБКА! Поля заполнены не корректно", "Error",
					MB_OK | MB_ICONWARNING);
				return FALSE;
			}
			double start = GetTickCount();
			vector<double> max = GSA::GSA(func, 2, restrict, G, N, it, mode);
			double end = GetTickCount();
			double time = end - start;
			double best = func(max);
			std::ostringstream out;
			out << std::fixed << max[0];

			char R[30];
			strcpy_s(R, out.str().c_str());
			SetDlgItemText(hwnd, IDC_RX, R);

			out.str("");

			out << std::fixed << max[1];
			strcpy_s(R, out.str().c_str());
			SetDlgItemText(hwnd, IDC_RY, R);

			out.str("");

			out << std::fixed << best;
			strcpy_s(R, out.str().c_str());
			SetDlgItemText(hwnd, IDC_RFUNC, R);

			out.str("");

			out << std::fixed << time;
			strcpy_s(R, out.str().c_str());
			SetDlgItemText(hwnd, IDC_TIME, R);

			return FALSE;
		}
		case IDC_N:
			SetDlgItemText(hwnd, IDC_RX, "");
			SetDlgItemText(hwnd, IDC_RY, "");
			SetDlgItemText(hwnd, IDC_RFUNC, "");
			SetDlgItemText(hwnd, IDC_TIME, "");
			return FALSE;
		case IDC_G:
			SetDlgItemText(hwnd, IDC_RX, "");
			SetDlgItemText(hwnd, IDC_RY, "");
			SetDlgItemText(hwnd, IDC_RFUNC, "");
			SetDlgItemText(hwnd, IDC_TIME, "");
			return FALSE;
		case IDC_IT:
			SetDlgItemText(hwnd, IDC_RX, "");
			SetDlgItemText(hwnd, IDC_RY, "");
			SetDlgItemText(hwnd, IDC_RFUNC, "");
			SetDlgItemText(hwnd, IDC_TIME, "");
			return FALSE;
		case IDC_EXS:
			SetDlgItemText(hwnd, IDC_RX, "");
			SetDlgItemText(hwnd, IDC_RY, "");
			SetDlgItemText(hwnd, IDC_RFUNC, "");
			SetDlgItemText(hwnd, IDC_TIME, "");
			return FALSE;
		case IDC_FUNC:
			SetDlgItemText(hwnd, IDC_RX, "");
			SetDlgItemText(hwnd, IDC_RY, "");
			SetDlgItemText(hwnd, IDC_RFUNC, "");
			SetDlgItemText(hwnd, IDC_TIME, "");
			return FALSE;
		case IDC_REST:
		{
			int n = SendMessage(hCombo3, CB_GETCURSEL, 0, 0L);
			switch (n)
			{
				case 0:
				{
					SetDlgItemText(hwnd, IDC_VREST, "- 4 <= x <= 4\r\n- 4 <= y <= 4");
					break;
				}
				case 1:
				{
					SetDlgItemText(hwnd, IDC_VREST, "- 4 <= x <= 4");
					break;
				}
			}
			return FALSE;
		}
		case IDC_VREST:
			return FALSE;
		case IDC_RFUNC:
			return FALSE;
		case IDC_RX:
			return FALSE;
		case IDC_RY:
			return FALSE;
		case IDC_TIME:
			return FALSE;
		}
	}
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return FALSE;
	}
	}
	return FALSE;
}