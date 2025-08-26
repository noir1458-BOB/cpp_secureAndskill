#pragma once
struct ST_SUMMARY : public core::IFormatterObject
{
	std::tstring strResult;
	std::tstring strDetectName;
	std::tstring strEngineName;
	std::tstring strEngineVersion;
	std::tstring strSeverity;
	std::tstring strSampleName;
	std::tstring strSampleExt;
	std::tstring strTotalElapsedTime;
	std::tstring strReachedFileLoopLimit;
	std::tstring strReachedUrlLoopLimit;
	//std::vector<std::pair<std::tstring, std::tstring>> summary_v;

	void OnSync(core::IFormatter& formatter)
	{	
		formatter.Sync(TEXT("Result"), strResult);
		formatter.Sync(TEXT("DetectName"), strDetectName);
		formatter.Sync(TEXT("EngineName"), strEngineName);
		formatter.Sync(TEXT("EngineVersion"), strEngineVersion);
		formatter.Sync(TEXT("Severity"), strSeverity);
		formatter.Sync(TEXT("SampleName"), strSampleName);
		formatter.Sync(TEXT("SampleExt"), strSampleExt);
		formatter.Sync(TEXT("TotalElapsedTime"), strTotalElapsedTime);
		formatter.Sync(TEXT("ReachedFileLoopLimit"), strReachedFileLoopLimit);
		formatter.Sync(TEXT("ReachedUrlLoopLimit"), strReachedUrlLoopLimit);
	}
};
struct ST_FILEDATA : public core::IFormatterObject
{
	std::tstring strTargetID;
	std::tstring strParentID;
	std::tstring strAbsolutePath;
	std::tstring strFileName;
	std::tstring strFileSize;
	std::tstring strFileEXT;
	std::tstring strMD5;
	std::tstring strSHA1;
	std::tstring strSHA256;
	std::tstring strHAS160;
	std::tstring strFileHeaderDump;

	void OnSync(core::IFormatter& formatter)
	{
		formatter.Sync(TEXT("TargetID"), strTargetID);
		formatter.Sync(TEXT("ParentID"), strParentID);
		formatter.Sync(TEXT("AbsolutePath"), strAbsolutePath);
		formatter.Sync(TEXT("FileName"), strFileName);
		formatter.Sync(TEXT("FileSize"), strFileSize);
		formatter.Sync(TEXT("FileEXT"), strFileEXT);
		formatter.Sync(TEXT("MD5"), strMD5);
		formatter.Sync(TEXT("SHA1"), strSHA1);
		formatter.Sync(TEXT("SHA256"), strSHA256);
		formatter.Sync(TEXT("HAS160"), strHAS160);
		formatter.Sync(TEXT("FileHeaderDump"), strFileHeaderDump);
	}
};
struct ST_FILETARGET : public core::IFormatterObject
{
	ST_FILEDATA invoice_doc_16880;
	ST_FILEDATA skeml_lnk_258;
	std::map<std::tstring, ST_FILEDATA> m;
	
	void OnSync(core::IFormatter& formatter)
	{
		m.insert({ TEXT("invoice.doc_16880"), invoice_doc_16880 });
		m.insert({ TEXT("skeml.lnk_258"), skeml_lnk_258 });
		for (auto& e : m) {
			formatter.Sync(e.first, e.second);
		}
	}
};
struct ST_KOKR : public core::IFormatterObject
{
	std::tstring strkoKR;
	void OnSync(core::IFormatter& formatter)
	{
		/*formatter
			+ core::sPair(TEXT("Summary"), Summary)
			;*/
		if (!strkoKR.empty()) // 또는 if (strkoKR.length() > 0)
			formatter.Sync(TEXT("ko-KR"), strkoKR);

	}
};
struct ST_DESCINTERNATIONAL : public core::IFormatterObject
{	
	std::vector<std::tstring> strkoKR;
	//std::map<std::tstring, std::tstring> national_m;

	void OnSync(core::IFormatter& formatter)
	{

		formatter.Sync(TEXT("ko-KR"), strkoKR);

		//// 디버그: national_m 내용 확인
		//for (const auto& e : national_m) {
		//	// 로그 출력으로 실제 데이터 확인
		//	printf("Key: [%ls], Value: [%ls], Empty: %s\n",
		//		e.first.c_str(), e.second.c_str(),
		//		e.second.empty() ? "true" : "false");
		//}

		//std::map<std::tstring, std::tstring> filtered;
		//for (const auto& e : national_m) {
		//	if (!e.second.empty())
		//		filtered[e.first] = e.second;
		//}
		//if (!filtered.empty())
		//	formatter.Sync(TEXT("National"), filtered);
	}
};
struct ST_EVENT : public core::IFormatterObject
{
	std::tstring strEngineName;
	std::tstring strEngineVersion;
	std::tstring strTargetID;
	std::tstring strName;
	std::tstring strAnalysisCode;
	std::tstring strSeverity;
	std::tstring strDesc;
	ST_DESCINTERNATIONAL DescInternational;

	void OnSync(core::IFormatter& formatter)
	{
		/*formatter
			+ core::sPair(TEXT("Summary"), Summary)
			;*/
		formatter.Sync(TEXT("EngineName"), strEngineName);
		formatter.Sync(TEXT("EngineVersion"), strEngineVersion);
		formatter.Sync(TEXT("TargetID"), strTargetID);
		formatter.Sync(TEXT("Name"), strName);
		formatter.Sync(TEXT("AnalysisCode"), strAnalysisCode);
		formatter.Sync(TEXT("Severity"), strSeverity);
		formatter.Sync(TEXT("Desc"), strDesc);
		/*for(auto& e : DescInternational.national_m) {
			formatter.Sync(e.first, e.second);
		}*/
		formatter.Sync(TEXT("DescInternational"), DescInternational);
	}
};

struct ST_DETECTION : public core::IFormatterObject
{
	std::vector<ST_EVENT> Events;

	void OnSync(core::IFormatter& formatter)
	{
		formatter.Sync(TEXT("Event"), Events);
	}
};
struct ST_ANALYSIS_REPORT : public core::IFormatterObject
{
	ST_SUMMARY Summary;
	ST_FILETARGET FileTarget;
	ST_DETECTION Detection;

	void OnSync(core::IFormatter& formatter)
	{
		/*formatter
			+ core::sPair(TEXT("Summary"), Summary)
			;*/
		formatter.Sync(TEXT("Summary"), Summary);
		formatter.Sync(TEXT("FileTarget"), FileTarget);
		formatter.Sync(TEXT("Detection"), Detection);
	}
};
