#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int server_sockfd, client_sockfd;   // Дескрипторы сокета сервера и клиента
    int server_len, client_len;         // Длина адреса сервера и клиента
    struct sockaddr_un server_address;  // Адрес сокета сервера
    struct sockaddr_un client_address;  // Адрес сокета клиента
    int process_query = 0;              // Число обработанных запросов клиентов
    char temp_files_paths[2][256];      // Путь к 2 временным файлам

    // Буфер сообщения
    char* buffer = (char*)malloc(8192*sizeof(char));        

    unlink("server socket");            // Удаление старых сокетов

    // Получение дескриптора сокета сервера
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    server_address.sun_family = AF_UNIX;               
    strcpy(server_address.sun_path, "server socket");
    server_len = sizeof(server_address);

    // Именование сокета сервера
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    listen(server_sockfd, 2);   // Создание очереди запросов к сокету

    // Пока не обработаются 2 запроса клиентов - ожидать запросы
    while (process_query != 2)
    {
        // Принятие запроса от клиента на сервер
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, &client_len);
        
        // Число считанных байт (Получение данных от клиента)
        int reading_bytes = read(client_sockfd, buffer, BUFSIZ);

        // Если сообщение пустое, то игнорировать
        if(reading_bytes == 0)
        {
            close(client_sockfd);   // Закрытике сокета клиента
            continue;
        }

        // Создание временного файла, полученного от клиента
        snprintf(temp_files_paths[process_query], 256, "file%d", process_query);

        // Дескриптор файла, подающегося в программу
        int temp_fd = open(temp_files_paths[process_query], O_CREAT | O_RDWR, 0777);

        // Пока клиент отправляет сообщения - читать их
        while(reading_bytes != 0)
        {
            //printf(" Server accpeting: %s", buffer);
            write(temp_fd, buffer, reading_bytes);

            // Получение данных от клиента
            reading_bytes = read(client_sockfd, buffer, BUFSIZ);
        }

        close(client_sockfd);   // Закрытике сокета клиента
        process_query += 1;     // Следующий клиент
    }

    /* Д О Б А В И Т Ь   О П Е Р А Ц И Ю    X O R  */

    //printf("path1:%s\n", temp_files_paths[0]);
    //printf("path1:%s\n", temp_files_paths[1]);
    
    free(buffer);

    return 0;
}