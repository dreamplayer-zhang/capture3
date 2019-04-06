#ifndef CAPTURE3_HELLO_H
#define CAPTURE3_HELLO_H


#include <QtCore/QObject>
#include <QtCore/QRunnable>
#include <QtCore/QThread>


namespace Capture3
{
	class Hello final : public QObject, public QRunnable
	{
		Q_OBJECT

		public:
			Hello(const QString &test);

			virtual ~Hello();

			virtual void run();

		signals:

			void done(const QString &test, Hello *hello);

		private:
			QString test;

	};
}


#endif // CAPTURE3_HELLO_H
