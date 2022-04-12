#include "QtSocketSimpleSever.h"

QtSocketSimpleSever::QtSocketSimpleSever(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->resize(600, 400);
	this->setWindowTitle(QStringLiteral("TCP�����"));
	InitWindow();
	InitServer();
}

void QtSocketSimpleSever::InitEvent(bool res)
{
	if (!res)
	{
		QMessageBox::information(this, "Init Server Error", QStringLiteral("��ʼ��ʧ�ܣ��뻻���˿ڻ�������"), QMessageBox::Yes);
		portButton->setText(QStringLiteral("����"));
		portButton->setEnabled(true);
		portEdit->setEnabled(true);
		AddRowInView(QStringLiteral("����ʧ��"));
	}
	else
	{
		portButton->setText(QStringLiteral("ȡ������"));
		portButton->setEnabled(true);
		AddRowInView(QStringLiteral("���óɹ�"));
	}
}

void QtSocketSimpleSever::CloseEvent(bool res)
{
	portButton->setText(QStringLiteral("����"));
	portButton->setEnabled(true);
	portEdit->setEnabled(true);
	AddRowInView(QStringLiteral("ȡ���ɹ�"));
}

void QtSocketSimpleSever::InitServer()
{
	server = new Server;
	serverThread = new QThread;
	server->moveToThread(serverThread);
	connect(this, &QtSocketSimpleSever::BeginInit, server, &Server::Init);
	connect(this, &QtSocketSimpleSever::BeginClose, server, &Server::Close);
	connect(server, SIGNAL(InitRes(bool)), this, SLOT(InitEvent(bool)));
	connect(server, SIGNAL(CloseRes(bool)), this, SLOT(CloseEvent(bool)));
	connect(server, SIGNAL(ReciveData(QString)), this, SLOT(AddRowInView(QString)));
	serverThread->start();
}

void QtSocketSimpleSever::InitWindow()
{
	QLabel* portLab = new QLabel(this);
	portLab->setText(QStringLiteral("�����˿�"));
	portLab->setFont(QFont("Microsoft YaHei", 10, 50));
	portLab->move(30, 30);

	portEdit = new QLineEdit(this);
	portEdit->setText("929");
	portEdit->resize(80, 30);
	portEdit->move(120, 30);

	portButton = new QPushButton(this);
	portButton->setText(QStringLiteral("����"));
	portButton->resize(120, 30);
	portButton->move(220, 30);
	connect(portButton, &QPushButton::clicked, this, &QtSocketSimpleSever::SetListenPort);

	model = new QStringListModel;
	view = new QListView(this);
	view->resize(540, 300);
	view->move(30, 80);
	view->setModel(model);
	view->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void QtSocketSimpleSever::AddRowInView(QString str)
{
	model->insertRow(model->rowCount());
	auto index = model->index(model->rowCount() - 1, 0);
	model->setData(index, str, Qt::DisplayRole);
}

void QtSocketSimpleSever::SetListenPort()
{
	if (portButton->text().compare(QStringLiteral("����")) == 0)
	{
		portButton->setEnabled(false);
		portEdit->setEnabled(false);
		AddRowInView(QStringLiteral("�������ü����˿�%1").arg(portEdit->text().toUInt()));
		portButton->setText(QStringLiteral("��������"));
		server->SetListenPort(portEdit->text().toUInt());
		emit BeginInit();
	}
	else
	{
		portButton->setEnabled(false);
		portEdit->setEnabled(false);
		AddRowInView(QStringLiteral("����ȡ�������˿�%1").arg(portEdit->text().toUInt()));
		portButton->setText(QStringLiteral("����ȡ��"));
		emit BeginClose();
	}
}