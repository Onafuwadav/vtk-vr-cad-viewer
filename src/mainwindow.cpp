#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include "ModelPart.h"
#include "ModelPartList.h"
#include <QFileDialog>
#include "optiondialog.h"
#include <QDir>

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkLight.h>

#include "VRRenderThread.h"
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindowInteractor.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);

    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList);

    ModelPart* rootItem = this->partList->getRootItem();
    QString name = QString("Model");
    QString visible("true");
    ModelPart* childItem = new ModelPart({ name, visible });
    rootItem->appendChild(childItem);

    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleEditButton);
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->treeView->addAction(ui->actionRemove_Item);
    ui->treeView->addAction(ui->actionItem_Options);

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    initializeVRComponents();

    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    addLightingToScene(renderer);

    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::initializeVRComponents() {
    vrRenderer = vtkSmartPointer<vtkOpenVRRenderer>::New();
    vrRenderWindow = vtkSmartPointer<vtkOpenVRRenderWindow>::New();
    vrInteractor = vtkSmartPointer<vtkOpenVRRenderWindowInteractor>::New();

    vrRenderWindow->AddRenderer(vrRenderer);
    vrRenderWindow->SetInteractor(vrInteractor);
    vrInteractor->SetRenderWindow(vrRenderWindow);
    vrInteractor->Initialize();
    vrRenderer->ResetCamera();
}
