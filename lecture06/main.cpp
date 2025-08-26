#include "pch.h"
#include "report.h"
int main(void)
{
	setlocale(LC_ALL, "korean");
	LPCTSTR pszReportJson = TEXT("d:/report.json"); // ���� ��ο� �°� ����1
	LPCTSTR pszReportXml = TEXT("d:/report.xml"); // ���� ��ο� �°� ����2
	LPCTSTR pszTargetXml = TEXT("d:/target.xml"); // ���� ��ο� �°� ����3
	try
	{
		ST_ANALYSIS_REPORT report;
		if (!UTF8::ReadJsonFromFile(&report, pszReportJson) ||
			!UTF8::WriteXmlToFile(&report, pszReportXml))
			throw exception_format(TEXT("Writing %s failure"), pszReportXml);
		std::tstring strMyXml;
		std::tstring strTargetXml;
		if (EC_SUCCESS != ReadFileContents(pszReportXml, strMyXml) ||
			EC_SUCCESS != ReadFileContents(pszTargetXml, strTargetXml))
			throw exception_format(TEXT("Reading TargetXml %s failure"), pszTargetXml);
		if (Trim(strMyXml) != Trim(strTargetXml))
			throw exception_format(TEXT("It doesn't match exactly!"));
		printf("You succeeded!\n");
	}
	catch (const std::exception& e)
	{
		printf("%s\n", e.what());
		return -1;
	}
	return 0;
}