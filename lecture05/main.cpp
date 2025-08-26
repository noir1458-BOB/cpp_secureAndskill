#include "pch.h"
#include <windows.h> // �߰�


void TraverseFileSystem(std::tstring strTargetDir, std::tstring strPattern)
{
	std::tstring strTargetPattern = std::wstring(strTargetDir.begin(), strTargetDir.end()) + TEXT("/") + std::wstring(strPattern.begin(), strPattern.end());
	WIN32_FIND_DATA findData;
	HANDLE hFile = FindFirstFile(strTargetPattern.c_str(), &findData);
	if (hFile == INVALID_HANDLE_VALUE) return;

	do {
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// ���͸� ó�� (���)
			if (wcscmp(findData.cFileName, L".") == 0) continue;
			if (wcscmp(findData.cFileName, L"..") == 0) continue;
			TraverseFileSystem(strTargetDir + TEXT("/") + findData.cFileName, strPattern);
		} else {
			// ���� ó��
			std::tstring filePath = strTargetDir + TEXT("/") + findData.cFileName;
			tprintf(TEXT("%s : "), filePath.c_str());

			std::vector<BYTE> outContents{};
			std::string strFile = std::string(strTargetDir.begin(), strTargetDir.end()) + "/" + ANSIFromWCS(findData.cFileName);
			ReadFileContents(strFile, outContents);

			if (outContents.size() < 4)
			{
				for (int i = 0; i < 4; i++)
					outContents.push_back(0);
			}
			printf("%02X %02X %02X %02X\n", outContents[0], outContents[1], outContents[2], outContents[3]);
		}
	} while (FindNextFile(hFile, &findData));
	core::FindClose(hFile);
}


int main(int argv, char* argc[])
{
	if (argv < 3)
	{
		// ���� ��ο� ���� �˻����� �Է��ϸ� �ش��ϴ� ���ϵ��� ù 4����Ʈ�� ����ϱ�
		// �������� ���ϸ� : ù 4����Ʈ �� ����� ���;� �Ѵ�.
		printf("Usage: %s <input>\n", argc[0]);
		return 1;
	}
	// ���� ��η� ����
	// traverse �ϸ鼭 �˻��Ŀ� �ش��ϴ� ������ 4����Ʈ ���

	std::tstring TargetDir = TCSFromUTF8(MakeFormalPath(argc[1]));
	std::tstring strPattern = TCSFromUTF8(argc[2]);
	TraverseFileSystem(TargetDir, strPattern);

	return 0;
}