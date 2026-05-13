#include "server.h"
#include "../database/database.h"

int main()
{

    Database &db = Database::getInstance();

    db.connect(
        "localhost",
        "admin",
        "1234",
        "tmps_db");

    HttpServer server(8080);

    server.start();

    db.disconnect();

    return 0;
}