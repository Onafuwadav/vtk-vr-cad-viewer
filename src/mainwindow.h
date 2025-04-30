#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPart.h"
#include "ModelPartList.h"
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
    ModelPartList* partList; // Pointer to the ModelPartList

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);

public slots:  // Add this missing line to declare the slot function
    void handleButton();  // Slot to handle button clicks
       void handleTreeClicked();  // Slot to handle tree item clicks
     void on_actionOpen_File_triggered();  // Slot for handling the "Open File" action
       void handleEditButton();  // New slot for the edit button
      void on_actionItem_Options_triggered();
      void on_actionOpen_Folder_triggered();  // New slot for opening a folder
      void on_actionRemove_Item_triggered();  // New slot for item removal

signals:
    void statusUpdateMessage(const QString &message, int timeout);  // Signal for status bar updates

};
#endif // MAINWINDOW_H
