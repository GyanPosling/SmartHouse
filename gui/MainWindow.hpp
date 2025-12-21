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

#include "../templates/Deque.hpp"
#include "../services/include/AuthenticationService.hpp"
#include "../services/include/DeviceService.hpp"
#include "../services/include/CommandHistory.hpp"
#include "../services/include/ClimateNormalizer.hpp"
#include "../models/include/ClimateData.hpp"
#include "../models/include/DeviceCommands.hpp"
#include "../models/include/RemoveDeviceCommand.hpp"
#include "../repositories/include/TextFile.hpp"
#include "DeviceDialog.hpp"
using namespace std;

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

    void refreshDevices(const Deque<SmartDevice*>& devices);
    QString deviceType(const SmartDevice* dev) const;
    void appendFeed(const QString& text);
    void appendEnvironment(const QString& text);
    void refreshClimateBadge();

    void setManualClimate(double temp, double hum, double co2);
    bool loadClimateDataFromFile(const string& path, string& error);

    SmartDevice* buildDeviceFromForm(const DeviceFormData& data, int lockedType = -1);
    bool addDevice(const DeviceFormData& data, string& error);
    bool updateDevice(int deviceId, const DeviceFormData& data, string& error);
    bool deleteDevice(int deviceId, string& error);

    Deque<SmartDevice*> searchDevices(DeviceSearchField field, const string& value, string& error);
    void sortDevices(DeviceSortField field);
    string deviceInfo(int id);
    string analyzeClimate();
    ClimateData normalizeClimate(string& details);

    void performAddDevice();
    void performEditDevice();
    void performDeleteDevice();
    void performSearch();
    void performSort();
    void performDeviceInfo();
    void performNormalize();
};

