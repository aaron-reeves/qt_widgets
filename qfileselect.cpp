#include "qfileselect.h"
#include "ui_qfileselect.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>


CFileSelect::CQFileSelect( QWidget* parent, Qt::WindowFlags f ) : QWidget( parent, f ), ui(new Ui::CFileSelect) {
  ui->setupUi(this);

  _manualEdit = false;

  // Filter format: "Plain text files (*.txt);; All Files (*.*)"
  _filter = "All Files (*.*)";
  _caption = "Please select a file";
  _pathName = QString();
  _dir = QString();
  _mode = ModeUnspecified;

  connect( ui->btnBrowse, SIGNAL( clicked() ), this, SLOT( selectFile() ) );
  connect( ui->leFilePath, SIGNAL( textEdited( const QString& ) ), this, SLOT( updatePathName( const QString& ) ) );
}


QFileSelect::~QFileSelect() {
  delete ui;
}


void CQFileSelect::clearPath() {
  ui->leFilePath->clear();
}

void QFileSelect::setMode( const int fileMode ) {
  switch( fileMode ) {
    case ModeOpenFile:
      // fall through
    case ModeSaveFile:
      _mode = fileMode;
      break;
    default:
      qDebug() << "There is a problem in QFileSelect::setMode()";
      _mode = ModeUnspecified;
      break;
  }


void QFileSelect::setCaption( const QString& val ) {
  _caption = val;
}


void QFileSelect::setLabel( const QString& val ) {
  ui->lblFilePath->setText( val );
}
void CQFileSelect::selectFolder() {
  QString s;

  s = QFileDialog::getExistingDirectory(
    this,
    _caption,
    _dir
  );

  if( !s.isEmpty() ) {
    ui->leFilePath->setText( s );
    _pathName =  s;
    _dir = QFileInfo( _pathName ).absoluteDir().absolutePath();
    _manualEdit = false;
  }
}


void CQFileSelect::selectFile() {
  if( ExistingDir == mode() )
    return selectFolder();
  else {
    QString s;

  switch( _mode ) {
    case ModeOpenFile:
    QFileDialog dialog(this);
    dialog.setDirectory( _dir );
    dialog.setNameFilter( _filter );

    if( (ExistingDir == mode()) || (ExistingFile == mode()) )
      dialog.setFileMode( QFileDialog::ExistingFile );
    else
      dialog.setFileMode( QFileDialog::AnyFile );

    if( dialog.exec() )
      s = dialog.selectedFiles().at(0);
    else
      s = "";

    /*
    s = QFileDialog::getOpenFileName(
      this,
      _caption,
      _dir,
      _filter,
      NULL,
      option
    );
    */
      break;
    case ModeSaveFile:
      s = QFileDialog::getSaveFileName(
        this,
        _caption,
        _dir,
        _filter
      );
      break;
    default:
      qDebug() << "There is a problem in QFileSelect::selectFile()";
      s = "";
      break;
  }

  if( !s.isEmpty() ) {
    ui->leFilePath->setText( s );
    _pathName =  s;
    _dir = QFileInfo( _pathName ).absoluteDir().absolutePath();
    _manualEdit = false;
  }
}
}


void QFileSelect::updatePathName( const QString& unused ) {
  qDebug() << unused;

  _manualEdit = true;
}


void QFileSelect::setText( const QString& val ) {
  ui->lblFilePath->setText( val );
}


QString QFileSelect::text( void ) {
  return( ui->lblFilePath->text() );
}


void QFileSelect::setPathName( const QString& val ) {
  _pathName = QFileInfo( val ).absoluteFilePath();
  _dir = QFileInfo( _pathName ).absoluteDir().absolutePath();
  _manualEdit = false;
  ui->leFilePath->setText( _pathName );
}


void QFileSelect::setDir( const QString& val ) {
  _dir = QFileInfo( val ).absoluteDir().absolutePath();
}


QString QFileSelect::dir( void ) {
  if( true == _manualEdit ) {
    setPathName( ui->leFilePath->text() );
  }
  return _dir;
}


QString QFileSelect::pathName( void ) {
  if( _manualEdit ) {
    setPathName( ui->leFilePath->text() );
  }
  return _pathName;
}
