#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argv, char* argc)
{
    int pid;            // Id процеса
    int result;         // Результат замещения процесса

    // if(argv != 3)
    // {
    //     printf(" Аргументов не достаточно\n");
    //     return -1;
    // }

    // Создание нужных процессов
    for(int i = 0; i < 3; ++i)
    {
        // Запуск сервера
        if (i == 0)
        {
            pid = fork();   // Создание процесса
            if(pid == 0)
            {
                printf("Запускается сервер i = %d", i);
                result = execlp("./server", "server", NULL);

                printf(" Сервер с result = %d\n", result);

                // Эхо-печать
                if (result == 0)
                {
                    printf(" Запущен сервер с pid = %d\n", pid);
                }
                else
                {
                    printf(" Не запущен сервер с pid = %d\n", pid);
                }
            }
        }
        // Запуск клиентов
        else
        {
            pid = fork();   // Создание процесса
            if(pid == 0)
            {   
                printf("Запускается клиент i = %d", i);
                result = execlp("./client", "client", argc[i], NULL);

                // Эхо-печать
                if (result == 0)
                {
                    printf(" Запущен клиент с pid = %d\n", pid);
                }
                else
                {
                    printf(" Не запущен клиент с pid = %d\n", pid);
                }
            }
        }
    }

    sleep(30);

    return 0;
}