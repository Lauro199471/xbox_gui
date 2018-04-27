#ifndef xbox_gui_MAIN_WINDOW_H
#define xbox_gui_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QtGui/QMainWindow>
#include "ui_main_window.h"
#include "qnode.hpp"
#include "sensor_msgs/Joy.h"
#include <QTimer>

/*****************************************************************************
** Namespace
*****************************************************************************/

namespace xbox_gui {
/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
class MainWindow : public QMainWindow {
Q_OBJECT

public:
	MainWindow(int argc, char** argv, QWidget *parent = 0);
	~MainWindow();

	void ReadSettings(); // Load up qt program settings at startup
	void WriteSettings(); // Save qt program settings when closing

	void closeEvent(QCloseEvent *event); // Overloaded function
	void showNoMasterMessage();

public Q_SLOTS:
	/******************************************
	** Auto-connections (connectSlotsByName())
	*******************************************/
	void on_button_connect_clicked(bool check );
	void on_checkbox_use_environment_stateChanged(int state);
  void timerEvent();

private:
	Ui::MainWindowDesign ui;
	QNode qnode;
  QTimer *timer;
};

}  // namespace xbox_gui

#endif // xbox_gui_MAIN_WINDOW_H
