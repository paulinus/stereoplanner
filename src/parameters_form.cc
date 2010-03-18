#include "parameters_form.h"

ParametersForm::ParametersForm(QWidget *parent) : QWidget(parent), doc_(0) {
  ui_.setupUi(this);
}

void ParametersForm::SetDocument(SpDocument *doc) {
  doc_ = doc;
  if (doc_) {
    // Make connections.
    connect(doc_, SIGNAL(DocumentChanged()), this, SLOT(UpdateParameters()));
  }
  UpdateParameters();
}
