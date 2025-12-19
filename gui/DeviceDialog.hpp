#pragma once

#include <QComboBox>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <memory>
#include "../models/include/devices/SmartDevice.hpp"
#include "../models/include/devices/SmartAirConditioner.hpp"
#include "../models/include/devices/SmartHeater.hpp"
#include "../models/include/devices/SmartHumidifier.hpp"
#include "../models/include/devices/SmartDehumidifier.hpp"
#include "../models/include/devices/SmartFan.hpp"
#include "../models/include/devices/SmartLight.hpp"

struct DeviceFormData {
    int typeIndex = 0;
    int id = 0;
    std::string name;
    std::string location;
    DeviceMode mode = DeviceMode::AUTOMATIC;
    int power = 0;
    double primaryValue = 0.0;
    double tolerance = 0.0;
    int turnOffHour = 0;
};

class DeviceDialog : public QDialog {
    Q_OBJECT
private slots:
    void updateFields(int index);

private:
    QComboBox* typeCombo;
    QSpinBox* idSpin;
    QLineEdit* nameEdit;
    QLineEdit* locationEdit;
    QComboBox* modeCombo;
    QSpinBox* powerSpin;
    QDoubleSpinBox* primarySpin;
    QDoubleSpinBox* toleranceSpin;
    QSpinBox* turnOffSpin;
    QLabel* primaryLabel;
    QLabel* toleranceLabel;

public:
    explicit DeviceDialog(QWidget* parent = nullptr);

    DeviceFormData data() const;
    void fillFromDevice(const std::shared_ptr<SmartDevice>& device);


};
