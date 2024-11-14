#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QSerialPort>
#include <QTimer>
#include <QQuickWidget>
#include <QGeoCoordinate>
#include <QtQuick>
#include <QtLocation>
#include <QtPositioning>
#include <QLCDNumber>
#include <QProgressBar>
#include <QPushButton>
#include "Led.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateMap(double lat, double lon);
    void handleButtonPress();
    void updateTimeLabel();

private:
    Ui::MainWindow *ui;
    QSerialPort *arduino;
    QTimer *timer;
    QVector<double> xData[7], yData[7];
    QCustomPlot* customPlots[5];  // Reduced to 5 as we're replacing the GPS plot
    QQuickWidget *mapWidget;      // New map widget
    double timeElapsed;
    double refLatitude;
    double refLongitude;
    double maxHeight;
    QProgressBar *maxHeightProgress;  // QProgressBar for max height
    QLCDNumber *sensorDisplays[5];    // Array of QLCDNumber for sensor values
    QLCDNumber *maxHeightLCD;
    QLabel *timeLabel;
    QVector<QGeoCoordinate> gpsTrack;
    QMap<QPushButton*, bool> buttonStates;
    QMap<QPushButton*, Led*> buttonLedPairs;
    Led* ledIndicator;

    void setupPlots();
    void readSerialData();
    void updatePlots();
    void setupMap();
    void toggleButtonColor(QPushButton *button);
};
#endif // MAINWINDOW_H
