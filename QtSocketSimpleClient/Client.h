#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>
#include "easylogging++.h"
class Client :public QObject
{
	Q_OBJECT
public:
	explicit Client(QObject* parent = nullptr, QHostAddress addr = QHostAddress::LocalHost, int num = 929);
	void SetAddress(QString addr);
	void SetListenPort(int num);

public slots:
	void Init();
	void ConnectEvent();
	void SendOutMessage(QString str);
	void ReciveMessage();
	void DisConnect();

signals:
	void Connected();
	void Closed();
	void Message(QString str);

private:
	QTcpSocket* client;
	QHostAddress address;
	int port;
};

#endif