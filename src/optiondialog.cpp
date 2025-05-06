#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    // Set the range for the RGB spin boxes (0-255)
    ui->spinBox->setRange(0, 255);       // Red
    ui->spinBox_2->setRange(0, 255);     // Green
    ui->spinBox_3->setRange(0, 255);     // Blue
}

OptionDialog::~OptionDialog()
{
    delete ui;
}


// Get the name from the QLineEdit
QString OptionDialog::getName() const {
    return ui->lineEdit->text();
}

// Set the name in the QLineEdit
void OptionDialog::setName(const QString& name) {
    ui->lineEdit->setText(name);
}



// Get the color from the QSpinBox widgets
QColor OptionDialog::getColour() const {
    int red = ui->spinBox->value();
    int green = ui->spinBox_2->value();
    int blue = ui->spinBox_3->value();
    return QColor(red, green, blue);
}

// Set the color in the QSpinBox widgets
void OptionDialog::setColour(const QColor& colour) {
    ui->spinBox->setValue(colour.red());
    ui->spinBox_2->setValue(colour.green());
    ui->spinBox_3->setValue(colour.blue());
}


// Get the visibility flag from the QCheckBox
bool OptionDialog::isItemVisible() const {
    return ui->checkBox->isChecked();
}


// Set the visibility flag in the QCheckBox
void OptionDialog::setItemVisible(bool visible) {
    ui->checkBox->setChecked(visible);
    ui->checkBox->setText("Visibility");
}

