![GitHub author](https://img.shields.io/badge/made_by-Grigory_Brusnik-blueviolet)
![GitHub repo size](https://img.shields.io/github/repo-size/Edinokuruz/client_server_application?color=blueviolet)
![GitHub author](https://img.shields.io/badge/made_on-C++-blueviolet)

![GitHub repo file count](https://img.shields.io/github/directory-file-count/Edinokuruz/client_server_application?color=informational&label=Files%20Main%20dir)
![GitHub repo file count (custom path)](https://img.shields.io/github/directory-file-count/Edinokuruz/client_server_application/ProgramFirst_Client?color=informational&label=Files%20First%20program%20dir)
![GitHub repo file count (custom path)](https://img.shields.io/github/directory-file-count/Edinokuruz/client_server_application/ProgramSecond_Server?color=informational&label=Files%20Second%20program%20dir)
## __Название проекта | Project name__ 
![GitHub author](https://img.shields.io/badge/-RUS-blue)  
Приложение клиент - сервер.  
![GitHub author](https://img.shields.io/badge/-ENG-blue)  
Client-server application.
___
## __Технологии | Technologies__ 
![GitHub author](https://img.shields.io/badge/C++-23-darkblue)  
![GitHub author](https://img.shields.io/badge/CMake-3.25-darkblue)
https://cmake.org/  
![GitHub author](https://img.shields.io/badge/JSON-3.11.2-darkblue)
https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
___
## __Техническая спецификация | Technical specification__
![GitHub author](https://img.shields.io/badge/-RUS-blue)    
Задание состоит из двух программ, которые необходимо реализовать. Взаимодействие программ должно быть реализовано через использование сокетов.

Программа №1. 
Должна состоять из двух потоков и одного общего буфера. Тип общего буфера должен быть: файл или shared memory.

Поток 1.  Принимает строку, которую введет пользователь. Должна быть проверка, что строка состоит только из цифр и не превышает 64 символа. После проверки строка должна быть отсортирована по убыванию и все четные элементы заменены на латинские буквы «КВ». После данная строка помещается в общий буфер и поток должен ожидать дальнейшего ввода пользователя.

Поток 2. Должен обрабатывать данные которые помещаются в общий буфер. После получения данных общий буфер затирается. Поток должен вывести полученные данные на экран, рассчитать общую сумму всех, которые являются численными значениями.  Полученную сумму передать в Программу №2. После этого поток ожидает следующие данные.

Примечание по Программе №1: Взаимодействие потоков должно быть синхронизировано, поток №2 не должен постоянно опрашивать общий буфер. Механизм синхронизации не должен быть глобальной переменной.

Программа №2. 
Ожидает данные от Программы №1. При получении данных происходит анализ из скольких символов состоит переданное значение. Если оно больше 2-ух символов и если оно кратно 32 выводит сообщение о полученных данных, иначе выводится сообщение об ошибке. Далее программа продолжает ожидать данные.

![GitHub author](https://img.shields.io/badge/-ENG-blue)  
The task consists of two programs that need to be implemented. Programs communication should be implemented through the use of sockets.

Program No. 1.
It should consist of two threads and one shared buffer. The type of shared buffer must be a file.

Thread 1. Accepts the string that the user enters. There should be a check that the string consists only of digits and does not exceed 64 characters. After checking, the string should be stored in descending order and all even elements replaced with Latin letters "KV". After that, this line is placed in the shared buffer and the thread should wait for further user input.

Thread 2. Must process data that is placed in a shared buffer. After receiving the data, the shared buffer is cleared. The thread should display the received data on the screen, calculate the total sum of all that are numerical values. Transfer the received amount to Program No. 2. After that, the thread waits for the following data.

Note on Program #1: The communication of threads should be synchronized, thread #2 should not constantly poll the shared buffer. The synchronization mechanism should not be a global variable.

Program No.2.
Waits for data from Program #1. When receiving data, an analysis is performed of how many characters the transmitted value consists of. If it is more than 2 characters and if it is a multiple of 32, it displays a message about the received data, otherwise an error message is displayed. Then the program continues to wait for data.
___  
## __Скриншот работы программы | Working program screenshot__
![Screenshot 1](https://github.com/Edinokuruz/client_server_application/blob/master/1.png)

![Screenshot 2](https://github.com/Edinokuruz/client_server_application/blob/master/2.png)
___
## __Сборка и запуск проекта | Project building and running__
* Шаг 1: Сборка проекта | Step 1: Build the project.  
  * Если компилятор Visual Studio будет использован | If Visual Studio compiler will be used:  
    * Для версии x64 | For x64 version:  
`cmake -A x64 -S . -B "build64"`  
`cmake --build build64 --config Release`  
    * Для версии x32 | For x32 version:  
`cmake -A Win32 -S . -B "build32"`  
`cmake --build build32 --config Release`  
  * Если другие компиляторы | In other compilers:  
`cmake -S . -B "build"`  
`cmake --build build --config Release`  
* Шаг 2: скопировать файлы: | Step 2: copy files:  
`.\bin\requests.json`, `.\bin\config.json`, `.\bin\answers.json` and `.\bin\resources` folder  
from `.\bin` folder  
to the `.\bin\Release` folder.  
* Step 3: run the application:  
`.\bin\Release\search-engine`  
___

