#include "VRRenderThread.h"
#include <vtkOpenVRRenderWindowInteractor.h>

VRRenderThread::VRRenderThread(vtkOpenVRRenderWindowInteractor* interactor, QObject* parent)
    : QThread(parent), interactor(interactor) {
}

void VRRenderThread::run() {
    if (interactor) {
        interactor->Start(); // Blocking VR render loop
    }
}
