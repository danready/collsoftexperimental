/********************************************************************************
** Form generated from reading UI file 'doublechecking.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOUBLECHECKING_H
#define UI_DOUBLECHECKING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DoubleChecking
{
public:
    QDialogButtonBox *buttonBox;
    QTextEdit *textEditDoubleChecking;
    QLabel *label;

    void setupUi(QDialog *DoubleChecking)
    {
        if (DoubleChecking->objectName().isEmpty())
            DoubleChecking->setObjectName(QStringLiteral("DoubleChecking"));
        DoubleChecking->resize(400, 300);
        buttonBox = new QDialogButtonBox(DoubleChecking);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(100, 240, 181, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        textEditDoubleChecking = new QTextEdit(DoubleChecking);
        textEditDoubleChecking->setObjectName(QStringLiteral("textEditDoubleChecking"));
        textEditDoubleChecking->setGeometry(QRect(110, 140, 171, 41));
        QPalette palette;
        QBrush brush(QColor(242, 241, 240, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        textEditDoubleChecking->setPalette(palette);
        textEditDoubleChecking->setFrameShape(QFrame::NoFrame);
        label = new QLabel(DoubleChecking);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 30, 81, 61));
        label->setPixmap(QPixmap(QString::fromUtf8(":/danger-hi.png")));
        label->setScaledContents(true);

        retranslateUi(DoubleChecking);
        QObject::connect(buttonBox, SIGNAL(accepted()), DoubleChecking, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DoubleChecking, SLOT(reject()));

        QMetaObject::connectSlotsByName(DoubleChecking);
    } // setupUi

    void retranslateUi(QDialog *DoubleChecking)
    {
        DoubleChecking->setWindowTitle(QApplication::translate("DoubleChecking", "Double checking panel", 0));
        textEditDoubleChecking->setHtml(QApplication::translate("DoubleChecking", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:600; color:#ff0000;\">Are you sure?</span></p></body></html>", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DoubleChecking: public Ui_DoubleChecking {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOUBLECHECKING_H
