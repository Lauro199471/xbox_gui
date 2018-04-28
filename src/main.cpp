#include <QtGui>
#include <QApplication>
#include "../include/xbox_gui/main_window.hpp"
#include "listner.hpp"

/*****************************************************************************
** Main
*****************************************************************************/

int main(int argc, char **argv)
{
  /*********************
  ** Qt
  **********************/
  QApplication app(argc, argv);
  Listener listener(argc,argv);
  MainWindow w(&listener);
  w.show();
  app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
  int result = app.exec();

	return result;
}
