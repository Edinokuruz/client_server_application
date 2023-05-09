#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <exception>

/*-----------------------------------------------------------------*/
class TermsCorrect : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return ("Correct data received.");
    }
};
/*-----------------------------------------------------------------*/
class TermsIncorrect : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return ("Error! Incorrect data received.");
    }
};
/*-----------------------------------------------------------------*/
bool CheckReceivedData(char *servBuff)
{
    int result = std::stoi(servBuff);
    int termsNumberSizeMin = 100;
    if((result > (termsNumberSizeMin - 1)) && ((result % 32) == false))
    {
        throw TermsCorrect();
    }else
    {
        throw TermsIncorrect();
    }
}
/*-----------------------------------------------------------------*/

WSADATA wsData;

int main()
{
    //Key constants.
    const char IP_SERV[] = "127.0.0.1";			//Enter local Server IP address.
    const int PORT_NUM = 1234;				    //Enter Open working server port.
    const short BUFF_SIZE = 256;			    //Maximum size of buffer for exchange info between server and client.

    //Key variables for all program.
    int erStat;								    // Keeps socket errors status.

    sockaddr_in servInfo;
    sockaddr_in clientInfo;

    //IP in string format to numeric format for socket functions. Data is in "ipToNum".
    in_addr ipToNum;
    erStat = inet_pton(AF_INET, IP_SERV, &ipToNum);

    if(erStat <= 0)
    {
        std::cout << "Error in IP translation to special numeric format" << std::endl;
        return 1;
    }

    //WinSock initialization.
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);

    if(erStat != 0)
    {
        std::cout << "Error WinSock version initialization #";
        std::cout << WSAGetLastError();
        return 1;
    }
    else
    {
        std::cout << "WinSock initialization is OK" << std::endl;
    }

    //Server socket initialization.
    SOCKET ServSock = socket(AF_INET, SOCK_STREAM, NULL);

    if(ServSock == INVALID_SOCKET)
    {
        std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Server socket initialization is OK" << std::endl;
    }

    //Server socket binding
    ZeroMemory(&servInfo, sizeof(servInfo));	        //Initializing servInfo structure.

    servInfo.sin_family = AF_INET;                      //Address family.
    servInfo.sin_addr = ipToNum;                        //IP address.
    servInfo.sin_port = htons(PORT_NUM);        //Port number.

    erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));

    if(erStat != 0)
    {
        std::cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Binding socket to Server info is OK" << std::endl;
    }

    //Starting to listen to any Clients.
    erStat = listen(ServSock, SOMAXCONN);

    if(erStat != 0)
    {
        std::cout << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Listening..." << std::endl;
    }

    //Client socket creation and acception in case of connection.
    ZeroMemory(&clientInfo, sizeof(clientInfo));	    //Initializing clientInfo structure.

    int clientInfoSize = sizeof(clientInfo);

    SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfoSize);

    if(ClientConn == INVALID_SOCKET)
    {
        std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        closesocket(ClientConn);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Connection to a client established successfully" << std::endl;
        char clientIP[22];

        inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);	// Convert connected client's IP to standard string format.

        std::cout << "Client connected with IP address " << clientIP << std::endl;
    }

    //Receive data from Client. Disconnection if a client send "no"
    char servBuff[BUFF_SIZE], clientBuff[BUFF_SIZE];					//Creation of buffers for sending and receiving data.
    short packetSize = 0;												//The size of sending / receiving packet in bytes.

    while(true)
    {
        packetSize = recv(ClientConn, servBuff, sizeof(servBuff), NULL);	 //Receiving packet from client. Program is waiting (system pause) until receive.

        if(servBuff[0] == 'n' && servBuff[1] == 'o')
        {
            shutdown(ClientConn, SD_BOTH);
            closesocket(ServSock);
            closesocket(ClientConn);
            WSACleanup();
            return 0;
        }

        try
        {
            CheckReceivedData(servBuff);
        }
        catch(const TermsCorrect &x)
        {
            std::cerr << "Message: " << x.what() << " Data: " << servBuff << std::endl;
        }
        catch(const TermsIncorrect &x)
        {
            std::cout << "Message: " << x.what() << std::endl;
        }
    }
    closesocket(ServSock);
    closesocket(ClientConn);
    WSACleanup();

    return 0;
}