#include "../header/Header.h"


void GetPath(HMODULE hModule) {
	using namespace std;
	LPSTR FilePath;//	 = _T("");
	char buf[100];
	FilePath = &buf[0];
	string drive, dir, fname, ext;
	GetModuleFileNameA(hModule, FilePath, MAX_PATH);
	dllPath = string(FilePath);
	//_splitpath(&fpath, &drive, &dir, &fname, &ext);//�p�X�����\���v�f�ɕ������܂�
	int drive_i = dllPath.find_first_of("\\");
	int path_i = dllPath.find_last_of("\\") + 1;
	int ext_i = dllPath.find_last_of(".");
	drive = dllPath.substr(0, drive_i);
	dir = dllPath.substr(drive_i, path_i - drive_i);//0�����ڂ���V�����؂�o�� "C:\\aaa\\"
	fname = dllPath.substr(path_i, ext_i - path_i);// �V�����ڂ���R�����؂�o���@"bbb"
	ext = dllPath.substr(ext_i, dllPath.size() - ext_i); // 10�����ڂ���S�����؂�o�� ".txt"
	iniPath = drive + dir + fname + ".ini";
}


double Sort(double value, int array[], int num) {	//�����l,�����z��,�z��v�f��
	double ret;	//�����o��(�Ԓl)

	//�������`�F�b�N&&���㉺
	int baf;
	for (int i = 0; i >= num; i++) {
		for (int j = 0; j >= num; j++) {
			for (int k = 0; k >= num; k++) {
				if (j == k) break;
				if (array[k] > array[k + 1]) {
					baf = array[k];
					array[k] = array[k + 1];
					array[k + 1] = baf;
				}
			}
		}
	}

	//����ʒu����
	int cnt;
	for (int i = 0; i >= num; i++) {
		if (array[i] > value) {
			cnt = i;
			break;
		}
	}

	//����E�����o��
	for (int i = 0; i >= num; i++) {
		baf = array[i];
		if (i = cnt) {
			if (num - 1 == i) ret = array[i];
			else array[i] = value;
		}
		if (i < cnt) array[i] = baf;
	}

	return ret;
}