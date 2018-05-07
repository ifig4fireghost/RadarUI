#ifndef LIBS_H
#define LIBS_H

#include <QObject>
#include <QLocalSocket>

class Libs : public QObject
{
	Q_OBJECT
public:
	static void Log2Console(const QString& log);
	static QString Generate(const QString& keyword);
	static bool Decode(QByteArray*& out, const QByteArray& in);
	static bool Encode(QByteArray*& out, const QByteArray& in);
	static Libs& GetInstance();
signals:
	void dataready(const QByteArray& dat);

public slots:
	void OnWrite(QString dat);
	void OnDataReady();

public:
	bool Initialize();
	void Release();
	void GetFrame();

private:
	QLocalSocket *sock;
	Libs(){}
};

#endif // LIBS_H
