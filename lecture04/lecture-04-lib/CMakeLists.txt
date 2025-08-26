--- Sending Packet 1003 (QuestionCode) ---
Packet 1003 sent.
Response header received -> ID: 1004, Data Length: 4911
Response data: {
        "Questions":[
                {
                        "Question":"다음 리눅스의 경로 중 가리키는 것이 다른 한 가지는?",
                        "Selection":[
                                "\/some\\\\path",
                                "\/some\/path",
                                "\/some\\\/path",
                                "\/\/\/some\/\/\/\/\/path",
                                "\/some\\path"
                        ]
                },
                {
                        "Question":"long 변수형에 대해서 올바르게 설명한 것은?",
                        "Selection":[
                                "8바이트짜리 변수이다.",
                                "리눅스와 윈도우에서 동일한 크기를 제공한다.",
                                "가급적 사용하지 않는 것이 좋다.",
                                "4바이트짜리 변수이다.",
                                "2바이트짜리 변수이다."
                        ]
                },
                {
                        "Question":"멀티바이트 및 유니코드 문자열에 대한 설명으로 틀린 것은?",
                        "Selection":[
                                "char형과 wchar_t형이 있다.",
                                "UTF16",
                                "char형은 유니코드 문자열이 아니다.",
                                "wchar_t형은 유니코드 문자열을 담는다.",
                                "UTF8"
                        ]
                },
                {
                        "Question":"소스코드 관리 기술에 관한 설명 중 틀린 것은?",
                        "Selection":[
                                "대표헤더 파일에서 공통헤더 파일을 include 한다.",
                                "함수의 길이는 모니터에 한번에 나올 정도가 딱 좋다.",
                                "대표헤더 파일은 라이브러리에서 제공하는 헤더파일을 대신 include하는 파일이며 이름은 {모듈명}.h라고 정한다.",
                                "공통헤더 파일은 모든 cpp파일이 참조하는 파일이며 이름은 대체로 pch.h라고 정한다.",
                                "main 함수가 포함된 cpp파일명을 main으로 지정해두면 제3개발자가 코드를 파악하기에 유리하다."
                        ]
                },
                {
                        "Question":"서로 다른 두 파일에 정의된 클래스가 서로를 include로 참조하는 것을 순환 참조의 오류라고 부른다. 이에 대한 설명과 거리가 먼 설명은?",
                        "Selection":[
                                "두 클래스를 하나의 클래스로 합쳐서 해결한다.",
                                "이 오류는 링커에서 발생한다.",
                                "두 클래스의 공통점을 분리하여 세 개의 클래스로 만들어 해결한다.",
                                "include가 아닌 클래스 정의만으로 컴파일러 오류를 해결할 수 있다.",
                                "두 파일 중 한 군데의 include를 제거하여 순환 문제를 일단 제거해야 한다."
                        ]
                },
                {
                        "Question":"다음은 윈도우의 파일 접근 API의 CreateFile의 인자에 대한 설명 중 틀린 것은?",
                        "Selection":[
                                "SharedMode는 내가 연 파일에 대해서 다른 프로세스의 읽기\/쓰기\/삭제 권한을 결정할 수 있다.",
                                "FILE_APPEND_DATA가 있다.",
                                "OPEN_ALWAYS는 지정한 경로에 파일이 없을 경우에 실패한다.",
                                "CREATE_NEW 옵션은 지정한 경로에 파일이 존재하지 않을 경우에만 동작한다.",
                                "DesiredAccess는 GENERIC_READ와 GENERIC_WRITE"
                        ]
                },
                {
                        "Question":"메모리 정복하기 33페이지 코드의 실행결과는 무엇인가?",
                        "Selection":[
                                "ABCDE",
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                                "ABCDEABCDEABCDEABCDEABCDEA",
                                "ZVWXY",
                                "ZVWXYZVWXYZVWXYZVWXYZVWXYZ"
                        ]
                },
                {
                        "Question":"다음 STL 자료 구조에 대한 설명 중 틀린 것은?",
                        "Selection":[
                                "stack",
                                "map은 key-value 쌍들로 이루어져 있으며 begin에는 가장 작은 키의 원소를 end에는 가장 큰 키의 원소를 조회할 수있다.",
                                "list와 deque",
                                "queue는 링크드 리스트 계열이다.",
                                "vector는 배열과 동작이 매우 유사하도록 인터페이스가 고안되었다."
                        ]
                },
                {
                        "Question":"다음은 포매터를 구현하기 위해 정의한 개념이다. 내용이 틀린 것은?",
                        "Selection":[
                                "Channel은 저장소를 가리키는 개념이며 Read나 Write를 수행하는 Access 메소드가 있다.",
                                "FormatterObject는 내가 정의한 구조체에서 각 변수의 이름과 레퍼런스(포인터)를 전달하기 위한 인터페이스이다.",
                                "Serializer는 Formatter의 하위 개념으로 parser와 동일한 의미를 가진다.",
                                "Formatter는 데이터를 저장하는 형식을 말한다.",
                                "Deserializer는 Formatter의 하위 개념으로 하나의 덩어리로 묶인 데이터를 다시 개별 값으로 분리하는 장치를 말한다."
                        ]
                },
                {
                        "Question":"다음 TCP와 UDP 소켓에 대한 설명 중 틀린 것은?",
                        "Selection":[
                                "TCP 소켓에서 recv할 때 0이 반환되는 경우에는 소켓을 닫아줘야 한다.",
                                "UDP 통신의 홀펀칭 기술은 공인 아이피와 포트를 가진 제3서버에 의해 구현될 수 있다.",
                                "TCP 서버는 Listen 소켓과 Connection 소켓으로 구성된다.",
                                "UDP 소켓에서 recv할 때 0이 반환되는 경우가 존재한다.(상대방이 0바이트를 send한 경우는 제외)",
                                "UDP는 recv할 버퍼의 크기가 최소 1500바이트여야 한다."
                        ]
                }
        ]
}
