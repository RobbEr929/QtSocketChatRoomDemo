#include <QtWidgets/QApplication>
#include <QDir>
#include <QTextCodec>
#include "easylogging++.h"
#include "QtSocketSimpleSever.h"

INITIALIZE_EASYLOGGINGPP;

int main(int argc, char* argv[])
{
	system("chcp 65001");
	QApplication a(argc, argv);

	START_EASYLOGGINGPP(argc, argv);

	QString logPath = QCoreApplication::applicationDirPath() % "/logs";
	QString logName = QCoreApplication::applicationDirPath() % "/logs/log_%datetime{%Y%M%d}.log";
	QDir dir;
	if (!dir.exists(logPath))
		dir.mkdir(logPath);

	el::Configurations conf;
	conf.set(el::Level::Global, el::ConfigurationType::Format, "%datetime %msg");
	conf.set(el::Level::Global, el::ConfigurationType::Filename, logName.toStdString());
	conf.set(el::Level::Global, el::ConfigurationType::Enabled, "true");
	conf.set(el::Level::Global, el::ConfigurationType::ToFile, "true");
	conf.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, "true");
	conf.set(el::Level::Global, el::ConfigurationType::SubsecondPrecision, "0");
	conf.set(el::Level::Global, el::ConfigurationType::PerformanceTracking, "true");
	conf.set(el::Level::Global, el::ConfigurationType::MaxLogFileSize, "1024000");
	conf.set(el::Level::Global, el::ConfigurationType::LogFlushThreshold, "1");
	el::Loggers::reconfigureAllLoggers(conf);

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	QtSocketSimpleSever w;
	w.show();
	return a.exec();
}