# TCP를 이용한 멀티스레드 채팅 서버

## 1. server

> 1.  Winsock 초기화
> 
> ```cpp
> 	WSADATA wsa;
> 	WSAStartup(MAKEWORD(2, 2), &wsa);
> ```
> 
> 2. Listen 초켓 초기화
> 
> ```cpp
> SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
> 
> 	SOCKADDR_IN addr = { 0 };
> 	addr.sin_addr.s_addr = htonl(INADDR_ANY);
> 	addr.sin_port = PORT;
> 	addr.sin_family = AF_INET;
> 
> 	bind(server, (SOCKADDR*)&addr, sizeof addr);
> 	listen(server, SOMAXCONN);
> ```
> 
> 3. Accept 스레드 생성과 Recv 스레드 생성
> 
> ```cpp
> void ACCEPT(SOCKET& s) { //클라이언트들의 접속을 받는 함수
> 	int cnt = 0;
> 	while (TRUE) {
> 		Client.push_back(pii(CLIENT(), ""));
> 		Client[cnt].first.client = accept(s, (SOCKADDR*)&Client[cnt].first.clientaddr, &Client[cnt].first.clientsize);
> 		Client[cnt].first.number = cnt;
> 		std::thread(recvData, Client[cnt].first.client, cnt).detach();
> 		cnt += 1;
> 	}
> }
> ```
> 
> > 단일 스레드로 Client를 추가 하고 있으므로 굳이 동기화 과정이 필요 없다. 하지만, 이후 제거 하거나 참조하면 문제가 생길 수 있다.
> 
> 4. 받은 메세지 출력
> 
> ```cpp
> 	while (TRUE) {
> 		ZeroMemory(buffer, sizeof buffer);
> 		recv(s, buffer, sizeof buffer, 0); //메세지를 받는부분
> 		std::cout << Client[num].second << " : " << buffer << std::endl;
> 	}
> 	
> ```
> 
> 5. 접속한 클라이언트에 메세지 보내기
> 
> ```cpp
> 	while (TRUE) {
> 		ZeroMemory(name, sizeof name);
> 		ZeroMemory(message, sizeof message);
> 		std::cin >> name >> message;
> 		for (int i = 0; i < Client.size(); i++)
> 			if (!strcmp(Client[i].second.c_str(), name))
> 				send(Client[i].first.client, message, sizeof message, 0);
> 	}
> ```
> 
> > 클라이언트 리스트에서 이름이 일치하는지 선형 탐색 후 일치하면 그 클라이언트에 메세지 전송

## 2. client



> 1. Winsock 초기화
> 
> 2. connect를 이용해 서버와 연결
> 
> ```cpp
> 	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
> 
> 	SOCKADDR_IN addr = { 0 };
> 	addr.sin_addr.s_addr = inet_addr(IP);
> 	addr.sin_port = PORT;
> 	addr.sin_family = AF_INET;
> 
> 	while (connect(server, (SOCKADDR*)&addr, sizeof addr));
> ```
> 
> > `std::thread(recvData, std::ref(server))` 에서 Recv용 스레드 생성
> > 
> > 보내려는 메세지를 입력 받는 스레드가 메인스레드입니다.
> 
> 3. 보낼 메세지 입력
> 
> ```cpp
> 	while (TRUE) {
> 		ZeroMemory(name, sizeof name);
> 		std::cout << "메세지를 입력하세요: ";
> 		std::cin >> name;
> 		send(server, name, sizeof name, 0);
> 	}
> ```
> 
> 
