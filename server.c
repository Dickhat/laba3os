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
    int server_sockfd, client_sockfd;   // Дескрипторы сокета сервера и клиента
    int server_len, client_len;         // Длина адреса сервера и клиента
    struct sockaddr_un server_address;  // Адрес сокета сервера
    struct sockaddr_un client_address;  // Адрес сокета клиента
    int process_query = 0;              // Число обработанных запросов клиентов
    char temp_files_paths[2][256];      // Путь к 2 временным файлам

    printf(" Сервер работает\n");

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

    listen(server_sockfd, 2);   // Создание очереди из 2 запросов к сокету

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

        //printf("process_query = %d \n", process_query);
        process_query += 1;     // Следующий клиент

        close(client_sockfd);   // Закрытике сокета клиента
    }

    /* О П Е Р А Ц И Я    X O R  */

    printf("path1:%s\n", temp_files_paths[0]);
    printf("path2:%s\n", temp_files_paths[1]);

    // Открытие файлов
    FILE* file1 = fopen(temp_files_paths[0], "r");
    FILE* file2 = fopen(temp_files_paths[1], "r");
    FILE* output = fopen("output_file", "w");

    // Символы 1-го, 2-го файлов и результ XOR
    char ch1, ch2, result;

    // if (file1 == NULL)
    // {
    //     printf("file1: not open\n");
    // }
    // else if (file2 == NULL)
    // {
    //     printf("file2: not open\n");
    // }
    // else if (output == NULL)
    // {
    //     printf("output: not open\n");
    // }

    printf(" Файлы открыты успешно\n");

    // Проверка на открытие
    if (file1 == NULL || file2 == NULL || output == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    // XOR для каждого байта из двух файлов и запись результата в третий файл
    // Проверка что считался 1 байт из 2-ух файлов
    while (fread(&ch1, 1, 1, file1) == 1) 
    {
        // Если последовательность закончилась - считывать с начала
        if (fread(&ch2, 1, 1, file2) != 1)
        {
            rewind(file2);              // Указатель на начало файла
            fread(&ch2, 1, 1, file2);   // Считывание
        }

        result = ch1 ^ ch2;             // XOR
        fwrite(&result, 1, 1, output);  // Запись в файл
    }

    // Очистка временных файлов
    remove(temp_files_paths[0]);
    remove(temp_files_paths[1]);
    
    free(buffer);

    exit (0);
}