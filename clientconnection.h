#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <memory>
#include <QTcpSocket>

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(QTcpSocket *socket,QObject *parent = nullptr);
    ~ClientConnection() override;

private slots:
    void readyRead();

private:
    std::unique_ptr<QTcpSocket> m_socket;
    void sendResponse();

    struct Request{
        std::size_t contentLength{};
        QString method;
        QString path;
        QString protocol;
        QMap<QString, QString> headers;
        QByteArray body;
    };
    Request m_request;

    enum ConnectionState{
        RequestLine,
        RequestHeaders,
        RequestBody,
        Response
    };

    ConnectionState m_state{ConnectionState::RequestLine};
};

#endif // CLIENTCONNECTION_H
