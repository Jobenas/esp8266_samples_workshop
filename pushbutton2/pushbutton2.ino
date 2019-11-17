//Definicion del estado del boton
int estadoBoton = 0;
int estadoLED = 0;
int estado = 0;

//Definicion de los pines para el boton y el pulsador
int led = D8;
int boton = D1;

void setup() {
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
      estado = 0;
      if(estadoLED)
      {
        digitalWrite(led, LOW);
        estadoLED = 0;
      }
      else
      {
        digitalWrite(led, HIGH);
        estadoLED = 1;
      }
    }
  }
  
}
