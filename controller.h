#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QSerialPort>

struct Angles {
    quint16 a;
    quint16 b;
    quint16 c;
    quint16 d;
};

struct Position {
    qreal x;
    qreal y;
    qreal z;
};

class Controller : public QObject{
    Q_OBJECT

    Q_PROPERTY(bool running
               READ running
               NOTIFY runningChanged)

    Q_PROPERTY(Angles angles
               READ angles
               NOTIFY anglesChanged)

    Q_PROPERTY(Position position
               READ position
               NOTIFY positionChanged)

public:
    static constexpr const quint16 STEPS_PRE_REVOLUTION = 600;

    static constexpr const qreal LENGTH_BASE = 72.5;
    static constexpr const qreal LENGTH_FIRST = 230.0;
    static constexpr const qreal LENGTH_SECOND = 230.0;
    static constexpr const qreal LENGTH_NEEDLE = 9.5 + 125.0;

    explicit Controller(QSerialPort* serialPort);

    bool running() const;
    Angles angles() const;
    Position position() const;

signals:
    void runningChanged(bool ok);
    void anglesChanged(Angles Angles);
    void positionChanged(Position position);

public slots:
    void process(quint16 a, quint16 b, quint16 c, quint16 d);

private slots:
    void onError(QSerialPort::SerialPortError);
    void onReadReady();

private:
    bool m_running;
    Angles m_angles;
    Position m_position;

    QSerialPort* m_serialPort;
};

#endif // CONTROLLER_H
