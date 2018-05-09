#include "LmsDataParser.h"

#include <iostream>
#include <cmath>

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
		ssa = -((~ssa_t.toUInt(nullptr, 16) + 1) / 10000 * Pi / 180);
	} else {
		ssa = ssa_t.toULong(nullptr, 16) / 10000 * Pi / 180;
	}
	cout << "扫描开始角度:" << ssa << endl;
	if (ssa < -1 || ssa > Pi) {
		cout << "扫描开始角度有误，请检查原因！" << endl;
		return;
	}
	double srt = double(all[SRT].toUInt(nullptr, 16)) / 10000 * Pi / 180;
	cout << "分辨率:" << srt << endl;
	int sdn = all[SDN].toUInt(nullptr, 16);
	cout << "总点数:" << sdn << endl;
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
		//if (md > 7000 || md < 5000) continue;
		double angle = ssa + (i - MDS) * srt;
		if (angle > Pi * 4 / 9 && angle < Pi * 5 / 9) {//?_?
			points->push_back(QPointF(md * cos(angle), md * sin(angle)));
		}
	}
}
