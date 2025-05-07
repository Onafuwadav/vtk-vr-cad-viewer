/** @file mainwindow.h
 * @brief Header for the MainWindow class. Manages UI and rendering logic.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPart.h"
#include "ModelPartList.h"
#include "VRRenderThread.h"
#include "optiondialog.h"

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);
    void addVRActorsFromTree(const QModelIndex& index, VRRenderThread* vrThread);

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

    VRRenderThread* vrThread = nullptr;

    ModelPartList* partList; // Pointer to the ModelPartList

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:  // Add this missing line to declare the slot function
    void handleButton();  // Slot to handle button clicks
    void handleTreeClicked();  // Slot to handle tree item clicks
    void on_actionOpen_File_triggered();  // Slot for handling the "Open File" action
    void handleEditButton();  // New slot for the edit button
    void addLightingToScene(vtkRenderer* targetRenderer); // ✅ Lighting function declaration
    void on_actionItem_Options_triggered();
    void on_actionOpen_Folder_triggered();
    void on_actionRemove_Item_triggered();
    void on_actionStart_Vr_triggered();
    void on_actionShrink_Filter_toggled(bool checked);
    void on_actionClip_Filter_toggled(bool checked);
    void on_actionStop_Vr_triggered();

signals:
    /**
     * @brief Updates the status bar message
     * @param message The message to show
     * @param timeout How long to display the message (ms)
     */
    void statusUpdateMessage(const QString &message, int timeout);
};

#endif // MAINWINDOW_H
