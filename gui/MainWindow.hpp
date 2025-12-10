#pragma once

#include <QButtonGroup>
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QTextEdit>
#include <QDoubleSpinBox>

#include "../services/include/AuthenticationService.hpp"
#include "../services/include/DeviceService.hpp"
#include "../services/include/CommandHistory.hpp"
#include "../services/include/ClimateNormalizer.hpp"
#include "../models/include/ClimateData.hpp"
#include "../models/include/DeviceCommands.hpp"
#include "../models/include/RemoveDeviceCommand.hpp"
#include "../repositories/include/TextFile.hpp"
#include "DeviceDialog.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    AuthenticationService authService;
    DeviceService deviceService;
    CommandHistory history;
    ClimateData climateData;

    QStackedWidget* stacked;

    QRadioButton* fileYes;
    QRadioButton* fileNo;
    QLineEdit* filePathEdit;
    QPushButton* loadFileButton;
    QDoubleSpinBox* tempSpin;
    QDoubleSpinBox* humSpin;
    QDoubleSpinBox* co2Spin;
    QLabel* climateInfo;

    QLineEdit* loginUser;
    QLineEdit* loginPass;
    QLineEdit* regUser;
    QLineEdit* regPass;
    QDateEdit* regDate;
    QLabel* authInfo;

    QLabel* climateBadge;
    QTableWidget* deviceTable;
    QTextEdit* environmentFeed;
    QTextEdit* feed;

    void applyStyleSheet();

    QWidget* buildClimatePage();
    QWidget* buildAuthPage();
    QWidget* buildAppPage();

    QPushButton* makeMenuButton(const QString& text);

    void refreshDevices(const std::vector<std::shared_ptr<SmartDevice>>& devices);
    void appendFeed(const QString& text);
    void appendEnvironment(const QString& text);
    void refreshClimateBadge();

    void setManualClimate(double temp, double hum, double co2);
    bool loadClimateDataFromFile(const std::string& path, std::string& error);

    std::shared_ptr<SmartDevice> buildDeviceFromForm(const DeviceFormData& data, int lockedType = -1);
    bool addDevice(const DeviceFormData& data, std::string& error);
    bool updateDevice(int deviceId, const DeviceFormData& data, std::string& error);
    bool deleteDevice(int deviceId, std::string& error);

    std::vector<std::shared_ptr<SmartDevice>> searchDevices(DeviceSearchField field, const std::string& value, std::string& error);
    void sortDevices(DeviceSortField field);
    std::string deviceInfo(int id);
    std::string analyzeClimate();
    ClimateData normalizeClimate(std::string& details);

    void performAddDevice();
    void performEditDevice();
    void performDeleteDevice();
    void performSearch();
    void performSort();
    void performDeviceInfo();
    void performNormalize();
};
