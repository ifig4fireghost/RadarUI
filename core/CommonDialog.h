#ifndef COMMONDIALOG_H
#define COMMONDIALOG_H

#include <QDialog>

class CommonDialog : public QDialog
{
    Q_OBJECT
public:
    CommonDialog() {}

signals:
    void change(const QString& to);
};

#endif // COMMONDIALOG_H
