#ifndef LED_H
#define LED_H

#include <QLabel>

class Led : public QLabel {
    Q_OBJECT

public slots:
    void setOn() {
        onPix.fill(Qt::green);
        setPixmap(onPix);
    }
    void setOff() {
        onPix.fill(Qt::red);
        setPixmap(onPix);
    }
    void setPower(bool val) {
        if (val) setOn();
        else setOff();
    }

public:
    virtual ~Led() = default;
    explicit Led(QWidget* parent = nullptr)
        : QLabel(parent)
        , onPix(48, 48) {
        onPix.fill(Qt::red);
        setPixmap(onPix);
        setScaledContents(true);
    }

private:
    QPixmap onPix;
};

#endif // LED_H
