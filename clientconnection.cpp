#include "clientconnection.h"

#include <QFile>

ClientConnection::ClientConnection(QTcpSocket *socket, QObject *parent)
    : QObject{parent}, m_socket(socket)
{
    connect(m_socket.get(), &QAbstractSocket::disconnected, this, &QObject::deleteLater);
    connect(m_socket.get(), &QIODevice::readyRead, this, &ClientConnection::readyRead);
}


ClientConnection::~ClientConnection() = default;

void ClientConnection::readyRead()
{
    while ((m_state == ConnectionState::RequestLine || m_state == ConnectionState::RequestHeaders) &&
           m_socket->canReadLine())
    {
        QByteArray line = m_socket->readLine();
        qDebug() << "Received line:" << line;

        if (line.endsWith("\r\n"))
            line.chop(2);

        if (m_state == ConnectionState::RequestLine)
        {
            const auto parts = line.split(' ');
            if (parts.size() != 3)
            {
                qWarning() << "Request Line doesn't consist of 3 parts:" << line;
                deleteLater();
                return;
            }

            m_request.method = parts.at(0);
            m_request.path = parts.at(1);
            m_request.protocol = parts.at(2);
            qDebug() << "Parsed request:" << m_request.method << m_request.path;

            m_state = ConnectionState::RequestHeaders;
            continue;
        }
        else
        {
            if (line.isEmpty())
            {
                qDebug() << "End of headers";
                if (m_request.path == "/") // Handle root path
                {
                    m_state = ConnectionState::Response;
                    sendResponse(); // Call to send response
                }
                else if (m_request.contentLength > 0)
                {
                    m_state = ConnectionState::RequestBody;
                    continue;
                }
                else
                {
                    m_state = ConnectionState::Response;
                    sendResponse();
                    return;
                }
            }
        }
    }
}

void ClientConnection::sendResponse()
{
    if (m_request.path == "/") // Handle root path
    {    
        const QString imagePath = "/home/kunal/workspace/Qt Practice/QScreenApp/Screen Image/ScreenImage_1.PNG"; // Update path as needed
        QFile imageFile(imagePath);

        if (!imageFile.open(QIODevice::ReadOnly))
        {
            qWarning() << "Failed to open image file:" << imagePath;
            m_socket->write("HTTP/1.1 404 Not Found\r\n\r\n");
            return;
        }

        QByteArray imageData = imageFile.readAll();
        imageFile.close();

        // Send HTTP response header
        m_socket->write("HTTP/1.1 200 OK\r\n");
        m_socket->write("Content-Type: image/jpeg\r\n");
        m_socket->write("Content-Length: " + QByteArray::number(imageData.size()) + "\r\n");
        m_socket->write("\r\n"); // End of headers

        // Send the image data
        m_socket->write(imageData);

        m_socket->write("Connection: close\r\n");
        m_socket->write("Refresh: 1\r\n\r\n");
    }
    else
    {
        m_socket->write("HTTP/1.1 404 Not Found\r\n\r\n");
    }

    m_state = ConnectionState::RequestLine; // Reset state
    m_request = {}; // Reset request
}
