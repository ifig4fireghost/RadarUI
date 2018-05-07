#include "LmsDataParser.h"

#include <iostream>
#include <math.h>

using std::cout;
using std::endl;
using std::cos;
using std::sin;
using std::abs;

static const double Pi = 3.1415926;

enum LMS_POS{
	Status,
	Command,
	Version,
	DeviceNumber,
	DeviceID,
	DeviceStatusH,
	DeviceStatusL,
	FrameCnt,
	ScanCnt,
	ScanTime,
	FrameSendStartTime,
	InputStatusH,
	InputStatusL,
	OutputStatusH,
	OutputStatusL,
	Reserve0,
	ScanFreq,
	MeasureFreq,
	Encoder,
	OutputChannel,
	Backwave,
	Ratio,
	Flag0,
	SSA,
	SRT,
	SDN,
	MDS,
	FE0 = MDS + 0x017D
};

LmsDataParser::LmsDataParser(QObject *parent) : QObject(parent)
{
	points = new QList<QPointF>;
}

LmsDataParser& LmsDataParser::GetInstace()
{
	static LmsDataParser lms;
	return lms;
}

void LmsDataParser::Parse(const QByteArray &data)
{
	QList<QByteArray> all = data.split(' ');
	if (all.size() < FE0 + 6) {
		cout << "Data length wrong!" << endl;
		return;
	}
	QByteArray ssa_t = all[SSA];
	double ssa;
	if (ssa_t[0] == 'F') {
		ssa = (~ssa_t.toUInt(nullptr, 16) + 1) / 10000 * Pi / 180;
	} else {
		ssa = ssa_t.toULong(nullptr, 16) / 10000 * Pi / 180;
	}
	double srt = double(all[SRT].toUInt(nullptr, 16)) / 10000 * Pi / 180;
	int sdn = all[SDN].toUInt(nullptr, 16);

	bool fd2 = false;
	if (all[Ratio][0] == '4') {
		fd2 = true;
	}

	points->clear();
	for (int i = MDS; i < MDS + sdn; ++i) {
		int md = all[i].toUInt(nullptr, 16);
		if (fd2) {
			md >>= 1;
		}
		if (md > 7000 || md < 5000) continue;//?_?
		double angle = ssa + i * srt;
		if (angle > Pi * 4 / 9 && angle < Pi * 5 / 9) {//?_?
			points->push_back(QPointF(md * cos(angle), md * sin(angle)));
		}
	}
}
