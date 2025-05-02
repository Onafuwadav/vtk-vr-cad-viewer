#ifndef MODELPART_H
#define MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>
#include <vtkSmartPointer.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkColor.h>

class ModelPart {
public:
    ModelPart(const QString& name);
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);
    ~ModelPart();

    void appendChild(ModelPart* item);
    ModelPart* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void set(int column, const QVariant& value);
    ModelPart* parentItem();
    int row() const;
    void removeChild(int row);

    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);
    unsigned char getColourR();
    unsigned char getColourG();
    unsigned char getColourB();
    void setItemVisible(bool isVisible);
    bool isItemVisible() const;
    void setVisibilityRecursive(bool visible);

    void loadSTL(const QString& fileName);

    vtkSmartPointer<vtkActor> getActor(); // Get the GUI actor
    vtkSmartPointer<vtkActor> addNewActor(); // Create a new VR actor

private:
    QList<ModelPart*> m_childItems;         /**< List (array) of child items */
    QList<QVariant> m_itemData;             /**< List (array) of column data for item */
    ModelPart* m_parentItem;                /**< Pointer to parent */

    bool isVisible;                         /**< True/false to indicate if visible in model rendering */
    unsigned char R, G, B;                  /**< RGB color values */

    vtkSmartPointer<vtkSTLReader> file;     /**< Datafile from which part is loaded */
    vtkSmartPointer<vtkMapper> mapper;      /**< Mapper for rendering */
    vtkSmartPointer<vtkActor> actor;        /**< Actor for GUI rendering */
    vtkColor3<unsigned char> colour;        /**< User-defined color */
};

#endif // MODELPART_H