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
    int sockfd;                 // Дескриптор сокета
    int len;                    // Длина адреса
    struct sockaddr_un address; // Адрес сокета
    int result;
    char* buffer = (char*)malloc(BUFSIZ*sizeof(char));        // Буфер сообщения

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);   // Получение дескриптора сокета

    // Заполнение адреса сокета
    address.sun_family = AF_UNIX;               
    strcpy(address.sun_path, "server socket");
    len = sizeof(address);

    // Запрос на соединение с сервером
    result = connect(sockfd, (struct sockaddr *) &address, len);

    // Обмен сообщениями по сокету с сервером
    if (result != -1)
    {
        printf(" КЛИЕНТ %d ПОДКЛЮЧЕН К СЕРВЕРУ\n", getpid());

        // Дескриптор файла, поддающегося в программу
        int fd = open(argv[1], O_RDONLY);

        // Число считанных байт
        int bytes_read;

        // Пока можно считать
        while((bytes_read = read(fd, buffer, BUFSIZ)) > 0)
        {
            buffer[bytes_read] = '\0';  // Установка нулевого символа в конец прочитанных данных
            //printf("%s", buffer);
            write(sockfd, buffer, bytes_read); // Запись через сокет
        }
        
        close(fd);       // Закрытие файла
        close(sockfd);   // Закрытие сокета
    }
    else
    {
        printf(" Клиент %d не может подключиться к серверу\n", getpid());
        exit(1);
    }

    free(buffer);  // Освобождение памяти, выделенной под буфер

    exit (0);
}
