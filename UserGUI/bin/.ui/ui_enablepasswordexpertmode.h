/********************************************************************************
** Form generated from reading UI file 'enablepasswordexpertmode.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENABLEPASSWORDEXPERTMODE_H
#define UI_ENABLEPASSWORDEXPERTMODE_H

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

class Ui_EnablePasswordExpertMode
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *LineEdit_UserNameExpertMode;
    QLabel *label;
    QLineEdit *LineEdit_PasswordExpertMode;
    QTextEdit *textEdit;

    void setupUi(QDialog *EnablePasswordExpertMode)
    {
        if (EnablePasswordExpertMode->objectName().isEmpty())
            EnablePasswordExpertMode->setObjectName(QStringLiteral("EnablePasswordExpertMode"));
        EnablePasswordExpertMode->resize(400, 300);
        buttonBox = new QDialogButtonBox(EnablePasswordExpertMode);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(110, 240, 181, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(EnablePasswordExpertMode);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 110, 251, 71));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        LineEdit_UserNameExpertMode = new QLineEdit(layoutWidget);
        LineEdit_UserNameExpertMode->setObjectName(QStringLiteral("LineEdit_UserNameExpertMode"));

        gridLayout->addWidget(LineEdit_UserNameExpertMode, 0, 1, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        LineEdit_PasswordExpertMode = new QLineEdit(layoutWidget);
        LineEdit_PasswordExpertMode->setObjectName(QStringLiteral("LineEdit_PasswordExpertMode"));
        LineEdit_PasswordExpertMode->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(LineEdit_PasswordExpertMode, 1, 1, 1, 1);

        textEdit = new QTextEdit(EnablePasswordExpertMode);
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

        retranslateUi(EnablePasswordExpertMode);
        QObject::connect(buttonBox, SIGNAL(accepted()), EnablePasswordExpertMode, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EnablePasswordExpertMode, SLOT(reject()));

        QMetaObject::connectSlotsByName(EnablePasswordExpertMode);
    } // setupUi

    void retranslateUi(QDialog *EnablePasswordExpertMode)
    {
        EnablePasswordExpertMode->setWindowTitle(QApplication::translate("EnablePasswordExpertMode", "Authentication Required", 0));
        label_2->setText(QApplication::translate("EnablePasswordExpertMode", "User Name:", 0));
        label->setText(QApplication::translate("EnablePasswordExpertMode", "Password:", 0));
        textEdit->setHtml(QApplication::translate("EnablePasswordExpertMode", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">You have to insert a username and password to start the expert mode.</p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class EnablePasswordExpertMode: public Ui_EnablePasswordExpertMode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENABLEPASSWORDEXPERTMODE_H
