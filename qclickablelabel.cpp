// From https://wiki.qt.io/Clickable_QLabel

#include "qclickablelabel.h"

QClickableLabel::QClickableLabel(QWidget* parent, Qt::WindowFlags f) : QLabel(parent, f) {
  // Nothing more to do here.
}

QClickableLabel::~QClickableLabel() {
  // Nothing to do here.
}

void QClickableLabel::mousePressEvent(QMouseEvent* event) {
  Q_UNUSED( event );
  emit clicked();
}
