/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date November 2010
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui>
#include <QMessageBox>
#include <iostream>
#include "../include/xbox_gui/main_window.hpp"
#include "../src/listner.hpp"

using namespace Qt;

/*****************************************************************************
** Implementation [MainWindow]
*****************************************************************************/

MainWindow::MainWindow(QNode *node, QWidget *parent) :
    QMainWindow(parent),
    qnode(node)
{
    ui.setupUi(this); // Calling this incidentally connects all ui's triggers to on_...() callbacks in this class.
    QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt())); // qApp is a global variable for the application

    ReadSettings();
    setWindowIcon(QIcon(":/images/Xbox.png"));
    this->setWindowTitle("Xbox GUI");

    setWindowTitle(QApplication::translate("MainWindowDesign", qnode->nodeName().c_str(), 0, QApplication::UnicodeUTF8));

    /*********************
    ** Logging
    **********************/
    QObject::connect(qnode, SIGNAL(loggingUpdated()), this, SLOT(updateLoggingView()));
    QObject::connect(qnode, SIGNAL(rosShutdown()), this, SLOT(close()));

    /*********************
    ** Auto Start
    **********************/
    if (ui.checkbox_remember_settings->isChecked())
        on_button_connect_clicked(true);
    /*********************
    ** Xbox Logo & Buttons
    **********************/
    QPixmap pix(":/images/XboxLogo.png");

    ui.label_XboxLogo->setPixmap(pix);

    QPixmap pix_A_OFF(":/images/aOFF.png");
    ui.label_A->setPixmap(pix_A_OFF);

    QPixmap pix_B_OFF(":/images/BOFF.png");
    ui.label_B->setPixmap(pix_B_OFF);

    QPixmap pix_X_OFF(":/images/XOff.png");
    ui.label_X->setPixmap(pix_X_OFF);

    QPixmap pix_Y_OFF(":/images/YOFf.png");
    ui.label_Y->setPixmap(pix_Y_OFF);

    /*********************
    ** Line Edit Stuff
    **********************/
    ui.lineEdit_servo0->setReadOnly(TRUE);
    ui.lineEdit_servo0->setText("N/A");
    ui.lineEdit_servo1->setReadOnly(TRUE);
    ui.lineEdit_servo1->setText("N/A");

    /*********************
     ** Timer
     **********************/
     timer = new QTimer(this);
     connect(timer,SIGNAL(timeout()),this,SLOT(timerEvent()));
     timer->start(1);

}

MainWindow::~MainWindow() {}

/*****************************************************************************
** Implementation [Slots]
*****************************************************************************/

void MainWindow::showNoMasterMessage() {
  QMessageBox msgBox;
  msgBox.setText("Couldn't find the ros master.");
  msgBox.exec();
    close();
}

/*
 * These triggers whenever the button is clicked, regardless of whether it
 * is already checked or not.
 */

void MainWindow::on_button_connect_clicked(bool check ) {
  if ( ui.checkbox_use_environment->isChecked() ) {
    if ( !qnode->on_init() ) {
      showNoMasterMessage();
    } else {
      ui.button_connect->setEnabled(false);
    }
  } else {
    if ( ! qnode->on_init(
          ui.line_edit_master->text().toStdString(),
          ui.line_edit_host->text().toStdString() )
        ) {
      showNoMasterMessage();
    } else {
      ui.button_connect->setEnabled(false);
      ui.line_edit_master->setReadOnly(true);
      ui.line_edit_host->setReadOnly(true);
    }
  }
}

void MainWindow::on_checkbox_use_environment_stateChanged(int state) {
  bool enabled;
  if ( state == 0 ) {
    enabled = true;
  } else {
    enabled = false;
  }
  ui.line_edit_master->setEnabled(enabled);
  ui.line_edit_host->setEnabled(enabled);
}

/*****************************************************************************
** Implementation [Menu]
*****************************************************************************/

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr("About ..."),tr("<h2>PACKAGE_NAME Test Program 0.10</h2><p>Copyright Yujin Robot</p><p>This package needs an about description.</p>"));
}

/*****************************************************************************
** Implementation [Configuration]
*****************************************************************************/

void MainWindow::ReadSettings() {
    QSettings settings("Qt-Ros Package", qnode->nodeName().c_str());
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    QString master_url = settings.value("master_url",QString("http://192.168.1.2:11311/")).toString();
    QString host_url = settings.value("host_url", QString("192.168.1.3")).toString();
    QString topic_name = settings.value("topic_name", QString("/chatter")).toString();
    ui.line_edit_master->setText(master_url);
    ui.line_edit_host->setText(host_url);
    bool remember = settings.value("remember_settings", false).toBool();
    ui.checkbox_remember_settings->setChecked(remember);
    bool checked = settings.value("use_environment_variables", false).toBool();
    ui.checkbox_use_environment->setChecked(checked);
    if ( checked ) {
      ui.line_edit_master->setEnabled(false);
      ui.line_edit_host->setEnabled(false);
    }
}

void MainWindow::WriteSettings() {
    QSettings settings("Qt-Ros Package", qnode->nodeName().c_str());
    settings.setValue("geometry", geometry());
    settings.setValue("master_url",ui.line_edit_master->text());
    settings.setValue("host_url",ui.line_edit_host->text());
    settings.setValue("use_environment_variables",QVariant(ui.checkbox_use_environment->isChecked()));
    settings.setValue("windowState", saveState());
    settings.setValue("remember_settings",QVariant(ui.checkbox_remember_settings->isChecked()));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  qnode->shutdown();
  WriteSettings();
  QMainWindow::closeEvent(event);
}

/*****************************************************************************
** Timer Function
*****************************************************************************/
void MainWindow::timerEvent()
{
  ui.lineEdit_servo0->setText(QString::number((int)getServo0()));
  ui.lineEdit_servo1->setText(QString::number((int)getServo1()));

  // A
  if(get_A_Status() == FALSE)
  {
      QPixmap pix_A_OFF(":/images/aOFF.png");
      ui.label_A->setPixmap(pix_A_OFF);
  }
  else
  {
      QPixmap pix_A_ON(":/images/aON.png");
      ui.label_A->setPixmap(pix_A_ON);
  }

  // B
  if(get_B_Status() == FALSE)
  {
      QPixmap pix_B_OFF(":/images/BOFF.png");
      ui.label_B->setPixmap(pix_B_OFF);
  }
  else
  {
      QPixmap pix_B_ON(":/images/BOn.png");
      ui.label_B->setPixmap(pix_B_ON);
  }

  // X
  if(get_X_Status() == FALSE)
  {
      QPixmap pix_X_OFF(":/images/XOff.png");
      ui.label_X->setPixmap(pix_X_OFF);
  }
  else
  {
      QPixmap pix_X_ON(":/images/XOn.png");
      ui.label_X->setPixmap(pix_X_ON);
  }

  // Y
  if(get_Y_Status() == FALSE)
  {
      QPixmap pix_Y_OFF(":/images/YOFf.png");
      ui.label_Y->setPixmap(pix_Y_OFF);
  }
  else
  {
      QPixmap pix_Y_ON(":/images/YOn.png");
      ui.label_Y->setPixmap(pix_Y_ON);
  }

}
