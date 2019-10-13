/*
qclickablelabel.h/cpp
----------------------
Begin: 2018-10-27
Author: Aaron Reeves <aaron.reeves@sruc.ac.uk>
---------------------------------------------------
Copyright (C) 2018 Epidemiology Research Unit, Scotland's Rural College (SRUC)

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

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
