#ifndef SERVER_H
#define SERVER_H
/**
 * @brief HTTP сервер.
 */
class HttpServer
{
private:
    int port;

public:
    /**
     * @brief Конструктор сервера.
     * @param p порт сервера.
     */
    HttpServer(int p);
    /**
     * @brief Запуск сервера.
     */
    void start();
};

#endif