#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector2D>
#include <QMouseEvent>

#include "controller.h"

class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

    Q_PROPERTY(QVector3D camera READ camera WRITE setCamera)

    static constexpr const float GRID_DIST = 10.0;
    static constexpr const int GRID_COUNT = 20;

public:
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget() override;

    QVector3D camera() const;
    void setCamera(const QVector3D& camera);

signals:

public slots:
    void cleanup();

    void onAnglesChanged(const Angles& angles);
    void onPositionChanged(const Position& position);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    void drawGrid();
    void drawArm();
    void drawPosition();

    Angles m_angles;
    Position m_position;

    QVector3D m_camera;

    QPoint m_lastMousePos;
};

#endif // RENDERWIDGET_H
