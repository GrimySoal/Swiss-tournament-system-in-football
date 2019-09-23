// WindowsProject1.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "WindowsProject1.h"
#include<vector>
#include<cstring>
#include<algorithm>
#include <fstream>
#include <Commdlg.h>
#include <winuser.h>
#include <windowsx.h>

#define MAX_LOADSTRING 100
#pragma warning(disable : 4996)
using namespace std;

struct Team {
	char*name;
	int score;
	int rate;
	int missed_goals;
	int goals_scored;
	int nmb;
	Team(char*_name, int _nmb) {
		name = new char[strlen(_name) + 1];
		strcpy(name, _name);
		nmb = _nmb;
		score = 0;
		rate = 0;
		missed_goals = 0;
		goals_scored = 0;
	}
};

struct TablePart {
	HWND hPlace;
	HWND hName;
	HWND hScore;
	HWND hRate;
	HWND hM_G;
	HWND hG_S;
};

// Глобальные переменные:
HINSTANCE hInst;                               // текущий экземпляр
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
char*TeamName;
vector<char*> Teams;
vector<Team> Tournament;
vector<pair<char*, char*>> Games;
bool Setka[50][50];
vector<Team>::iterator It;
int cnt = 0;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Tour(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    CreateNewTour(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ChooseTour(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			Setka[i][j] = false;
		}
		Setka[i][i] = true;
	}

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow, hWnd))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND hWnd)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

void Calculate() {
	vector<Team> temp(Tournament);
	for (int i = 0; 1 < temp.size(); i++) {
		int gg = temp[0].score;
		auto p = find_if(temp.rbegin(), temp.rend(), [gg](Team&obj) {
			return obj.score == gg;
		});
		while (p + 1 != temp.rend()) {
			if (!Setka[temp[0].nmb][p->nmb]) {
				pair<char*, char*> Temp;
				Temp.first = temp[0].name;
				Temp.second = p->name;
				Games.push_back(Temp);
				temp.erase(temp.rend() - p - 1 + temp.begin());
				temp.erase(temp.begin());
				goto leave;
			}
			p++;
		}
		for (int j = 1; j < temp.size(); j++) {
			if (!Setka[temp[0].nmb][temp[j].nmb]) {
				pair<char*, char*> Temp;
				Temp.first = temp[0].name;
				Temp.second = temp[j].name;
				Games.push_back(Temp);
				temp.erase(temp.begin() + j);
				temp.erase(temp.begin());
				break;
			}
		}
		if (temp.size() > 1 && i > Tournament.size() / 2) {
			while (temp.size() > 1) {
				char*team = Games[Games.size() - 1].first;
				auto it = find_if(Tournament.begin(), Tournament.end(), [team](Team&obj) {
					if (strcmp(team, obj.name) == 0) return true;
					else return false;
				});
				temp.push_back(Tournament[it - Tournament.begin()]);
				team = Games[Games.size() - 1].second;
				it = find_if(Tournament.begin(), Tournament.end(), [team](Team&obj) {
					if (strcmp(team, obj.name) == 0) return true;
					else return false;
				});
				temp.push_back(Tournament[it - Tournament.begin()]);
				Games.pop_back();
				sort(temp.begin(), temp.end(), [](Team&obj1, Team&obj2) {
					return	obj1.nmb < obj2.nmb;
				});
				do {
					for (int j = 0; j < (temp.size()/2)*2; j += 2) {
						if (Setka[temp[j].nmb][temp[j + 1].nmb]) goto badd;
					}
					for (int j = 0; 1 < temp.size(); j += 2) {
						pair<char*, char*> Temp;
						Temp.first = temp[0].name;
						Temp.second = temp[1].name;
						Games.push_back(Temp);
						temp.erase(temp.begin());
						temp.erase(temp.begin());
					}
					break;
				badd:
					1 > 2;
				} while (next_permutation(temp.begin(), temp.end(), [](Team&obj1, Team&obj2) { return obj1.nmb < obj2.nmb; }));
			}
		}
	leave:
		if (temp.size() == 1) {
			char*temperary = temp[0].name;
			It = find_if(Tournament.begin(), Tournament.end(), [temperary](Team&obj) {
				if (strcmp(temperary, obj.name) == 0) return true;
				else return false; });
		}
	}
}

void SortTeams() {
	for (int i = 0; i < Tournament.size(); i++) {
		Tournament[i].rate = 0;
		for (int j = 0; j < Tournament.size(); j++) {
			if (Setka[i][j] && i != j) {
				Tournament[i].rate += Tournament[j].score;
			}
		}
	}
	sort(Tournament.begin(), Tournament.end(), [](Team&obj1, Team&obj2) {
		if (obj1.score > obj2.score) return true;
		else if (obj1.score < obj2.score) return false;
		else if (obj1.rate > obj2.rate) return true;
		else if (obj1.rate < obj2.rate) return false;
		else if (obj1.goals_scored - obj1.missed_goals > obj2.goals_scored - obj2.missed_goals) return true;
		else if (obj1.goals_scored - obj1.missed_goals < obj2.goals_scored - obj2.missed_goals) return false;
		else if (obj1.goals_scored > obj2.goals_scored) return true;
		else return false;
	});
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hNextRound;
	static HWND teams[50];
	static HWND scores[50];
	static TablePart Table[50];
	static TablePart hat;
    switch (message)
    {
	case WM_CREATE:
	{
		RECT hRect;
		GetClientRect(hWnd, &hRect);
		hNextRound = CreateWindow(L"button", L"Рассчитать", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, hRect.right - 100, hRect.bottom - 50, 80, 30, hWnd, (HMENU)9999, hInst, NULL);
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Tour);
		if (cnt >= Tournament.size() - 1) goto skip2;
		Calculate();
	skip2:
		int j = 0;
		int k = 0;
		for (int i = 0; i < Games.size(); i++) {
			if (50 + 60 * k >= hRect.bottom - 50) {
				j += 220;
				k = 0;
			}
			teams[i * 2] = CreateWindowA("edit", Games[i].first, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, 100 + j, 10 + 60 * k, 160, 20, hWnd, (HMENU)(11000 + 2 * i), hInst, NULL);
			teams[i * 2 + 1] = CreateWindowA("edit", Games[i].second, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, 100 + j, 30 + 60 * k, 160, 20, hWnd, (HMENU)(11001 + 2 * i), hInst, NULL);
			scores[i * 2] = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 260 + j, 10 + 60 * k, 25, 20, hWnd, (HMENU)(11100 + 2 * i), hInst, NULL);
			scores[i * 2 + 1] = CreateWindowA("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 260 + j, 30 + 60 * k, 25, 20, hWnd, (HMENU)(11101 + 2 * i), hInst, NULL);
			k++;
		}
		hat.hPlace = CreateWindowA("edit", "№", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 320, 10, 20, 20, hWnd, (HMENU)12600, hInst, NULL);
		hat.hName = CreateWindowA("edit", "Команда", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 300, 10, 160, 20, hWnd, (HMENU)12601, hInst, NULL);
		hat.hScore = CreateWindowA("edit", "О", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 140, 10, 20, 20, hWnd, (HMENU)12602, hInst, NULL);
		hat.hRate = CreateWindowA("edit", "Р", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 120, 10, 20, 20, hWnd, (HMENU)12603, hInst, NULL);
		hat.hG_S = CreateWindowA("edit", "З", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 100, 10, 20, 20, hWnd, (HMENU)12604, hInst, NULL);
		hat.hM_G = CreateWindowA("edit", "П", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 80, 10, 20, 20, hWnd, (HMENU)12605, hInst, NULL);
		for (int i = 0; i < Tournament.size(); i++) {
			Table[i].hName = CreateWindowA("edit", Tournament[i].name, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 300, 30 + 20 * i, 160, 20, hWnd, (HMENU)(12000 + i), hInst, NULL);
			char temp[10];
			itoa(Tournament[i].score, temp, 10);
			Table[i].hScore = CreateWindowA("edit", temp, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 140, 30 + 20 * i, 20, 20, hWnd, (HMENU)(12100 + i), hInst, NULL);
			itoa(Tournament[i].rate, temp, 10);
			Table[i].hRate = CreateWindowA("edit", temp, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 120, 30 + 20 * i, 20, 20, hWnd, (HMENU)(12200 + i), hInst, NULL);
			itoa(Tournament[i].goals_scored, temp, 10);
			Table[i].hG_S = CreateWindowA("edit", temp, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 100, 30 + 20 * i, 20, 20, hWnd, (HMENU)(12300 + i), hInst, NULL);
			itoa(Tournament[i].missed_goals, temp, 10);
			Table[i].hM_G = CreateWindowA("edit", temp, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 80, 30 + 20 * i, 20, 20, hWnd, (HMENU)(12400 + i), hInst, NULL);
			itoa(i + 1, temp, 10);
			Table[i].hPlace = CreateWindowA("edit", temp, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_EX_NOACTIVATE, hRect.right - 320, 30 + 20 * i, 20, 20, hWnd, (HMENU)(12500 + i), hInst, NULL);
		}
		ofstream fout(TeamName);
		fout << Tournament.size() << endl;
		for (int i = 0; i < Tournament.size(); i++) {
			fout << Tournament[i].name << endl << Tournament[i].nmb << " " << Tournament[i].score << " " << Tournament[i].rate << " " << Tournament[i].goals_scored << " " << Tournament[i].missed_goals << endl;
		}
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				fout << Setka[i][j] << " ";
			}
			fout << endl;
		}
		fout << cnt << endl;
	}
	break;
	case WM_SIZE:
	{
		RECT hRect;
		GetClientRect(hWnd, &hRect);
		MoveWindow(hNextRound, hRect.right - 100, hRect.bottom - 50, 80, 30, true);
		MoveWindow(hat.hPlace, hRect.right - 320, 10, 20, 20, true);
		MoveWindow(hat.hName, hRect.right - 300, 10, 160, 20, true);
		MoveWindow(hat.hScore, hRect.right - 140, 10, 20, 20, true);
		MoveWindow(hat.hRate, hRect.right - 120, 10, 20, 20, true);
		MoveWindow(hat.hG_S, hRect.right - 100, 10, 20, 20, true);
		MoveWindow(hat.hM_G, hRect.right - 80, 10, 20, 20, true);
		for (int i = 0; i < Tournament.size(); i++) {
			MoveWindow(Table[i].hPlace, hRect.right - 320, 30 + 20 * i, 20, 20, true);
			MoveWindow(Table[i].hName, hRect.right - 300, 30 + 20 * i, 160, 20, true);
			MoveWindow(Table[i].hScore, hRect.right - 140, 30 + 20 * i, 20, 20, true);
			MoveWindow(Table[i].hRate, hRect.right - 120, 30 + 20 * i, 20, 20, true);
			MoveWindow(Table[i].hG_S, hRect.right - 100, 30 + 20 * i, 20, 20, true);
			MoveWindow(Table[i].hM_G, hRect.right - 80, 30 + 20 * i, 20, 20, true);
		}
		int j = 0;
		int k = 0;
		for (int i = 0; i < Games.size(); i++) {
			if (50 + 60 * k >= hRect.bottom - 50) {
				j += 220;
				k = 0;
			}
			MoveWindow(teams[i * 2], 100 + j, 10 + 60 * k, 160, 20, true);
			MoveWindow(teams[i * 2 + 1], 100 + j, 30 + 60 * k, 160, 20, true);
			MoveWindow(scores[i * 2], 260 + j, 10 + 60 * k, 25, 20, true);
			MoveWindow(scores[i * 2 + 1], 260 + j, 30 + 60 * k, 25, 20, true);
			k++;
		}
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
			case 9999:
			{
				for (int i = 0; i < Games.size(); i++) {
					char*temp = Games[i].first;
					auto p1 = find_if(Tournament.begin(), Tournament.end(), [temp](Team&obj) {
						if (strcmp(temp, obj.name) == 0) return true;
						else return false;
					});
					temp = Games[i].second;
					auto p2 = find_if(Tournament.begin(), Tournament.end(), [temp](Team&obj) {
						if (strcmp(temp, obj.name) == 0) return true;
						else return false;
					});
					Setka[p1->nmb][p2->nmb] = true;
					Setka[p2->nmb][p1->nmb] = true;
				}
				cnt++;
				if(Tournament.size() % 2) It->score += 3;
				for (int i = 0; i < Games.size(); i++) {
					char*temperary = Games[i].first;
					auto team1 = find_if(Tournament.begin(), Tournament.end(), [temperary](Team&obj) {
						if (strcmp(temperary, obj.name) == 0) return true;
						else return false; });
					temperary = Games[i].second;
					auto team2 = find_if(Tournament.begin(), Tournament.end(), [temperary](Team&obj) {
						if (strcmp(temperary, obj.name) == 0) return true;
						else return false; });
					char goals[5];
					GetWindowTextA(scores[i * 2], goals, 4);
					int goals1 = atoi(goals);
					GetWindowTextA(scores[i * 2 + 1], goals, 4);
					int goals2 = atoi(goals);
					if (goals1 > goals2) team1->score += 3;
					else if (goals1 < goals2) team2->score += 3;
					else {
						team1->score++;
						team2->score++;
					}
					team1->goals_scored += goals1;
					team1->missed_goals += goals2;
					team2->goals_scored += goals2;
					team2->missed_goals += goals1;
				}
				SortTeams();
				if (cnt >= Tournament.size() - 1) goto skip;
				Games.erase(Games.begin(), Games.end());
				Calculate();
				skip:
				for (int i = 0; i < Games.size(); i++) {
					SetWindowTextA(teams[i * 2], Games[i].first);
					SetWindowTextA(teams[i * 2 + 1], Games[i].second);
					SetWindowTextA(scores[i * 2], "");
					SetWindowTextA(scores[i * 2 + 1], "");
				}
				for (int i = 0; i < Tournament.size(); i++) {
					SetWindowTextA(Table[i].hName, Tournament[i].name);
					char temp[10];
					itoa(i + 1, temp, 10);
					SetWindowTextA(Table[i].hPlace, temp);
					itoa(Tournament[i].score, temp, 10);
					SetWindowTextA(Table[i].hScore, temp);
					itoa(Tournament[i].rate, temp, 10);
					SetWindowTextA(Table[i].hRate, temp);
					itoa(Tournament[i].goals_scored, temp, 10);
					SetWindowTextA(Table[i].hG_S, temp);
					itoa(Tournament[i].missed_goals, temp, 10);
					SetWindowTextA(Table[i].hM_G, temp);
				}
				ofstream fout(TeamName);
				fout << Tournament.size() << endl;
				for (int i = 0; i < Tournament.size(); i++) {
					fout << Tournament[i].name << endl << Tournament[i].nmb << " " << Tournament[i].score << " " << Tournament[i].rate << " " << Tournament[i].goals_scored << " " << Tournament[i].missed_goals << endl;
				}
				for (int i = 0; i < 50; i++) {
					for (int j = 0; j < 50; j++) {
						fout << Setka[i][j] << " ";
					}
					fout << endl;
				}
				fout << cnt << endl;
			}
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK CreateNewTour(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	static HWND edit1, hList;
	switch (message)
	{
	case WM_INITDIALOG:
		edit1 = GetDlgItem(hDlg, IDC_TEAMNAME);
		hList = GetDlgItem(hDlg, IDC_LIST1);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_ADDTEAM)
		{
			char*temp = new char[100];
			GetDlgItemTextA(hDlg, IDC_TEAMNAME, temp, 99);
			SetDlgItemText(hDlg, IDC_TEAMNAME, L"");
			SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)temp);
			Teams.push_back(temp);
			SetFocus(edit1);
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_DELETETEAM) {
			int index = SendMessage(hList, LB_GETCURSEL, 0, 0);
			char temp[100];
			SendMessageA(hList, LB_GETTEXT, index, (LPARAM)temp);
			SendMessage(hList, LB_DELETESTRING, index, 0);
			auto p = find_if(Teams.begin(), Teams.end(), [temp](char*&obj) {
				if (strcmp(obj, temp) == 0) return true;
				else return false;
			});
			Teams.erase(p);
		}
		if (LOWORD(wParam) == IDC_CREATETOUR)
		{
			TeamName = new char[100];
			GetDlgItemTextA(hDlg, IDC_TOURNAME, TeamName, 99);
			strcat(TeamName, ".txt");
			for (int i = 0; i < Teams.size(); i++) {
				Team temp(Teams[i], i);
				Tournament.push_back(temp);
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL) {
			exit(0);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ChooseTour(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	static wchar_t szFilter[] = L"Text Files (*.TXT)\0*.txt\0";
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1)
		{
			static OPENFILENAME ofn;
			wchar_t szFile[260];
			szFile[0] = '\0';
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hInstance = hInst;
			ofn.hwndOwner = hDlg;
			ofn.lpstrFilter = szFilter;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrInitialDir = L".\\";
			ofn.lpstrDefExt = L"txt";
			if (GetOpenFileName(&ofn)) {
				SetDlgItemText(hDlg, IDC_EDIT1, szFile);
				HWND temp = GetDlgItem(hDlg, IDOK);
				EnableWindow(temp, true);
			}
			else MessageBox(hDlg, L"Файл не был выбран", L"Ошибка", MB_OK);
		}
		if (LOWORD(wParam) == IDOK) {
			char FileName[260];
			GetDlgItemTextA(hDlg, IDC_EDIT1, FileName, 259);
			TeamName = new char[strlen(FileName)];
			ifstream fin(FileName);
			strcpy(TeamName, FileName);
			int nmb;
			fin >> nmb;
			for (int i = 0; i < nmb; i++) {
				char Name[50];
				char Name2[50];
				fin >> Name;
				fin.getline(Name2, 49);
				strcat(Name, Name2);
				Name2[0] = '\0';
				int Nmb;
				fin >> Nmb;
				Team temp(Name, Nmb);
				fin >> Nmb;
				temp.score = Nmb;
				fin >> Nmb;
				temp.rate = Nmb;
				fin >> Nmb;
				temp.goals_scored = Nmb;
				fin >> Nmb;
				temp.missed_goals = Nmb;
				Tournament.push_back(temp);
			}
			for (int i = 0; i < 50; i++) {
				for (int j = 0; j < 50; j++) {
					fin >> Setka[i][j];
				}
			}
			fin >> cnt;
			fin.close();
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL) {
			exit(0);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Tour(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_NEW)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, CreateNewTour);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_NOTNEW)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hDlg, ChooseTour);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL) {
			exit(0);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна "О программе".
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
