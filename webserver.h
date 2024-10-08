#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>
#include <QTcpServer>
#include <memory>

class WebServer : public QObject
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = nullptr);
    ~WebServer() override;
    void start();
private slots:
    void newConnection();
private:
    const std::unique_ptr<QTcpServer> m_server;
};

#endif // WEBSERVER_H
