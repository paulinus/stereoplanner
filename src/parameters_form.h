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

 private:
   Ui::ParametersForm ui_;
   SpDocument *doc_;
};

#endif // SP_PARAMETERS_FORM_H_
