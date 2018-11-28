#include "controller.h"
#include <QDebug>
#include <QTimer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QtEndian>
#include <QtMath>

struct Packet {
    quint8 preamble;

    quint16 a;
    quint16 b;
    quint16 c;
    quint16 d;

    quint8 epilogue;
} __attribute__ ((packed));

Controller::Controller(QSerialPort* serialPort) :
    QObject(),
    m_running(true),
    m_angles(Angles {0,0,0,0}),
    m_serialPort(serialPort) {

    if (this->m_serialPort != nullptr) {
        connect(this->m_serialPort, &QSerialPort::errorOccurred, this, &Controller::onError);
        connect(this->m_serialPort, &QSerialPort::readyRead, this, &Controller::onReadReady);
    }
}

bool Controller::running() const {
    return this->m_running;
}

Angles Controller::angles() const {
    return this->m_angles;
}

Position Controller::position() const {
    return this->m_position;
}

void Controller::process(quint16 a, quint16 b, quint16 c, quint16 d) {
    this->m_angles.a = a;
    this->m_angles.b = b;
    this->m_angles.c = c;
    this->m_angles.d = d;

    qDebug() << "V:" << a << b << c << d;

    const qreal ra = qreal(a) * M_PI / STEPS_PRE_REVOLUTION * 2;
    const qreal rb = qreal(b) * M_PI / STEPS_PRE_REVOLUTION * 2;
    const qreal rc = qreal(c) * M_PI / STEPS_PRE_REVOLUTION * 2;
    const qreal rd = qreal(d) * M_PI / STEPS_PRE_REVOLUTION * 2;

    qDebug() << "R:" << ra << rb << rc << rd;

    const qreal r = qSin(rb) * LENGTH_FIRST + qSin(rb + rc) * LENGTH_SECOND + qSin(rb + rc + rd) * LENGTH_NEEDLE;
    const qreal h = qCos(rb) * LENGTH_FIRST + qCos(rb + rc) * LENGTH_SECOND + qCos(rb + rc + rd) * LENGTH_NEEDLE + LENGTH_BASE;

    qDebug() << "X:" << r << h;

    this->m_position.x = r * qSin(ra);
    this->m_position.y = r * -qCos(ra);
    this->m_position.z = h;

    qDebug() << "P:" << this->m_position.x << this->m_position.y << this->m_position.z;

    emit this->anglesChanged(this->m_angles);
    emit this->positionChanged(this->m_position);
}

void Controller::onError(QSerialPort::SerialPortError) {
    qCritical() << QObject::tr("An I/O error occurred while reading data from port %1, error: %2")
                   .arg(m_serialPort->portName())
                   .arg(m_serialPort->errorString());
    this->m_running = false;
}

void Controller::onReadReady() {
    if (this->m_serialPort->bytesAvailable() >= sizeof(Packet)) {
        const auto buffer = this->m_serialPort->read(sizeof(Packet));
        const Packet* packet = reinterpret_cast<const Packet*>(buffer.data());

        qDebug() << "Packet:" << buffer;

        assert(packet->preamble == 0x55);
        assert(packet->epilogue == 0xAA);

        const quint16 a = qFromBigEndian(packet->a);
        const quint16 b = qFromBigEndian(packet->b);
        const quint16 c = qFromBigEndian(packet->c);
        const quint16 d = qFromBigEndian(packet->d);

        this->process(a, b, c, d);
    }
}
