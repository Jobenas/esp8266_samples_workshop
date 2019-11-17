//Definimos el puerto de lectura analogica
const int pot = A0;

//Valor de lectura analogica
int potVal = 0;

void setup() {
  //Iniciamos configuracion serial
  Serial.begin(9600);
  Serial.println("Programa iniciando...");


}

void loop() {
  //Leer el valor analogico del potenciometro
  potVal = analogRead(pot);

  //Imprimimos el valor del potenciometro
  Serial.print("El valor del potenciometro es: ");
  Serial.println(potVal);

  //generamos un retardo de 2 segundos
  delay(2000);
}
