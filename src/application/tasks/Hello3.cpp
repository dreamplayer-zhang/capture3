#include "Hello3.h"


#include <QDebug>


namespace Capture3
{

	Hello3::Hello3() : QThread(),
		abort(false),
		restart(false),
		cameraModel("Nikon DSC D3300"),
		cameraPort("usb:")
	{
		qDebug() << "Hello3::Hello3()" << QThread::currentThread();
	}


	Hello3::~Hello3()
	{
		qDebug() << "Hello3::~Hello3()" << QThread::currentThread();

		gp_port_info_list_free(cameraPortInfoList);
		gp_abilities_list_free(cameraAbilitiesList);
		gp_camera_exit(camera, context);
		gp_camera_unref(camera);
		gp_context_unref(context);
		camera = nullptr;
		context = nullptr;

		mutex.lock();
		condition.wakeOne();
		abort = true;
		mutex.unlock();
		wait();
	}

	void Hello3::render(const QString &test)
	{
		qDebug() << "Hello3::render()" << QThread::currentThread();

		QMutexLocker locker(&mutex);

		this->test = test;

		if (!isRunning()) {
			start(LowPriority);
		} else {
			restart = true;
			condition.wakeOne();
		}
	}


	void Hello3::run()
	{
		qDebug() << "Hello3::run()" << QThread::currentThread();

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

			// Kill any USB processes
			system("killall PTPCamera");

			// Create new camera object
			gp_camera_new(&camera);

			// Create camera context
			context = gp_context_new();

			// Find port
			gp_port_info_list_new(&cameraPortInfoList);
			gp_port_info_list_load(cameraPortInfoList);
			indexPort = gp_port_info_list_lookup_path(cameraPortInfoList, cameraPort.toLatin1().constData());

			qDebug() << "indexPort" << indexPort << QThread::currentThread();

			// Find port information
			gp_port_info_new(&cameraPortInfo);
			gp_port_info_list_get_info(cameraPortInfoList, indexPort, &cameraPortInfo);

			// Find port name and path
			gp_port_info_get_name(cameraPortInfo, &cameraPortInfoName);
			gp_port_info_get_path(cameraPortInfo, &cameraPortInfoPath);

			// Find camera model
			gp_abilities_list_new(&cameraAbilitiesList);
			gp_abilities_list_load(cameraAbilitiesList, context);
			indexModel = gp_abilities_list_lookup_model(cameraAbilitiesList, cameraModel.toLatin1().constData());

			qDebug() << "indexModel" << indexModel << QThread::currentThread();

			// Find camera model options
			gp_abilities_list_get_abilities(cameraAbilitiesList, indexModel, &cameraAbilities);
			gp_camera_set_port_info(camera, cameraPortInfo);
			gp_camera_set_abilities(camera, cameraAbilities);

			qDebug() << "before" << QThread::currentThread();

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