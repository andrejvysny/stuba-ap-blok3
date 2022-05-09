#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "header.h"

#pragma comment (lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 4096
#define HOST "147.175.115.34"
#define HOST_PORT "777"
#define STUDENT_ID "115031"
#define INTERACTIVE 1
#define MIDDLE_MARGIN 6

char buffer[DEFAULT_BUFLEN] = "";
int iResult, terminate = 0;
SOCKET ConnectSocket = INVALID_SOCKET;

HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO csbi;
COORD cursor;

FILE *fw;

int main() {
    configAndConnect();

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleTextAttribute(hConsole, 10);

    if ((fw = fopen("logs.txt", "w")) == NULL) {
        printf("Could not create logs file!");
        return 1;
    }

    cursor.Y = 3;


    int index = 1;
    while (1) {
        //  Sleep(500);
        handle(INTERACTIVE ? index : 0);
        //   Sleep(1000);
        receiveData();
        //   Sleep(500);

        index++;
        if (terminate)
            break;
    }

    fclose(fw);

    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}

void handle(int index) {
    switch (index) {
        case 1: {
            sendData("Hello");
            break;
        }
        case 2: {
            sendData(STUDENT_ID);
            break;
        }
        case 3: {
            sendData("What?");
            break;
        }
        case 4: {
            sendData("7545477");
            break;
        }
        case 5: {
            sendData("753421");
            break;
        }
        case 6: {
            int val;
            for (int i = 1; i <= 5; ++i) {
                val += STUDENT_ID[i - 1] - '0';
            }
            itoa(val % (STUDENT_ID[4] - '0'), buffer, 10);
            sendData(buffer);
            break;
        }
        case 7: {
            sendData("333222111");
            break;
            //TODO: split code to sides
        }
        case 8: {
            sendData("123");
            break;
        }
        case 9: {
            int i = 0;
            while (i < 132) {
                buffer[i] = (char) (buffer[i] ^ 55);
                i++;
            }

            i=0;
            while (i < strlen(buffer)){
                if (buffer[i] == '\n'){
                    buffer[i] = ' ';
                }
                i++;
            }
            display(buffer, strlen(buffer), 1);
            sendData("What do I have to do?");
            break;
        }
        case 10: {
            sendData("48");
            break;
        }
        case 11: {
            sendData("2");
            break;
        }
        case 12: {
            sendData("E.T.");
            break;
        }
        case 13: {
            sendData("PRIMENUMBER");
            break;
        }
        case 14: {
            char temp[strlen(buffer)];
            int j = 0;
            for (int i = 0; i < strlen(buffer); ++i) {
                if (IsPrime(i)) {
                    temp[j++] = buffer[i - 1];
                }
            }
            temp[j] = '\0';
            sendData(temp);
            break;
        }
        case 15: {
            sendData("Trinity");
            break;
        }
        case 16: {
            sendData("half-duplex");
            break;
        }
        case 17: {
            sendData("baud rate");
            break;
        }
        default: {
            display("Input: ", strlen("Input: "), 1);
            fgets(buffer, DEFAULT_BUFLEN, stdin);
            if (buffer[0] == '!') {
                terminate = 1;
            }
            sendData(buffer);
            break;
        }
    }
}


void logMessage(char data[], int sent) {
    if (sent) {
        fprintf(fw, "\nYou: %s", data);
    } else {
        fprintf(fw, "\nMorpheus: %s\n", data);
    }
}


void display(char data[], int len, int left) {

    short int half = (short) ( (csbi.dwSize.X / 2) - 8);

    int space_position, string_offset = 0;
    for (int index = 0; index < strlen(data); index++) {
        if (data[index] == ' ') {
            space_position = index;
        }
        if (string_offset >= half - 1) {
            data[space_position] = '\n';
            string_offset = 0;
        }
        string_offset++;
    }

    cursor.Y++;

    if (left) {
        int i = 0;
        cursor.X = 0;
        SetConsoleCursorPosition(hConsole, cursor);
        while (data[i] != '\0') {
            if (data[i] == '\n') {
                cursor.Y++;
            }
            putchar(data[i]);
            i++;
        }
    } else {
        int i = 0;
        short int start = (short) (half);
        cursor.X = start;
        SetConsoleCursorPosition(hConsole, cursor);
        while (data[i] != '\0') {

            if (data[i - 1] == '\n') {
                cursor.Y++;
                cursor.X = start;
                SetConsoleCursorPosition(hConsole, cursor);
            }

            putchar(data[i]);
            i++;
        }
    }

    logMessage(data, left);
}


int sendData(char data[]) {
    iResult = send(ConnectSocket, data, (int) strlen(data), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        exit(1);
    }
    display(data, iResult, 1);
    return iResult;
}

int receiveData() {
    iResult = recv(ConnectSocket, buffer, DEFAULT_BUFLEN, 0);     //funkcia na príjimanie

    if (iResult > 0) {
        display(buffer, iResult, 0);
    } else if (iResult == 0) {
        printf("\n\tConnection closed\n");//v tomto pripade server ukoncil komunikaciu
        terminate = 1;
    } else {
        printf("\n\tRecv failed with error: %d\n", WSAGetLastError());     //ina chyba
        terminate = 1;
    }
    return iResult;
}


void configAndConnect() {
    WSADATA wsaData;  //struktura WSADATA pre pracu s Winsock
    struct addrinfo *result = NULL, *ptr = NULL;     //struktura pre pracu s adresami
    struct addrinfo hints;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        exit(1);
    }


    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;     //pracujeme s protokolom TCP/IP

    // Resolve the server address and port
    iResult = getaddrinfo(HOST, HOST_PORT, &hints, &result);
    if (iResult != 0)     //kontrola, ci nenastala chyba
    {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr = result;

    // Create a SOCKET for connecting to server => pokus o vytvorenie socketu
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);


    if (ConnectSocket == INVALID_SOCKET)     //kontrola, ci nenastala chyba
    {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Connect to server. => pokus o pripojenie sa na server

    iResult = connect(ConnectSocket, ptr->ai_addr, (int) ptr->ai_addrlen);

    if (iResult == SOCKET_ERROR)     //kontrola, ci nenastala chyba
        printf("Not connected to server! \n");

    if (iResult == SOCKET_ERROR)    //osetrenie chyboveho stavu
    {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        WSACleanup();
        exit(1);
    }

    Sleep(500);
    SetConsoleTextAttribute(hConsole, 160);
    printf("\n\tConnection established\t\t\n\n");
    SetConsoleTextAttribute(hConsole, 15);
}
