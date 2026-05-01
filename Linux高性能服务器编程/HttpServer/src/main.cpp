#include "http_server.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    int port = 8888;
    if (argc >= 2) {
        port = std::atoi(argv[1]);
    }
    std::cout << "Starting HTTP server on port " << port << std::endl;
    HttpServer server(port, 4);
    server.run();
    return 0;
}