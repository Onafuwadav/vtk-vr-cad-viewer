#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();


    // Public methods to get and set dialog data
    QString getName() const;

    void setName(const QString& name);

    QColor getColour() const;
    void setColour(const QColor& color);

    bool isItemVisible() const;
    void setItemVisible(bool visible);

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
