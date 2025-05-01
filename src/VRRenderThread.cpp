#include "VRRenderThread.h"
#include <vtkOpenVRRenderWindowInteractor.h>

VRRenderThread::VRRenderThread(vtkOpenVRRenderWindowInteractor* interactor, QObject* parent)
    : interactor(interactor), QThread(parent){
}
