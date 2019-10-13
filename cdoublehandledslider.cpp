/*
cdoublehandledslider.h/cpp
--------------------------
Begin: 2015-06-15
Author: Aaron Reeves <aaron.reeves@sruc.ac.uk>
---------------------------------------------------
Copyright (C) 2015 Epidemiology Research Unit, Scotland's Rural College (SRUC)

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/

//Project
#include "cdoublehandledslider.h"

//Qt
#include <QMouseEvent>
#include <qmimedata.h>
#include <qdrag.h>
#include <qwidgetaction.h>
#include <qapplication.h>
#include <qpixmap.h>
#include <qcursor.h>
#include <qguiapplication.h>
#include <qdir.h>
#include <QProxyStyle>
#include <QDebug>
#include <QStyleOptionSlider>

#include <math.h>

class SliderProxy : public QProxyStyle {
  public:
    int pixelMetric ( PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const
    {
      switch(metric) {
      case PM_SliderThickness        : return 16;
      case PM_SliderLength           : return 8;
      default                        : return (QProxyStyle::pixelMetric(metric,option,widget));
      }
    }
};


CDoubleHandledSlider::CDoubleHandledSlider(QWidget *parent)  : QSlider(parent) {
  //styling
  setOrientation(Qt::Horizontal);
  setAcceptDrops(true);
  SliderProxy *aSliderProxy = new SliderProxy();

  QString path = ":/slider/handle3.png";
  setStyleSheet("QSlider::handle { image: url(" + path + "); }");
  setStyle(aSliderProxy);

  //setting up the alternate handle
  altHandle = new SuperSliderHandle(this);
  addAction(new QWidgetAction(altHandle));
  //altHandle->move(this->pos().x() + this->width()- altHandle->width(), this->pos().y() );

  _hasBoundedMin = false;
  _hasBoundedMax = false;
}


int CDoubleHandledSlider::altValue() {
  return altHandle->value();
}


void CDoubleHandledSlider::setAltValue(int value) {
  altHandle->setValue(value);
}


void CDoubleHandledSlider::mouseReleaseEvent(QMouseEvent *mouseEvent) {
  if (mouseEvent->button() == Qt::LeftButton) {
    altHandle->show();
    altHandle->handleActivated = false;
  }
  mouseEvent->accept();
}


void CDoubleHandledSlider::altUpdate( QMouseEvent* event ) {
  QPoint point( this->mapFromParent( event->pos() ) );
  int result = QStyle::sliderValueFromPosition( this->minimum(), this->maximum(), point.x(), this->width() );

  if( _hasBoundedMin && ( result < _boundedMin ) )
    result = _boundedMin;

  if( _hasBoundedMax && ( result > _boundedMax ) )
    result = _boundedMax;

  this->setAltValue( result );
  emit altValueChanged( altValue() );
  //int pos = QStyle::sliderPositionFromValue( this->minimum(), this->maximum(), result, this->width() );
  //qDebug() << "Alt :" << point.x() << result << this->sliderPosition() << pos;
}


//void CDoubleHandledSlider::mousePressEvent( QMouseEvent* event ) {
//  QStyleOptionSlider opt;
//  initStyleOption(&opt);
//  QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
//  _clickOffset = event->pos() - sr.topLeft();
//  qDebug() << _clickOffset;
//}


void CDoubleHandledSlider::mouseMoveEvent( QMouseEvent* event ) {
  int result = QStyle::sliderValueFromPosition( this->minimum(), this->maximum(), event->x(), this->width() );

  if( _hasBoundedMin && ( result < _boundedMin ) )
    result = _boundedMin;

  if( _hasBoundedMax && ( result > _boundedMax ) )
    result = _boundedMax;

  this->setValue( result );
  emit valueChanged( result );
}


void SuperSliderHandle::setValue(int value) {
  _value = value;

//  int width = parent->width();
//  int range = parent->maximum() - parent->minimum();
//  int stepSize = trunc( double(width)/double(range) );

//  int xPos1 = (stepSize * value) - (stepSize * parent->minimum());
//  int xPos2 = trunc( (value - parent->minimum()) * double(width)/double(range) );
//  int xPos3 = round( (value - parent->minimum()) * double(width)/double(range) );
//  int xPos4 = floor( (value - parent->minimum()) * double(width)/double(range) );
//  int xPos5 = ceil( (value - parent->minimum()) * double(width)/double(range) );
//  int xPos6 = (value - parent->minimum()) * width/range;
//  int qtxPos = QStyle::sliderPositionFromValue( parent->minimum(), parent->maximum(), value, parent->width() );

//  //qDebug() << parent->sliderPosition() << xPos1 << xPos2 << xPos3 << xPos4 << xPos5 << xPos6 << qtxPos;

  int xPos = QStyle::sliderPositionFromValue( parent->minimum(), parent->maximum(), value, parent->width() );

  int range = parent->maximum() - parent->minimum();
  double adj = double( this->pixmap()->width() ) / double( range );

  //qDebug() << this->pixmap()->width() << range << adj << xPos << xPos - ( adj * value );

  xPos = xPos - ( adj * value ); // 5; // int( double(xPos) - 4);

  xPos = qMax( 0, qMin( xPos, (parent->width() - this->pixmap()->width()) ) );

  this->move( xPos, 0 );

//  qDebug() << xPos;
//  qDebug() << this->size().width() << this->size().height() << this->geometry().left() << this->pos().x() << this->rect().left();
}


int SuperSliderHandle::value() {
  return _value;
}


void CDoubleHandledSlider::reset() {
  int horBuffer = (altHandle->width());
  QPoint myPos = mapToGlobal(pos()); 
  QPoint point(myPos.x() + width() - horBuffer, myPos.y()- altHandle->height());
  point = altHandle->mapFromParent(point);

  altHandle->move(point);
  altHandle->show();
  altHandle->raise();
}


SuperSliderHandle::SuperSliderHandle(CDoubleHandledSlider *_parent) : QLabel(_parent) {
  parent = _parent;
  filter = new SuperEventFilter(parent);

  //styling
  setAcceptDrops(true);
  QPixmap pix = QPixmap( ":/slider/handle3.png" );
  //pix =  pix.scaled( 8, 16, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );
  setPixmap(pix);
}


void SuperSliderHandle::mousePressEvent(QMouseEvent *mouseEvent) {
  Q_UNUSED( mouseEvent );

//  QStyleOptionSlider opt;
//  initStyleOption(&opt);
//  QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
//  _clickOffset = mouseEvent->pos() - this->geometry().topLeft();
//  _offset = mouseEvent->pos().x();

//  qDebug() << mouseEvent->pos() << this->geometry().topLeft() << _clickOffset;

  qGuiApp->installEventFilter(filter);
  parent->clearFocus();
}


bool SuperEventFilter::eventFilter(QObject* obj, QEvent* event) {
  switch(event->type()) {
  case QEvent::MouseButtonRelease:
    qGuiApp->removeEventFilter(this);
    return true;
    break;
  case QEvent::MouseMove:
    grandParent->altUpdate( static_cast<QMouseEvent *>(event) );
    return true;
    break;
  default:
    return QObject::eventFilter(obj, event);
  }
  return false;
}








