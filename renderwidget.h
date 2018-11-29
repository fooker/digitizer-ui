#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector2D>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLTexture>

#include "controller.h"

class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

    Q_PROPERTY(QVector3D camera READ camera WRITE setCamera)
    Q_PROPERTY(bool flatten READ flatten WRITE setFlatten)
    Q_PROPERTY(Projection projection READ projection WRITE setProjection)


    static constexpr const float GRID_DIST = 10.0;
    static constexpr const int GRID_COUNT = 20;

public:
    enum Projection {
        PERSPECTIVE,
        ORTHOGONAL,
    };

    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget() override;

    QVector3D camera() const;
    bool flatten() const;
    Projection projection() const;

signals:

public slots:
    void cleanup();

    void setCamera(const QVector3D& camera);
    void setFlatten(bool flatten);
    void setProjection(Projection projection);

    void onAnglesChanged(const Angles& angles);
    void onPositionChanged(const Position& position);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void drawGrid();
    void drawArm();
    void drawPosition();

    Angles m_angles;
    Position m_position;

    QVector3D m_camera;
    bool m_flatten;
    Projection m_projection;

    QPoint m_lastMousePos;

    QOpenGLTexture* m_pointerTexture;
};

#endif // RENDERWIDGET_H
