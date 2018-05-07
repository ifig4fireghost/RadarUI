#ifndef LMSDATAPARSER_H
#define LMSDATAPARSER_H

#include <QObject>
#include <QPointF>

class LmsDataParser : public QObject
{
	Q_OBJECT
public:
	static LmsDataParser& GetInstace();
	void Parse(const QByteArray& data);
	QList<QPointF>* points;
signals:

public slots:

private:
	explicit LmsDataParser(QObject *parent = nullptr);
};

#endif // LMSDATAPARSER_H
