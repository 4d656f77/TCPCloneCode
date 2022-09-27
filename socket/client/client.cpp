#include "Client.h"

void recvData(SOCKET& s) {
	char buffer[PACKET_SIZE];
	while (TRUE) {
		ZeroMemory(buffer, sizeof buffer);
		recv(s, buffer, sizeof buffer, 0);
		std::cout << "Server : " << buffer << std::endl;
	}
}

int main() {
	char IP[PACKET_SIZE] = { 0 },
		name[PACKET_SIZE] = { 0 };

	std::cout << "이름을 입력하세요 : ";
	std::cin >> name;
	std::cout << "아이피를 입력하세요 : ";
	std::cin >> IP;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = { 0 };
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = PORT;
	addr.sin_family = AF_INET;

	while (connect(server, (SOCKADDR*)&addr, sizeof addr));
	send(server, name, sizeof name, 0); //이름보내기

	std::thread(recvData, std::ref(server)).detach();

	while (TRUE) {
		ZeroMemory(name, sizeof name);
		std::cout << "메세지를 입력하세요: ";
		std::cin >> name;
		send(server, name, sizeof name, 0);
	}
}