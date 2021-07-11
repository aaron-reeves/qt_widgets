#ifndef CPROGRESSWIDGET_H
#define CPROGRESSWIDGET_H

#include <QWidget>

namespace Ui {
  class CProgressWidget;
  }

class CProgressWidget : public QWidget {
  Q_OBJECT

  public:
    explicit CProgressWidget(QWidget *parent = nullptr);
    ~CProgressWidget();

    static QString defaultStyle();
    static QString terminatedStyle();

    void setCaption( const QString& str );

    void restart();
    void start( const int nSteps, const QString& caption = QString() );
    void setComplete();
    void setTerminated();

    void setValue( const int val );
    int value() const;
    void setMaximum( const int val );
    int maximum() const;
    void setMinimum( const int val );
    int minimum() const;

  protected:
    void setCaption();

    QString _caption;
    QString _addlCaption;

  private:
    Ui::CProgressWidget *ui;

};

#endif // CPROGRESSWIDGET_H
