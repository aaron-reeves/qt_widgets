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

#ifndef CDOUBLEHANDLEDSLIDER_H
#define CDOUBLEHANDLEDSLIDER_H 

#include <qslider.h>
#include <qlabel.h>

/*
* A QSlider with two handles.
* Based heavily on a "Super sick nasty awesome double handled slider!",
* a class written by Stack Overflow user AmusingTeebs (Steve).
* http://stackoverflow.com/questions/17361885/how-to-get-two-handles-in-a-qslider
*/
class SuperSliderHandle;

class CDoubleHandledSlider: public QSlider {
  Q_OBJECT

  public:
    /** Constructor */
    CDoubleHandledSlider(QWidget *parent = 0);

    /** Overridden mouse release event */
    void mouseReleaseEvent( QMouseEvent *event );

    void mouseMoveEvent( QMouseEvent* event );

//    void mousePressEvent( QMouseEvent* event );

    /** Returns the slider value for the alternate handle */
    int altValue();

    /** Convenience function for setting the value of the alternate handle */
    void setAltValue(int value);

    /** Resets the alternate handle to the right side of the slider */
    void reset();

    /** Used to update the position of the alternate handle through the use of an event filter */
    void altUpdate( QMouseEvent* event );

    void setLimits( const int minimum, const int maximum ) { setMinimum( qMin( minimum, maximum ) ); setMaximum( qMax( minimum, maximum ) ); }

    /** Return values specified by the slider handles (not the same as the range minimum and maximum). */
    int min() { return qMin( altValue(), value() ); }
    int max() { return qMax( altValue(), value() ); }

    void setMin( const int val ) { setValue( val ); }
    void setMax( const int val ) { setAltValue( val ); }


    void setBoundedMinimum( const int val ) { _hasBoundedMin = true; _boundedMin = val; }
    void setBoundedMaximum( const int val ) { _hasBoundedMax = true; _boundedMax = val; }

  signals:
    void altValueChanged(int);

  protected:
    /** Store the alternate handle for this slider*/
    SuperSliderHandle *altHandle;

    // Values for blocked minimum and blocked maximum
    bool _hasBoundedMin;
    bool _hasBoundedMax;
    int _boundedMin;
    int _boundedMax;

//    QPoint _clickOffset;
};


class SuperEventFilter : public QObject {
  public:
    /** Constructor */
    SuperEventFilter(CDoubleHandledSlider *_grandParent) { grandParent = _grandParent; }

  protected:
    /*
    *	overloaded functions for object that inherit from this class
    */
    bool eventFilter(QObject* obj, QEvent* event);

  private:
    /** Store the CDoubleHandledSlider that this event filter is used within. */
    CDoubleHandledSlider *grandParent;
};


class SuperSliderHandle: public QLabel {
  Q_OBJECT

  public:
    /** Constructor */
    SuperSliderHandle(CDoubleHandledSlider *parent = 0);

    /** An overloaded mousePressevent so that we can start grabbing the cursor and using it's position for the value */
    void mousePressEvent(QMouseEvent *event);

    /** Returns the value of this handle with respect to the slider */
    int value();

    /** Maps mouse coordinates to slider values */
    int mapValue();

    /** Store the parent as a slider so that you don't have to keep casting it  */
    CDoubleHandledSlider *parent;

    /** Store a bool to determine if the alternate handle has been activated  */
    bool handleActivated;

//    int offset() { return _offset; }

  public slots:
    /** Sets the value of the handle with respect to the slider */
    void setValue(int value);

  private:
    /** Store the filter for installation on the qguiapp */
    SuperEventFilter *filter;

//    QPoint _clickOffset;
//    int _offset;
    int _value;
};

#endif

