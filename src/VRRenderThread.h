#ifndef VRRENDERTHREAD_H
#define VRRENDERTHREAD_H

#include <QThread>
#include <vtkSmartPointer.h>

class vtkOpenVRRenderWindowInteractor;

class VRRenderThread : public QThread {
    Q_OBJECT

public:
    explicit VRRenderThread(vtkOpenVRRenderWindowInteractor* interactor, QObject* parent = nullptr);
    void run() override;

private:
    vtkOpenVRRenderWindowInteractor* interactor; // raw pointer; assumed managed externally
};

#endif // VRRENDERTHREAD_H
