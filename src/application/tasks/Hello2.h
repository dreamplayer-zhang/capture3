#ifndef CAPTURE3_HELLO2_H
#define CAPTURE3_HELLO2_H


#include <vector>
#include <cmath>
#include <omp.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <libraw/libraw.h>
#include <libraw/libraw_types.h>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>


namespace Capture3
{
	class Hello2 final : public QThread
	{
		Q_OBJECT

		public:
			Hello2();

			virtual ~Hello2();

			void render(const QString &test);

		signals:
			void done(const QString &test);

		protected:
			void run() override;

		private:
			QMutex mutex;
			QWaitCondition condition;
			QString test;
			bool abort;
			bool restart;
			LibRaw processor;
	};
}


#endif // CAPTURE3_HELLO2_H
