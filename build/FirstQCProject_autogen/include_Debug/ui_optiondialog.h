/********************************************************************************
** Form generated from reading UI file 'optiondialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONDIALOG_H
#define UI_OPTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_OptionDialog
{
public:
    QDialogButtonBox *buttonBox;
    QSpinBox *spinBox_2;
    QLineEdit *lineEdit;
    QCheckBox *checkBox;
    QSpinBox *spinBox_3;
    QSpinBox *spinBox;

    void setupUi(QDialog *OptionDialog)
    {
        if (OptionDialog->objectName().isEmpty())
            OptionDialog->setObjectName("OptionDialog");
        OptionDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(OptionDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        spinBox_2 = new QSpinBox(OptionDialog);
        spinBox_2->setObjectName("spinBox_2");
        spinBox_2->setGeometry(QRect(170, 40, 42, 26));
        lineEdit = new QLineEdit(OptionDialog);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(170, 120, 113, 26));
        checkBox = new QCheckBox(OptionDialog);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(70, 190, 91, 24));
        spinBox_3 = new QSpinBox(OptionDialog);
        spinBox_3->setObjectName("spinBox_3");
        spinBox_3->setGeometry(QRect(300, 40, 42, 26));
        spinBox = new QSpinBox(OptionDialog);
        spinBox->setObjectName("spinBox");
        spinBox->setGeometry(QRect(40, 40, 42, 26));

        retranslateUi(OptionDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, OptionDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, OptionDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(OptionDialog);
    } // setupUi

    void retranslateUi(QDialog *OptionDialog)
    {
        OptionDialog->setWindowTitle(QCoreApplication::translate("OptionDialog", "Dialog", nullptr));
        lineEdit->setText(QCoreApplication::translate("OptionDialog", "nameLineEdit", nullptr));
        checkBox->setText(QCoreApplication::translate("OptionDialog", "CheckBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OptionDialog: public Ui_OptionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONDIALOG_H
