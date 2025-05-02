#ifndef VRRENDERTHREAD_H
#define VRRENDERTHREAD_H

#include <QThread>
#include <vtkSmartPointer.h>

class vtkOpenVRRenderWindowInteractor;

class VRRenderThread : public QThread {
    Q_OBJECT

public:
    explicit VRRenderThread(vtkOpenVRRenderWindowInteractor* interactor, QObject* parent = nullptr);

protected:
    void run() override;  // Main thread loop

private:
    vtkOpenVRRenderWindowInteractor* interactor; // Managed externally
};

#endif // VRRENDERTHREAD_H