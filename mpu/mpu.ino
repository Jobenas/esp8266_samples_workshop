#include <Wire.h>
#include "definitions.h"

// Direccion del dispositivo
const uint8_t MPU6050SlaveAddress = 0x68;

//Valores de ingreso, valores extraidos directamente del dispositivo
int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

//Valores convertidos, A = aceleracion (en relacion a g), G valores de giroscopio
double Ax, Ay, Az, T, Gx, Gy, Gz;

void setup() {
  Serial.begin(9600);
  Wire.begin(sda, scl);
  MPU6050_Init();
}

void loop() {  
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);

  convert_raw_vals();
  print_readings();
  
  delay(100);
}
