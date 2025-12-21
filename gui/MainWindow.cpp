#include "MainWindow.hpp"

#include <QApplication>
#include <QComboBox>
#include <QDate>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QHeaderView>
#include <QFrame>
#include <QSpacerItem>
#include <QTabWidget>
#include <QVBoxLayout>
#include "../models/include/devices/SmartAirConditioner.hpp"
#include "../models/include/devices/SmartHeater.hpp"
#include "../models/include/devices/SmartHumidifier.hpp"
#include "../models/include/devices/SmartDehumidifier.hpp"
#include "../models/include/devices/SmartFan.hpp"
#include "../models/include/devices/SmartLight.hpp"
#include <chrono>
#include <iostream>
#include <sstream>
using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      stacked(nullptr),
      fileYes(nullptr),
      fileNo(nullptr),
      filePathEdit(nullptr),
      loadFileButton(nullptr),
      tempSpin(nullptr),
      humSpin(nullptr),
      co2Spin(nullptr),
      climateInfo(nullptr),
      loginUser(nullptr),
      loginPass(nullptr),
      regUser(nullptr),
      regPass(nullptr),
      regDate(nullptr),
      authInfo(nullptr),
      climateBadge(nullptr),
      deviceTable(nullptr),
      environmentFeed(nullptr),
      feed(nullptr) {
    setWindowTitle("Smart House вЂ” Qt");
    resize(1180, 760);
    applyStyleSheet();

    stacked = new QStackedWidget(this);
    setCentralWidget(stacked);

    stacked->addWidget(buildClimatePage());
    stacked->addWidget(buildAuthPage());
    stacked->addWidget(buildAppPage());
}

void MainWindow::applyStyleSheet() {
    QString style = R"(

        QMainWindow {
            background-color: #101217;
        }

        #Card {
            background-color: #181a1f;
            border-radius: 16px;
            border: 1px solid #000000;
        }

        #Menu {
            background-color: #11141b;
            border-radius: 16px;
            border: 1px solid #000000;
        }

        QLabel#Title {
            color: #ffffff;
            font-size: 26px;
            font-weight: 700;
            letter-spacing: 0.5px;
        }

        QLabel#Subtitle {
            color: #c7c9d3;
            font-size: 13px;
        }

        QLabel#Badge {
            background-color: #1a8f4a;
            color: #ffffff;
            border-radius: 14px;
            padding: 8px 12px;
            font-weight: 700;
            border: 1px solid #27c463;
        }

        QLabel {
            color: #ffffff;
        }

        QLineEdit,
        QSpinBox,
        QDoubleSpinBox,
        QDateEdit {
            background-color: #1f222b;
            color: #ffffff;
            border: 1px solid #3a3f4d;
            border-radius: 8px;
            padding: 6px 8px;
        }

        QComboBox {
            background-color: #1f222b;
            color: #ffffff;
            border: 1px solid #3a3f4d;
            border-radius: 8px;
            padding: 6px 8px;
        }

        QComboBox QAbstractItemView {
            background-color: #181a1f;
            color: #ffffff;
            selection-background-color: #23314a;
            selection-color: #ffffff;
            border: 1px solid #3a3f4d;
        }

        QTextEdit {
            background-color: #141b27;
            border: 1px solid #3f4a63;
            border-radius: 10px;
            padding: 10px;
            color: #ffffff;
        }

        QListWidget {
            background-color: #141b27;
            border: 1px solid #3f4a63;
            border-radius: 10px;
            padding: 6px;
            color: #ffffff;
        }

        QTableWidget {
            background-color: #141b27;
            border: 1px solid #3f4a63;
            border-radius: 10px;
            color: #ffffff;
            gridline-color: #ffffff;
        }

        QHeaderView::section {
            background-color: #1f2a3c;
            color: #ffffff;
            border: 1px solid #3a3f4d;
            padding: 6px;
        }

        QTableWidget::item:selected {
            background-color: #23314a;
            color: #ffffff;
        }

        QRadioButton,
        QCheckBox {
            color: #ffffff;
        }

        QPushButton {
            background-color: #1f2a3c;
            color: #ffffff;
            border: 1px solid #000000;
            border-radius: 10px;
            padding: 8px 14px;
            font-weight: 600;
        }

        QPushButton:hover {
            background-color: #273755;
        }

        QPushButton:pressed {
            background-color: #1b2234;
        }

        QDialog {
            background-color: #181a1f;
        }

        QTabWidget::pane {
            border: 1px solid #3a3f4d;
            border-radius: 12px;
            background-color: #181a1f;
        }

        QTabBar::tab {
            background: transparent;
            color: #ffffff;
            padding: 8px 14px;
        }

        QTabBar::tab:selected {
            background-color: #1f2a3c;
        }
    )";
    qApp->setStyleSheet(style);
}

QWidget* MainWindow::buildClimatePage() {
    auto page = new QWidget(this);
    auto outer = new QVBoxLayout(page);
    outer->setContentsMargins(32, 32, 32, 32);

    auto card = new QFrame(page);
    card->setObjectName("Card");
    auto cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(16);

    auto title = new QLabel("Smart House", card);
    title->setObjectName("Title");
    auto subtitle = new QLabel("First, load climate readings just like in the console version.", card);
    subtitle->setObjectName("Subtitle");

    cardLayout->addWidget(title);
    cardLayout->addWidget(subtitle);

    auto question = new QLabel("Do you already have a file with climate readings?", card);
    fileYes = new QRadioButton("Yes, choose file", card);
    fileNo = new QRadioButton("No, I will enter manually", card);
    fileNo->setChecked(true);

    auto radioLayout = new QHBoxLayout();
    radioLayout->addWidget(question);
    radioLayout->addStretch();
    cardLayout->addLayout(radioLayout);
    cardLayout->addWidget(fileYes);
    cardLayout->addWidget(fileNo);

    auto fileRow = new QHBoxLayout();
    filePathEdit = new QLineEdit(card);
    filePathEdit->setPlaceholderText("Select a file with climate data...");
    filePathEdit->setReadOnly(true);
    loadFileButton = new QPushButton("Browse", card);
    fileRow->addWidget(filePathEdit, 1);
    fileRow->addWidget(loadFileButton);
    cardLayout->addLayout(fileRow);

    tempSpin = new QDoubleSpinBox(card);
    tempSpin->setRange(-100.0, 100.0);
    tempSpin->setDecimals(2);
    tempSpin->setValue(22.0);

    humSpin = new QDoubleSpinBox(card);
    humSpin->setRange(0.0, 100.0);
    humSpin->setDecimals(2);
    humSpin->setValue(50.0);

    co2Spin = new QDoubleSpinBox(card);
    co2Spin->setRange(0.0, 10000.0);
    co2Spin->setDecimals(2);
    co2Spin->setValue(600.0);

    auto grid = new QGridLayout();
    grid->addWidget(new QLabel("Temperature (В°C)"), 0, 0);
    grid->addWidget(tempSpin, 0, 1);
    grid->addWidget(new QLabel("Humidity (%)"), 1, 0);
    grid->addWidget(humSpin, 1, 1);
    grid->addWidget(new QLabel("CO2 (ppm)"), 2, 0);
    grid->addWidget(co2Spin, 2, 1);
    cardLayout->addLayout(grid);

    climateInfo = new QLabel("Waiting for data...", card);
    cardLayout->addWidget(climateInfo);

    auto continueBtn = new QPushButton("Continue to authentication", card);
    cardLayout->addWidget(continueBtn);

    outer->addWidget(card);

    connect(loadFileButton, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getOpenFileName(this, "Select file with climate readings");
        if (path.isEmpty()) {
            return;
        }
        filePathEdit->setText(path);
        string error;
        if (loadClimateDataFromFile(path.toStdString(), error)) {
            climateInfo->setText("Climate data loaded from file");
            refreshClimateBadge();
        } else {
            climateInfo->setText(QString("Error: %1").arg(QString::fromStdString(error)));
        }
    });

    connect(continueBtn, &QPushButton::clicked, this, [this]() {
        if (fileYes->isChecked()) {
            if (filePathEdit->text().isEmpty()) {
                climateInfo->setText("Select a file or switch to manual input");
                return;
            }
            if (climateInfo->text().startsWith("Error")) {
                return;
            }
        } else {
            setManualClimate(tempSpin->value(), humSpin->value(), co2Spin->value());
            climateInfo->setText("Values saved manually");
            refreshClimateBadge();
        }
        stacked->setCurrentIndex(1);
    });

    return page;
}

QWidget* MainWindow::buildAuthPage() {
    auto page = new QWidget(this);
    auto layout = new QVBoxLayout(page);
    layout->setContentsMargins(32, 32, 32, 32);

    auto card = new QFrame(page);
    card->setObjectName("Card");
    card->setMaximumWidth(420);
    auto cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(14);

    auto title = new QLabel("Authentication", card);
    title->setObjectName("Title");
    auto subtitle = new QLabel("Just like in the console: sign in or register first.", card);
    subtitle->setObjectName("Subtitle");
    cardLayout->addWidget(title);
    cardLayout->addWidget(subtitle);

    auto tabs = new QTabWidget(card);

    auto loginTab = new QWidget(tabs);
    auto loginLayout = new QFormLayout(loginTab);
    loginUser = new QLineEdit(loginTab);
    loginPass = new QLineEdit(loginTab);
    loginPass->setEchoMode(QLineEdit::Password);
    auto loginButton = new QPushButton("Login", loginTab);

    loginLayout->addRow("Username", loginUser);
    loginLayout->addRow("Password", loginPass);
    loginLayout->addRow(loginButton);
    tabs->addTab(loginTab, "Login");

    auto regTab = new QWidget(tabs);
    auto regLayout = new QFormLayout(regTab);
    regUser = new QLineEdit(regTab);
    regPass = new QLineEdit(regTab);
    regPass->setEchoMode(QLineEdit::Password);
    regDate = new QDateEdit(QDate::currentDate(), regTab);
    regDate->setCalendarPopup(true);
    auto regButton = new QPushButton("Register", regTab);

    regLayout->addRow("Username", regUser);
    regLayout->addRow("Birthday", regDate);
    regLayout->addRow("Password", regPass);
    regLayout->addRow(regButton);
    tabs->addTab(regTab, "Register");

    authInfo = new QLabel(" ", card);

    cardLayout->addWidget(tabs);
    cardLayout->addWidget(authInfo);
    layout->addWidget(card, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    auto tryLogin = [this](const QString& user, const QString& pass) -> bool {
        try {
            authService.loginUser(user.toStdString(), pass.toStdString());
            authInfo->setText("Login successful. Welcome!");
            stacked->setCurrentIndex(2);
            refreshDevices(deviceService.getAllDevices());
            refreshClimateBadge();
            appendFeed("Login successful.");
            return true;
        } catch (const exception& e) {
            authInfo->setText(QString::fromStdString(e.what()));
            return false;
        }
    };

    connect(loginButton, &QPushButton::clicked, this, [this, tryLogin]() {
        tryLogin(loginUser->text(), loginPass->text());
    });

    connect(regButton, &QPushButton::clicked, this, [this, tryLogin, tabs]() {
        Date birthday(regDate->date().day(), regDate->date().month(), regDate->date().year());
        if (!birthday.isValid()) {
            authInfo->setText("Invalid birthday date");
            return;
        }
        try {
            authService.registerUser(regUser->text().toStdString(), regPass->text().toStdString(), birthday);
            appendFeed("New user registered.");
            authInfo->setText("Registration successful. Signing in...");
            loginUser->setText(regUser->text());
            loginPass->setText(regPass->text());
            tabs->setCurrentIndex(0);
            tryLogin(loginUser->text(), loginPass->text());
        } catch (const exception& e) {
            authInfo->setText(QString::fromStdString(e.what()));
        }
    });

    return page;
}

QPushButton* MainWindow::makeMenuButton(const QString& text) {
    auto btn = new QPushButton(text, this);
    btn->setMinimumHeight(46);
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

QWidget* MainWindow::buildAppPage() {
    auto page = new QWidget(this);
    auto layout = new QHBoxLayout(page);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(16);

    auto menu = new QFrame(page);
    menu->setObjectName("Menu");
    menu->setMaximumWidth(280);
    auto menuLayout = new QVBoxLayout(menu);
    menuLayout->setSpacing(10);

    auto menuTitle = new QLabel("Main menu", menu);
    menuTitle->setObjectName("Title");
    menuLayout->addWidget(menuTitle);

    auto btn1 = makeMenuButton("1. View all devices");
    auto btn2 = makeMenuButton("2. Add device");
    auto btn3 = makeMenuButton("3. Modify device");
    auto btn4 = makeMenuButton("4. Delete device");
    auto btn5 = makeMenuButton("5. Search devices");
    auto btn6 = makeMenuButton("6. Sort devices");
    auto btn7 = makeMenuButton("7. View device information");
    auto btn8 = makeMenuButton("8. Undo last action");
    auto btn9 = makeMenuButton("9. Redo last action");
    auto btn10 = makeMenuButton("10. View environment indicators");
    auto btn11 = makeMenuButton("11. View climate recommendations");
    auto btn12 = makeMenuButton("12. Normalize smart home climate");
    auto btn0 = makeMenuButton("0. Logout");
    btn0->setStyleSheet("QPushButton{background-color:#b3261e;color:#ffffff;border-radius:12px;font-weight:800;border:1px solid #000000;} QPushButton:hover{background-color:#d23a32;} QPushButton:pressed{background-color:#7f1511;}");

    QList<QPushButton*> buttons = {btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, btn9, btn10, btn11, btn12, btn0};
    for (auto* b : buttons) {
        menuLayout->addWidget(b);
    }
    menuLayout->addStretch();

    auto content = new QFrame(page);
    content->setObjectName("Card");
    auto contentLayout = new QVBoxLayout(content);
    contentLayout->setSpacing(12);

    climateBadge = new QLabel("Climate is not loaded", content);
    climateBadge->setObjectName("Badge");
    contentLayout->addWidget(climateBadge, 0, Qt::AlignLeft);

    auto deviceLabel = new QLabel("Devices", content);
    deviceLabel->setObjectName("Subtitle");
    deviceTable = new QTableWidget(content);
    deviceTable->setColumnCount(6);
    deviceTable->setHorizontalHeaderLabels({"ID", "Name", "Location", "Type", "Mode", "Power"});
    deviceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    deviceTable->verticalHeader()->setVisible(false);
    deviceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    deviceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    deviceTable->setShowGrid(true);
    contentLayout->addWidget(deviceLabel);
    contentLayout->addWidget(deviceTable, 1);

    // Use custom Deque to seed the table with current devices
    Deque<SmartDevice*> initialDevices = deviceService.getAllDevices();
    refreshDevices(initialDevices);

    auto envLabel = new QLabel("Environment", content);
    envLabel->setObjectName("Subtitle");
    environmentFeed = new QTextEdit(content);
    environmentFeed->setReadOnly(true);
    contentLayout->addWidget(envLabel);
    contentLayout->addWidget(environmentFeed, 1);

    auto feedLabel = new QLabel("Activity", content);
    feedLabel->setObjectName("Subtitle");
    feed = new QTextEdit(content);
    feed->setReadOnly(true);
    contentLayout->addWidget(feedLabel);
    contentLayout->addWidget(feed, 1);

    layout->addWidget(menu);
    layout->addWidget(content, 1);

    connect(btn1, &QPushButton::clicked, this, [this]() {
        refreshDevices(deviceService.getAllDevices());
        appendFeed("Showing all devices.");
    });

    connect(btn2, &QPushButton::clicked, this, [this]() { performAddDevice(); });
    connect(btn3, &QPushButton::clicked, this, [this]() { performEditDevice(); });
    connect(btn4, &QPushButton::clicked, this, [this]() { performDeleteDevice(); });
    connect(btn5, &QPushButton::clicked, this, [this]() { performSearch(); });
    connect(btn6, &QPushButton::clicked, this, [this]() { performSort(); });
    connect(btn7, &QPushButton::clicked, this, [this]() { performDeviceInfo(); });
    connect(btn8, &QPushButton::clicked, this, [this]() {
        string info;
        if (history.canUndo()) {
            history.undo();
            info = "Last action undone.";
        } else {
            info = "Nothing to undo.";
        }
        appendFeed(QString::fromStdString(info));
        refreshDevices(deviceService.getAllDevices());
    });
    connect(btn9, &QPushButton::clicked, this, [this]() {
        string info;
        if (history.canRedo()) {
            history.redo();
            info = "Action redone.";
        } else {
            info = "Nothing to redo.";
        }
        appendFeed(QString::fromStdString(info));
        refreshDevices(deviceService.getAllDevices());
    });
    connect(btn10, &QPushButton::clicked, this, [this]() {
        auto cd = climateData;
        QString text = QString(
                           "Environment snapshot\n"
                           "в”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђ\n"
                           "Temperature : %1 В°C\n"
                           "Humidity    : %2 %%\n"
                           "COв‚‚ level   : %3 ppm")
                           .arg(cd.getTemperature(), 0, 'f', 2)
                           .arg(cd.getHumidity(), 0, 'f', 2)
                           .arg(cd.getCO2(), 0, 'f', 2);
        appendEnvironment(text);
        refreshClimateBadge();
    });
    connect(btn11, &QPushButton::clicked, this, [this]() {
        QString rec = QString::fromStdString(analyzeClimate());
        appendEnvironment(QString(
                              "Climate analysis & recommendations\n"
                              "в”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђ\n%1").arg(rec));
    });
    connect(btn12, &QPushButton::clicked, this, [this]() { performNormalize(); });
    connect(btn0, &QPushButton::clicked, this, [this]() {
        authService.logout();
        stacked->setCurrentIndex(1);
        appendFeed("Logged out.");
    });

    return page;
}

QString MainWindow::deviceType(const SmartDevice* dev) const {
    if (dynamic_cast<const SmartAirConditioner*>(dev)) {
        return "Air Conditioner";
    }
    if (dynamic_cast<const SmartHeater*>(dev)) {
        return "Heater";
    }
    if (dynamic_cast<const SmartHumidifier*>(dev)) {
        return "Humidifier";
    }
    if (dynamic_cast<const SmartDehumidifier*>(dev)) {
        return "Dehumidifier";
    }
    if (dynamic_cast<const SmartFan*>(dev)) {
        return "Fan";
    }
    if (dynamic_cast<const SmartLight*>(dev)) {
        return "Light";
    }
    return "Device";
}

void MainWindow::refreshDevices(const Deque<SmartDevice*>& devices) {
    deviceTable->setRowCount(static_cast<int>(devices.size()));
    int row = 0;
    for (auto it = devices.begin(); it != devices.end(); ++it) {
        SmartDevice* dev = *it;
        deviceTable->setItem(row, 0, new QTableWidgetItem(QString::number(dev->getId())));
        deviceTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(dev->getDeviceName())));
        deviceTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(dev->getLocation())));
        deviceTable->setItem(row, 3, new QTableWidgetItem(deviceType(dev)));
        deviceTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(dev->getModeString())));
        deviceTable->setItem(row, 5, new QTableWidgetItem(QString("%1%").arg(dev->getPowerLevel())));
        ++row;
    }
}

void MainWindow::appendFeed(const QString& text) {
    if (text.trimmed().isEmpty()) {
        return;
    }
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    QString stamp = QDateTime::fromSecsSinceEpoch(now).toString("hh:mm:ss");
    feed->append(QString("[%1] %2").arg(stamp, text));
}

void MainWindow::appendEnvironment(const QString& text) {
    if (text.trimmed().isEmpty()) {
        return;
    }
    auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    QString stamp = QDateTime::fromSecsSinceEpoch(now).toString("hh:mm:ss");
    environmentFeed->append(QString("[%1]\n%2\n").arg(stamp, text));
}

void MainWindow::refreshClimateBadge() {
    auto cd = climateData;
    QString label = QString("t %1 В°C  |  h %2 %  |  CO2 %3 ppm")
                        .arg(cd.getTemperature(), 0, 'f', 1)
                        .arg(cd.getHumidity(), 0, 'f', 1)
                        .arg(cd.getCO2(), 0, 'f', 0);
    climateBadge->setText(label);
}

void MainWindow::performAddDevice() {
    DeviceDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        auto data = dlg.data();
        string error;
        if (addDevice(data, error)) {
            appendFeed("Device added.");
            refreshDevices(deviceService.getAllDevices());
        } else {
            appendFeed(QString("Error: %1").arg(QString::fromStdString(error)));
        }
    }
}

void MainWindow::performEditDevice() {
    bool ok = false;
    int id = QInputDialog::getInt(this, "Edit device", "Device ID:", 0, 0, 999999, 1, &ok);
    if (!ok) {
        return;
    }
    auto devices = deviceService.getAllDevices();
    SmartDevice* target = nullptr;
    for (auto it = devices.begin(); it != devices.end(); ++it) {
        SmartDevice* d = *it;
        if (d->getId() == id) {
            target = d;
            break;
        }
    }
    if (!target) {
        appendFeed("Device with this ID was not found.");
        return;
    }

    DeviceDialog dlg(this);
    dlg.fillFromDevice(target);
    if (dlg.exec() == QDialog::Accepted) {
        auto data = dlg.data();
        string error;
        if (updateDevice(id, data, error)) {
            appendFeed("Device updated.");
            refreshDevices(deviceService.getAllDevices());
        } else {
            appendFeed(QString("Error: %1").arg(QString::fromStdString(error)));
        }
    }
}

void MainWindow::performDeleteDevice() {
    bool ok = false;
    int id = QInputDialog::getInt(this, "Delete device", "Device ID:", 0, 0, 999999, 1, &ok);
    if (!ok) {
        return;
    }
    string error;
    if (deleteDevice(id, error)) {
        appendFeed("Device deleted.");
        refreshDevices(deviceService.getAllDevices());
    } else {
        appendFeed(QString("Error: %1").arg(QString::fromStdString(error)));
    }
}

void MainWindow::performSearch() {
    QDialog dlg(this);
    dlg.setWindowTitle("Search devices");
    auto layout = new QFormLayout(&dlg);
    QComboBox* field = new QComboBox(&dlg);
    field->addItems({"ID", "Name", "Location", "Power level", "Mode"});
    QLineEdit* value = new QLineEdit(&dlg);
    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow("Field", field);
    layout->addRow("Value", value);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    DeviceSearchField f = DeviceSearchField::ID;
    switch (field->currentIndex()) {
        case 0: f = DeviceSearchField::ID; break;
        case 1: f = DeviceSearchField::DEVICE_NAME; break;
        case 2: f = DeviceSearchField::LOCATION; break;
        case 3: f = DeviceSearchField::POWER_LEVEL; break;
        case 4: f = DeviceSearchField::MODE; break;
    }

    string error;
    auto results = searchDevices(f, value->text().toStdString(), error);
    if (!error.empty()) {
        appendFeed(QString::fromStdString(error));
        return;
    }
    refreshDevices(results);
    appendFeed(QString("Found %1 device(s).").arg(results.size()));
}

void MainWindow::performSort() {
    QDialog dlg(this);
    dlg.setWindowTitle("Sort devices");
    auto layout = new QVBoxLayout(&dlg);
    QComboBox* field = new QComboBox(&dlg);
    field->addItems({"ID", "Name", "Location", "Power level", "Mode"});
    layout->addWidget(field);
    auto buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    DeviceSortField f = DeviceSortField::ID;
    switch (field->currentIndex()) {
        case 0: f = DeviceSortField::ID; break;
        case 1: f = DeviceSortField::DEVICE_NAME; break;
        case 2: f = DeviceSortField::LOCATION; break;
        case 3: f = DeviceSortField::POWER_LEVEL; break;
        case 4: f = DeviceSortField::MODE; break;
    }
    sortDevices(f);
    refreshDevices(deviceService.getAllDevices());
    appendFeed("Devices sorted.");
}

void MainWindow::performDeviceInfo() {
    bool ok = false;
    int id = QInputDialog::getInt(this, "Device information", "Device ID:", 0, 0, 999999, 1, &ok);
    if (!ok) {
        return;
    }
    appendFeed(QString::fromStdString(deviceInfo(id)));
}

void MainWindow::performNormalize() {
    string details;
    normalizeClimate(details);
    refreshClimateBadge();
    appendEnvironment(QString(
        "Normalized climate\n"
        "в”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђв”Ђ\n%1").arg(QString::fromStdString(details)));
}

void MainWindow::setManualClimate(double temp, double hum, double co2) {
    climateData.setTemperature(temp);
    climateData.setHumidity(hum);
    climateData.setCO2(co2);
}

bool MainWindow::loadClimateDataFromFile(const string& path, string& error) {
    try {
        TextFile<ClimateData> file(path);
        auto records = file.readAllRecords();

        if (records.isEmpty()) {
            error = "File is empty. Please enter the readings manually.";
            return false;
        }

        auto it = records.begin();
        climateData = **it;
        for (auto ptr : records) {
            delete ptr;
        }
        return true;
    } catch (const FileException& e) {
        error = e.what();
        return false;
    }
}

SmartDevice* MainWindow::buildDeviceFromForm(const DeviceFormData& data, int lockedType) {
    int typeIndex = lockedType >= 0 ? lockedType : data.typeIndex;
    switch (typeIndex) {
        case 0:
            return new SmartAirConditioner(data.id, data.name, data.location, data.mode, data.power, data.primaryValue, data.tolerance);
        case 1:
            return new SmartHeater(data.id, data.name, data.location, data.mode, data.power, data.primaryValue, data.tolerance);
        case 2:
            return new SmartHumidifier(data.id, data.name, data.location, data.mode, data.power, data.primaryValue, data.tolerance);
        case 3:
            return new SmartDehumidifier(data.id, data.name, data.location, data.mode, data.power, data.primaryValue, data.tolerance);
        case 4:
            return new SmartFan(data.id, data.name, data.location, data.mode, data.power, data.primaryValue, data.tolerance);
        case 5:
            return new SmartLight(data.id, data.name, data.location, data.mode, data.power, data.turnOffHour);
        default:
            return nullptr;
    }
}

bool MainWindow::addDevice(const DeviceFormData& data, string& error) {
    auto newDevice = buildDeviceFromForm(data);
    if (!newDevice) {
        error = "Failed to build device";
        return false;
    }

    try {
        history.executeCommand(new AddDeviceCommand(&deviceService, newDevice));
        return true;
    } catch (const exception& e) {
        error = e.what();
        return false;
    }
}

bool MainWindow::updateDevice(int deviceId, const DeviceFormData& data, string& error) {
    auto existing = deviceService.getDeviceById(deviceId);
    if (!existing) {
        error = "Device not found";
        return false;
    }

    int lockedType = -1;
    if (dynamic_cast<SmartAirConditioner*>(existing)) {
        lockedType = 0;
    } else if (dynamic_cast<SmartHeater*>(existing)) {
        lockedType = 1;
    } else if (dynamic_cast<SmartHumidifier*>(existing)) {
        lockedType = 2;
    } else if (dynamic_cast<SmartDehumidifier*>(existing)) {
        lockedType = 3;
    } else if (dynamic_cast<SmartFan*>(existing)) {
        lockedType = 4;
    } else if (dynamic_cast<SmartLight*>(existing)) {
        lockedType = 5;
    }

    auto updated = buildDeviceFromForm(data, lockedType);
    if (!updated) {
        error = "Failed to update device";
        return false;
    }

    try {
        deviceService.updateDevice(deviceId, updated);
        return true;
    } catch (const exception& e) {
        error = e.what();
        return false;
    }
}

bool MainWindow::deleteDevice(int deviceId, string& error) {
    try {
        history.executeCommand(new RemoveDeviceCommand(&deviceService, deviceId));
        return true;
    } catch (const exception& e) {
        error = e.what();
        return false;
    }
}

Deque<SmartDevice*> MainWindow::searchDevices(DeviceSearchField field, const string& value, string& error) {
    SmartDevice sample;
    Device::setSearchMode(field);

    try {
        switch (field) {
            case DeviceSearchField::ID:
                sample.setId(stoi(value));
                break;
            case DeviceSearchField::DEVICE_NAME:
                sample.setDeviceName(value);
                break;
            case DeviceSearchField::LOCATION:
                sample.setLocation(value);
                break;
            case DeviceSearchField::MODE:
                sample.setMode(static_cast<DeviceMode>(stoi(value)));
                break;
            case DeviceSearchField::POWER_LEVEL:
                sample.setPowerLevel(stoi(value));
                break;
        }
    } catch (const exception&) {
        error = "Invalid value for search";
        Device::setSearchMode(DeviceSearchField::ID);
        return {};
    }

    Deque<SmartDevice*> results;
    auto allDevices = deviceService.getAllDevices();
    for (auto it = allDevices.begin(); it != allDevices.end(); ++it) {
        SmartDevice* device = *it;
        if (*device == sample) {
            results.pushBack(device);
        }
    }

    Device::setSearchMode(DeviceSearchField::ID);
    return results;
}

void MainWindow::sortDevices(DeviceSortField field) {
    int option = 1;
    switch (field) {
        case DeviceSortField::ID: option = 1; break;
        case DeviceSortField::DEVICE_NAME: option = 2; break;
        case DeviceSortField::LOCATION: option = 3; break;
        case DeviceSortField::POWER_LEVEL: option = 4; break;
        case DeviceSortField::MODE: option = 5; break;
    }
    deviceService.sortDevices(option);
}

string MainWindow::deviceInfo(int id) {
    auto device = deviceService.getDeviceById(id);
    if (!device) {
        return "Device not found";
    }
    return device->getDeviceInfo();
}

string MainWindow::analyzeClimate() {
    ostringstream capture;
    auto* oldBuf = cout.rdbuf(capture.rdbuf());
    ClimateNormalizer::analyzeAndRecommend(climateData, deviceService.getAllDevices());
    cout.rdbuf(oldBuf);
    return capture.str();
}

ClimateData MainWindow::normalizeClimate(string& details) {
    ClimateData normalized = ClimateNormalizer::normalize(climateData, deviceService.getAllDevices());

    ostringstream oss;
    oss << "Temperature: " << normalized.getTemperature()
        << "\nHumidity: " << normalized.getHumidity()
        << "\nCO2: " << normalized.getCO2();
    details = oss.str();

    TextFile<ClimateData> resultFile("normalized_climate.txt");
    resultFile.clearFile();
    resultFile.saveRecord(normalized);
    climateData = normalized;
    return normalized;
}

