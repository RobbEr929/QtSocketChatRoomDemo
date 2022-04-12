#include "QtSocketSimpleClient.h"

QtSocketSimpleClient::QtSocketSimpleClient(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->resize(600, 400);
	this->setWindowTitle(QStringLiteral("TCP客户端"));
	InitWindow();
	InitClient();
}

void QtSocketSimpleClient::BeginConversation()
{
	LOG(INFO) << QStringLiteral("连接成功");
	AddRowInView(QStringLiteral("连接成功"));
	clientButton->setEnabled(false);
	sendButton->setEnabled(true);
}

void QtSocketSimpleClient::SendOutMessage()
{
	AddRowInView(inputEdit->toPlainText());
	emit Send(inputEdit->toPlainText());
	inputEdit->clear();
};

void QtSocketSimpleClient::SetHostAndPort()
{
	if (clientButton->text().compare("断开连接"))
	{
		if (addrEdit->text().compare("127.0.0.1"))
		{
			client->SetAddress(addrEdit->text());
		}
		if (portEdit->text().compare("929"))
		{
			client->SetListenPort(portEdit->text().toInt());
		}
		clientButton->setEnabled(false);
		addrEdit->setEnabled(false);
		portEdit->setEnabled(false);
		AddRowInView(QStringLiteral("正在连接%1:%2").arg(addrEdit->text()).arg(portEdit->text()));
		clientButton->setText(QStringLiteral("正在连接"));
		emit BeginInit();
	}
	else
	{
		client->DisConnect();
		clientButton->setText(QStringLiteral("正在关闭"));
		AddRowInView(QStringLiteral("正在关闭连接连接"));
	}
}

void QtSocketSimpleClient::AddRowInView(QString str)
{
	model->insertRow(model->rowCount());
	auto index = model->index(model->rowCount() - 1, 0);
	model->setData(index, str, Qt::DisplayRole);
}

void QtSocketSimpleClient::InitClient()
{
	client = new Client;
	clientThread = new QThread;
	client->moveToThread(clientThread);
	clientThread->start();

	connect(this, &QtSocketSimpleClient::BeginInit, client, &Client::Init);
	connect(client, &Client::Connected, this, &QtSocketSimpleClient::BeginConversation);
	connect(client, SIGNAL(Message(QString)), this, SLOT(AddRowInView(QString)));
	connect(this, SIGNAL(Send(QString)), client, SLOT(SendOutMessage(QString)));
}

void QtSocketSimpleClient::InitWindow()
{
	QLabel* hostLab = new QLabel(this);
	hostLab->setText(QStringLiteral("地址"));
	hostLab->setFont(QFont("Microsoft YaHei", 10, 50));
	hostLab->move(30, 30);

	addrEdit = new QLineEdit(this);
	addrEdit->setText("127.0.0.1");
	addrEdit->resize(150, 30);
	addrEdit->move(80, 30);
	QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
	QRegExpValidator* ip = new QRegExpValidator(rx, this);
	addrEdit->setValidator(ip);

	QLabel* portLab = new QLabel(this);
	portLab->setText(QStringLiteral("端口"));
	portLab->setFont(QFont("Microsoft YaHei", 10, 50));
	portLab->move(240, 30);

	portEdit = new QLineEdit(this);
	portEdit->setText("929");
	portEdit->resize(80, 30);
	portEdit->move(300, 30);

	clientButton = new QPushButton(this);
	clientButton->setText(QStringLiteral("连接"));
	clientButton->move(400, 30);
	connect(clientButton, &QPushButton::clicked, this, &QtSocketSimpleClient::SetHostAndPort);

	model = new QStringListModel;
	view = new QListView(this);
	view->resize(540, 200);
	view->move(30, 80);
	view->setModel(model);
	view->setEditTriggers(QAbstractItemView::NoEditTriggers);

	inputEdit = new QTextEdit(this);
	inputEdit->resize(440, 80);
	inputEdit->move(30, 300);

	sendButton = new QPushButton(this);
	sendButton->setText(QStringLiteral("发送"));
	sendButton->resize(80, 80);
	sendButton->move(490, 300);
	sendButton->setEnabled(false);
	connect(sendButton, &QPushButton::clicked, this, &QtSocketSimpleClient::SendOutMessage);
}