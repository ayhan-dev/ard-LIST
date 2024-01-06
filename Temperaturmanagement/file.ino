		#include <Adafruit_GFX.h>
		#include <Adafruit_SH110X.h>

  	const int thermistor_output = A0;
		const int antenna = A1;

		#define OLED_MOSI 11
		#define OLED_CLK 13
		#define OLED_DC 9
		#define OLED_CS 10
		#define OLED_RST 8 

		Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS); 

		const int led[] =  {3,4,5,A2, A3, A4, A5, A6}; 
		const int sample_size = 60;  

		int high_emf = 0, curr_emf = 0;
		int last_emf_reading = 0;
		int current_sample_index = 0; 

		double temperature_samples[sample_size];

		void setup() {
		  Serial.begin(9600); 
      
		  for (int i = 0; i < sample_size; i++) {
			temperature_samples[i] = NAN;
		  } 
      
		  for (int i = 0; i < 8; i++) {
			pinMode(led[i], OUTPUT);
		  } 
      
		  if (!display.begin()) {
			Serial.println(F("SH1106 allocation failed"));
			for (;;);
		  } 
      
		  display.display();
		  delay(2000);
		  display.clearDisplay();
		}

		void loop() { 
      
		  double temperature = readTemperature();
		  updateTemperatureSamples(temperature);
		  double baseline = calculateAverage();
		  double std_dev = calculateStandardDeviation(baseline);
		  double num_std_dev = (temperature - baseline) / std_dev;
		  setLEDsBasedOnStdDev(num_std_dev);
		  updateOLED(temperature, baseline, num_std_dev);
		  int val = analogRead(antenna); 
      
		  if(val >= 80) {
			last_emf_reading = val;
		  }
		  delay(1000);
		} 

		double readTemperature() {
		  int thermistor_adc_val = analogRead(thermistor_output);
		  double output_voltage = (thermistor_adc_val * 5.0) / 1023.0;
		  double thermistor_resistance = ((5 * (10.0 / output_voltage)) - 10) * 1000;
		  double therm_res_ln = log(thermistor_resistance);
		  double temperature = 1 / (0.001129148 + (0.000234125 * therm_res_ln) + (0.0000000876741 * therm_res_ln * therm_res_ln * therm_res_ln));
		  temperature = temperature - 273.15; 
		  temperature = temperature * 1.8 + 32;
		  return temperature;
		}

		void updateTemperatureSamples(double temperature) {
		  temperature_samples[current_sample_index] = temperature;
		  current_sample_index = (current_sample_index + 1) % sample_size;
		} 


		double calculateAverage() {
		  double sum = 0.0;
		  int count = 0;
		  for (int i = 0; i < sample_size; i++) {
			if (!isnan(temperature_samples[i])) {
			  sum += temperature_samples[i];
			  count++;
			}} 
      
		  if (count == 0) return 0.0;
		  return sum / count;
		}

		double calculateStandardDeviation(double mean) {
		  double sum = 0.0;
		  int count = 0;
		  for (int i = 0; i < sample_size; i++) {
			if (!isnan(temperature_samples[i])) {
			  sum += pow(temperature_samples[i] - mean, 2);
			  count++;
			}} 
      
		  if (count == 0) return 0.0;
		  return sqrt(sum / count);
		} 

		void setLEDsBasedOnStdDev(double deviations) {
		  for (int i = 0; i < 8; i++) {
			digitalWrite(led[i], LOW);
		  } 
      
		  if (deviations <= -1.0) {
			digitalWrite(led[0], HIGH);
		  } 
      
		  if (deviations <= -2.0) {
			digitalWrite(led[0], HIGH);
			digitalWrite(led[1], HIGH);
		  } 
      
		  if (deviations <= -3.0) {
			digitalWrite(led[0], HIGH);
			digitalWrite(led[1], HIGH);
			digitalWrite(led[2], HIGH);
		  }
		} 

		void updateOLED(double temperature, double baseline, double num_std_dev) {
		  display.clearDisplay();
		  display.setTextSize(1);
		  display.setTextColor(SH110X_WHITE);
		  display.setCursor(0, 15);
		  display.print("Temp:");
		  display.print(temperature, 1);
		  display.print("F");
		  display.setCursor(0, 25);
		  display.print("Base:");
		  display.print(baseline, 1);
		  display.setCursor(64, 15);
		  display.print("Std:");
		  display.print(num_std_dev, 1);
		  display.setCursor(80, 25);
		  display.print("EMF:");
		  display.print(last_emf_reading);
		  for (int i = 0; i < 5; i++) {
			digitalWrite(led[i], LOW);
		  }
		  if (last_emf_reading >= 10) digitalWrite(led[3], HIGH);
		  if (last_emf_reading >= 100) digitalWrite(led[4], HIGH);
		  if (last_emf_reading >= 200) digitalWrite(led[5], HIGH);
		  if (last_emf_reading >= 300) digitalWrite(led[6], HIGH);
		  if (last_emf_reading >= 400) digitalWrite(led[7], HIGH);
		  display.display();
		}
