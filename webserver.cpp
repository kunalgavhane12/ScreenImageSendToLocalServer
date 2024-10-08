#include "webserver.h"
#include "clientconnection.h"

WebServer::WebServer(QObject *parent)
    : QObject{parent}
    , m_server(std::make_unique<QTcpServer>())
{
    connect(m_server.get(), &QTcpServer::newConnection, this, &WebServer::newConnection);
}

void WebServer::start()
{
    if(!m_server->listen(QHostAddress("192.168.1.39"), 8080))
    {
        qDebug() << "Server could not start!";
        qDebug() << m_server->errorString();
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void WebServer::newConnection()
{
    qDebug() << "New Connection()";
    const auto socket = m_server->nextPendingConnection();
    if(!socket)
        return;
    new ClientConnection(socket);
}

WebServer::~WebServer() = default;
