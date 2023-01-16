#include <Servo.h>          // servo kütüphanesi
#include <SoftwareSerial.h>  // nano ilteşim kütüphanesi

SoftwareSerial  yeniSeriPort (2, 3);   // Nnao iletişim

int C = 262;    //nato tanıtma veya oluşturma
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_ = 523;
int notalar[] = {C, D, E, F, G, A, B, C_};

const int analogPin = A3; // sıcaklık sensörü pini
float sicaklikDeger = 0;  // sıcaklık sensörü değeri

float gerilimDeger = 0;
float sensorDeger = 0;


int smoke_level;        // alev sensörü değişkeni
int isik_degeri;        // karanlık sensörü değeri

int b = 0;
int c = 0;

int veri;              // nano iletişim için değişken
int veri2;             // bluetooth iletişim için değişken
int hareket = 0;      // hareket sensörü için değer


int bahce = 4 ;       // bahçe ışıklandırması
int buz = 5 ;         // fırn açama kapama
int cama = 6 ;        // çamaşır açma kapama
int ocak = 7 ;        // doğalgaz açma kapama
int priz = 11 ;        // prizler açama kapama
int kl = 8 ;          // klima açma kapama
int pirPin = 12 ;     // hareket sensörü
int buzzer = 13 ;     // buzzer pini

Servo ka;             // kapı servosu
Servo gar;            // garaj servosu

int yagmur = A0;    // yağmur sensörü pini
int degery = 100;   // yağmur sensörü değeri
int veriy;          // yağmur sensörü değişkeni

int notaadet = 2;  // nota kaç kere çalıncağı

bool alarm ;
bool eko ;
bool bahc ;
bool seyehat ;
bool part ;
bool l;
bool kli;

void setup() {
  alarm = false;
  eko = false;
  bahc = false;
  seyehat = false;
  part = false;
  l = false;
  kli=false;
  for (int i = 0; i < notaadet ; i++) // açılma müziği
  {
    tone(buzzer, 392);
    delay(50);
    noTone(buzzer);
    delay(50);
    tone(buzzer, 440);
    delay(50);
    noTone(buzzer);
    delay(50);
    tone(buzzer, 494);
    delay(50);
    noTone(buzzer);
    delay(50);
    tone(buzzer, 523);
    delay(50);
    noTone(buzzer);
    delay(50);

  }                               // açılma müziği bitti

  Serial.begin(9600);           // ekrandan gelen giden veriyi okuma
  yeniSeriPort.begin(9600);     // ekrandan gelen giden veriyi okuma


  pinMode(bahce, OUTPUT);        // pinlerin çıkış veya giriş olduğunu tanıtık
  pinMode(buz, OUTPUT);
  pinMode(cama, OUTPUT);
  pinMode(ocak, OUTPUT);
  pinMode(priz, OUTPUT);
  pinMode(kl, OUTPUT);
  pinMode(buzzer, OUTPUT);

  pinMode(pirPin, INPUT);

  ka.attach(9); 
  gar.attach(10);   

}
void loop() {

  if (Serial.available() > 0) {       // bluetooth için veri okuma gönderme
    veri = Serial.read();
    Serial.write(veri);
    yeniSeriPort.write(veri);
    delay(1);
  }
  if ( yeniSeriPort.available() > 0) {  // nano için veri okuma gönderme
    veri2 =  yeniSeriPort.read();
    Serial.write(veri2);
    delay(1);
  }
   //*****************veri alma gönderme bitti ********************
  int buton_deger = analogRead(A4);  // dokunma sensörü kapıyı açar
  if (buton_deger > 500) {
    delay(10);
    ka.write(0);
    yeniSeriPort.write('0');
    digitalWrite(bahce, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);
    delay(1000);
    ka.write(180);

  }
  //**************dokunma sensörü ile kapı aç kapa bitti*****************
  sensorDeger = analogRead(analogPin);    //sıcaklık sensörü ısıya göre veri gönderme
  if (sensorDeger < 50 && sensorDeger > 30) {
    yeniSeriPort.write('C');
  }
  if (sensorDeger < 30) {

    yeniSeriPort.write('B');
  }
  if (veri == 'G') {                       // otomatik klima açma
    l = true;
  }
  if (l == true) {
    if (sensorDeger > 50) {               // otomatik klima derceye göre açıp kapıyor
      digitalWrite(l, HIGH);
    }
  }
  //*******************sıcaklık sensörü ile kullanılacaklar bitti************************
 smoke_level  = analogRead(A2);       // alev sensörü tanıtma
  delay(10);
  if (smoke_level < 25) {                // alev sensörü çalışma

    tone(buzzer, 300);
    delay(15);
    noTone(buzzer);
    delay(15);
    yeniSeriPort.write('p');     // ev yanıyor mesajı
    ka.write(180);
    delay(10);
    digitalWrite(buz, LOW);   // doğalgaz kesme
    delay(10);
  }
  else {
    noTone(buzzer);     // değilse buzzer kapa
  }
  //*************************alev sensörü bitti********************
  veriy = analogRead(yagmur);   // değişkeni yağmur sensörüne göre ata
  if (veriy > degery) {         // sensör değeri normal değeri göre veri gönder
    yeniSeriPort.write('<');
    delay(500);
  }
  else {
    delay(500);
    yeniSeriPort.write('>');
  }
//**************************yağmur sensörü bitti**************************
//**************************aydınltmalar vb açma kapama********************
/*     if (veri == '0') {                    // bluetooth tan gelen veri 3 ise bahçe ışığı aç
     yeniSeriPort.write('0');
  }
    if (veri == '1') {                    // bluetooth tan gelen veri 3 ise bahçe ışığı aç
    yeniSeriPort.write('1');
  }
   if (veri == '2') {                    // bluetooth tan gelen veri 3 ise bahçe ışığı aç
     yeniSeriPort.write('3');
  }
    if (veri == '3') {                    // bluetooth tan gelen veri 3 ise bahçe ışığı aç
    yeniSeriPort.write('3');
  }*/
  if (veri == '4') {                    // bluetooth tan gelen veri 3 ise bahçe ışığı aç
    digitalWrite(bahce, HIGH);
    bahc = false;
     part = false;
 }
  if (veri == '5') {                       // bluetooth tan gelen veri 5 ise bahçe ışığı kapa
    digitalWrite(bahce, LOW);
    bahc = false;
    part = false;
  }
  if (veri == '6') {                      // bluetooth tan gelen veri 7 ise doğalgaz aç

    digitalWrite(buz, HIGH);
     part = false;

  }
  if (veri == '7') {                    // bluetooth tan gelen veri 6 ise doğalgaz kapa

    digitalWrite(buz, LOW);
     part = false;

  }
  if (veri == '8') {                   // bluetooth tan gelen veri 8 ise çamaşır makinesi  aç

    digitalWrite(cama, HIGH);

  }
  if (veri == '9') {                   // bluetooth tan gelen veri 3 ise çamaşır makinesi kapa

    digitalWrite(cama, LOW);

  }
  if (veri == 'a') {                    // bluetooth tan gelen veri a ise fırını  aç

    digitalWrite(ocak, HIGH);

  }
  if (veri == 'b') {                   // bluetooth tan gelen veri b ise fırını kapa

    digitalWrite(ocak, LOW);

  }
  if (veri == 'e') {                      // bluetooth tan gelen veri e ise prizleri aç

    digitalWrite(priz, HIGH);
     part = false;

  }
  if (veri == 'f') {                        // bluetooth tan gelen veri f ise prizleri kapa

    digitalWrite(priz, LOW);
     part = false;

  }
  if (veri == 'h') {                       // bluetooth tan gelen veri h ise klima aç
    kli=true;
  }
  if(kli ==true){
    digitalWrite(kl, HIGH);
  }
  if (veri == 'g') {                     // bluetooth tan gelen veri g ise klimayı aç
    kli=false;
  }
  if(kli==false){
    digitalWrite(kl, LOW);
    l = false;
     part = false;
  }
  if (veri == 'v') {                    // bluetooth tan gelen veri n ise garaj aç ve sinyal ver

    gar.write(90);
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(50);

  }
  if (veri == 'n') {                // bluetooth tan gelen veri v ise garaj kapa ve sinyalı kapa

    gar.write(0);
    digitalWrite(buzzer, LOW);


  }
  if (veri == 'k') {           // bluetooth tan gelen veri k ise kapı aç

    ka.write(180);

  }
  if (veri == 'l') {                  // bluetooth tan gelen veri l ise kapı kapa

    ka.write(0);


  }
  //**************************aydınltmalar vb açma kapama bitti********************
    if (veri == 'M') {                // bluetooth tan gelen veri 3 ise oto bahçe aç
    bahc = true;
    part = false;
  }
  if (bahc == true) {
    isik_degeri = analogRead(A5);          // karanlık sensörü değişkenini A5 pininden gelen değerte ata
    delay(500);
    if(kli==true){
    if (isik_degeri > 15)                // değer 1000 den yüksek ise bahçe ışığını aç değilse kapa
    {
      digitalWrite(bahce, LOW);
    }
    else
    {
      digitalWrite(bahce, HIGH);

    }
    }
       if(kli==false){
    if (isik_degeri > 6)                // değer 1000 den yüksek ise bahçe ışığını aç değilse kapa
    {
      digitalWrite(bahce, LOW);
    }
    else
    {
      digitalWrite(bahce, HIGH);

    }
    }
    //**********************otomatik bahçe bitti************************
  }
  if (veri == 'c') {       // bluetooth tan gelen veri c ise hırsız alarmı aç
    alarm = true ;
     part = false;
    eko = false;
    bahce = false;
    l = false;
    seyehat = false;
  }
   if (veri2 == 'c') {       // bluetooth tan gelen veri c ise hırsız alarmı aç
    alarm = true ;
     part = false;
    eko = false;
    bahce = false;
    l = false;
    seyehat = false;
  }
   hareket = digitalRead(pirPin);    // hareket değişkenini hareket sensörü değerine göre ata
  if (alarm == true) {
    if (hareket == HIGH) {   // veri var ise odalır yak söndür ve alarm çal
    
      yeniSeriPort.write('0');
      delay(25);
      yeniSeriPort.write('1');
      delay(25);
      yeniSeriPort.write('2');
      delay(25);
      yeniSeriPort.write('3');
      delay(25);
      digitalWrite(bahce, HIGH);
      delay(25);
      digitalWrite(bahce, LOW);
      delay(25);
      tone(buzzer, 300);
      delay(100);
      noTone(buzzer);
      delay(100);
     // yeniSeriPort.write('m');
       
    }
  }
  if (veri == 'd') {            // bluetooth tan gelen veri d ise hırsız alarmı kapa
    alarm = false;
    yeniSeriPort.write('1');
    yeniSeriPort.write('3');
    digitalWrite(bahce, LOW);
    digitalWrite(buzzer, LOW);

  }
   if (veri2 == 'd') {            // bluetooth tan gelen veri d ise hırsız alarmı kapa
    alarm = false;
    yeniSeriPort.write('1');
    yeniSeriPort.write('3');
    digitalWrite(bahce, LOW);
    digitalWrite(buzzer, LOW);

  }
  //****************************hırsız alarmı bitti*****************************
 if (veri == 'y') {       // bluetooth tan gelen veri y ise seyehat modu aç
    seyehat = true;
    part = false;
    alarm = false;
    eko = false;
    bahce = false;
    l = false;
  }
  if ( seyehat == true) {
    yeniSeriPort.write('0');
    digitalWrite(buz, LOW);
    digitalWrite(cama, LOW);
    digitalWrite(ocak, LOW);
    Serial.write('3');
    digitalWrite(bahce, LOW);
    digitalWrite(priz, LOW);
    kli=false;
    if (hareket == HIGH) {

      yeniSeriPort.write('0');
      delay(10);
      yeniSeriPort.write('1');
      delay(10);
      yeniSeriPort.write('2');
      delay(10);
      yeniSeriPort.write('3');
      delay(10);
      digitalWrite(bahce, HIGH);
      delay(10);
      digitalWrite(bahce, LOW);
      delay(10);
      digitalWrite(buzzer, HIGH);
      delay(10);
      digitalWrite(buzzer, LOW);
      delay(10);
      //yeniSeriPort.print('m');

    }
  }
  if (veri == 'z') {        // bluetooth tan gelen veri z ise seyehat modu kapa
    seyehat = false;
    yeniSeriPort.write('0');
    digitalWrite(buz, HIGH);
    digitalWrite(cama, LOW);
    digitalWrite(ocak, LOW);
    yeniSeriPort.write('2');
    digitalWrite(bahce, LOW);
    digitalWrite(priz, HIGH);
    kli=false;


  }
  //*************************seyehat modu bitti***************************
 if (veri == 'u') {           // bluetooth tan gelen veri u ise parti aç
    part = true;
    alarm = false;
    eko = false;
    bahce = true;
    l = true;
    seyehat = false;
    
    
  }
  if (part == true) {
    delay(100);
    yeniSeriPort.write('0');
    delay(100);
    digitalWrite(buz, HIGH);
    digitalWrite(bahce, HIGH);
    digitalWrite(priz, HIGH);
    kli=true;
  }
  if (veri == 'o') {       // bluetooth tan gelen veri o ise parti kapa aç
    part = false;
    yeniSeriPort.write(1);
    digitalWrite(buz, LOW);
    digitalWrite(bahce, LOW);
    digitalWrite(priz, LOW);
    kli=false;

//*****************************parti modu bitti****************************
  }
  if (veri == 'r') {  // bluetooth tan gelen veri r ise ekonomik modu aç
    eko = true;
    alarm = false;
    bahc = true;
    seyehat = false;
    part = false;
    l = true;
  }
  if (eko == true) {

    if (hareket == HIGH) {   // harekette veri varsa


      Serial.write('0');  // salon ışığı aç


    }
    digitalWrite(buz, HIGH);
    digitalWrite(priz, HIGH);
  }
  if (veri == 's') {   // bluetooth tan gelen veri s ise ekonomik modu kapa
    eko = false;
    yeniSeriPort.write(1);
    delay(1);
  }
  //************************ekonomik mod bitti******************************
  if (veri2 == 'e') {       // nanodan gelen veri e ise prizleri aç

    digitalWrite(priz, HIGH);
     part = false;
    delay(1);
  }
  if (veri2 == 'f') {     // nanodan gelen veri f ise prizleri kapa

    digitalWrite(priz, LOW);
     part = false;
    delay(1);
  }
   if (veri2 == '6') {                      // bluetooth tan gelen veri 7 ise doğalgaz aç

    digitalWrite(buz, HIGH);
     part = false;

  }
  if (veri2 == '7') {                    // bluetooth tan gelen veri 6 ise doğalgaz kapa

    digitalWrite(buz, LOW);
     part = false;

  }
  if (veri2 == 'v') {      // nanodan gelen veri I ise garaj aç

    gar.write(90);
  }
  if (veri2 == 'n') {          //  nanodan gelen veri j ise garaj kapa

    gar.write(0);
  }
  if (veri2 == '4') {             // nanodan gelen veri 4 ise bahçe ışığı  aç
    digitalWrite(bahce, HIGH);
    bahc = false;
     part = false;
    delay(1);
  }
  if (veri2 == '5') {              // nanodan gelen veri 5 ise bahçe ışığı kapa
    bahc =false;
    digitalWrite(bahce, LOW);
     part = false;
    delay(1);
  }
  if (veri2 == 'z') {           // nanodan gelen veri k ise kapı kapat 500 mili saniye sonra kapa
    ka.write(180);
    Serial.write('0');
    digitalWrite(bahce, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(50);
    digitalWrite(buzzer, LOW);
    delay(500);
    ka.write(0);

  }
 /* if (veri2 == 'c') {            // nanodan gelen veri c ise 1000 mili saniye sonra alarm  aç
    alarm = true;
  }*/
  if (veri2 == 'C') {
    tone(buzzer, 392);
    delay(50);
    noTone(buzzer);
    delay(50);
    tone(buzzer, 440);
    delay(50);
    noTone(buzzer);
    delay(50);
    tone(buzzer, 494);
    delay(50);
    noTone(buzzer);
    delay(50);
    tone(buzzer, 523);
    delay(50);
    noTone(buzzer);
    delay(50);

  }
  veri2 = 'P';
  veri = 'P';
}
