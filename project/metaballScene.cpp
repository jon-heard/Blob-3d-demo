
#include "metaballScene.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include "mesh_sphere.h"
#include "mesh_metaball.h"
#include <QDebug>
#include <QListWidget>

using namespace std;

MetaballScene::MetaballScene(QWidget *parent)
    : QOpenGLWidget(parent),
      _isRenderingMetaballs(false), zoom(-5) {}

MetaballScene::~MetaballScene()
{
    delete blob;
    delete shader_basic;
}

QListWidget* MetaballScene::list() const
{
    return _list;
}

void MetaballScene::setList(QListWidget* value)
{
    _list = value;
}


Mesh_Sphere* MetaballScene::makeSphere() const
{
    shader_basic->bind();
    Mesh_Sphere* result = new Mesh_Sphere(shader_basic);
    shader_basic->release();
    return result;
}

void MetaballScene::updateBlob()
{
    blob->setDirty(true);
}

bool MetaballScene::isRenderingMetaballs() const
{
    return _isRenderingMetaballs;
}

void MetaballScene::setIsRenderingMetaballs(bool value)
{
    _isRenderingMetaballs = value;
    repaint();
}

void MetaballScene::initializeGL()
{
    glClearColor(.25,.25,.25,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    sceneTransform.setToIdentity();
    sceneTransform.translate(0, 0, -5);

    shader_basic = new QOpenGLShaderProgram();
    shader_basic->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex_basic.vert");
    shader_basic->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/frag_basic.frag");
    shader_basic->link();

    shader_basic->bind();
    blob = new Mesh_Metaball(shader_basic);
    blob->setList(_list);
    Mesh_Sphere* sphere = new Mesh_Sphere(shader_basic);
    delete sphere;
    shader_basic->release();
}

void MetaballScene::paintGL()
{
    if (blob->dirty())
    {
        blob->setDirty(false);
        shader_basic->bind();
        blob->genMesh_Metaball();
        shader_basic->release();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_list != NULL)
    {
        shader_basic->bind();
        shader_basic->setUniformValue("projectionTransform", projectionTransform);
        shader_basic->setUniformValue("sceneTransform", sceneTransform);
        if (_isRenderingMetaballs)
        {
            blob->draw();
        } else {
            for (int i = 0; i < _list->count(); ++i)
            {
                ((Mesh_Sphere*)_list->item(i))->draw();
            }
        }
        shader_basic->release();
    }
}

void MetaballScene::resizeGL(int w, int h)
{
    projectionTransform.setToIdentity();
    projectionTransform.perspective(45, (float)w/h, 0.001f, 1000.0f);
}

void MetaballScene::mousePressEvent(QMouseEvent *event)
{
    previousPos = event->pos();
    previousRotation = rotation;
}

void MetaballScene::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        rotation.setY(previousRotation.y() + (event->x() - previousPos.x()));
        rotation.setX(previousRotation.x() + (event->y() - previousPos.y()));
        refreshSceneTransform();
    }
}

void MetaballScene::wheelEvent(QWheelEvent* event)
{
    zoom += event->delta() / 120;
    if (zoom > -2) { zoom = -2; }
    refreshSceneTransform();
}

void MetaballScene::refreshSceneTransform()
{
    sceneTransform.setToIdentity();
    sceneTransform.translate(0, 0, zoom);
    sceneTransform.rotate(rotation.x(), 1, 0, 0);
    sceneTransform.rotate(rotation.y(), 0, 1, 0);
    repaint();
}
