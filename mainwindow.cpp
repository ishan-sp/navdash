#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QQmlContext>
#include <QQuickItem>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timeElapsed(0)
    , refLatitude(0.0)
    , refLongitude(0.0)
    , maxHeight(0.0)
{
    // Initialize the UI
    ui->setupUi(this);

    // Set up QLCDNumber for displaying real-time sensor data
    for (int i = 0; i < 5; ++i) {
        sensorDisplays[i] = findChild<QLCDNumber*>(QString("sensorDisplay%1").arg(i + 1));
        if (sensorDisplays[i]) {
            sensorDisplays[i]->setSegmentStyle(QLCDNumber::Flat);
        }
    }

    // Set up maxHeightProgress
    ui->maxHeightProgress->setRange(0, 5000);
    ui->maxHeightProgress->setValue(0);

    // Connect push buttons and initialize LED indicators
    for (int i = 1; i <= 6; ++i) {
        QPushButton *button = findChild<QPushButton*>(QString("pushButton_%1").arg(i));
        Led *led = findChild<Led*>(QString("ledIndicator_%1").arg(i));

        if (button && led) {
            buttonStates[button] = false;
            button->setStyleSheet("background-color: #1c1c1c; color: white;");
            connect(button, &QPushButton::clicked, this, &MainWindow::handleButtonPress);

            // Store the LED indicator associated with this button
            buttonLedPairs[button] = led;
            led->setOff(); // Initialize LED to off state
        } else {
            qDebug() << "Failed to find button or LED for index" << i;
        }
    }

    // Set up timer to update the time label
    QTimer *timeUpdateTimer = new QTimer(this);
    connect(timeUpdateTimer, &QTimer::timeout, this, &MainWindow::updateTimeLabel);
    timeUpdateTimer->start(1000);

    // Set dark theme for the entire window
    this->setStyleSheet(
        "QWidget { background-color: #1c1c1c; color: #ffffff; }"
        "QTabWidget::pane { background: #1f1c1c; }"
        "QLabel { color: #ffffff; }"
        "QLineEdit, QComboBox, QPushButton { background-color: #2e2e2e; color: #ffffff; border: 1px solid #444444; }"
        "QTabBar::tab { background: #333333; color: #ffffff; padding: 8px; }"
        "QTabBar::tab:selected { background: #444444; }"
        );

    // Initialize the customPlots array with the UI plot widgets
    for (int i = 0; i < 5; ++i) {
        customPlots[i] = findChild<QCustomPlot*>(QString("customPlot%1").arg(i + 1));
    }

    setupPlots();
    setupMap();

    // Initialize serial communication
    arduino = new QSerialPort(this);
    arduino->setPortName("COM44");
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);

    if (arduino->open(QIODevice::ReadOnly)) {
        connect(arduino, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
        qDebug() << "Connected to Arduino";
    } else {
        qDebug() << "Failed to open COM port";
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updatePlots);
    timer->start(100);
}


MainWindow::~MainWindow()
{
    if (arduino->isOpen()) {
        arduino->close();
    }
    delete ui;
}


void MainWindow::setupPlots()
{
    // Set up QCustomPlot for 5 time series graphs with dark theme
    for (int i = 0; i < 5; ++i) {
        customPlots[i]->addGraph();

        // Dark background with a subtle grid color
        customPlots[i]->setBackground(QBrush(QColor(28, 28, 28)));
        customPlots[i]->xAxis->grid()->setPen(QPen(QColor(60, 60, 60)));
        customPlots[i]->yAxis->grid()->setPen(QPen(QColor(60, 60, 60)));

        customPlots[i]->xAxis->setLabel("Time (ms)");
        customPlots[i]->yAxis->setLabel("Telemetry Data");
        customPlots[i]->xAxis->setRange(0, 10000);
        customPlots[i]->yAxis->setRange(0, 1023);

        // Set axes and labels to white
        customPlots[i]->xAxis->setBasePen(QPen(Qt::white));
        customPlots[i]->yAxis->setBasePen(QPen(Qt::white));
        customPlots[i]->xAxis->setTickPen(QPen(Qt::white));
        customPlots[i]->yAxis->setTickPen(QPen(Qt::white));
        customPlots[i]->xAxis->setSubTickPen(QPen(Qt::white));
        customPlots[i]->yAxis->setSubTickPen(QPen(Qt::white));
        customPlots[i]->xAxis->setLabelColor(Qt::white);
        customPlots[i]->yAxis->setLabelColor(Qt::white);
        customPlots[i]->xAxis->setTickLabelColor(Qt::white);
        customPlots[i]->yAxis->setTickLabelColor(Qt::white);

        // Set plot lines to a professional shade of green
        customPlots[i]->graph(0)->setPen(QPen(QColor(50, 205, 50), 2)); // Lime green with thicker lines
    }
}

void MainWindow::setupMap()
{
    // Create QQuickWidget for the map
    mapWidget = new QQuickWidget(ui->tabWidget);
    mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mapWidget->setSource(QUrl("qrc:/map.qml"));

    // Replace the GPS plot tab with the map
    int gpsTabIndex = ui->tabWidget->indexOf(ui->gpsPlot->parentWidget());
    delete ui->gpsPlot->parentWidget();
    ui->tabWidget->insertTab(gpsTabIndex, mapWidget, "GPS Map");
}

void MainWindow::updateMap(double lat, double lon)
{
    QGeoCoordinate coord(lat, lon);
    gpsTrack.append(coord);

    // Update the map through QML
    QQuickItem *mapObject = mapWidget->rootObject();
    if (mapObject) {
        QVariant latVar(lat);
        QVariant lonVar(lon);
        QMetaObject::invokeMethod(mapObject, "addTrackPoint",
                                  Q_ARG(QVariant, latVar),
                                  Q_ARG(QVariant, lonVar));
    }
}

void MainWindow::readSerialData()
{
    QByteArray serialData = arduino->readAll();
    QString dataString = QString(serialData).trimmed();
    QStringList dataList = dataString.split(',');

    if (dataList.size() == 7) {
        for (int i = 0; i < 5; ++i) {
            bool ok;
            double receivedData = dataList[i].toDouble(&ok);
            if (ok) {
                yData[i].append(receivedData);
                xData[i].append(timeElapsed);

                // Update QLCDNumber for each sensor display
                sensorDisplays[i]->display(receivedData);

                if (i == 0 && receivedData > maxHeight) {
                    maxHeight = receivedData;
                    ui->maxHeightData->setText(QString::number(maxHeight)); // Update QLineEdit (if still needed)
                    ui->maxHeightProgress->setValue(static_cast<int>(maxHeight));  // Update QProgressBar
                }
            }
        }

        // Process GPS coordinates
        bool latOk, lonOk;
        double latitude = dataList[5].toDouble(&latOk);
        double longitude = dataList[6].toDouble(&lonOk);

        if (latOk && lonOk) {
            updateMap(latitude, longitude);
        }

        // Limit the number of points for time series data
        for (int i = 0; i < 5; ++i) {
            if (xData[i].size() > 100) {
                xData[i].remove(0);
                yData[i].remove(0);
            }
        }
    }
}


void MainWindow::updatePlots()
{
    // Update time series plots
    for (int i = 0; i < 5; ++i) {
        customPlots[i]->graph(0)->setData(xData[i], yData[i]);
        customPlots[i]->xAxis->setRange(timeElapsed - 1000, timeElapsed);
        customPlots[i]->graph(0)->rescaleAxes(true);
        customPlots[i]->replot();
    }

    timeElapsed += 100;
}

void MainWindow::handleButtonPress()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        toggleButtonColor(button);
        qDebug() << "Button pressed:" << button->objectName();
    }
}

void MainWindow::updateTimeLabel()
{
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm AP dddd dd/MM/yy");
    ui->timeLabel->setText(currentTime);
}

void MainWindow::toggleButtonColor(QPushButton *button)
{
    // Toggle button state
    buttonStates[button] = !buttonStates[button];

    // Apply color based on the new state
    if (buttonStates[button]) {
        button->setStyleSheet("background-color: green; color: white;");
        if (buttonLedPairs.contains(button)) {
            buttonLedPairs[button]->setOn(); // Turn on the corresponding LED
        }
    } else {
        button->setStyleSheet("background-color: #1c1c1c; color: white;");
        if (buttonLedPairs.contains(button)) {
            buttonLedPairs[button]->setOff(); // Turn off the corresponding LED
        }
    }
}

