#include "../include/ClimateData.hpp"
#include "../exceptions/include/InputHandler.hpp"
#include <sstream>

ClimateData::ClimateData() : temperature(20.0), humidity(50.0), co2(500.0) {}
ClimateData::ClimateData(double temp, double hum, double co2Level)
    : temperature(temp), humidity(hum), co2(co2Level) {}

double ClimateData::getTemperature() const { return temperature; }
double ClimateData::getHumidity() const { return humidity; }
double ClimateData::getCO2() const { return co2; }

void ClimateData::setTemperature(double temp) { temperature = temp; }
void ClimateData::setHumidity(double hum) { humidity = hum; }
void ClimateData::setCO2(double co2Level) { co2 = co2Level; }

bool ClimateData::isTemperatureNormal() const { return temperature >= MIN_TEMP && temperature <= MAX_TEMP; }
bool ClimateData::isHumidityNormal() const { return humidity >= MIN_HUMIDITY && humidity <= MAX_HUMIDITY; }
bool ClimateData::isCO2Normal() const { return co2 >= MIN_CO2 && co2 <= MAX_CO2; }
bool ClimateData::isAllNormal() const { return isTemperatureNormal() && isHumidityNormal() && isCO2Normal(); }

ostream& operator<<(ostream& os, const ClimateData& data) {
    os << data.temperature << "\n" << data.humidity << "\n" << data.co2;
    return os;
}

istream& operator>>(istream& is, ClimateData& data) {
    bool isCin = (&is == &cin);

    if (isCin) {
        bool success = false;
        while (!success) {
            try {
                data.temperature = safeInputNumeric<double>(is, -100.0, 100.0, "Enter temperature: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

        success = false;
        while (!success) {
            try {
                data.humidity = safeInputNumeric<double>(is, 0.0, 100.0, "Enter humidity: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }

        success = false;
        while (!success) {
            try {
                data.co2 = safeInputNumeric<double>(is, 0.0, 10000.0, "Enter CO2 level: ");
                success = true;
            } catch (const InputException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
    } else {
        string line;
        if (getline(is, line)) {
            try {
                data.temperature = stod(line);
            } catch (...) {
                is.setstate(ios::failbit);
                return is;
            }
        }
        if (getline(is, line)) {
            try {
                data.humidity = stod(line);
            } catch (...) {
                is.setstate(ios::failbit);
                return is;
            }
        }
        if (getline(is, line)) {
            try {
                data.co2 = stod(line);
            } catch (...) {
                is.setstate(ios::failbit);
                return is;
            }
        }
    }

    return is;
}