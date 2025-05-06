#include <QThread>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>  // Include QMessageBox for displaying message boxes
#include "ModelPart.h"  // Include ModelPart header
#include "ModelPartList.h"  // Include ModelPartList header
#include <QFileDialog>  // Include QFileDialog for file dialogs
#include "optiondialog.h"  // Include Option_Dialog header
#include <QDir>

// VTK Headers
#include <QVTKOpenGLNativeWidget.h>  // Include QVTKOpenGLNativeWidget
#include <vtkSmartPointer.h>  // Include VTK smart pointers
#include <vtkRenderer.h>  // Include VTK renderer
#include <vtkGenericOpenGLRenderWindow.h>  // Include VTK render window
#include <vtkCylinderSource.h>  // Include VTK cylinder source
#include <vtkPolyDataMapper.h>  // Include VTK poly data mapper
#include <vtkActor.h>  // Include VTK actor
#include <vtkNamedColors.h>  // Include VTK named colors
#include <vtkProperty.h>  // Include vtkProperty header
#include <vtkCamera.h>  // Include vtkCamera header

#include <vtkClipPolyData.h>
#include <vtkShrinkPolyData.h>
#include <vtkPlane.h>
#include <vtkPolyData.h>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Connect button click to handleButton() slot
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
        // Create / allocate the ModelPartList
    this->partList = new ModelPartList("PartsList");

    // Link it to the treeview in the GUI
    ui->treeView->setModel(this->partList);

    // Manually create a model tree
    ModelPart* rootItem = this->partList->getRootItem();
    // Create strings for both data columns
    QString name = QString("Model");
    QString visible("true");

    // Create child item
    ModelPart* childItem = new ModelPart({ name, visible });

    // Append to tree top-level
    rootItem->appendChild(childItem);
    // Add 3 top-level items
   /* for (int i = 0; i < 3; i++) {
        // Create strings for both data columns
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        // Create child item
        ModelPart* childItem = new ModelPart({ name, visible });

        // Append to tree top-level
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart* childChildItem = new ModelPart({ name, visible });

            // Append to parent

            childItem->appendChild(childChildItem);

        }

    }*/
    // Connect the custom signal to QStatusBar’s showMessage() slot
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    // Connect the treeView's clicked signal to the handleTreeClicked slot
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    // Connect the edit button's clicked signal to the handleEditButton slot
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleEditButton);
    // Set the context menu policy for the treeView
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    // Add the action to the treeView's context menu
    ui->treeView->addAction(ui->actionRemove_Item);

    // Add the action to the treeView's context menu
    ui->treeView->addAction(ui->actionItem_Options);
    // Link a render window with the Qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    // Add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    /* Create an object and add to renderer (this will change later to display a CAD model) */
    /* Will just copy and paste cylinder example from before */
    // This creates a polygonal cylinder model with eight circumferential facets
    // (i.e, in practice an octagonal prism).
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    // The mapper is responsible for pushing the geometry into the graphics
    // library. It may also do color mapping, if scalars or other attributes are
    // defined.
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // The actor is a grouping mechanism: besides the geometry (mapper), it
    // also has a property, transformation matrix, and/or texture map.
    // Here we set its color and rotate it around the X and Y axes.
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton() {
    QMessageBox msgBox;
    msgBox.setText("Add button was clicked");
    msgBox.exec();
    emit statusUpdateMessage("Button clicked!", 3000);
}

void MainWindow::addVRActorsFromTree(const QModelIndex& index, VRRenderThread* vrThread) {
    // Base case: if the index is invalid, return
    if (!index.isValid()) return;

    // Retrieve the ModelPart associated with the current index
    auto modelPart = static_cast<ModelPart*>(index.internalPointer());
    // Check if the ModelPart is valid
    if (modelPart && modelPart->isFile()) {
        // Create a VR actor for the ModelPart and add it to the VR render thread
        vrThread->addActorOffline(modelPart->getFilePath());
    } 
    // Recursively process all children of the current index
    int childCount = partList->rowCount(index);
    for (int i = 0; i < childCount; i++) {
        // Get the child index and recursively call addActorsFromTree
        addVRActorsFromTree(partList->index(i, 0, index), vrThread);
    }
}

void MainWindow::on_actionStart_Vr_triggered() {
    if (vrThread && vrThread->isRunning()) {
        emit statusUpdateMessage("VR is already running.", 0);
        return;
    }

    // Initialize VR rendering thread
    vrThread = new VRRenderThread(this);

    // Add all actors from the tree to the VR thread
    if (partList->rowCount(QModelIndex()) > 0) {
        int rootCount = partList->rowCount(QModelIndex());
        for (int i = 0; i < rootCount; i++) {
            addVRActorsFromTree(partList->index(i, 0, QModelIndex()), vrThread);
        }
    }

    // Start the VR thread
    vrThread->start();
    emit statusUpdateMessage("VR rendering started.", 3000);
}

void MainWindow::on_actionStop_Vr_triggered() {
    if (vrThread) {
        vrThread->quit();
        vrThread->stopAndReset();
        delete vrThread;
        vrThread = nullptr;
    }
}

void MainWindow::handleTreeClicked() {
    // Get the index of the selected item
    QModelIndex index = ui->treeView->currentIndex();

    // Get a pointer to the item from the index
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Retrieve the name string from the internal QVariant data array
    QString text = selectedPart->data(0).toString();

    // Emit a signal to update the status bar with the selected item's name
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered() {
    QString fileName = QFileDialog::getOpenFileName(
        this,                               // Parent widget
        tr("Open File"),                    // Dialog title
        "C:\\",                             // Default directory
        tr("STL Files (*.stl);;Text Files (*.txt)")  // File filters
    );

    if (!fileName.isEmpty()) {
        emit statusUpdateMessage(QString("Selected file: ") + fileName, 0);

        QModelIndex index = ui->treeView->currentIndex();
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
            );
            // Check if a file 
            // was selected
            if (!fileName.isEmpty()) {
                // Display the selected file's name in the status bar
                emit statusUpdateMessage(QString("Selected file: ") + fileName, 0);
                // Get the currently selected item in the tree view
                QModelIndex index = ui->treeView->currentIndex();
                ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

                // If no item is selected, use the root item
                if (!selectedPart) {
                    selectedPart = partList->getRootItem();
                }
                // Create a new child item for the selected part
                QString name = QFileInfo(fileName).fileName();  // Use the file name as the item name
                QString visible("true");

                ModelPart* childItem = new ModelPart({ name, visible });
                selectedPart->appendChild(childItem);

                // Load the STL file into the new item
                childItem->loadSTL(fileName);

                // Refresh the renderer to display the new STL file
                updateRender();
                // Reset the camera to ensure the new model is visible
                renderer->ResetCamera();
                renderer->GetActiveCamera()->Azimuth(30);
                renderer->GetActiveCamera()->Elevation(30);
                renderer->ResetCameraClippingRange();
                renderWindow->Render(); // Force immediate render
            }
            else {
                // Display a message if no file was selected
                emit statusUpdateMessage("No file selected.", 0);
            }
    }
}

void MainWindow::handleEditButton() {
    // Get the selected item from the tree view
    OptionDialog dialog(this);

    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart) {
        emit statusUpdateMessage("No item selected.", 0);
        return;
    }
    // Create an instance of the OptionDialog

    // Set the dialog's initial values from the selected part
    dialog.setName(selectedPart->data(0).toString());
    dialog.setColour(QColor(selectedPart->getColourR(), selectedPart->getColourG(), selectedPart->getColourB()));
    dialog.setItemVisible(selectedPart->isItemVisible());

    // Show the dialog and check if it was accepted
    if (dialog.exec() == QDialog::Accepted) {
        // Update the selected part with the dialog's values
        selectedPart->set(0, dialog.getName());
        selectedPart->setColour(dialog.getColour().red(), dialog.getColour().green(), dialog.getColour().blue());
        selectedPart->setItemVisible(dialog.isItemVisible());

        emit statusUpdateMessage(QString("Dialog accepted"), 0);
    } else {
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
}

void MainWindow::on_actionItem_Options_triggered() {
    // Get the selected item from the tree view
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart) {
        emit statusUpdateMessage("No item selected.", 0);
        return;
    }

    // Create an instance of the OptionDialog
    OptionDialog dialog(this);

    // Set the dialog's initial values from the selected part

    dialog.setName(selectedPart->data(0).toString());
    dialog.setColour(QColor(selectedPart->getColourR(), selectedPart->getColourG(), selectedPart->getColourB()));
    dialog.setItemVisible(selectedPart->isItemVisible());

    // Show the dialog and check if it was accepted
    if (dialog.exec() == QDialog::Accepted) {
        // Update the selected part with the dialog's values
        selectedPart->set(0, dialog.getName());
        selectedPart->setColour(dialog.getColour().red(), dialog.getColour().green(), dialog.getColour().blue());
        selectedPart->setItemVisible(dialog.isItemVisible());

        if (!selectedPart->isItemVisible()) {
            // Update the visibility status text to "false"
            selectedPart->set(1, "false");
        }
        else {
            selectedPart->set(1, "true");
        }

        updateRender();

        QString msg = QString("Changed color of %1 to RGB(%2, %3, %4)")
            .arg(dialog.getName())
            .arg(dialog.getColour().red())
            .arg(dialog.getColour().green())
            .arg(dialog.getColour().blue());

        emit statusUpdateMessage(msg, 0);
    } else {
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
}

void MainWindow::updateRender() {
    // Clear all existing actors from the renderer
    renderer->RemoveAllViewProps();

    // Start rendering from the root item
    if (partList->rowCount(QModelIndex()) > 0) {
        int rootCount = partList->rowCount(QModelIndex()); // Get the number of top-level items
        for (int i = 0; i < rootCount; i++) {
            // Process each top-level item and its children recursively
            updateRenderFromTree(partList->index(i, 0, QModelIndex()));
        }
    }

    // Force an immediate render update
    renderer->Render();
    renderWindow->Render();
    renderer->ResetCamera();

    if (vrThread) {
        vrThread->removeViwerProps();
        if (partList->rowCount(QModelIndex()) > 0) {
            int rootCount = partList->rowCount(QModelIndex());
            for (int i = 0; i < rootCount; i++) {
                addVRActorsFromTree(partList->index(i, 0, QModelIndex()), vrThread);
            }
        }
        vrThread->addActors();
        vrThread->reset();
	}

}

void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    if (!index.isValid()) return;

    // Retrieve the ModelPart associated with the current index
    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (!part) return;

    // Check visibility and add the actor
    if (part->getActor() && part->isItemVisible()) {
        renderer->AddActor(part->getActor());
    }

    // Recursively process all children of the current part
    int childCount = partList->rowCount(index);
    for (int i = 0; i < childCount; i++) {
        updateRenderFromTree(partList->index(i, 0, index)); // Process each child
    }
}

void MainWindow::on_actionOpen_Folder_triggered() {
    QString folderPath = QFileDialog::getExistingDirectory(
        this,
        tr("Select Folder"),
        "C:\\", // Default folder
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (folderPath.isEmpty()) {
        emit statusUpdateMessage("No folder selected.", 0);
        return;
    }

    QDir directory(folderPath);
    QStringList stlFiles = directory.entryList(QStringList() << "*.stl", QDir::Files);

    if (stlFiles.isEmpty()) {
        emit statusUpdateMessage("No STL files found in folder.", 0);
        return;
    }

    // Create a new top-level item for the folder
    QString folderName = directory.dirName();
    ModelPart* folderPart = new ModelPart({ folderName, "true" });

    ModelPart* rootItem = this->partList->getRootItem();
    rootItem->appendChild(folderPart);

    // Load each STL file into the new folder part
    for (const QString& fileName : stlFiles) {
        QString fullPath = directory.filePath(fileName);

        // Create a new child part for each file
        ModelPart* filePart = new ModelPart({ fileName, "true" });
        folderPart->appendChild(filePart);

        filePart->loadSTL(fullPath);
    }
    emit statusUpdateMessage(QString("Loaded %1 STL files into a new folder part.").arg(stlFiles.count()), 0);

    updateRender();
    ui->treeView->reset(); // Reset the tree view to include the new parts
}

void MainWindow::on_actionRemove_Item_triggered() {
    QModelIndex index = ui->treeView->currentIndex();

    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected to remove.", 0);
        return;
    }

    // Optional: Ask user for confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Remove Item",
                                  "Are you sure you want to remove the selected item?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        emit statusUpdateMessage("Removal cancelled.", 0);
        return;
    }

    if (!partList->removePart(index)) {
        emit statusUpdateMessage("Could not remove item.", 0);
        return;
    }

    updateRender(); // Refresh 3D view
    emit statusUpdateMessage("Item removed.", 0);
}

void MainWindow::addLightingToScene(vtkRenderer* targetRenderer) {
    vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToSceneLight();
    light->SetPosition(5, 5, 15);
    light->SetPositional(true);
    light->SetConeAngle(30);
    light->SetFocalPoint(0, 0, 0);
    light->SetDiffuseColor(1.0, 1.0, 1.0);
    light->SetAmbientColor(0.3, 0.3, 0.3);
    light->SetSpecularColor(1.0, 1.0, 1.0);
    light->SetIntensity(0.7);
    targetRenderer->AddLight(light);
}

void MainWindow::on_actionShrink_Filter_toggled(bool checked) {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected.", 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Invalid item.", 0);
        return;
    }

    if (checked) {
        vtkSmartPointer<vtkPolyData> inputData = selectedPart->getPolyData();
        if (!inputData) {
            emit statusUpdateMessage("No polydata to shrink.", 0);
            return;
        }

        vtkNew<vtkShrinkPolyData> shrinkFilter;
        shrinkFilter->SetInputData(inputData);
        shrinkFilter->SetShrinkFactor(0.8);
        shrinkFilter->Update();

        vtkNew<vtkPolyDataMapper> shrinkMapper;
        shrinkMapper->SetInputConnection(shrinkFilter->GetOutputPort());

        vtkNew<vtkActor> shrinkActor;
        shrinkActor->SetMapper(shrinkMapper);
        shrinkActor->GetProperty()->SetColor(
            selectedPart->getColourR() / 255.0,
            selectedPart->getColourG() / 255.0,
            selectedPart->getColourB() / 255.0);
        shrinkActor->SetVisibility(selectedPart->isItemVisible());

        selectedPart->getActor()->ShallowCopy(shrinkActor);
        emit statusUpdateMessage("Shrink filter applied.", 0);
    } else {

        selectedPart->loadSTL(selectedPart->getFilePath());

        emit statusUpdateMessage("Shrink filter removed.", 0);
    }

    updateRender();
}

void MainWindow::on_actionClip_Filter_toggled(bool checked) {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage("No item selected.", 0);
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        emit statusUpdateMessage("Invalid item.", 0);
        return;
    }

    if (checked) {
        vtkSmartPointer<vtkPolyData> inputData = selectedPart->getPolyData();
        if (!inputData) {
            emit statusUpdateMessage("No polydata to clip.", 0);
            return;
        }

        double bounds[6];
        inputData->GetBounds(bounds);
        double padding = (bounds[1] - bounds[0]) * 0.1;

        vtkNew<vtkPlane> plane;
        plane->SetOrigin(bounds[1] - padding, 0.0, 0.0);
        plane->SetNormal(-1.0, 0.0, 0.0);  // Adjust as needed
        vtkNew<vtkClipPolyData> clipFilter;
        clipFilter->SetInputData(inputData);
        clipFilter->SetClipFunction(plane);
        clipFilter->Update();

        vtkNew<vtkPolyDataMapper> clipMapper;
        clipMapper->SetInputConnection(clipFilter->GetOutputPort());

        vtkNew<vtkActor> clipActor;
        clipActor->SetMapper(clipMapper);
        clipActor->GetProperty()->SetColor(
            selectedPart->getColourR() / 255.0,
            selectedPart->getColourG() / 255.0,
            selectedPart->getColourB() / 255.0);
        clipActor->SetVisibility(selectedPart->isItemVisible());

        selectedPart->getActor()->ShallowCopy(clipActor);
        emit statusUpdateMessage("Clip filter applied.", 0);
    } else {

        selectedPart->loadSTL(selectedPart->getFilePath());

        emit statusUpdateMessage("Clip filter removed.", 0);
    }

    updateRender();
}
