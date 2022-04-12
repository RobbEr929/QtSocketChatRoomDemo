#ifndef QTSOCKETSIMPLESERVER_H
#define QTSOCKETSIMPLESERVER_H
#include <QtWidgets/QMainWindow>
#include <QThread>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QListView>
#include <QStringListModel>
#include <QRegExpValidator>
#include "ui_QtSocketSimpleSever.h"
#include "Server.h"
class QtSocketSimpleSever : public QMainWindow
{
	Q_OBJECT

public:
	QtSocketSimpleSever(QWidget* parent = nullptr);

private slots:
	void SetListenPort();
	void InitEvent(bool res);
	void CloseEvent(bool res);
	void AddRowInView(QString str);

signals:
	void BeginInit();
	void BeginClose();

private:
	void InitServer();
	void InitWindow();
	Ui::QtSocketSimpleSeverClass ui;
	Server* server;
	QThread* serverThread;
	QLineEdit* portEdit;
	QPushButton* portButton;
	QListView* view;
	QStringListModel* model;
};
#endif