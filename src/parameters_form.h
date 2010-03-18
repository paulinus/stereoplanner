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
 
 public slots:
  
 private:
   Ui::ParametersForm ui_;
   SpDocument *doc_;
};

#endif // SP_PARAMETERS_FORM_H_
