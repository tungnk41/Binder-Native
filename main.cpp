
int main() {
    Server server = new Server();
    Client client = new Client();

    server->init();
    client->push(10);
}