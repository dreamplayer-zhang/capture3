#include "Application.h"


namespace Capture3 {

    Application::Application() :
            QObject(),
            engine(),
            window(),
            welcomeModule(engine),
            exposureModule(engine),
            ambientModule(engine),
            colorChartModule(engine),
            colorProfileModule(engine),
            captureModule(engine),
            exportModule(engine) {

        // Add modules
        window.add(&welcomeModule);
        window.add(&exposureModule);
        window.add(&ambientModule);
        window.add(&colorChartModule);
        window.add(&colorProfileModule);
        window.add(&captureModule);
        window.add(&exportModule);

        // Load GUI styles
        loadStylesheet();

        // Show window
        updateTitle();
        window.show();

        // auto *hello1 = new Hello("Hello from 1");
        // auto *hello2 = new Hello("Hello from 2");
        // auto *hello3 = new Hello("Hello from 3");
        // hello1->setAutoDelete(false);
        // hello2->setAutoDelete(false);
        // hello3->setAutoDelete(false);
        // QObject::connect(hello1, &Hello::done, this, &Application::done1);
        // QObject::connect(hello2, &Hello::done, this, &Application::done1);
        // QObject::connect(hello3, &Hello::done, this, &Application::done1);
        // QThreadPool *threadPool = QThreadPool::globalInstance();
        // threadPool->start(hello1);
        // threadPool->start(hello2);
        // threadPool->start(hello3);

        // hello21.render("/Users/ferdi/Documents/Capture3/files1/dsc/DSC_0001.NEF");
        // hello22.render("/Users/ferdi/Documents/Capture3/files1/dsc/DSC_0002.NEF");
        // hello23.render("/Users/ferdi/Documents/Capture3/files1/dsc/DSC_0003.NEF");
        // QObject::connect(&hello21, &Hello2::done, this, &Application::done2);
        // QObject::connect(&hello22, &Hello2::done, this, &Application::done2);
        // QObject::connect(&hello23, &Hello2::done, this, &Application::done2);

        // test.render("test model");
    }


    // void Application::done1(const QString &test, Hello *hello)
    // {
    // 	qDebug() << "Application::done1()" << test;
    // 	QObject::disconnect(hello, nullptr, nullptr, nullptr);
    // 	hello->deleteLater();
    // }


    // void Application::done2(const QString &test)
    // {
    // 	qDebug() << "Application::done2()" << test;
    // }


    Application::~Application() {
        window.deleteLater();
        welcomeModule.deleteLater();
        exposureModule.deleteLater();
        ambientModule.deleteLater();
        colorChartModule.deleteLater();
        colorProfileModule.deleteLater();
        captureModule.deleteLater();
        exportModule.deleteLater();

        // QObject::disconnect(&hello21, nullptr, nullptr, nullptr);
        // QObject::disconnect(&hello22, nullptr, nullptr, nullptr);
        // QObject::disconnect(&hello23, nullptr, nullptr, nullptr);
        // hello21.deleteLater();
        // hello22.deleteLater();
        // hello23.deleteLater();
    }


    void Application::updateTitle() {
        // Show current working dir in title
        QStringList title;
        title << APPLICATION_NAME;
        title << " - ";
        title << "[";
        title << QDir::current().currentPath();
        title << "]";
        window.setWindowTitle(title.join(""));
    }


    void Application::loadStylesheet() {
        qApp->setStyleSheet(
                readString(FILE_STYLE)
        );
    }
}