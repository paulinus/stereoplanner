#ifndef SP_PARAMETERS_FORM_H_
#define SP_PARAMETERS_FORM_H_

#include <QWidget>

#include "ui_parameters_form.h"
#include "document.h"


class ParametersForm : public QWidget {
  Q_OBJECT

 public:
  ParametersForm(QWidget *parent = 0);
  ~ParametersForm() {}
  void SetDocument(SpDocument *doc);
 
 public slots:
   void UpdateParameters() {
     ui_.focalLengthSpinBox->setValue(doc_->FocalLegth());
     ui_.sensorWidthSpinBox->setValue(doc_->SensorWidth());
     ui_.sensorHeightSpinBox->setValue(doc_->SensorHeight());

     ui_.rigInterocularSpinBox->setValue(doc_->RigInterocular());
     ui_.rigXSpinBox->setValue(doc_->RigX());
     ui_.rigYSpinBox->setValue(doc_->RigY());
     ui_.rigZSpinBox->setValue(doc_->RigZ());
     ui_.rigPanSpinBox->setValue(doc_->RigPan());
     ui_.rigTiltSpinBox->setValue(doc_->RigTilt());
     ui_.rigRollSpinBox->setValue(doc_->RigRoll());

     ui_.screenWidthSpinBox->setValue(doc_->ScreenWidth());
     ui_.screenHeightSpinBox->setValue(doc_->ScreenHeight());

     ui_.observerInterocularSpinBox->setValue(doc_->ObserverInterocular());
     ui_.observerXSpinBox->setValue(doc_->ObserverX());
     ui_.observerYSpinBox->setValue(doc_->ObserverY());
     ui_.observerZSpinBox->setValue(doc_->ObserverZ());
     ui_.observerPanSpinBox->setValue(doc_->ObserverPan());
     ui_.observerTiltSpinBox->setValue(doc_->ObserverTilt());
     ui_.observerRollSpinBox->setValue(doc_->ObserverRoll());
   }

   void on_focalLengthSpinBox_valueChanged(double v) {
     doc_->SetFocalLegth(v);
   }
   void on_sensorWidthSpinBox_valueChanged(double v) {
     doc_->SetSensorWidth(v);
   }
   void on_sensorHeightSpinBox_valueChanged(double v) {
     doc_->SetSensorHeight(v);
   }
   void on_rigInterocularSpinBox_valueChanged(double v) {
     doc_->SetRigInterocular(v);
   }
   void on_rigXSpinBox_valueChanged(double v) {
     doc_->SetRigX(v);
   }
   void on_rigYSpinBox_valueChanged(double v) {
     doc_->SetRigY(v);
   }
   void on_rigZSpinBox_valueChanged(double v) {
     doc_->SetRigZ(v);
   }
   void on_rigPanSpinBox_valueChanged(double v) {
     doc_->SetRigPan(v);
   }
   void on_rigTiltSpinBox_valueChanged(double v) {
     doc_->SetRigTilt(v);
   }
   void on_rigRollSpinBox_valueChanged(double v) {
     doc_->SetRigRoll(v);
   }

   void on_screenWidthSpinBox_valueChanged(double v) {
     doc_->SetScreenWidth(v);
   }
   void on_screenHeightSpinBox_valueChanged(double v) {
     doc_->SetScreenHeight(v);
   }
   void on_observerInterocularSpinBox_valueChanged(double v) {
     doc_->SetObserverInterocular(v);
   }
   void on_observerXSpinBox_valueChanged(double v) {
     doc_->SetObserverX(v);
   }
   void on_observerYSpinBox_valueChanged(double v) {
     doc_->SetObserverY(v);
   }
   void on_observerZSpinBox_valueChanged(double v) {
     doc_->SetObserverZ(v);
   }
   void on_observerPanSpinBox_valueChanged(double v) {
     doc_->SetObserverPan(v);
   }
   void on_observerTiltSpinBox_valueChanged(double v) {
     doc_->SetObserverTilt(v);
   }
   void on_observerRollSpinBox_valueChanged(double v) {
     doc_->SetObserverRoll(v);
   }


 private:
   Ui::ParametersForm ui_;
   SpDocument *doc_;
};

#endif // SP_PARAMETERS_FORM_H_
