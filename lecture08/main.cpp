#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

const WORD g_wPort = 42345;

#pragma pack(push, 1) 
struct PacketHeader {
	int packet_id;
	int length;
};
#pragma pack(pop)

// 함수 선언
std::string ConvertToUtf8(const std::string& mbcsStr);
void ReceiveResponse(SOCKET sock);

int main(void) {
	// 콘솔의 출력 코드 페이지를 UTF-8로 설정
	SetConsoleOutputCP(CP_UTF8);

	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSock == INVALID_SOCKET) { WSACleanup(); return 1; }

	sockaddr_in remote;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(g_wPort);
	inet_pton(AF_INET, "192.168.2.243", &remote.sin_addr);

	if (connect(clientSock, (sockaddr*)&remote, sizeof(remote)) == SOCKET_ERROR) {
		printf("connect failed with error %d\n", WSAGetLastError());
		closesocket(clientSock);
		WSACleanup();
		return 1;
	}

	printf("Successfully connected to server.\n\n");

	// --- 1. Resume 패킷 전송 ---
	printf("--- Sending Packet 1001 (Resume) ---\n");
	std::string resumeJsonRaw = R"({"Resume":{"Name":"서정민","Greeting":"쓰고 싶은 인사말","Introduction":"자기 소개 간단히~"}})";
	std::string resumeJsonData = ConvertToUtf8(resumeJsonRaw);
	PacketHeader resumeHeader = { 1001, (int)resumeJsonData.length() }; // 경고 C4267 수정
	send(clientSock, (const char*)&resumeHeader, sizeof(PacketHeader), 0);
	send(clientSock, resumeJsonData.c_str(), resumeJsonData.length(), 0);
	printf("Packet 1001 sent.\n");
	ReceiveResponse(clientSock);

	printf("\n----------------------------------------\n\n");

	// --- 2. QuestionCode 패킷 전송 및 문제 수신 ---
	printf("--- Sending Packet 1003 (QuestionCode) ---\n");
	std::string questionJsonData = R"({"QuestionCode":"BOB13th"})";
	PacketHeader questionHeader = { 1003, (int)questionJsonData.length() }; // 경고 C4267 수정
	send(clientSock, (const char*)&questionHeader, sizeof(PacketHeader), 0);
	send(clientSock, questionJsonData.c_str(), questionJsonData.length(), 0);
	printf("Packet 1003 sent.\n");
	printf("Receiving questions from the server...\n");
	ReceiveResponse(clientSock);

	printf("\n----------------------------------------\n");
	printf("--- Starting the Quiz ---\n");
	printf("Please enter the full text of the answer for each question.\n");
	printf("----------------------------------------\n\n");

	// cin 사용 전 입력 버퍼를 비워줍니다.
	std::cin.ignore(INT_MAX, '\n');

	// --- 3. 10개 문제에 대한 답변을 사용자로부터 입력받아 전송 ---
	for (int i = 1; i <= 10; ++i) {
		printf("Q%d. Answer: ", i);

		std::string answer;
		std::getline(std::cin, answer);
		std::string utf8Answer = ConvertToUtf8(answer);

		std::string answerJson = "{\"QuestionIdx\":" + std::to_string(i) + ",\"Answer\":\"" + utf8Answer + "\"}";
		std::cout << answerJson << std::endl;
		PacketHeader answerHeader = { 1005, (int)answerJson.length() }; // 경고 C4267 수정
		send(clientSock, (const char*)&answerHeader, sizeof(PacketHeader), 0);
		send(clientSock, answerJson.c_str(), answerJson.length(), 0);
		printf("Sent answer for Q%d.\n", i);

		ReceiveResponse(clientSock);
		printf("\n");
	}

	printf("Quiz finished. Closing connection.\n");

	closesocket(clientSock);
	WSACleanup();

	return 0;
}


// --- Helper Functions ---

std::string ConvertToUtf8(const std::string& mbcsStr) {
	if (mbcsStr.empty()) return std::string();
	int wideLen = MultiByteToWideChar(CP_ACP, 0, mbcsStr.c_str(), -1, NULL, 0);
	std::vector<wchar_t> wideStr(wideLen);
	MultiByteToWideChar(CP_ACP, 0, mbcsStr.c_str(), -1, &wideStr[0], wideLen);

	// 오류 C3861 수정: WideByteToMultiByte -> WideCharToMultiByte
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, &wideStr[0], -1, NULL, 0, NULL, NULL);
	std::vector<char> utf8Str(utf8Len);
	WideCharToMultiByte(CP_UTF8, 0, &wideStr[0], -1, &utf8Str[0], utf8Len, NULL, NULL);
	return std::string(utf8Str.begin(), utf8Str.end() - 1);
}

void ReceiveResponse(SOCKET sock) {
	PacketHeader responseHeader;
	int bytesReceived = recv(sock, (char*)&responseHeader, sizeof(PacketHeader), 0);

	if (bytesReceived > 0 && bytesReceived == sizeof(PacketHeader)) {
		int responseDataLength = responseHeader.length;
		printf("Response header received -> ID: %d, Data Length: %d\n", responseHeader.packet_id, responseDataLength);

		if (responseDataLength > 0) {
			std::vector<char> buffer(responseDataLength);
			// TCP는 스트림 프로토콜이므로 데이터가 나뉘어 올 수 있어 루프를 통해 모두 받습니다.
			int totalReceived = 0;
			while (totalReceived < responseDataLength) {
				bytesReceived = recv(sock, &buffer[totalReceived], responseDataLength - totalReceived, 0);
				if (bytesReceived <= 0) break;
				totalReceived += bytesReceived;
			}

			if (totalReceived == responseDataLength) {
				std::string responseData(buffer.begin(), buffer.end());
				printf("Response data: %s\n", responseData.c_str());
			}
			else {
				printf("Failed to receive complete response data.\n");
			}
		}
		else {
			printf("Response has no data.\n");
		}
	}
	else if (bytesReceived == 0) {
		printf("Connection closed by server.\n");
	}
	else {
		printf("recv for response header failed with error %d\n", WSAGetLastError());
	}
}