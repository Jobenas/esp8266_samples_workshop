//Definimos el puerto de lectura analogica
const int pot = A0;

//Valor de lectura analogica
int potVal = 0;

//Variables de inicio y fin cuenta
long inicio;

void setup() {
  //Iniciamos configuracion serial
  Serial.begin(9600);
  Serial.println("Programa iniciando...");

  inicio = millis();
}

void loop() {
  if((millis() - inicio) >= 1000)
  {
    //Leer el valor analogico del potenciometro
    potVal = analogRead(pot);
  
    //Imprimimos el valor del potenciometro
    Serial.print("El valor del potenciometro es: ");
    Serial.println(potVal); 
    inicio = millis();
  }
}
