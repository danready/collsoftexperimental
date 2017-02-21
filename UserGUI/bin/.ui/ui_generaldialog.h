/********************************************************************************
** Form generated from reading UI file 'generaldialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERALDIALOG_H
#define UI_GENERALDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_GeneralDialog
{
public:
    QLabel *label;
    QTextEdit *textEdit;
    QPushButton *OkButton_GeneralDialog;

    void setupUi(QDialog *GeneralDialog)
    {
        if (GeneralDialog->objectName().isEmpty())
            GeneralDialog->setObjectName(QStringLiteral("GeneralDialog"));
        GeneralDialog->resize(400, 300);
        label = new QLabel(GeneralDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 20, 91, 81));
        label->setPixmap(QPixmap(QString::fromUtf8(":/danger-hi.png")));
        label->setScaledContents(true);
        textEdit = new QTextEdit(GeneralDialog);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(20, 130, 361, 151));
        QPalette palette;
        QBrush brush(QColor(242, 241, 240, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        textEdit->setPalette(palette);
        textEdit->setFrameShape(QFrame::NoFrame);
        textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse);
        OkButton_GeneralDialog = new QPushButton(GeneralDialog);
        OkButton_GeneralDialog->setObjectName(QStringLiteral("OkButton_GeneralDialog"));
        OkButton_GeneralDialog->setGeometry(QRect(150, 230, 99, 31));

        retranslateUi(GeneralDialog);

        QMetaObject::connectSlotsByName(GeneralDialog);
    } // setupUi

    void retranslateUi(QDialog *GeneralDialog)
    {
        GeneralDialog->setWindowTitle(QApplication::translate("GeneralDialog", "General Dialog", 0));
        label->setText(QString());
        textEdit->setHtml(QApplication::translate("GeneralDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; color:#ff0000;\">Warning you have to be enabled first!</span></p></body></html>", 0));
        OkButton_GeneralDialog->setText(QApplication::translate("GeneralDialog", "Ok", 0));
    } // retranslateUi

};

namespace Ui {
    class GeneralDialog: public Ui_GeneralDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERALDIALOG_H
