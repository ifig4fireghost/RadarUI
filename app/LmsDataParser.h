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

public slots:
	void OnHeightChanged(double h);

private:
	explicit LmsDataParser(QObject *parent = nullptr);
	double height;
};

#endif // LMSDATAPARSER_H
