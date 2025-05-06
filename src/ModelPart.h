#ifndef MODELPART_H
#define MODELPART_H
/**     @file ModelPart.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  *      Declares the ModelPart class, representing a 3D model part.
  */
 
#include <QString>
#include <QList>
#include <QVariant>
#include <vtkSmartPointer.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkColor.h>
#include <vtkPolyData.h>

/**
 * @class ModelPart
 *  Represents a 3D part with rendering, color, and STL geometry attributes.
 */

class ModelPart {
public:
    ModelPart(const QString& name);
    /** Constructor
     * @param data is a List (array) of strings for each property of this item (part name and visiblity in our case
     * @param parent is the parent of this item (one level up in tree)
     */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);
    ~ModelPart();

    void appendChild(ModelPart* item);

    /** Return child at position 'row' below this item
      * @param row is the row number (below this item)
      * @return pointer to the item requested.
      */
    ModelPart* child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    QString getFilename() const;
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
    bool isFile() const;
    void setVisibilityRecursive(bool visible);

    void loadSTL(const QString& fileName);

    vtkSmartPointer<vtkActor> getActor(); // Get the GUI actor
    vtkSmartPointer<vtkActor> addNewActor(); // Create a new VR actor

    /** Get visible flag
      * @return visible flag as boolean 
      */
    bool isItemVisible() const;
	
	/** Load STL file
      * @param fileName
      */
    void loadSTL(const QString& fileName);

    /** Return actor
      * @return pointer to default actor for GUI rendering
      */
    vtkSmartPointer<vtkActor> getActor();

    /**
     *  Gets the PolyData from the STL reader.
     */


     vtkSmartPointer<vtkPolyData> getPolyData();


    /** Return new actor for use in VR
      * @return pointer to new actor
      */
    //vtkActor* getNewActor();

     /**
     *  Removes a child at the given row.
     */

    void removeChild(int row);

     /**
     *  Sets the STL file path for reloading.
     * @param path Full file path.
     */

     void setFilePath(const QString& path);

    /**
     * @brief Gets the STL file path.
     * @return QString file path.
     */

     QString getFilePath() const;

private:
    QString                                     filename;
    QList<ModelPart*>                           m_childItems;       /**< List (array) of child items */
    QList<QVariant>                             m_itemData;         /**< List (array of column data for item */
    ModelPart*                                  m_parentItem;       /**< Pointer to parent */
    QString filePath;  // Full path to the STL file

    bool isVisible;                         /**< True/false to indicate if visible in model rendering */
    unsigned char R, G, B;                  /**< RGB color values */
	
	/* These are vtk properties that will be used to load/render a model of this part,
	 * commented out for now but will be used later
	 */
    vtkSmartPointer<vtkSTLReader>               file;               /**< Datafile from which part loaded */
    vtkSmartPointer<vtkMapper>                  mapper;             /**< Mapper for rendering */
    vtkSmartPointer<vtkActor>                   actor;              /**< Actor for rendering */
    vtkColor3<unsigned char>                    colour;             /**< User defineable colour */


};  

#endif // MODELPART_H