#include "MySocket.h"
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>
#include <dirent.h>
std::vector<SOCKET> client_sockets;
std::mutex broadcast_mutex;
void MySocket::http_request(SOCKET aSock)
{
	const int request_buffer_size = 65536;
	char      request[request_buffer_size];

	int bytes_recvd = recv(aSock, request, request_buffer_size - 1, 0);

	if (bytes_recvd < 0)
	{
		fprintf(stderr, "error recv\n");
		return;
	}
	request[bytes_recvd] = '\0';

	printf("request:\n%s\n", request);

	sHTTPHeader req;
	parse_http_request(request, &req);

	if (req.type == eHTTP_GET)
	{
		//send_message(aSock, "sensor 1: 10<br> sensor 2: 20<br><a href=\"http://cppprosto.blogspot.com/2017/09/blog-post_23.html\">external</a><br><a href=\"internal\">internal</a>");send_message(aSock, "sensor 1: 10<br> sensor 2: 20<br><a href=\"http://cppprosto.blogspot.com/2017/09/blog-post_23.html\">external</a><br><a href=\"internal\">internal</a>");
		std::stringstream sfile;
		sfile << req.path;
		sfile.ignore(1);
		std::ifstream fin;
		std::string file;
		sfile >> file;
		if (file == "") { // главная страница
			send_message(aSock, "sensor 1: 10<br> sensor 2: "
				"20<br><a href=\"http://edu.tu-bryansk.ru/\">"
				"BSTU</a><br><a href=\"myfolder\">myfolder</a>");
		}
		else {
			fin.open(file.c_str(), std::ios::in);
			if (fin.is_open()) { // открываем запрашиваемый файл
				std::string data;
				std::getline(fin, data);
				send_message(aSock, data.c_str());
			}
			else //если это не файл
			{
				std::string path = "C:\\Users\\enigm\\Desktop\\HTTP-Server\\Debug";
				path += "\\";
				path += file;
				DIR* dp = opendir(path.c_str()); // пробуем открыть его как папку
				if (dp != NULL) {
					dirent* ep;
					std::string folder_data;
					while ((ep = readdir(dp)) != NULL) {
						if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0) continue;
						folder_data += ep->d_name;
						folder_data += "\r\n";
					}
					send_message(aSock, folder_data.c_str());
				}
				else { // иначе это непонятно что
					send_404(aSock); // отправляем 404
				}
			}
		}
	}
	else // если неизвестен запрос
	{
		send_501(aSock); // отправляем 501
	}
}
void MySocket::parse_http_request(const char* apstrRequest, sHTTPHeader* apHeader)
{
	int  type_length = 0;
	char type[255] = { 0 };
	int  index = 0;

	apHeader->type = eHTTP_UNKNOWN;

	sscanf(&apstrRequest[index], "%s", type);
	type_length = strlen(type);

	if (type_length == 3)
	{
		if (type[0] == 'G' && type[1] == 'E' && type[2] == 'T')
			apHeader->type = eHTTP_GET;

		index += type_length + 1;
		sscanf(&apstrRequest[index], "%s", apHeader->path);
	}
}

void MySocket::send_message(SOCKET aSock, const char* apstrMessage)
{
	char buffer[65536] = { 0 };

	strcat(buffer, "HTTP/1.1 200 OK\n\n");
	strcat(buffer, "<h1>");
	strcat(buffer, apstrMessage);
	strcat(buffer, "</h1>");

	int len = strlen(buffer);
	send(aSock, buffer, len, 0);
}
void MySocket::send_404(SOCKET aSock)
{
	const char* buffer = "HTTP/1.1 404 \n\n";
	int len = strlen(buffer);
	send(aSock, buffer, len, 0);
}

void MySocket::send_501(SOCKET aSock)
{
	const char* buffer = "HTTP/1.1 501 \n\n";
	int len = strlen(buffer);
	send(aSock, buffer, len, 0);
}


void MySocket::HandleMessages(SOCKET sock, std::string& receivedmsg, std::string user_to_handle)
{
	std::string servermsg1;
	if (strcmp(receivedmsg.c_str(), "exit") == 0)
	{
		servermsg1 = "Bye...\r\n";
		send_string(sock, servermsg1.c_str(), servermsg1.size(), 0);
		printf("Client initialize disconnection.\r\n");
		return;
	}
	else if (strcmp(receivedmsg.c_str(), "shutdown") == 0)
	{
		servermsg1 = "Server go to shutdown.\r\n";
		send_string(sock, servermsg1.c_str(), servermsg1.size(), 0);
		Sleep(200);
		bTerminate = true;
	}
	else if (strcmp(receivedmsg.c_str(), "time") == 0) {
		time_t t;
		struct tm* t_m;
		t = time(NULL);
		t_m = localtime(&t);
		std::stringstream ss;
		std::string h(std::to_string(t_m->tm_hour)), m(std::to_string(t_m->tm_min)), s(std::to_string(t_m->tm_sec));
		if (std::stoi(h) < 10) h.insert(h.begin(), '0');
		if (std::stoi(m) < 10) m.insert(m.begin(), '0');
		if (std::stoi(s) < 10) s.insert(s.begin(), '0');
		ss << t_m->tm_mday << '/' << t_m->tm_mon + 1 << '/' << t_m->tm_year + 1900 << ' '
			<< h << ":" << m << ":" << s;
		servermsg1 = "Server time - ";
		servermsg1 += ss.str();
		servermsg1 += '\r\n';
		send_string(sock, servermsg1.c_str(), servermsg1.size(), 0);
	}
	else if (strcmp(receivedmsg.c_str(), "info") == 0) {
		servermsg1 = "Babaev Michael Dmitrievich.\r\n";
		send_string(sock, servermsg1.c_str(), servermsg1.size(), 0);
	}
	else if (strcmp(receivedmsg.c_str(), "task") == 0) {
		servermsg1 = "Добавить в сервис поддержку дополнительной команды запроса содержимого файла с сервера. \nВходной параметр : имя файла.\nОтвет сервера : содержимое файла.\r\n";
		send_string(sock, servermsg1.c_str(), servermsg1.size(), 0);
	}
	else if (receivedmsg.find("getfile ") != std::string::npos) { // если нашли команду getfile
		std::string filename = receivedmsg.substr(8, receivedmsg.size() - 8);
		std::ifstream fin(filename);
		servermsg1 = "Cannot open file!\r\n";
		if (fin.is_open()) {
			servermsg1 = "";
			for (std::string line; std::getline(fin, line);) {
				servermsg1 += line;
			}
			servermsg1 += "\r\n";
		}
		send_string(sock, servermsg1.c_str(), servermsg1.size(), 0);
	}
	else
	{
		//char sock_str[10];
		//// Преобразовываем строку в верхний регистр
		//servermsg1 = user_to_handle;
		//servermsg1 += ": ";
		//servermsg1 += receivedmsg.c_str();
		//servermsg1 += "\r\n";
		//broadcast_mutex.lock();
		//for (auto& socks : client_sockets) {
		//	if (socks != sock) {
		//		send_string(socks, servermsg1.c_str(), servermsg1.size(), 0);
		//	}
		//}
		//broadcast_mutex.unlock();
	}
}

int MySocket::send_string(SOCKET sock, const char* sString, int len, int flags)
{
	int total = 0;
	int n = -1;
	while (total < len)
	{
		n = send(sock, sString + total, len - total, flags);
		if (n == -1) break;
		total += n;
	}
	return (n == -1 ? -1 : total);
}

MySocket::MySocket(int family, int type, int protocol)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	if ((S = socket(family, type, protocol)) == INVALID_SOCKET)
	{
		throw std::exception("Can not create socket!");
	}
}

MySocket::~MySocket()
{
	WSACleanup();
}

std::string MySocket::getname()
{
	gethostname(sName, sizeof(sName));
	return std::string(sName);
}

void MySocket::bindsock(int family, std::string addr, int port)
{
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = family;
	// Разрешаем работу на всех доступных сетевых интерфейсах,
	// в частности на localhost
	serv_addr.sin_addr.s_addr = inet_addr(addr.c_str());
	// обратите внимание на преобразование порядка байт
	serv_addr.sin_port = htons((u_short)port);
	if (bind(S, (sockaddr*)&serv_addr, sizeof(serv_addr)) == INVALID_SOCKET)
	{
		throw std::exception("Can not bind socket!");
	}
}

void MySocket::listensock(int qsize)
{
	if (listen(S, qsize) == INVALID_SOCKET)
	{
		throw std::exception("Can not listen!");
	}
}

bool MySocket::HandleConnetctions()
{
	return !bTerminate;
}

void RunPS(MySocket& s, sockaddr_in clnt_addr, SOCKET NS)
{
	s.RunPersonalSession(clnt_addr, NS);
}

void MySocket::RunSession()
{
	sockaddr_in clnt_addr;
	int addrlen = sizeof(clnt_addr);
	memset(&clnt_addr, 0, sizeof(clnt_addr));

	// Переводим сервис в режим ожидания запроса на соединение.
	// Вызов синхронный, т.е. возвращает управление только при 
	// подключении клиента или ошибке 
	NS = accept(S, (sockaddr*)&clnt_addr, &addrlen);
	if (NS == INVALID_SOCKET)
	{
		throw std::exception("Can not accept connection");
	}
	// Получаем параметры присоединенного сокета NS и
	// информацию о клиенте

	client_sockets.push_back(NS);
	std::thread t1(RunPS, std::ref(*this), clnt_addr, NS);
	t1.detach();

	// закрываем присоединенный сокет
}

void MySocket::RunPersonalSession(sockaddr_in clnt_addr, SOCKET NS)
{
	
	int addrlen = sizeof(serv_addr);
	getsockname(NS, (sockaddr*)&serv_addr, &addrlen);
	// Функция inet_ntoa возвращает указатель на глобальный буффер, 
	// поэтому использовать ее в одном вызове printf не получится
	printf("Accepted connection on %s:%d ",
		inet_ntoa(serv_addr.sin_addr), ntohs(serv_addr.sin_port));
	printf("from client %s:%d\n",
		inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

	http_request(NS);
	closesocket(NS);
	broadcast_mutex.lock();
	client_sockets.erase(std::find(client_sockets.begin(), client_sockets.end(), NS));
	broadcast_mutex.unlock();
}
