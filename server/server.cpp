/*
	Простой TCP-сервер
	Поучает текстовую строку, преобразует ее в верхний регистр и
	возвращает клиенту результат.

	Файл: server.cpp

	Пример компиляции:
	bcc32 -oserver.exe server.cpp Ws2_32.lib
*/
#include "MySocket.h"
#include <thread>
// TCP-порт сервера
//WEB DAV

void socket_work(std::string addr, int16_t port) {
	MySocket sock(AF_INET, SOCK_STREAM, 0);
	sock.bindsock(AF_INET, addr.c_str(), port);
	sock.listensock(5);
	while (sock.HandleConnetctions()) {
		sock.RunSession();
	}
}
int main(void)
{
	setlocale(LC_ALL, "rus");
	socket_work("127.0.0.1", 8000);
	return 0;
}
