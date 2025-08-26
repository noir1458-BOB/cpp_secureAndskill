#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#include "json_util.h"
#include "network_util.h"

#include <iostream>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

int main(void) {
  printf("[DEBUG] WSAStartup 시작\n");
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    printf("[ERROR] WSAStartup failed: %d\n", iResult);
    return 1;
  }

  printf("[DEBUG] 소켓 생성\n");
  SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (clientSock == INVALID_SOCKET) {
    printf("[ERROR] socket failed with error: %ld\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }

  sockaddr_in remote;
  remote.sin_family = AF_INET;
  remote.sin_addr.s_addr = inet_addr(g_szServerIP.c_str());
  remote.sin_port = htons(g_wPort);

  printf("[DEBUG] 서버 연결 시도: %s:%d\n", g_szServerIP.c_str(), g_wPort);
  iResult = connect(clientSock, (sockaddr *)&remote, sizeof(remote));
  if (iResult == SOCKET_ERROR) {
    printf("[ERROR] connect failed with error: %ld\n", WSAGetLastError());
    closesocket(clientSock);
    WSACleanup();
    return 1;
  }

  printf("[DEBUG] 서버 연결 성공\n");

  try {
    // JSON body 생성
    std::string body = create_resume_json();
    printf("[DEBUG] JSON body: %s\n", body.c_str());

    // 패킷 구성: [header(4)][length(4)][body]
    uint32_t header = 1001;
    uint32_t length = (uint32_t)body.size();

    std::string packet;
    packet.resize(8 + body.size());
    write_le_uint32(reinterpret_cast<uint8_t *>(&packet[0]), header);
    write_le_uint32(reinterpret_cast<uint8_t *>(&packet[4]), length);
    memcpy(&packet[8], body.data(), body.size());

    printf("[DEBUG] 패킷 전송: %zu bytes\n", packet.size());
    iResult = send(clientSock, packet.data(), (int)packet.size(), 0);
    if (iResult == SOCKET_ERROR) {
      printf("[ERROR] send failed with error: %d\n", WSAGetLastError());
      closesocket(clientSock);
      WSACleanup();
      return 1;
    }
    printf("[DEBUG] Client sent %d bytes\n", iResult);
  } catch (const std::exception& e) {
    printf("[ERROR] JSON error: %s\n", e.what());
    closesocket(clientSock);
    WSACleanup();
    return 1;
  }

  // 응답 수신
  char szBuffer[1501];
  int nReadSize = recv(clientSock, szBuffer, 1500, 0);
  if (nReadSize > 0) {
    szBuffer[nReadSize] = 0;
    printf("[DEBUG] Client received: %s\n", szBuffer);
  } else if (nReadSize == 0) {
    printf("[DEBUG] Connection closed\n");
  } else {
    printf("[ERROR] recv failed with error: %d\n", WSAGetLastError());
  }

  closesocket(clientSock);
  WSACleanup();

  printf("[DEBUG] 프로그램 종료\n");
  return 0;
}
