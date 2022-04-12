#ifndef QTSOCKETSIMPLECLIENT_H
#define QTSOCKETSIMPLECLIENT_H

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <QListView>
#include <QStringListModel>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QThread>
#include <QTextEdit>
#include "ui_QtSocketSimpleClient.h"
#include "Client.h"
class QtSocketSimpleClient : public QMainWindow
{
	Q_OBJECT
public:
	QtSocketSimpleClient(QWidget* parent = nullptr);

private slots:
	void SetHostAndPort();
	void BeginConversation();
	void SendOutMessage();
	void AddRowInView(QString str);

signals:
	void BeginInit();
	void Send(QString str);

private:
	void InitClient();
	void InitWindow();
	Ui::QtSocketSimpleClientClass ui;
	Client* client;
	QThread* clientThread;
	QLineEdit* addrEdit, * portEdit;
	QPushButton* clientButton, * sendButton;
	QStringListModel* model;
	QListView* view;
	QTextEdit* inputEdit;
};

#endif