//Definicion del estado del boton
int estadoBoton = 0;

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
    //Prendemos el LED
    digitalWrite(led, HIGH);
  }
  else
  {
    //apagamos el LED
    digitalWrite(led, LOW);
  }
  
}
