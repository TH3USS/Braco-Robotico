#include <IRremote.h> //Biblioteca para receptor IR
#include <Servo.h> //Biblioteca para servo motores

//Analógico 1
#define eixo_x1 A0
#define eixo_y1 A1
//Analogico 2
#define botao 2
#define eixo_x2 A2
//Sensor IR
#define pinSensor 4

Servo motorA; //Articulação eixo x
Servo motorB; //Articulação eixo z
Servo motorC; //Articulação garra
Servo motorD; //Articulação eixo y

int anguloA = 45, anguloB = 100, anguloD = 90, anguloC = 0;

IRrecv irrecv(pinSensor);
int resul;
int aux = 0, aux1 = 0, cont = 0;

void setup() {               //Definição de pinos de entrada e saída
 Serial.begin(9600);
 pinMode(eixo_x1, INPUT);
 pinMode(eixo_y1, INPUT);
 pinMode(botao, INPUT_PULLUP);
 pinMode(eixo_x2, INPUT);
 motorA.attach(3);
 motorB.attach(5);
 motorC.attach(6);
 motorD.attach(9);

 irrecv.enableIRIn();
}

void loop() {
 int x = map(analogRead(eixo_x1),0,1023,0,180); //Guardar variação do eixo x do analógico 1 - map é uma função que converte uma sequência p/ outra
 int y = map(analogRead(eixo_y1),0,1023,0,180); //Guardar variação do eixo y do analógico 1 - map é uma função que converte uma sequência p/ outra
 int x1 = map(analogRead(eixo_x2),0,1023,0,180);//Guardar variação do eixo x do analógico 2 - map é uma função que converte uma sequência p/ outra

  if(irrecv.decode()){
    delay(280);
    resul = irrecv.decodedIRData.command;
    cont = 0;
    Serial.println(resul);
    if(resul == 69 && aux == 0) aux = 1;
    else if(resul == 69) aux = 0;
    irrecv.resume();
  }
 if(resul != 22 && resul != 25){
  if(digitalRead(botao) == HIGH){  //Botão solto, garra fechada
    anguloC = 90;
    motorC.write(anguloC);                           //Põe o servo motor na posição indicada pelo ângulo
  } else{                                     //Botão precionado, garra aberta
    anguloC = 45;
    motorC.write(anguloC);                           //Põe o servo motor na posição indicada pelo ângulo
  } 
 } else {
   if(resul == 22){
      anguloC = 45;
      motorC.write(anguloC);
      delay(25);
   } 
   if(resul == 25){
     anguloC = 90;
     motorC.write(anguloC);
     delay(25);
   }
 }

 if(aux == 1){
  moveA(x, resul);  //Chama o movimento de eixo x
  moveB(x1, resul); //Chama o movimento de eixo z
  moveD(y, resul);  //Chama o movimento de eixo y
 }

 if(cont < 10) cont++;
 else{
   resul = 0;
 } 
}

void moveA(int x, int resul){                        //Verifica a posição do analógico (se aumentará ou diminuirá o ângulo) e respeita os limites impostos dos ângulos
 if((x >= 170 && anguloA > 10) || (resul == 90 && anguloA > 10)){
  anguloA = anguloA - 1;
 } else if((x == 0 && anguloA < 165) || (resul == 8 && anguloA < 165)){
  anguloA = anguloA + 3;
 }
 motorA.write(anguloA);             //Põe o servo motor na posição indicada pelo ângulo
 return;
}

void moveB(int x, int resul){
 if((anguloB < 150 && x == 0) || (anguloB < 150 && resul == 7)){
  anguloB = anguloB + 10;
 } else if((anguloB > 110 && x == 180) || (anguloB > 110 && resul == 21)){
  anguloB = anguloB - 20;
 }
 motorB.write(anguloB);
 return;
}

void moveD(int y, int resul){
 if((anguloD < 170 && y == 179) || (anguloD < 170 && resul == 82)){
  anguloD = anguloD + 3;
 }
 else if((anguloD > 10 && y == 0) || (anguloD > 10 && resul == 24)){
  anguloD = anguloD - 3;
 }
 motorD.write(anguloD);
 return;
}