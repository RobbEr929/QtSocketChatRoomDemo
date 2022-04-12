#include "Server.h"

Server::Server(QObject* parent) :QObject(parent)
{
}

void Server::SetListenPort(unsigned int num)
{
	port = num;
}

int Server::NowConnectNum() const
{
	return sockets.size();
}

void Server::Close()
{
	LOG(INFO) << QStringLiteral("�رշ�����");
	QString closeMsg = QStringLiteral("�����������رգ����Ӽ����Ͽ�");
	for (auto& i : sockets)
	{
		i->write(closeMsg.toStdString().data());
		i->close();
	}
	server->close();
	delete server;
	emit CloseRes(true);
}

void Server::Init()
{
	LOG(INFO) << "Server Init";
	server = new QTcpServer(this);
	if (server->listen(QHostAddress::Any, port))
	{
		LOG(INFO) << QStringLiteral("���� localhost:%1 �ɹ�").arg(port);
		connect(server, SIGNAL(newConnection()), this, SLOT(NewConnection()));
		emit InitRes(true);
	}
	else
	{
		LOG(INFO) << QStringLiteral("���� localhost:%1 ʧ��").arg(port);
		emit InitRes(false);
	}
}

void Server::NewConnection()
{
	QTcpSocket* socket = server->nextPendingConnection();
	sockets.push_back(socket);
	connect(socket, SIGNAL(readyRead()), this, SLOT(ReadMessage()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Error(QAbstractSocket::SocketError)));
	LOG(INFO) << QStringLiteral("���������� %1").arg(socket->QAbstractSocket::peerAddress().toString());
}

void Server::ReadMessage()
{
	QTcpSocket* socket = (QTcpSocket*)QObject::sender();
	QString data = socket->QAbstractSocket::peerAddress().toString() %":" % socket->readAll().data();
	LOG(INFO) << QString("%1 : %2").arg(socket->QAbstractSocket::peerAddress().toString()).arg(data);
	emit ReciveData(QStringLiteral("%1 : %2").arg(socket->QAbstractSocket::peerAddress().toString()).arg(data));
	for (auto& i : sockets)
	{
		i->write(data.toStdString().data());
	}
}
void Server::Error(QAbstractSocket::SocketError)
{
	QTcpSocket* socket = (QTcpSocket*)QObject::sender();
	int error = socket->error();
	switch (error)
	{
	case QAbstractSocket::RemoteHostClosedError:
	{
		QString hostAddress = socket->QAbstractSocket::peerAddress().toString();
		auto i = sockets.begin();
		for (; i != sockets.end(); ++i)
		{
			if (*i == socket)
				break;
		}
		socket->close();
		sockets.erase(i);
		for (auto& i : sockets)
		{
			i->write(QStringLiteral("%1 �Ѿ��Ͽ�����").arg(hostAddress).toStdString().data());
		}
		emit ReciveData(QStringLiteral("%1 �Ѿ��Ͽ�����").arg(hostAddress).toStdString().data());
		LOG(INFO) << QStringLiteral("%1 �Ѿ��Ͽ�����").arg(hostAddress);
		break;
	}
	default:
		break;
	}
}