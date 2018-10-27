/*
cfileselect.h/cpp
-----------------
Begin: 2008/02/18
Author: Aaron Reeves <aaron.reeves@naadsm.org>
--------------------------------------------------
Copyright (C) 2008 Aaron Reeves

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
*/


#ifndef CFileSelect_H
#define CFileSelect_H

#include <QWidget>
#include <QString>

namespace Ui {
  class CFileSelect;
}

class CFileSelect : public QWidget {
  Q_OBJECT

  public:
    /*
    enum Mode {
      AnyFile,
      ExistingFile,
      ExistingDir
    };
    */
    enum FileDialogModes {
      ModeUnspecified = 0x0000,
      ModeOpenFile = 0x0001,
      ModeSaveFile = 0x0002,
      ModeAnyFile = 0x0004,
      ModeExistingFile = 0x0008,
      ModeExistingDir = 0x0010
    };

    explicit CFileSelect( QWidget* parent = 0, Qt::WindowFlags f = 0 );

    ~CFileSelect( void );

    // Filter format: "Plain text files (*.txt);; All Files (*.*)"
    void setFilter( const QString& val ) { _filter = val; }
    void setPathName( const QString& val );
    void setText( const QString& val );
    void setCaption( const QString& val );
    void setDir( QString val );
    void setMode( const int mode );
    void setLabel( const QString& val );


    QString filter( void ) { return _filter; }
    QString pathName( void );
    QString text( void );
    QString caption( void ) { return _caption; }
    QString dir( void );
    int mode() { return _mode; }
    QString label();

    void clearPath();

    // FIX ME: Make this a real function!
    bool fileIsValid( void ) { return true; }

  protected slots:
    void selectFile();
    void selectFolder();
    void editText( const QString& val );
    void changeText( const QString& val );
    void finishEditing();
    void setPathNameInternal( const QString& val );

  signals:
    void pathNameChanged();
    void filterChanged(); // FIXME: Not emitted.  What was this for?
    void editingFinished();
    void textEdited( const QString& str );
    void textChanged( const QString& str );

  protected:
    QString _filter;
    QString _pathName;
    QString _caption;
    QString _dir;
    int _mode;

  private:
    Ui::CFileSelect *ui;
};

#endif // CFileSelect_H
