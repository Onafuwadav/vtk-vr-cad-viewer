/**		@file VRRenderThread.cpp
  *
  *		EEEE2046 - Software Engineering & VR Project
  *
  *		Template to add VR rendering to your application.
  *
  *		P Evans 2022
  */

#include "VRRenderThread.h"
#include <QDebug>

  /* Vtk headers */
#include <vtkActor.h>
#include <vtkOpenVRRenderWindow.h>				
#include <vtkOpenVRRenderWindowInteractor.h>	
#include <vtkOpenVRRenderer.h>					
#include <vtkOpenVRCamera.h>	

#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkNamedColors.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSTLReader.h>
#include <vtkDataSetmapper.h>
#include <vtkCallbackCommand.h>

/* The class constructor is called by MainWindow and runs in the primary program thread, this thread
 * will go on to handle the GUI (mouse clicks, etc). The OpenVRRenderWindowInteractor cannot be start()ed
 * in the constructor, as it will take control of the main thread to handle the VR interaction (headset
 * rotation etc. This means that a second thread is needed to handle the VR.
 */
VRRenderThread::VRRenderThread(QObject* parent) {
	/* Initialise actor list */
	actors = vtkActorCollection::New();

	/* Initialise command variables */
	rotateX = 0.;
	rotateY = 0.;
	rotateZ = 0.;
}


/* Standard destructor - this is important here as the class will be destroyed when the user
 * stops the VR thread, and recreated when the user starts it again. If class variables are
 * not deallocated properly then there will be a memory leak, where the program's total memory
 * usage will increase for each start/stop thread cycle.
 */
VRRenderThread::~VRRenderThread() {
	if (actors) {
		actors->Delete();
		actors = nullptr;
	}
}

void VRRenderThread::addActorOffline(const QString& filePath) {

	/* Check to see if render thread is running */
	if (!this->isRunning()) {
		auto reader = vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName(filePath.toStdString().c_str());
		reader->Update();

		// Create mapper in the VR thread
		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(reader->GetOutputPort());

		// Create actor in the VR thread
		auto actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);

		double* ac = actor->GetOrigin();

		/* I have found that these initial transforms will position the FS
		 * car model in a sensible position but you can experiment
		 */
		actor->RotateX(-90);
		actor->AddPosition(-ac[0] + 0, -ac[1] - 100, -ac[2] - 200);

		actors->AddItem(actor);
	}
}

void VRRenderThread::removeViwerProps() {
	vrrenderer->RemoveAllViewProps();
}

void VRRenderThread::reset() {
	vrrenderer->Render();
	vrwindow->Render();
	vrrenderer->ResetCamera();
}

void VRRenderThread::addActors() {
	/* Loop through list of actors provided and add to scene */
	actors->InitTraversal();
	vtkActor* currentActor = nullptr;
	while ((currentActor = actors->GetNextActor())) {      
		vrrenderer->AddActor(currentActor);
	}
}

void VRRenderThread::issueCommand(int cmd, double value) {

	/* Update class variables according to command */
	switch (cmd) {
		/* These are just a few basic examples */
	case END_RENDER:
		this->endRender = true;
		break;

	case ROTATE_X:
		this->rotateX = value;
		break;

	case ROTATE_Y:
		this->rotateY = value;
		break;

	case ROTATE_Z:
		this->rotateZ = value;
		break;
	}
}

/* This function runs in a separate thread. This means that the program
 * can fork into two separate execution paths. This thread is triggered by
 * calling VRRenderThread::start()
 */
void VRRenderThread::run() {
    vtkNew<vtkNamedColors> colors;

    // Set the background color
    std::array<unsigned char, 4> bkg{ {26, 51, 102, 255} };
    colors->SetColor("BkgColor", bkg.data());

    // The renderer generates the image which is then displayed on the render window.
    vrrenderer = vtkOpenVRRenderer::New();
    vrrenderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

    // Add actors to the scene
    addActors();

    // Create the VR render window
    vrwindow = vtkOpenVRRenderWindow::New();
    vrwindow->Initialize();
    vrwindow->AddRenderer(vrrenderer);

    // Create OpenVR Camera
    vrcamera = vtkOpenVRCamera::New();
    vrrenderer->SetActiveCamera(vrcamera);

    // Create the render window interactor
    vrinteractor = vtkOpenVRRenderWindowInteractor::New();
    vrinteractor->SetRenderWindow(vrwindow);
    vrinteractor->Initialize();
    vrwindow->Render();

    // Start the VR rendering loop
    endRender = false;
    t_last = std::chrono::steady_clock::now();

    while (!vrinteractor->GetDone() && !this->endRender) {
        vrinteractor->DoOneEvent(vrwindow, vrrenderer);

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - t_last)
            .count() > 20) {

            // Perform animation or actor transformations
            vtkActorCollection* actorList = vrrenderer->GetActors();
            vtkActor* a;

            // X Rotation
            actorList->InitTraversal();
            while ((a = (vtkActor*)actorList->GetNextActor())) {
                a->RotateX(rotateX);
            }

            // Y Rotation
            actorList->InitTraversal();
            while ((a = (vtkActor*)actorList->GetNextActor())) {
                a->RotateY(rotateY);
            }

            // Z Rotation
            actorList->InitTraversal();
            while ((a = (vtkActor*)actorList->GetNextActor())) {
                a->RotateZ(rotateZ);
            }

            // Update the last time
            t_last = std::chrono::steady_clock::now();
        }
    }

    // Clean up OpenGL resources
}

void VRRenderThread::stopAndReset() {
	// Set the endRender flag to true to stop the main loop
	endRender = true;

	// Remove all actors from the renderer and delete the renderer
	if (vrrenderer) {
		vrrenderer->RemoveAllViewProps();
		// renderer = nullptr;
	}

	// Ensure the interactor is stopped and cleaned up
	if (vrinteractor) {
		vrinteractor->TerminateApp();
		vrinteractor = nullptr;
	}

	// Clean up the render window
	if (vrwindow) {
		vrwindow->Finalize(); // Close any VR-related resources
		vrwindow = nullptr;
	}

	
}
