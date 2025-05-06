#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "ModelPart.h"
#include "ModelPartList.h"
#include "optiondialog.h"
#include "VRRenderThread.h" 

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindowInteractor.h>

class vtkOpenVRRenderWindowInteractor;
class vtkRenderer;
class vtkGenericOpenGLRenderWindow;
class VRRenderThread;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ModelPartList* partList; // Pointer to the ModelPartList

    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);
    void addVRActorsFromTree(const QModelIndex& index, VRRenderThread* vrThread);

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    VRRenderThread* vrThread = nullptr;

public slots:  // Add this missing line to declare the slot function
    void handleButton();  // Slot to handle button clicks
    void handleTreeClicked();  // Slot to handle tree item clicks
    void on_actionOpen_File_triggered();  // Slot for handling the "Open File" action
    void handleEditButton();  // New slot for the edit button
    void on_actionItem_Options_triggered();
    void on_actionOpen_Folder_triggered();  // New slot for opening a folder
    void on_actionRemove_Item_triggered();  // New slot for item removal
    void on_actionStart_Vr_triggered();
    void on_actionStop_Vr_triggered();

signals:
    void statusUpdateMessage(const QString &message, int timeout);  // Signal for status bar updates

};
#endif // MAINWINDOW_H
