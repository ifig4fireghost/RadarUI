#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts>
QT_CHARTS_USE_NAMESPACE

#include <QWidget>
#include <QPointF>
namespace Ui {
class ChartView;
}

class ChartView : public QChartView
{
	Q_OBJECT

public:
	explicit ChartView(QWidget *parent = 0);
	~ChartView();
	void keyPressEvent(QKeyEvent*);
	void ChangeHeight(double);
signals:
	void HeightChanged(double);
public slots:
	void OnUpdatePoints(const QList<QPointF>* points);
private slots:
	void on_et_standard_editingFinished();

private:
	Ui::ChartView *ui;
	QScatterSeries* series;

	bool isClicking;
	int xOld;
	int yOld;
};

#endif // CHARTVIEW_H
