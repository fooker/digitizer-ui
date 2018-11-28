#include "renderwidget.h"

#include <QtMath>
#include <QSurfaceFormat>

RenderWidget::RenderWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_angles(),
    m_position(),
    m_camera(30.0f, 45.0f, 500.0f) {

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(4);
    this->setFormat(format);
}

RenderWidget::~RenderWidget() {
    this->cleanup();
}

QVector3D RenderWidget::camera() const {
    return this->m_camera;
}

void RenderWidget::setCamera(const QVector3D& camera) {
    this->m_camera = camera;
    this->update();
}

void RenderWidget::cleanup() {
}

void RenderWidget::onAnglesChanged(const Angles& angles) {
    this->m_angles = angles;
    this->update();
}

void RenderWidget::onPositionChanged(const Position& position) {
    this->m_position = position;
    this->update();
}

void RenderWidget::initializeGL() {
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &RenderWidget::cleanup);

    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void RenderWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);

    // Position Camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -this->m_camera.z());
    glRotatef(this->m_camera.y(), -1.0f, 0.0f, 0.0f);
    glRotatef(this->m_camera.x(), 0.0f, 0.0f, 1.0f);

    this->drawGrid();
    this->drawArm();
    this->drawPosition();
}

void RenderWidget::resizeGL(int width, int height) {
    // Update projection
    QMatrix4x4 projection;
    projection.perspective(45.0f, GLfloat(width) / GLfloat(height), 0.01f, 1000.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection.data());
}

void RenderWidget::mousePressEvent(QMouseEvent* event) {
    this->m_lastMousePos = event->pos();
}

void RenderWidget::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::RightButton) {
        int dx = event->x() - this->m_lastMousePos.x();
        int dy = event->y() - this->m_lastMousePos.y();

        qDebug() << "ROT:" << dx << dy;

        this->m_camera += QVector2D(float(dx) / 10.0f, float(dy) / -10.0f);
        this->update();

        this->m_lastMousePos = event->pos();
    }
}

void RenderWidget::drawGrid() {
    glLineWidth(0.8f);
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_LINES);

    for (int i = -GRID_COUNT; i <= GRID_COUNT; i++) {
        glVertex3f(i * GRID_DIST, -1 * GRID_DIST * GRID_COUNT, 0.0f);
        glVertex3f(i * GRID_DIST, +1 * GRID_DIST * GRID_COUNT, 0.0f);
        glVertex3f(-1 * GRID_DIST * GRID_COUNT, i * GRID_DIST, 0.0f);
        glVertex3f(+1 * GRID_DIST * GRID_COUNT, i * GRID_DIST, 0.0f);
    }

    glEnd();
}
void RenderWidget::drawArm() {
    // Calculate arm vectors
    const float ra = static_cast<float>(this->m_angles.a) * 360.0f / Controller::STEPS_PRE_REVOLUTION;
    const float rb = static_cast<float>(this->m_angles.b) * 360.0f / Controller::STEPS_PRE_REVOLUTION;
    const float rc = static_cast<float>(this->m_angles.c) * 360.0f / Controller::STEPS_PRE_REVOLUTION;
    const float rd = static_cast<float>(this->m_angles.d) * 360.0f / Controller::STEPS_PRE_REVOLUTION;

    QMatrix4x4 m;

    m.rotate(ra, QVector3D(0.0f, 0.0f, 1.0f));
    m.translate(0.0f, 0.0f, Controller::LENGTH_BASE);
    const QVector3D arm_base = m * QVector3D();

    m.rotate(rb, QVector3D(1.0f, 0.0f, 0.0f));
    m.translate(0.0f, 0.0f, Controller::LENGTH_FIRST);
    const QVector3D arm_first = m * QVector3D();

    m.rotate(rc, QVector3D(1.0f, 0.0f, 0.0f));
    m.translate(0.0f, 0.0f, Controller::LENGTH_SECOND);
    const QVector3D arm_second = m * QVector3D();

    m.rotate(rd, QVector3D(1.0f, 0.0f, 0.0f));
    m.translate(0.0f, 0.0f, Controller::LENGTH_NEEDLE);
    const QVector3D arm_needle = m * QVector3D();

    // Draw
    glLineWidth(5.3f);
    glColor3f(0.0f, 0.5f, 0.8f);

    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(arm_base.x(), arm_base.y(), arm_base.z());

    glVertex3f(arm_base.x(), arm_base.y(), arm_base.z());
    glVertex3f(arm_first.x(), arm_first.y(), arm_first.z());

    glVertex3f(arm_first.x(), arm_first.y(), arm_first.z());
    glVertex3f(arm_second.x(), arm_second.y(), arm_second.z());

    glVertex3f(arm_second.x(), arm_second.y(), arm_second.z());
    glVertex3f(arm_needle.x(), arm_needle.y(), arm_needle.z());
    glEnd();
}

void RenderWidget::drawPosition() {
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);

    glVertex3d(this->m_position.x + 1.3, this->m_position.y, this->m_position.z);
    glVertex3d(this->m_position.x - 1.3, this->m_position.y, this->m_position.z);

    glVertex3d(this->m_position.x, this->m_position.y + 1.3, this->m_position.z);
    glVertex3d(this->m_position.x, this->m_position.y - 1.3, this->m_position.z);

    glVertex3d(this->m_position.x, this->m_position.y, this->m_position.z + 1.3);
    glVertex3d(this->m_position.x, this->m_position.y, this->m_position.z - 1.3);

    glEnd();
}
