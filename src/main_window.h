#ifndef SP_MAIN_WINWOW_H_
#define SP_MAIN_WINWOW_H_

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMdiArea>
#include <qgl.h>

#include "document.h"
#include "parameters_form.h"

//TODO(pau) Link the parameters_form_ widget to the actual values.

class SpMainWindow : public QMainWindow {
  Q_OBJECT

 public:
  SpMainWindow(QWidget *parent = 0);
  ~SpMainWindow();
 
 public slots:
  void NewCaptureViewer();
  void NewSensorViewer();
  void NewScreenViewer();
  void NewTheaterViewer();

 private:
  void CreateActions();
  void CreateMenus();
  
 private:
  // TODO(pau): Add a dialog to tune the parameters.
  QMenu *file_menu_;
  QAction *open_action_;

  QMenu *view_menu_;
  QAction *new_capture_viewer_action_;
  QAction *new_sensor_viewer_action_;
  QAction *new_screen_viewer_action_;
  QAction *new_theater_viewer_action_;

  QDockWidget *parameters_dock_;
  ParametersForm *parameters_form_;

  SpDocument *doc_;
};

#endif // SP_MAIN_WINWOW_H_
