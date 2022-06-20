#ifndef SPEEDSENSORSPLOTING_H
#define SPEEDSENSORSPLOTING_H

#include <QWidget>

namespace Ui {
class SpeedSensorsPloting;
}

class SpeedSensorsPloting : public QWidget
{
    Q_OBJECT

public:
    explicit SpeedSensorsPloting(QWidget *parent = nullptr);
    ~SpeedSensorsPloting();

private slots:
    void on_btnPlot_clicked();

    void on_btnClear_clicked();

private:
    Ui::SpeedSensorsPloting *ui;
    double key;
};

#endif // SPEEDSENSORSPLOTING_H
