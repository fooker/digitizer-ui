#include "controller.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("digitizer");
    QApplication::setApplicationVersion("v0.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Digitizer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(QCommandLineOption(QStringList() << "p" << "port",
                                        QCoreApplication::translate("main", "The serial port the digitizer is attached to."),
                                        "port", "/dev/digitizer"));
    parser.process(app);

    //const QString serialPortName = parser.value("port");

    //QSerialPort serialPort;
//    serialPort.setPortName(serialPortName);
//    serialPort.setBaudRate(115200);
//    if (!serialPort.open(QIODevice::ReadOnly)) {
//        qCritical() << QObject::tr("Failed to open port %1, error: %2")
//                          .arg(serialPortName)
//                          .arg(serialPort.errorString());
//        return 1;
//    }

//    Controller controller(&serialPort);
    Controller controller(nullptr);

    MainWindow w(&controller);
    w.show();

    return app.exec();
}
