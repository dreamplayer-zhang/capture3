#include "Hello.h"


#include <QDebug>


namespace Capture3
{

	Hello::Hello(const QString &test) :
		QObject(),
		QRunnable(),
		test(test)
	{
		qDebug() << "Hello::Hello()" << QThread::currentThread();
	}


	Hello::~Hello()
	{
		qDebug() << "Hello::~Hello()" << QThread::currentThread();
	}


	void Hello::run()
	{
		qDebug() << "Hello::run()" << test << QThread::currentThread();

		emit done(test, this);
	}

}