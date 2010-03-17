#include <QMenuBar>
#include <QFileDialog>
#include <QtGui>

#include "3D_viewer.h"
#include "main_window.h"

SpMainWindow::SpMainWindow(QWidget *parent)
  : QMainWindow(parent) {

  CreateActions();
  CreateMenus();

  viewers_area_ = new QMdiArea;
  setCentralWidget(viewers_area_);
  setWindowTitle("Stereo Planer");

  doc_ = new SpDocument;
}

SpMainWindow::~SpMainWindow() {
  delete doc_;
}

void SpMainWindow::NewCaptureViewer() {
  CaptureViewer *viewer = new CaptureViewer(NULL, this);
  viewer->SetDocument(doc_);
  viewers_area_->addSubWindow(viewer);
  viewer->show();
}

void SpMainWindow::NewTheaterViewer() {
  TheaterViewer *viewer = new TheaterViewer(NULL, this);
  viewer->SetDocument(doc_);
  viewers_area_->addSubWindow(viewer);
  viewer->show();
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
  file_menu_->addAction(new_theater_viewer_action_);
}

void SpMainWindow::Show3DView() {
//  Viewer3D *viewer = new Viewer3D(&context_, this);
  Viewer3D *viewer = new Viewer3D(NULL, this);
  viewers_area_->addSubWindow(viewer);
  viewer->show();
}

