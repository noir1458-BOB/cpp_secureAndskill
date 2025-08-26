#include "pch.h"
#include <windows.h> // 추가


void TraverseFileSystem(std::tstring strTargetDir, std::tstring strPattern)
{
	std::tstring strTargetPattern = std::wstring(strTargetDir.begin(), strTargetDir.end()) + TEXT("/") + std::wstring(strPattern.begin(), strPattern.end());
	WIN32_FIND_DATA findData;
	HANDLE hFile = FindFirstFile(strTargetPattern.c_str(), &findData);
	if (hFile == INVALID_HANDLE_VALUE) return;

	do {
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// 디렉터리 처리 (재귀)
			if (wcscmp(findData.cFileName, L".") == 0) continue;
			if (wcscmp(findData.cFileName, L"..") == 0) continue;
			TraverseFileSystem(strTargetDir + TEXT("/") + findData.cFileName, strPattern);
		} else {
			// 파일 처리
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
		// 실행 경로와 파일 검색식을 입력하면 해당하는 파일들의 첫 4바이트를 출력하기
		// 실행결과가 파일명 : 첫 4바이트 의 목록이 나와야 한다.
		printf("Usage: %s <input>\n", argc[0]);
		return 1;
	}
	// 실행 경로로 가서
	// traverse 하면서 검색식에 해당하는 파일의 4바이트 출력

	std::tstring TargetDir = TCSFromUTF8(MakeFormalPath(argc[1]));
	std::tstring strPattern = TCSFromUTF8(argc[2]);
	TraverseFileSystem(TargetDir, strPattern);

	return 0;
}