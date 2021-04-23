// Client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#pragma comment(lib,"Ws2_32.lib")
#include <Winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "MyClientSocket.h"
#pragma warning(disable : 4996) 

// TCP-порт сервера
#define SERVICE_PORT 1500 

int main()
try
{


    MyClientSocket S(AF_INET, SOCK_STREAM, 0, "Gena");
    S.connectServerSock(AF_INET, "192.168.1.2", SERVICE_PORT);
    S.RunSession();
    //SOCKET S;
    //sockaddr_in serv_addr;
    //WSADATA     wsadata;
    //char        sName[128];
    //bool        bTerminate = false;

    //setlocale(LC_ALL, "rus");
    //// Инициализируем библиотеку сокетов
    //WSAStartup(MAKEWORD(2, 2), &wsadata);
    //if ((S = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    //{
    //    fprintf(stderr, "Can't create socket\n");
    //    exit(1);
    //}

    ////биндим серверный сокет
    //// Заполняем структуру адресов 
    //memset(&serv_addr, 0, sizeof(serv_addr));
    //serv_addr.sin_family = AF_INET;
    //// Разрешаем работу на всех доступных сетевых интерфейсах,
    //// в частности на localhost
    //serv_addr.sin_addr.s_addr = inet_addr("192.168.1.2");
    //// обратите внимание на преобразование порядка байт
    //serv_addr.sin_port = htons((u_short)SERVICE_PORT);

    //// соединение нашего сокета с serv addr
    //if (connect(S, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    //{
    //    std::cout << "Cannot connect to server\n";
    //    system("pause");
    //    exit(1);
    //}
    //
    ///////////////////////////////SESSION////////////////////////////
    //char    sReceiveBuffer[1024] = { 0 };

    //if (recv(S, sReceiveBuffer, sizeof(sReceiveBuffer) - 1, 0) == -1) {
    //    std::cout << "Error: cannot receive message" << std::endl;
    //    system("pause");
    //    exit(1);
    //}
    //std::cout << sReceiveBuffer;
    //while (true) {
    //    std::string hellomsg;
    //    std::cout << '>'; getline(std::cin, hellomsg, '\n');
    //    if (hellomsg == "exit")
    //        break;
    //    if (send(S, hellomsg.c_str(), hellomsg.size(), 0) == -1) {
    //        std::cout << "Error: cannot send message" << std::endl;
    //        system("pause");
    //        break;
    //    }
    //    int n = recv(S, sReceiveBuffer, sizeof(sReceiveBuffer) - 1, 0);
    //    if (n == -1) {
    //        std::cout << "Error: cannot receive message" << std::endl;
    //        system("pause");
    //        break;
    //    }
    //    sReceiveBuffer[n] = '\0';
    //    std::cout << sReceiveBuffer;
    //}
    //closesocket(S);
    ///////////////////////////////////////////////////////////
    return 0;
}
catch (std::exception& ex) {
    std::cout << ex.what() << std::endl;
    system("pause");
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
