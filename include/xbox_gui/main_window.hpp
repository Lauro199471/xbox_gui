#ifndef ros_gui_MAIN_WINDOW_H
#define ros_gui_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include <QTimer>
#include <QtGui>
#include <QMessageBox>
#include <iostream>

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow {
Q_OBJECT

public:
  MainWindow(QNode *node, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();

public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
	void on_actionAbout_triggered();
	void on_button_connect_clicked(bool check );
	void on_checkbox_use_environment_stateChanged(int state);
    void timerEvent();

private:
	Ui::MainWindowDesign ui;
    QNode *qnode;
    QTimer *timer;
   /* QPixmap pix_A_OFF;
    QPixmap pix_B_OFF(":/images/BOFF.png");
    QPixmap pix_X_OFF(":/images/XOff.png");
    QPixmap pix_Y_OFF(":/images/YOFf.png");

    QPixmap pix_A_ON(":/images/aON.png");
    QPixmap pix_B_ON(":/images/BOn.png");
    QPixmap pix_X_ON(":/images/XOn.png");
    QPixmap pix_Y_ON(":/images/YOn.png");*/
};


#endif // apada_gui_MAIN_WINDOW_H
