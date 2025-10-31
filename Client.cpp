#include <iostream> 
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <WinSock2.h>
#include <Windows.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

#define SRV_HOST "localhost"
#define SRV_PORT 1234

#define BUF_SIZE 64

using namespace std;

struct Employee {
    char name[20];
    int projects_completed;
    int overtime_hours;
    int efficiency;
    int initiatives;
};


// ������� ��� �������������� ���� ������ � �������� ���������
string getBonusMessage(const string& bonusCode) {
    if (bonusCode == "NO_BONUS") return "������ �� ���������";
    else if (bonusCode == "STANDARD_BONUS") return "����������� ������";
    else if (bonusCode == "MEDIUM_BONUS") return "������� ������";
    else if (bonusCode == "HIGH_BONUS") return "������� ������";
    else return "����������� ��� ������: " + bonusCode;
}

int main() {
    setlocale(LC_ALL, "rus");
    cout << "\t ������ ��� ������� ������\n";
    for (int i = 0; i < 30; i++)
        cout << "-";
    cout << endl;

    char buff[1024];
    if (WSAStartup(0x0202, (WSADATA*)&buff[0])) {
        cout << "������ �������������! \n" << WSAGetLastError();
        return -1;
    }

    //�������� ����������� ������
    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "������ �������� ������! \n" << WSAGetLastError();
        return -1;
    }

    sockaddr_in  srvSin;
   //�������� ������ � �����(�����)
   

    //��� ����� � ip 
    hostent* hp;
    srvSin.sin_family = AF_INET;
    srvSin.sin_port = htons(SRV_PORT);
    hp = gethostbyname(SRV_HOST);
    ((unsigned long*)&srvSin.sin_addr)[0] = ((unsigned long**)hp->h_addr_list)[0][0];

     //���������� � ��������
    cout << "������������ � �������..." << endl;
    if (connect(s, (sockaddr*)&srvSin, sizeof(srvSin)) == SOCKET_ERROR) {
        cout << "������ ����������� � �������! \n" << WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return -1;
    }
    cout << "����������� �����������!" << endl;

    int len = 0;
    char buf[BUF_SIZE] = { 0 };

    // �������� ����������� �� �������
    len = recv(s, (char*)&buf, BUF_SIZE, 0);
    if (len == SOCKET_ERROR) {
        cout << "������ ������ ���������! \n" << WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return -1;
    }

    // ���������� ��������� �������� �����������
    if (len >= 0 && len < BUF_SIZE) {
        buf[len] = 0;
    }
    else if (len >= BUF_SIZE) {
        buf[BUF_SIZE - 1] = 0; // ���������� ��� �� ������ �� �������
    }
    cout << "��������� �� �������: " << buf << endl;

    while (true) {
        // ���� ������ ����������
        Employee E;
        cout << "\n������� ������ ����������:" << endl;
        cout << "������� ��� ����������: ";
        cin.getline(E.name, 20);
        cout << "������� ���������� ����������� ��������: ";
        cin >> E.projects_completed;
        cout << "������� ���������� ������������ �����: ";
        cin >> E.overtime_hours;
        cout << "������� ���������� ������������� (0-10): ";
        cin >> E.efficiency;
        cout << "������� ���������� ���������: ";
        cin >> E.initiatives;

        

        // ���������� ������ ���������� �� ������
        send(s, (char*)&E, sizeof(Employee), 0);

        // �������� ��� ������ �� �������
        len = recv(s, (char*)&buf, BUF_SIZE, 0);
        if (len == SOCKET_ERROR) {
            cout << "������ ������ ���������! \n" << WSAGetLastError();
            break;
        }

        // ���������� ��������� �������� �����������
        if (len >= 0 && len < BUF_SIZE) {
            buf[len] = 0;
        }
        else if (len >= BUF_SIZE) {
            buf[BUF_SIZE - 1] = 0;
        }
        string bonusCode = string(buf);
        string bonusMessage = getBonusMessage(bonusCode);

        // �������  ��������� �� ������ ���� �� �������
        cout << "\n" << string(40, '=') << endl;
        cout << "��������� ��� ���������� " << E.name << ": " << bonusMessage << endl;
        cout << string(40, '=') << "\n" << endl;
    }

    closesocket(s);
    WSACleanup();
    return 0;
}