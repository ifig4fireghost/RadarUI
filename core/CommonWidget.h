#ifndef COMMONWIDGET_H
#define COMMONWIDGET_H

#include <QWidget>

class CommonWidget : public QWidget
{
    Q_OBJECT
public:
    CommonWidget() {}

public slots:
    virtual void onReceived(const QVariant&) = 0;

signals:
    void change(const QString& to);
};

#endif // COMMONWIDGET_H
