#include <QMenuBar>

#include <QFileDialog>
#include <QtGui>

#include "3D_viewer.h"
#include "main_window.h"

SpMainWindow::SpMainWindow(QWidget *parent)
  : QMainWindow(parent) {

  setDockNestingEnabled(true);
  CreateActions();
  CreateMenus();

  setWindowTitle("Stereo Planer");

  doc_ = new SpDocument;

  parameters_form_ = new ParametersForm(this);
  parameters_dock_ = new QDockWidget("Tools", this);
  parameters_dock_->setWidget(parameters_form_);
  addDockWidget(Qt::RightDockWidgetArea, parameters_dock_);

  parameters_form_->SetDocument(doc_);

  NewCaptureViewer();
  //NewSensorViewer();
  //NewScreenViewer();
  NewTheaterViewer();
}

SpMainWindow::~SpMainWindow() {
  delete doc_;
}

void SpMainWindow::NewCaptureViewer() {
  CaptureViewer *viewer = new CaptureViewer(NULL, this);
  viewer->SetDocument(doc_);
  QDockWidget *dock = new QDockWidget("Capture Geometry", this);
  dock->setWidget(viewer);
  addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void SpMainWindow::NewSensorViewer() {
  GeometryViewer *viewer = new GeometryViewer(NULL, this);
  viewer->SetGeometry(&doc_->SensorGeometry(0));
  QDockWidget *dock = new QDockWidget("Sensor Geometry", this);
  dock->setWidget(viewer);
  addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void SpMainWindow::NewScreenViewer() {
  GeometryViewer *viewer = new GeometryViewer(NULL, this);
  viewer->SetGeometry(&doc_->ScreenGeometry(0));
  QDockWidget *dock = new QDockWidget("Screen Geometry", this);
  dock->setWidget(viewer);
  addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void SpMainWindow::NewTheaterViewer() {
  TheaterViewer *viewer = new TheaterViewer(NULL, this);
  viewer->SetDocument(doc_);
  QDockWidget *dock = new QDockWidget("Theater Geometry", this);
  dock->setWidget(viewer);
  addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void SpMainWindow::CreateActions() {
  open_action_ = new QAction(tr("&Open..."), this);
  open_action_->setShortcut(tr("Ctrl+O"));
  open_action_->setStatusTip(tr("Open something"));
//  connect(open_images_action_, SIGNAL(triggered()),
//          this, SLOT(OpenImages()));

  new_capture_viewer_action_ = new QAction(tr("&Capture"), this);
  new_capture_viewer_action_->setStatusTip(tr("New Capture Viewer"));
  connect(new_capture_viewer_action_, SIGNAL(triggered()),
          this, SLOT(NewCaptureViewer()));

  new_sensor_viewer_action_ = new QAction(tr("&Sensor"), this);
  new_sensor_viewer_action_->setStatusTip(tr("New Sensor Viewer"));
  connect(new_sensor_viewer_action_, SIGNAL(triggered()),
          this, SLOT(NewSensorViewer()));

  new_screen_viewer_action_ = new QAction(tr("&Screen"), this);
  new_screen_viewer_action_ ->setStatusTip(tr("New Screen Viewer"));
  connect(new_screen_viewer_action_, SIGNAL(triggered()),
          this, SLOT(NewScreenViewer()));

  new_theater_viewer_action_ = new QAction(tr("&Theater"), this);
  new_theater_viewer_action_->setStatusTip(tr("New Theater Viewer"));
  connect(new_theater_viewer_action_, SIGNAL(triggered()),
          this, SLOT(NewTheaterViewer()));
}

void SpMainWindow::CreateMenus() {
  file_menu_ = menuBar()->addMenu(tr("&File"));
  file_menu_->addAction(open_action_);
  file_menu_ = menuBar()->addMenu(tr("&View"));
  file_menu_->addAction(new_capture_viewer_action_);
  file_menu_->addAction(new_sensor_viewer_action_);
  file_menu_->addAction(new_screen_viewer_action_);
  file_menu_->addAction(new_theater_viewer_action_);
}

