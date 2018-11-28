#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(Controller* controller,
                        QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAnglesChanged(const Angles angles);
    void onPositionChanged(const Position position);

    void onFakerChanged();

private:
    Ui::MainWindow* m_ui;
    Controller* m_controller;
};

#endif // MAINWINDOW_H
