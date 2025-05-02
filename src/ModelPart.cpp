#include "ModelPart.h"
#include <QFileInfo>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkColor.h>
#include <vtkProperty.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
    : m_itemData(data), m_parentItem(parent), isVisible(true), R(255), G(255), B(255) {
}

ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}

void ModelPart::appendChild(ModelPart* item) {
    item->m_parentItem = this;
    m_childItems.append(item);
}

ModelPart* ModelPart::child(int row) {
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const {
    return m_childItems.count();
}

int ModelPart::columnCount() const {
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const {
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

void ModelPart::set(int column, const QVariant& value) {
    if (column < 0 || column >= m_itemData.size())
        return;
    m_itemData.replace(column, value);
}

ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}

int ModelPart::row() const {
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    this->R = R;
    this->G = G;
    this->B = B;

    if (actor) {
        actor->GetProperty()->SetColor(R / 255.0, G / 255.0, B / 255.0);
    }
}

unsigned char ModelPart::getColourR() {
    return R;
}

unsigned char ModelPart::getColourG() {
    return G;
}

unsigned char ModelPart::getColourB() {
    return B;
}

void ModelPart::setItemVisible(bool isVisible) {
    this->isVisible = isVisible;
    if (actor) {
        actor->SetVisibility(isVisible);
    }
}

bool ModelPart::isItemVisible() const {
    return isVisible;
}

void ModelPart::loadSTL(const QString& fileName) {
    filename = fileName;
    QFileInfo fileInfo(filename);

    file = vtkSmartPointer<vtkSTLReader>::New();
    file->SetFileName(fileName.toStdString().c_str());
    file->Update();

    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(file->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(R / 255.0, G / 255.0, B / 255.0);
    actor->SetVisibility(isVisible);
}

vtkSmartPointer<vtkActor> ModelPart::getActor() {
    return actor;
}

vtkSmartPointer<vtkActor> ModelPart::addNewActor() {
    qDebug() << "Creating new actor for ModelPart";
    // Create a new mapper and link it to the same STL reader
    auto newMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
 

    // Create a new VR actor
    auto newActor = vtkSmartPointer<vtkActor>::New();
    newActor->SetMapper(newMapper);
    qDebug() << "Actor created successfully";

    // Share properties with the GUI actor
    if (actor) {
        newActor->SetProperty(actor->GetProperty());
    }
    qDebug() << "addNewActor(): Actor state:"
        << "Mapper:" << newActor->GetMapper()
        << "Visibility:" << newActor->GetVisibility()
        << "Color:" << newActor->GetProperty()->GetColor();

    return newActor; // Return the new actor for VR
}

void ModelPart::removeChild(int row) {
    if (row >= 0 && row < m_childItems.size())
        m_childItems.removeAt(row);
}

void ModelPart::setVisibilityRecursive(bool visible) {
    QString visibilityStr = visible ? "true" : "false";
    this->set(1, visibilityStr);
    this->setItemVisible(visible);

    for (int i = 0; i < childCount(); ++i) {
        if (ModelPart* c = child(i)) {
            c->setVisibilityRecursive(visible);
        }
    }
}

bool ModelPart::isFile() const {
    // Implement logic to determine if this ModelPart is a file
    return QFileInfo(filename).isFile();
}

QString ModelPart::getFilename() const {
    return filename; // Return the stored filename
}