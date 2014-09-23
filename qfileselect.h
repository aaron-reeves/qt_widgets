#ifndef QFILESELECT_H
#define QFILESELECT_H

#include <QWidget>
#include <QString>

namespace Ui {
  class QFileSelect;
}

class QFileSelect : public QWidget {
  Q_OBJECT

  public:
    enum FileDialogModes {
      ModeUnspecified,
      ModeOpenFile,
      ModeSaveFile
    };

    explicit QFileSelect(QWidget *parent = 0);
    ~QFileSelect();

    // Filter format: "Plain text files (*.txt);; All Files (*.*)"
    void setFilter( const QString& val ) { _filter = val; }
    void setPathName( const QString& val );
    void setText( const QString& val );
    void setCaption( const QString& val ) { _caption = val; }
    void setDir( const QString& val );
    void setMode( const int fileMode );
    void setLabel( const QString& val );


    QString filter( void ) { return _filter; }
    QString pathName( void );
    QString text( void );
    QString caption( void ) { return _caption; }
    QString dir( void );
    int mode( void ) { return _mode; }
    QString label();

    // FIX ME: Make this a real function!
    bool fileIsValid( void ) { return true; }

  protected slots:
    void selectFile();
    void updatePathName( const QString& unused );

  signals:
    void pathNameChanged( void );
    void filterChanged( void );

  protected:
    bool _manualEdit;

    QString _filter;
    QString _pathName;
    QString _caption;
    QString _dir;
    int _mode;

  private:
    Ui::QFileSelect *ui;
};

#endif // QFILESELECT_H
