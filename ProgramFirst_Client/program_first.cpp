#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include "nlohmann/json.hpp"
#include <condition_variable>

/*-----------------------------------------------------------------*/
class EnteredData
{
public:
    EnteredData() = default;

    void ProgramFirstPartFirst()
    {
        while(!writeDataToFile)
        {
            bool sequenceCorrectStatus = false;
            do
            {
                std::cout << "Enter symbol sequence:" << std::endl;
                std::cin >> symbolSequence;

                if(symbolSequence.length() > symbolLength)
                {
                    std::cout << "Incorrect length of the sequence!" << std::endl;
                    std::cout << "Entered sequence length is " << symbolSequence.length() << std::endl;
                }else
                {
                    for(int i = 0; i < symbolSequence.length(); ++i)
                    {
                        if((symbolSequence[i] < 48) || (symbolSequence[i] > 59))
                        {
                            std::cout << "Wrong characters in the sequence!" << std::endl;
                            break;
                        }

                        if(i == (symbolSequence.length() - 1))
                        {
                            sequenceCorrectStatus = true;
                        }
                    }
                }

                if(sequenceCorrectStatus)
                {
                    std::cout << "Correct input!" << std::endl;
                }
            }while(!sequenceCorrectStatus);

            //Sequence sort------------------------------------------------------
            for(int i = 0; i < symbolSequence.length(); ++i)
            {
                for(int j = i; (symbolSequence[j] > symbolSequence[j - 1]) && ((j - 1) >= 0); --j)
                {
                    char saveSymbol = symbolSequence[j - 1];
                    symbolSequence[j - 1] = symbolSequence[j];
                    symbolSequence[j] = saveSymbol;
                }
            }
            std::cout << "Sorting result: " << symbolSequence << std::endl;

            //Even values replace-------------------------------------------------
            std::string newSymbolSequence;
            for(int i = 0; i < symbolSequence.length(); ++i)
            {
                if((i%2) == 0)
                {
                    newSymbolSequence += 'K';
                    newSymbolSequence += 'B';
                }else
                {
                    newSymbolSequence += symbolSequence[i];
                }
            }
            symbolSequence = newSymbolSequence;
            std::cout << "Replacing numbers result: " << symbolSequence << std::endl;

            //Check json file existence or clear an existing file-------------------------
            std::string filePath = "D:\\Projects C++\\Client server application\\ProgramFirst_Client\\data.json";
            std::ifstream dataRead(filePath.c_str());
            if(dataRead.is_open())
            {
                std::ofstream fileClear("data.json");
                fileClear << " ";
                fileClear.close();
                std::cout << "File opened and cleared!" << std::endl;
            }else
            {
                std::cout << "File not found!" << std::endl;
            }

            //Write to json file-------------------------
            std::ofstream dataWrite("D:\\Projects C++\\Client server application\\ProgramFirst_Client\\data.json");
            if(dataWrite.is_open())
            {
                dataWrite << "{" << std::endl;
                dataWrite << std::setw(8) << "\"Symbol sequence\": \"" << symbolSequence << "\"" << std::endl;
                dataWrite << "}" << std::endl;
                dataWrite.close();
                std::cout << "File successfully written!" << std::endl;
                writeDataToFile = true;
            }else
            {
                std::cout << "File not found!" << std::endl;
            }

            std::lock_guard<std::mutex> lk(mut);
            dataCond.notify_one();
        }
    }

    void ProgramFirstPartSecond()
    {
        while(true)
        {
            std::unique_lock<std::mutex> lk(mut);
            dataCond.wait(lk,[this]
            {
                if(writeDataToFile)
                {
                    std::cout << "Second thread works. First finished." << std::endl;
                    return (true);
                }else
                {
                    std::cout << "First thread works. Second blocked." << std::endl;
                    return (false);
                }
            });
            lk.unlock();

            //Read data from json file-----------------------------------------------------
            std::ifstream filePath("D:\\Projects C++\\Client server application\\ProgramFirst_Client\\data.json");
            nlohmann::json dataFromFile;
            std::string dataFromFileInString;
            if(filePath.is_open())
            {
                filePath >> dataFromFile;
                dataFromFileInString = dataFromFile["Symbol sequence"];
            }

            //Clean json file--------------------------------------------------------------
            std::ofstream fileClean("D:\\Projects C++\\Client server application\\ProgramFirst_Client\\data.json");
            fileClean << " ";
            fileClean.close();

            sequenceSum = 0;
            for (auto i : dataFromFileInString)
            {
                if((i <= 57) && (i >= 48))
                {
                    sequenceSum += (i - 48);
                }
            }
            std::cout << "Sequence sum to send to Server: " << sequenceSum << std::endl;

            if(writeDataToFile)
            {
                break;
            }
        }
    }

    void FirstProgramThreadsStart()
    {
        std::thread threadFirst(&EnteredData::ProgramFirstPartFirst, this);
        threadFirst.detach();
        std::thread threadSecond(&EnteredData::ProgramFirstPartSecond, this);
        threadSecond.join();
    }

    std::string GetSequenceSumInString()
    {
        return std::to_string(sequenceSum);
    }

private:
    int sequenceSum;
    bool writeDataToFile = false;
    std::string symbolSequence;
    int symbolLength = 64;
    std::mutex mut;
    std::condition_variable dataCond;
};
/*-----------------------------------------------------------------*/

WSADATA wsData;

int main()
{

    //Key constants.
    const char SERVER_IP[] = "127.0.0.1";		    //IPv4 address of Server.
    const short SERVER_PORT_NUM = 1234;				//Listening port on Server side.
    const short BUFF_SIZE = 256;					//Maximum size of buffer for exchange info between server and client.

    //Key variables for all program.
    int erStat;										//For checking errors in sockets functions.

    //Struct for Socket info keeping.
    sockaddr_in servInfo;

    //IP in string format to numeric format for socket functions. Data is in "ipToNum"
    in_addr ipToNum;
    inet_pton(AF_INET, SERVER_IP, &ipToNum);

    //WinSock initialization. Download WinSock library.

    //Download WinSock library function.
    //first parameter: WORD DLLVersion = MAKEWORD(2,2) - WinSock library version.
    //second parameter: &wsData - reference to WSDATA struct.
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);  //Download WinSock library function.

    //Check library download success.
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

    //Client socket initialization for Server connection.
    SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, NULL);

    if(ClientSock == INVALID_SOCKET)
    {
        std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
        closesocket(ClientSock);
        WSACleanup();
    }
    else
    {
        std::cout << "Client socket initialization is OK" << std::endl;
    }

    //Establishing a connection to Server.
    ZeroMemory(&servInfo, sizeof(servInfo));                    //Initializing servInfo structure.

    servInfo.sin_family = AF_INET;                              //Protocol family.
    servInfo.sin_addr = ipToNum;                                //IP address.
    servInfo.sin_port = htons(SERVER_PORT_NUM);         //Port for receiving data. Port number.
    erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo)); //Connection to server function.

    //Checking connection status.
    if(erStat != 0)
    {
        std::cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << std::endl;
        closesocket(ClientSock);
        WSACleanup();
        return 1;
    }
    else
    {
        std::cout << "Connection established SUCCESSFULLY. Ready to send data to Server" << std::endl;
    }

    //Sending data to Server.

    char servBuff[BUFF_SIZE], clientBuff[BUFF_SIZE];				//Buffers for sending and receiving data.
    short packetSize = 0;											//The size of sending / receiving packet in bytes.

    while(true)
    {
        std::cout << "Client message to Server:" << std::endl;

        EnteredData data;
        data.FirstProgramThreadsStart();

        std::cout << "Send data to server or interrupt connection. yes - to send data, no - to interrupt:" << std::endl;
        std::string action;
        std::cin >> action;

        if(action == "no")
        {
            clientBuff[0] = 'n';
            clientBuff[1] = 'o';
            packetSize = send(ClientSock, clientBuff, sizeof(clientBuff), NULL);

            shutdown(ClientSock, SD_BOTH);
            closesocket(ClientSock);
            WSACleanup();
            return 0;
        }else
        {
            for(int i=0; i<data.GetSequenceSumInString().length(); ++i)
            {
                clientBuff[i] = data.GetSequenceSumInString()[i];
                if(i == (data.GetSequenceSumInString().length() - 1))
                {
                    clientBuff[i+1] = '\0';
                }
            }
            packetSize = send(ClientSock, clientBuff, sizeof(clientBuff), NULL);
        }
    }

    closesocket(ClientSock);
    WSACleanup();

    return 0;
}