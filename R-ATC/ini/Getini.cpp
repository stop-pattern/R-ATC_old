#include "Getini.h"

#include <Windows.h>
#include <string>
using namespace std;

void ImportConfig::GetIni(string p) {
	LPCTSTR path = p.c_str();

	for (int i = 0; i >= PATTERN_BRAKE; i++) {
		int def = 100 / PATTERN_BRAKE * i;
		brake_div[i] = GetPrivateProfileInt("brake_div", LPCTSTR(i), def, path);
	}

	handle_div = GetPrivateProfileInt("handle_div", "value", 10, path);
	brake_div[0] = 1;
	brake_div[1] = 2;
	brake_div[2] = 3;
	brake_div[3] = 4;
	brake_div[4] = 5;
}

/*
UINT GetPrivateProfileInt(
						LPCTSTR lpAppName,
						LPCTSTR lpKeyName,
						INT nDefault,
						LPCTSTR lpFileName
						);

DWORD GetPrivateProfileString(
							LPCTSTR lpAppName,	//セクション名
							LPCTSTR lpKeyName,	//キー名
							LPCTSTR nDefault,	//デフォルト値
							LPTSTR lpReturnedString,	//返り値格納先ポインタ
							DWORD nSize	//lpReturnedStringのバッファサイズ
							LPCTSTR lpFileName	//ファイル名
							);

DWORD GetPrivateProfileSection(
							LPTSTR lpszReturnedString,
							DWORD nSize,
							LPCTSTR lpFileName
							);

DWORD GetPrivateProfileSection(
							LPCTSTR lpAppName,
							LPTSTR lpReturnedString,
							DWORD nSize,
							LPCTSTR lpFileName
							);


*/
