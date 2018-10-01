#include "CFileSelect.h"
#include "ui_CFileSelect.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>


CFileSelect::CFileSelect( QWidget* parent, Qt::WindowFlags f ) : QWidget( parent, f ), ui(new Ui::CFileSelect) {
  ui->setupUi(this);

  this->setFocusProxy( ui->leFilePath );

  // Filter format: "Plain text files (*.txt);; All Files (*.*)"
  _filter = "All Files (*.*)";
  _caption = "Please select a file";
  _pathName = QString();
  _dir = QString();
  _mode = ModeUnspecified;

  connect( ui->btnBrowse, SIGNAL( clicked() ), this, SLOT( selectFile() ) );

  connect( ui->leFilePath, SIGNAL( textEdited( QString ) ), this, SLOT( editText( QString ) ) );
  connect( ui->leFilePath, SIGNAL( textChanged( QString ) ), this, SLOT( changeText( QString ) ) );
  connect( ui->leFilePath, SIGNAL( editingFinished() ), this, SLOT( finishEditing() ) );
}


CFileSelect::~CFileSelect() {
  delete ui;
}


void CFileSelect::clearPath() {
  ui->leFilePath->clear();
}

void CFileSelect::setMode( const int mode ) {
  _mode = mode;
}

void CFileSelect::setCaption( const QString& val ) {
  _caption = val;
}


void CFileSelect::setLabel( const QString& val ) {
  ui->lblFilePath->setText( val );
}


void CFileSelect::selectFolder() {
  QString s;

  s = QFileDialog::getExistingDirectory(
    this,
    _caption,
    _dir
  );

  if( !s.isEmpty() ) {
    ui->leFilePath->setText( s );
    _pathName =  s;

    QFileInfo fi( _pathName );

    if( fi.isDir() ) {
      _dir = fi.absoluteFilePath();
    }
    else {
      _dir = fi.absolutePath();
    }

    qDebug() << "_dir from selectFolder():" << _dir;
    emit pathNameChanged();
  }
}


void CFileSelect::finishEditing() {
  ui->leFilePath->setText( _pathName );
  emit editingFinished();
}


void CFileSelect::selectFile() {
  if( _mode & ModeExistingDir )
    return selectFolder();
  else {
    QString s;
    QFileDialog dialog(this);

    if( _mode & ModeOpenFile ) {
      dialog.setDirectory( _dir );
      dialog.setNameFilter( _filter );

      if( _mode & ModeExistingFile )
        dialog.setFileMode( QFileDialog::ExistingFile );
      else
        dialog.setFileMode( QFileDialog::AnyFile );

      // Something about this call causes the text "Folder createdFolder created" to be written to the console.
      // It's a little weird.
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
    }
    else if( _mode & ModeSaveFile ) {
      s = QFileDialog::getSaveFileName(
        this,
        _caption,
        _dir,
        _filter
      );
    }
    else {
      qDebug() << "There is a problem in CFileSelect::selectFile()";
      s = "";
    }

    if( !s.isEmpty() ) {
      ui->leFilePath->setText( s );
      _pathName =  s;

      QFileInfo fi( _pathName );

      if( fi.isDir() ) {
        _dir = fi.absoluteFilePath();
      }
      else {
        _dir = fi.absolutePath();
      }

      qDebug() << "_dir from selectFile():" << _dir;
      emit pathNameChanged();
    }
  }
}


void CFileSelect::editText( const QString& val ) {
  setPathNameInternal( val );
  emit textEdited( val );
}


void CFileSelect::changeText( const QString& val ) {
  setPathNameInternal( val );
  emit textChanged( val );
}


void CFileSelect::setText( const QString& val ) {
  ui->lblFilePath->setText( val );
}


QString CFileSelect::text( void ) {
  return( ui->lblFilePath->text() );
}


void CFileSelect::setPathName( const QString& val ) {
  setPathNameInternal( val.trimmed() );
  ui->leFilePath->setText( val.trimmed() );
}


void CFileSelect::setPathNameInternal( const QString& val ) {
  if( val.trimmed().isEmpty() ) {
    _pathName = QString();
    _dir = QString();
  }
  else {
    QFileInfo fi( val );

    _pathName = fi.absoluteFilePath();

    if( fi.isDir() ) {
      _dir = fi.absoluteFilePath();
    }
    else {
      _dir = fi.absolutePath();
    }

    qDebug() << "_dir from setPathNameInternal():" << _dir;
    emit pathNameChanged();
  }
}


void CFileSelect::setDir( QString val ) {
  val = val.trimmed();
  if( !val.isEmpty() ) {
    QFileInfo fi( val );

    if( fi.isDir() ) {
      _dir = fi.absoluteFilePath();
    }
    else {
      _dir = fi.absolutePath();
    }
  }
  else
    _dir = QString();
}


QString CFileSelect::dir( void ) {
  return _dir;
}


QString CFileSelect::pathName( void ) {
  return _pathName;
}


