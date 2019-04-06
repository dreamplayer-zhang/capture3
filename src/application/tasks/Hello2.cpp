#include "Hello2.h"


#include <QDebug>


namespace Capture3
{

	Hello2::Hello2() : QThread(),
		abort(false),
		restart(false)
	{
		qDebug() << "Hello2::Hello2()" << QThread::currentThread();
	}


	Hello2::~Hello2()
	{
		qDebug() << "Hello2::~Hello2()" << QThread::currentThread();

		mutex.lock();
		condition.wakeOne();
		abort = true;
		mutex.unlock();
		wait();
	}

	void Hello2::render(const QString &test)
	{
		qDebug() << "Hello2::render()" << QThread::currentThread();

		QMutexLocker locker(&mutex);

		this->test = test;

		if (!isRunning()) {
			start(LowPriority);
		} else {
			restart = true;
			condition.wakeOne();
		}
	}


	void Hello2::run()
	{
		qDebug() << "Hello2::run()" << QThread::currentThread();

		forever {
			mutex.lock();
			const QString test = this->test;
			mutex.unlock();

			if (restart) {
				break;
			}

			if (abort) {
				return;
			}

			// TODO: Test if OMP adds any value in this case
			if (processor.open_file(test.toLatin1().constData()) == LIBRAW_SUCCESS) {
				processor.unpack();

				qDebug() << "inputWidth" << processor.imgdata.sizes.width;
				qDebug() << "inputHeight" << processor.imgdata.sizes.height;
				qDebug() << "rangeMin" << processor.imgdata.color.black;
				qDebug() << "rangeMax" << processor.imgdata.color.maximum;

				// Fetch image information
				const unsigned short *raw = processor.imgdata.rawdata.raw_image;
				const unsigned int inputWidth = processor.imgdata.sizes.width;
				const unsigned int inputHeight = processor.imgdata.sizes.height;
				const unsigned int rangeMin = processor.imgdata.color.black;
				const unsigned int rangeMax = processor.imgdata.color.maximum;
				const unsigned int range = rangeMax - rangeMin;

				// Create new image
				cv::Mat image(cv::Size(3008, 2008), CV_64FC3, cv::Scalar(0));
				double *imageData = (double *) image.data;

				#pragma omp parallel for schedule(static) collapse(2)
				for (unsigned int y = 0; y < 2008; y++) {
					for (unsigned int x = 0; x < 3008; x++) {

						// Calculate position
						const unsigned int col = x * 2;
						const unsigned int row = y * 2;

						// Calculate Bayer positions:
						// Row 1: [R, G]
						// Row 2: [B, G]
						const unsigned int indexR = (inputWidth * row) + col;
						const unsigned int indexG1 = (inputWidth * row) + col + 1;
						const unsigned int indexG2 = (inputWidth * (row + 1)) + col;
						const unsigned int indexB = (inputWidth * (row + 1)) + col + 1;

						// Calculate output positions
						const unsigned int index = (y * 3008 + x) * 3;

						// Fetch colors
						double colorR = (double) raw[indexR];
						double colorG1 = (double) raw[indexG1];
						double colorG2 = (double) raw[indexG2];
						double colorB = (double) raw[indexB];
						double colorG = (colorG1 + colorG2) / 2.0;

						// Map to 0-1 range
						colorR = (colorR - rangeMin) / range;
						colorG = (colorG - rangeMin) / range;
						colorB = (colorB - rangeMin) / range;

						// Cap to 0-1 range
						colorR = colorR < 0 ? 0 : colorR > 1 ? 1 : colorR;
						colorG = colorG < 0 ? 0 : colorG > 1 ? 1 : colorG;
						colorB = colorB < 0 ? 0 : colorB > 1 ? 1 : colorB;

						// Store values
						imageData[index + 0] = colorR;
						imageData[index + 1] = colorG;
						imageData[index + 2] = colorB;
					}
				}

				processor.free_image();
				processor.recycle();
			}

			qDebug() << test << QThread::currentThread();

			if (!restart) {
				emit done(test);
			}

			mutex.lock();
			if (!restart) {
				condition.wait(&mutex);
			}
			restart = false;
			mutex.unlock();
		}
	}
}