#include "DeviceDialog.hpp"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

DeviceDialog::DeviceDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Configure device");
    setModal(true);
    setMinimumWidth(420);

    typeCombo = new QComboBox(this);
    typeCombo->addItems({"Air Conditioner", "Heater", "Humidifier", "Dehumidifier", "Fan", "Smart Light"});

    idSpin = new QSpinBox(this);
    idSpin->setRange(0, 999999);

    nameEdit = new QLineEdit(this);
    locationEdit = new QLineEdit(this);

    modeCombo = new QComboBox(this);
    modeCombo->addItems({"Automatic", "Manual", "Off"});

    powerSpin = new QSpinBox(this);
    powerSpin->setRange(0, 100);
    powerSpin->setSuffix(" %");

    primarySpin = new QDoubleSpinBox(this);
    primarySpin->setDecimals(2);
    primarySpin->setRange(-1000, 20000);

    toleranceSpin = new QDoubleSpinBox(this);
    toleranceSpin->setDecimals(2);
    toleranceSpin->setRange(0, 20000);

    turnOffSpin = new QSpinBox(this);
    turnOffSpin->setRange(0, 23);
    turnOffSpin->setSuffix(":00");

    primaryLabel = new QLabel("Target", this);
    toleranceLabel = new QLabel("Tolerance", this);

    auto form = new QFormLayout();
    form->addRow("Type", typeCombo);
    form->addRow("ID", idSpin);
    form->addRow("Name", nameEdit);
    form->addRow("Location", locationEdit);
    form->addRow("Mode", modeCombo);
    form->addRow("Power", powerSpin);
    form->addRow(primaryLabel, primarySpin);
    form->addRow(toleranceLabel, toleranceSpin);
    form->addRow("Turn off hour", turnOffSpin);

    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &DeviceDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &DeviceDialog::reject);

    auto layout = new QVBoxLayout();
    layout->addLayout(form);
    layout->addWidget(buttons);
    setLayout(layout);

    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DeviceDialog::updateFields);
    updateFields(typeCombo->currentIndex());
}

void DeviceDialog::updateFields(int index) {
    primaryLabel->setVisible(index != 5);
    primarySpin->setVisible(index != 5);
    toleranceLabel->setVisible(index != 5);
    toleranceSpin->setVisible(index != 5);
    turnOffSpin->setVisible(index == 5);

    switch (index) {
        case 0:
        case 4:
            primaryLabel->setText("Target CO2 (ppm)");
            primarySpin->setRange(0, 10000);
            primarySpin->setValue(400.0);
            toleranceLabel->setText("Tolerance (ppm)");
            toleranceSpin->setRange(0, 1000);
            toleranceSpin->setValue(50.0);
            break;
        case 1:
            primaryLabel->setText("Target temperature (°C)");
            primarySpin->setRange(-50, 50);
            primarySpin->setValue(22.0);
            toleranceLabel->setText("Tolerance (°C)");
            toleranceSpin->setRange(0, 10);
            toleranceSpin->setValue(2.0);
            break;
        case 2:
        case 3:
            primaryLabel->setText("Target humidity (%)");
            primarySpin->setRange(0, 100);
            primarySpin->setValue(50.0);
            toleranceLabel->setText("Tolerance (%)");
            toleranceSpin->setRange(0, 50);
            toleranceSpin->setValue(5.0);
            break;
        case 5:
            turnOffSpin->setValue(22);
            break;
        default:
            break;
    }
}

DeviceFormData DeviceDialog::data() const {
    DeviceFormData d;
    d.typeIndex = typeCombo->currentIndex();
    d.id = idSpin->value();
    d.name = nameEdit->text().toStdString();
    d.location = locationEdit->text().toStdString();
    d.mode = static_cast<DeviceMode>(modeCombo->currentIndex());
    d.power = powerSpin->value();
    d.primaryValue = primarySpin->value();
    d.tolerance = toleranceSpin->value();
    d.turnOffHour = turnOffSpin->value();
    return d;
}

void DeviceDialog::fillFromDevice(const std::shared_ptr<SmartDevice>& device) {
    if (!device) {
        return;
    }

    int typeIndex = 0;
    if (dynamic_cast<SmartAirConditioner*>(device.get())) {
        typeIndex = 0;
    } else if (dynamic_cast<SmartHeater*>(device.get())) {
        typeIndex = 1;
    } else if (dynamic_cast<SmartHumidifier*>(device.get())) {
        typeIndex = 2;
    } else if (dynamic_cast<SmartDehumidifier*>(device.get())) {
        typeIndex = 3;
    } else if (dynamic_cast<SmartFan*>(device.get())) {
        typeIndex = 4;
    } else if (dynamic_cast<SmartLight*>(device.get())) {
        typeIndex = 5;
    }

    typeCombo->setCurrentIndex(typeIndex);
    typeCombo->setEnabled(false);

    idSpin->setValue(device->getId());
    nameEdit->setText(QString::fromStdString(device->getDeviceName()));
    locationEdit->setText(QString::fromStdString(device->getLocation()));
    modeCombo->setCurrentIndex(static_cast<int>(device->getMode()));
    powerSpin->setValue(device->getPowerLevel());

    if (auto ac = dynamic_cast<SmartAirConditioner*>(device.get())) {
        primarySpin->setValue(ac->getTargetCO2());
        toleranceSpin->setValue(ac->getTolerance());
    } else if (auto heater = dynamic_cast<SmartHeater*>(device.get())) {
        primarySpin->setValue(heater->getTargetTemperature());
        toleranceSpin->setValue(heater->getTolerance());
    } else if (auto hum = dynamic_cast<SmartHumidifier*>(device.get())) {
        primarySpin->setValue(hum->getTargetHumidity());
        toleranceSpin->setValue(hum->getTolerance());
    } else if (auto deh = dynamic_cast<SmartDehumidifier*>(device.get())) {
        primarySpin->setValue(deh->getTargetHumidity());
        toleranceSpin->setValue(deh->getTolerance());
    } else if (auto fan = dynamic_cast<SmartFan*>(device.get())) {
        primarySpin->setValue(fan->getTargetCO2());
        toleranceSpin->setValue(fan->getTolerance());
    } else if (auto light = dynamic_cast<SmartLight*>(device.get())) {
        turnOffSpin->setValue(light->getTurnOffHour());
    }
}
