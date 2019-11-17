//Definicion del estado del boton
int estadoBoton = 0;
int estadoLED = 0;
int estado = 0;

//Definicion de los pines para el boton y el pulsador
int led = D8;
int boton = D1;

void setup() {
  //Iniciamos configuracion serial
  Serial.begin(9600);
  Serial.println("Programa iniciando...");

  Serial.println("Configurando Entradas/Salidas...");
  //Pin de salida para el LED
  pinMode(led, OUTPUT);
  //Pin de entrada para el pulsador
  pinMode(boton, INPUT);
  
  //apagamos el LED al comienzo
  digitalWrite(led, LOW);
}

void loop() {
  //Leemos el estado del pulsador
  estadoBoton = digitalRead(boton);

  //Revisamos el estado del pulsador
  if(estadoBoton == HIGH)
  {
    estado = 1;
  }
  else
  {
    if(estado)
    {
      //Usamos un solo print que no tiene salto de linea que esta mostrando
      //la parte generica del mensaje
      Serial.print("Cambiando el estado del LED a ");
      estado = 0;
      if(estadoLED)
      {
        digitalWrite(led, LOW);
        estadoLED = 0;
        //Usamos un print con salto de linea que indica la parte dinamica del mensaje
        Serial.println("APAGADO");
      }
      else
      {
        digitalWrite(led, HIGH);
        estadoLED = 1;
        //Usamos un print con salto de linea que indica la parte dinamica del mensaje
        Serial.println("ENCENDIDO");
      }
    }
  }
}
