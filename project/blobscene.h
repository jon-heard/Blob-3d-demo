#ifndef BLOBSCENE_H
#define BLOBSCENE_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <vector>

class QOpenGLShaderProgram;
class QListWidget;
class Mesh_Sphere;
class Mesh_Metaball;

class BlobScene : public QOpenGLWidget
{
public:
    BlobScene(QWidget *parent = 0);
    ~BlobScene();
    QListWidget* list() const;
    void setList(QListWidget* value);
    Mesh_Sphere* makeSphere() const;
    void updateBlob();
    bool isRenderingMetaballs() const;
    void setIsRenderingMetaballs(bool value);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void refreshSceneTransform();
private:
    // Projection transform
    QMatrix4x4 projectionTransform;

    // Scene transform
    QMatrix4x4 sceneTransform;
    float zoom;
    QPoint rotation;
    QPoint previousRotation;
    QPoint previousPos;

    // Meshes
    Mesh_Metaball* blob;

    // Shaders
    QOpenGLShaderProgram* shader_basic;

    // UI
    QListWidget* _list;
    bool _isRenderingMetaballs;
};

#endif // BLOBSCENE_H
