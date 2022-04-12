#include "Client.h"

Client::Client(QObject* parent, QHostAddress addr, int num) :
	QObject(parent),
	address(addr),
	port(num)
{
}

void Client::SetAddress(QString addr)
{
	address = addr;
}

void Client::SetListenPort(int num)
{
	port = num;
}

void Client::ConnectEvent()
{
	emit Connected();
}

void Client::SendOutMessage(QString str)
{
	if (!str.isEmpty())
	{
		client->write(str.toLatin1());
	}
}

void Client::ReciveMessage()
{
	emit Message(client->readAll());
}

void Client::DisConnect()
{
	client->close();
	client->deleteLater();
}

void Client::Init()
{
	LOG(INFO) << QStringLiteral("��������%1:%2").arg(address.toString()).arg(port);
	client = new QTcpSocket;
	client->connectToHost(address, port);
	connect(client, SIGNAL(connected()), this, SLOT(ConnectEvent()));
	connect(client, SIGNAL(readyRead()), this, SLOT(ReciveMessage()));
	connect(client, SIGNAL(disconnected()), this, SLOT(DisConnect()));
}