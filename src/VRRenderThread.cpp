#include "VRRenderThread.h"
#include <vtkOpenVRRenderWindowInteractor.h>

VRRenderThread::VRRenderThread(vtkOpenVRRenderWindowInteractor* interactor, QObject* parent)
    : QThread(parent), interactor(interactor) {
}

void VRRenderThread::run() {
    while (!isInterruptionRequested()) {
        if (interactor) {
            interactor->ProcessEvents();  // Process VR interactor events
        }
        QThread::msleep(16); // ~60 FPS (1000ms / 60 = ~16ms)
    }
}