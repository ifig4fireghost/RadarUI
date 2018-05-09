#include "ChartView.h"
#include "ui_ChartView.h"

#include <iostream>
using std::cout;
using std::endl;

#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

void ChartView::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_A:
		chart()->scroll(-10, 0);
		break;
	case Qt::Key_D:
		chart()->scroll(10, 0);
		break;
	case Qt::Key_W:
		chart()->scroll(0, 10);
		break;
	case Qt::Key_S:
		chart()->scroll(0, -10);
		break;
	case Qt::Key_Q:
		chart()->zoom(1.1);
		break;
	case Qt::Key_E:
		chart()->zoom(10.0 / 11);
		break;
	case Qt::Key_R:
		chart()->zoomReset();
		break;
	default:
		return;
	}
	event->accept();
}

ChartView::ChartView(QWidget *parent) :
	QChartView(parent),
	ui(new Ui::ChartView),
	isClicking(false),
	xOld(0), yOld(0)
{
	ui->setupUi(this);
	series = new QScatterSeries;
	series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	series->setMarkerSize(16);

	QChart* chart = ui->chart->chart();
	chart->legend()->setAlignment(Qt::AlignBottom);
	chart->setAnimationOptions(QChart::SeriesAnimations);
	chart->createDefaultAxes();
	chart->addSeries(series);

	QValueAxis* ax = new QValueAxis;
	ax->setRange(-2048, 2048);
	chart->setAxisX(ax, series);

	QValueAxis* ay = new QValueAxis;
	ay->setRange(-16, 7000);

	chart->setAxisY(ay, series);

	setChart(chart);
	setRubberBand(QChartView::RectangleRubberBand);
}

ChartView::~ChartView()
{
	delete ui;
}

void ChartView::OnUpdatePoints(const QList<QPointF>* points)
{
	series->clear();
	foreach (QPointF p, *points) {
		*series << p;
		ui->lb_lcz->setText(QString("%1").arg(p.x()));
		ui->lb_dg->setText(QString("%1").arg(p.y()));
	}
}

