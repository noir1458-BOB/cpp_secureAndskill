#include "pch.h"
#include "StringFunc.h"

void PrintString(LPCTSTR pszContext) {
	// tprintf(TEXT("%s\n"), pszContext);
	printf("%s\n", core::MBSFromTCS(pszContext).c_str());
}
// tprintf가 wprintf로 바뀌면 리눅스에서는 안되는..
