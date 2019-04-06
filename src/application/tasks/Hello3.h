#ifndef CAPTURE3_HELLO3_H
#define CAPTURE3_HELLO3_H


#include <vector>
#include <cmath>
#include <omp.h>
#include <gphoto2/gphoto2-camera.h>
#include <gphoto2/gphoto2-context.h>
#include <gphoto2/gphoto2-widget.h>
#include <gphoto2/gphoto2-port.h>
#include <gphoto2/gphoto2-port-info-list.h>
#include <gphoto2/gphoto2-abilities-list.h>
#include <gphoto2/gphoto2-file.h>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>


namespace Capture3
{
	class Hello3 final : public QThread
	{
		Q_OBJECT

		public:
			Hello3();

			virtual ~Hello3();

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

			const QString cameraModel;
			const QString cameraPort;
			Camera *camera;
			GPContext *context;
			GPPortInfoList *cameraPortInfoList;
			GPPortInfo cameraPortInfo;
			CameraAbilitiesList *cameraAbilitiesList;
			CameraAbilities cameraAbilities;
			char *cameraPortInfoName;
			char *cameraPortInfoPath;
			int indexPort;
			int indexModel;
	};
}


#endif // CAPTURE3_HELLO3_H
