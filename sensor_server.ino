#include <DHT.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#define DHTPIN 2
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

Adafruit_INA219 ina219;
DHT dht(DHTPIN, DHTTYPE);
int analog_value;
float input_voltage = 0.0;
float s=0.0;
float r1=100200.0;
float r2=9850.0;
char humidityI2C[15], temperatureI2C[15], currentI2C[15];

void setup()
{
    Serial.begin(115200);     //  opens serial port, sets data rate to 9600 bps
    Wire.setClock(400000);  // Set i2c speed
    Wire.begin(8);          // Channel for DHT22
                            // Channel for Current Sensor
    Wire.onRequest(requestEvent);
    dht.begin();
    delay(2000);            // Delay to let everything connect
    uint32_t currentFrequency;
    // Initialize the INA219.
    // By default the initialization will use the largest range (32V, 2A).  However
    // you can call a setCalibration function to change this range (see comments).
    ina219.begin();
    // To use a slightly lower 32V, 1A range (higher precision on amps):
    //ina219.setCalibration_32V_1A();
    // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
    ina219.setCalibration_16V_400mA();
}
void loop()
{
    delay(2000);
    
    //////////////////////////////////
    //Current Sensor for Solar Panel//
    //////////////////////////////////
    
    float shuntvoltage = 0;
    float busvoltage = 0;
    float current_mA = 0;
    float loadvoltage = 0;
    float power_mW = 0;

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
    Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
    Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    Serial.println("");

    // Convert float to string for i2c
    dtostrf(current_mA,6, 2, currentI2C);
    
    ///////////////////////////////////////
    //DHT22 Temperature & humidity sensor//
    ///////////////////////////////////////
    
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
       Serial.println("Failed to read from DHT sensor!");
    return;
    }
    
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");
    //Serial.print("V = ");
    //Serial.println(input_voltage);
    // Convert float to string for i2c
    dtostrf(h,6, 2, humidityI2C);
    dtostrf(f,6, 2, temperatureI2C);
}

///////////////////////////
//Send data to Pi via I2C//
///////////////////////////

void requestEvent() {
    Wire.write(humidityI2C); // Send humidity Pi
    Wire.write(temperatureI2C); // Send ambient temperature to Pi
    Wire.write(currentI2C); // Send solar panel current to Pi
}
