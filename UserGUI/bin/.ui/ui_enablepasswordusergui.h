/********************************************************************************
** Form generated from reading UI file 'enablepasswordusergui.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENABLEPASSWORDUSERGUI_H
#define UI_ENABLEPASSWORDUSERGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EnablePasswordUserGUI
{
public:
    QDialogButtonBox *buttonBox;
    QTextEdit *textEdit;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *LineEdit_UserNameEnable;
    QLabel *label;
    QLineEdit *LineEdit_PasswordEnable;

    void setupUi(QDialog *EnablePasswordUserGUI)
    {
        if (EnablePasswordUserGUI->objectName().isEmpty())
            EnablePasswordUserGUI->setObjectName(QStringLiteral("EnablePasswordUserGUI"));
        EnablePasswordUserGUI->resize(370, 308);
        buttonBox = new QDialogButtonBox(EnablePasswordUserGUI);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(110, 240, 181, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        textEdit = new QTextEdit(EnablePasswordUserGUI);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(30, 30, 331, 51));
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
        layoutWidget = new QWidget(EnablePasswordUserGUI);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 120, 251, 71));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        LineEdit_UserNameEnable = new QLineEdit(layoutWidget);
        LineEdit_UserNameEnable->setObjectName(QStringLiteral("LineEdit_UserNameEnable"));

        gridLayout->addWidget(LineEdit_UserNameEnable, 0, 1, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        LineEdit_PasswordEnable = new QLineEdit(layoutWidget);
        LineEdit_PasswordEnable->setObjectName(QStringLiteral("LineEdit_PasswordEnable"));
        LineEdit_PasswordEnable->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(LineEdit_PasswordEnable, 1, 1, 1, 1);


        retranslateUi(EnablePasswordUserGUI);
        QObject::connect(buttonBox, SIGNAL(accepted()), EnablePasswordUserGUI, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EnablePasswordUserGUI, SLOT(reject()));

        QMetaObject::connectSlotsByName(EnablePasswordUserGUI);
    } // setupUi

    void retranslateUi(QDialog *EnablePasswordUserGUI)
    {
        EnablePasswordUserGUI->setWindowTitle(QApplication::translate("EnablePasswordUserGUI", "Authentication Required", 0));
        textEdit->setHtml(QApplication::translate("EnablePasswordUserGUI", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">The CollSoft UserGUI requires a username and password.</p></body></html>", 0));
        label_2->setText(QApplication::translate("EnablePasswordUserGUI", "User Name:", 0));
        label->setText(QApplication::translate("EnablePasswordUserGUI", "Password:", 0));
    } // retranslateUi

};

namespace Ui {
    class EnablePasswordUserGUI: public Ui_EnablePasswordUserGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENABLEPASSWORDUSERGUI_H
