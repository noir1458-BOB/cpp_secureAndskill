#include "pch.h"
#include "StringFunc.h"

void PrintString(LPCTSTR pszContext) {
	// tprintf(TEXT("%s\n"), pszContext);
	printf("%s\n", core::MBSFromTCS(pszContext).c_str());
}
// tprintf가 wprintf로 바뀌면 리눅스에서는 안되는..
// wprintf 사용 볼려는게 아니라 듀얼컴파일이 목적이므로 이렇게 하자