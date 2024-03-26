#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
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
        pid = fork();   // Создание процесса

        // В дочернем процессе
        if(pid == 0)
        {
            if(i == 0)
            {
                printf("Запускается сервер i = %d ", i);
                printf("Pid = %d", getpid());
                result = execlp("./server", "server", "&" , NULL);

                printf(" Сервер с result = %d\n", result);

                // Эхо-печать
                if (result == 0)
                {
                    printf(" Запущен сервер с pid = %d\n", getpid());
                }
                else
                {
                    printf(" Не запущен сервер с pid = %d\n", getpid());
                }
            }
            else
            {
                printf("Запускается клиент i = %d ", i);
                printf("argc[%d] = %s ", i, argv[i]);
                result = execlp("./client", "client", argv[i], NULL);
                printf("Pid = %d", getpid());

                printf(" Клиент с result = %d\n", result);

                // Эхо-печать
                if (result == 0)
                {
                    printf(" Запущен клиент с pid = %d\n", getpid());
                }
                else
                {
                    printf(" Не запущен клиент с pid = %d\n", getpid());
                }
            }
            exit(0);
        }

        // Ожидание создания сервера
        if (i == 0)
        {
            sleep(2);
        }
    }

    while(1)
    {
        //
    }

    return 0;
}