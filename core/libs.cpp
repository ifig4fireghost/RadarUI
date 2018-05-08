#include "libs.h"

#include <zlib.h>
#include <string.h>
#include <iostream>

#include <QCryptographicHash>

static const QString& LMS_SET_ACCESS_MODE_MT = "sMN SetAccessMode 02 B21ACE26";
static const QString& LMS_SET_ACCESS_MODE_AC = "sMN SetAccessMode 03 F4724744";

static const QString& LMS_GET_SCAN_CFG = "sRN LMPscancfg";
static const QString& LMS_GET_STATUS   = "sRN STlms";
static const QString& LMS_START_SCAN = "sMN run";

static const QString& CURRENT_FRAME_GET   = "sRN LMDscandata";
static const QString& CURRENT_STREAM_GET  = "sEN LMDscandata 1";
static const QString& CURRENT_STREAM_STOP = "sEN LMDscandata 0";

void Libs::Log2Console(const QString& log)
{
	std::cout << log.toStdString() << std::endl;
}

QString Libs::Generate(const QString& keyword)
{
	QByteArray ba = QCryptographicHash::hash(keyword.toLatin1(), QCryptographicHash::Md5);
	return "/tmp/" + QString(ba.toHex()) + ".socket";
}

bool Libs::Decode(QByteArray*& out, const QByteArray& in)
{
	QByteArray data = QByteArray::fromBase64(in);
	size_t data_len = data.length();
	uLongf buf_len = compressBound(data_len * 4);
	Bytef *decode_data = new Bytef[buf_len];
	if (decode_data == NULL) {
		return false;
	}
	int ret = uncompress(decode_data, &buf_len, (Bytef*)data.constData(), data_len);
	if (ret != Z_OK) {
		return false;
	}
	out = new QByteArray((const char*) decode_data, buf_len);
	delete decode_data;
	return true;
}

bool Libs::Encode(QByteArray*& out, const QByteArray& in)
{
	size_t in_len = in.length();
	uLongf buf_len = compressBound(in_len);
	Bytef *encode_data = new Bytef[buf_len];
	if (encode_data == NULL) {
		return false;
	}
	int ret = compress(encode_data, &buf_len, (Bytef*)in.constData(), in_len);
	if (ret != Z_OK) {
		return false;
	}
	out = new QByteArray(QByteArray((const char*) encode_data, buf_len).toBase64());

	delete encode_data;
	return true;
}

Libs& Libs::GetInstance()
{
	static Libs libs;
	return libs;
}

void Libs::OnWrite(QString dat)
{
	Log2Console(dat);
	sock->write(dat.toLocal8Bit().toBase64());
	sock->waitForBytesWritten();
}

void Libs::OnDataReady()
{
	QByteArray dat = QByteArray::fromBase64(sock->readAll());
	Log2Console(QString(dat));
	emit dataready(dat);
}

void Libs::GetFrame()
{
	OnWrite("&C=GF");
}

void Libs::Release()
{
	OnWrite("&C=QT");
	sock->disconnectFromServer();
	sock->deleteLater();
	Log2Console("Libs released");
}

bool Libs::Initialize()
{
	QString path = Generate("ifig-graduate-project");
	Log2Console(path);

	sock = new QLocalSocket;
	sock->connectToServer(path);
	connect(sock, SIGNAL(readyRead()), this, SLOT(OnDataReady()));
	return sock->waitForConnected();
}
