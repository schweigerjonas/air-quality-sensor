#include <Adafruit_PM25AQI.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEA_LEVEL_PRESSURE_HPA (1013.25)

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
Adafruit_BME280 bme;

unsigned long delayTime;

void setup() {
  // Wait for serial monitor to open
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("Adafruit PMSA003I Air Quality Sensor");
  Serial.println("BME280 Barometric Sensor");

  // wait 3 seconds for sensor to boot up
  delay(3000);

  if (!aqi.begin_I2C()) {
    Serial.println("Could not find PM2.5 sensor");
    while (1) {
      delay(10);
    }
  }
  Serial.println("PM2.5 found");

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("Sensor ID was: 0x");
    Serial.println(bme.sensorID(), HEX);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280.\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");

    while (1) {
      delay(10);
    }
  }
  Serial.println("BME280 found");

  delayTime = 1000;

  Serial.println();
}

void loop() {
  PM25_AQI_Data data;

  if (!aqi.read(&data)) {
    Serial.println("Could not read from AQI");

    delay(500);
    return;
  }

  printPMValues(data);
  printBMEValues();

  // sensor reads every second
  delay(delayTime);
}

void printPMValues(PM25_AQI_Data data) {
  Serial.println("AQI reading success");

  Serial.println(F("-----------------------------------------"));
  Serial.println(F("Concentration Units (standard)"));

  Serial.print(F("PM 1.0: "));
  Serial.print(data.pm10_standard);
  Serial.print(F("\t\tPM 2.5: "));
  Serial.print(data.pm25_standard);
  Serial.print(F("\t\tPM 10.0: "));
  Serial.println(data.pm100_standard);

  Serial.println(F("-----------------------------------------"));
  Serial.println(F("Concentration Units (environmental)"));

  Serial.print(F("PM 1.0: "));
  Serial.print(data.pm10_env);
  Serial.print(F("\t\tPM 2.5: "));
  Serial.print(data.pm25_env);
  Serial.print(F("\t\tPM 10.0: "));
  Serial.println(data.pm100_env);

  Serial.println(F("-----------------------------------------"));
  Serial.print(F("Particles > 0.3um / 0.1L air:"));
  Serial.println(data.particles_03um);
  Serial.print(F("Particles > 0.5um / 0.1L air:"));
  Serial.println(data.particles_05um);
  Serial.print(F("Particles > 1.0um / 0.1L air:"));
  Serial.println(data.particles_10um);
  Serial.print(F("Particles > 2.5um / 0.1L air:"));
  Serial.println(data.particles_25um);
  Serial.print(F("Particles > 5.0um / 0.1L air:"));
  Serial.println(data.particles_50um);
  Serial.print(F("Particles > 10 um / 0.1L air:"));
  Serial.println(data.particles_100um);

  Serial.println(F("-----------------------------------------"));
  Serial.println(F("AQI"));

  Serial.print(F("PM2.5 AQI US: "));
  Serial.print(data.aqi_pm25_us);
  Serial.print(F("\tPM10 AQI US: "));
  Serial.println(data.aqi_pm100_us);

  Serial.println(F("-----------------------------------------"));
  Serial.println();
}

void printBMEValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEA_LEVEL_PRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}
