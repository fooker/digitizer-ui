#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSpinBox>

MainWindow::MainWindow(Controller* controller,
                       QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_controller(controller) {

    this->m_ui->setupUi(this);

    connect(this->m_controller, SIGNAL(anglesChanged(Angles)), this, SLOT(onAnglesChanged(Angles)));
    connect(this->m_controller, SIGNAL(positionChanged(Position)), this, SLOT(onPositionChanged(Position)));

    connect(this->m_controller, SIGNAL(anglesChanged(Angles)), this->m_ui->renderWidget, SLOT(onAnglesChanged(Angles)));
    connect(this->m_controller, SIGNAL(positionChanged(Position)), this->m_ui->renderWidget, SLOT(onPositionChanged(Position)));

    connect(this->m_ui->aFakerSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFakerChanged()));
    connect(this->m_ui->bFakerSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFakerChanged()));
    connect(this->m_ui->cFakerSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFakerChanged()));
    connect(this->m_ui->dFakerSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFakerChanged()));
}

MainWindow::~MainWindow() {
    delete this->m_ui;
}

void MainWindow::onPositionChanged(const Position position) {
    this->m_ui->xDroLcdNumber->display(position.x);
    this->m_ui->yDroLcdNumber->display(position.y);
    this->m_ui->zDroLcdNumber->display(position.z);
}

void MainWindow::onAnglesChanged(const Angles angles) {
    this->m_ui->aDroLcdNumber->display(angles.a);
    this->m_ui->bDroLcdNumber->display(angles.b);
    this->m_ui->cDroLcdNumber->display(angles.c);
    this->m_ui->dDroLcdNumber->display(angles.d);
}

void MainWindow::onFakerChanged() {
    this->m_controller->process(this->m_ui->aFakerSpinBox->value(),
                                this->m_ui->bFakerSpinBox->value(),
                                this->m_ui->cFakerSpinBox->value(),
                                this->m_ui->dFakerSpinBox->value());
}
