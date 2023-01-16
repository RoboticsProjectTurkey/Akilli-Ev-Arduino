#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>   // lcd ekran kütüphanesi
#include <Keypad.h>                   // keypad kütüphanesi

int ba = 0;
int bc = 0;
int pa = 0;
int pc = 0;
int ka = 0;
int kc = 0;

Adafruit_LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);      // lcd ekran pinleri

int salon = 13;    // salon ışığı pini
int yatak = 2;     // yatak odası pini

int elek;         // kablolardan geçen akımın değişkeni


int upButton = 5;        // menü tuşlarından yukarı
int downButton = 4;      // menü tuşlarından aşağı
int selectButton = 3;    // menü tuşlarından seç
int menu = 1;

int veri;             // unoya veri göndermek için değişken
int a = 0;
int b = 0;
bool mur;
bool sayfa;

int control = 0;
int counter = 0;
String enter_password;     // keypad şifre
String password = "#1499";   // keypad şifre

const byte satir = 4;    //keypad tanıt
const byte sutun = 4;     //keypad tanıt

char tus;    // keypad için değişken
char tus_takimi[satir][sutun] = {   // keypad tanıt
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte satir_pinleri[satir] = {9, 8, 7, 6};  //keypad tanıt
byte sutun_pinleri[satir] = {12, 11, 10};   //keypad tanıt

Keypad mahmut = Keypad(makeKeymap(tus_takimi) , satir_pinleri , sutun_pinleri , satir , sutun);  //keypad tanıt

void setup() {
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);  // menü düğmeler
  pinMode(selectButton, INPUT_PULLUP);
  updateMenu();

  pinMode(salon, OUTPUT);
  pinMode(yatak, OUTPUT);

  mur = false;
  sayfa= false;

  Serial.begin(9600);
}

void loop() {

  elek = analogRead(A6);   // kablolardan geçen elektrik akımı için pin tanıtma
  delay(100);

  if (Serial.available() > 0) {     // unoya veri gönderme   alma
    veri = Serial.read();
  }
    if (veri == '0') {  // unodan gelen veri 0 ise salon ışıığı aç

      digitalWrite(salon, HIGH);

    }
    if (veri == '1') {       // unodan gelen veri 1 ise salon ışığı kapa

      digitalWrite(salon,   LOW);


    }
    if (veri == '2') {         // unodan gelen veri 2 ise yak odası aç

      digitalWrite(yatak, HIGH);



    }
    if (veri == '3') {        // unodan gelen veri 3 ise yatak odası ışığı kapa

      digitalWrite(yatak, LOW);

    }
    if(ba == 1){
        Serial.write('4');
      ba = 0;
    }
    if(bc == 1){
        Serial.write('5');
      bc = 0;
    }
    if(pa == 1){
        Serial.write('e');
      pa = 0;
    }
    if(pc == 1){
        Serial.write('f');
      pc = 0;
    }
    if(ka == 1){
       Serial.write('v');  
      ka = 0;
    }
    if(kc == 1){
       Serial.write('n');  
      kc = 0;
    }

    if (veri == 'p') { // unodan gelen yangın sensörünün değerine gelen veri + ise yangın uyarısı
      sayfa=false;
       lcd.clear();
      lcd.begin(16, 2);
      lcd.print(" UYARI!!!");
      lcd.setCursor(0, 1);
      lcd.print(" YANGIN");
      delay(200);
      lcd.clear();
      delay(200);
      lcd.clear();
      lcd.begin(16, 2);
      lcd.print(" UYARI!!!");
      lcd.setCursor(0, 1);
      lcd.print(" YANGIN");
      delay(200);
    }
    if (veri == '<') {     // unodan gelen veri yağmmur sensörü ile belirlenen veriy göre  hava durumu
      mur = true;
    /* lcd.clear();
      lcd.begin(16, 2);
      lcd.print(" YAGMUR YAGIYOR");
      lcd.setCursor(0, 1);
      lcd.print("");
      delay(200);
      lcd.clear();
      delay(200);
      lcd.clear();
      lcd.begin(16, 2);
      lcd.print(" YAGMUR YAGIYOR");
      lcd.setCursor(0, 1);
      lcd.print("");
      delay(200);*/
    }
    if (veri == '>') { 
      mur = false;
    }
  
  if (!digitalRead(downButton)) {  // menü olayları
    sayfa= false;
    menu++;
    updateMenu();
    delay(100);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(upButton)) {
    menu--;
    sayfa= false;
    updateMenu();
    delay(100);
    while (!digitalRead(upButton));
  }
  if (!digitalRead(selectButton)) {
 //   sayfa= false;
    executeAction();
    delay(100);
    while (!digitalRead(selectButton));
  }
  if(sayfa == true){   
      lcd.clear();
      lcd.begin(16, 2); 
      if(mur == true){
         lcd.print("Y  AKILLI EV  :)");  // 1. sayfa
      }
       if(mur == false){
         lcd.print("   AKILLI EV  :)");  // 1. sayfa
      }
      lcd.setCursor(0, 1);
      lcd.print("    ARDUINO");
  }
  char tus = mahmut.getKey();  // şifre için
    if (tus != NO_KEY) {
     if(tus == '#') {    // keypad üzerinden  # tuşa basıldığında şifre girme etkinleştirme
       control = 1;
     }
     if(tus == '*') {    
        enter_password = "";
        control = 0;
        counter = 0;
     }
      if(control == 1){
       enter_password = enter_password + tus;
       counter++;
       if(enter_password == password) {
         for(int value = 0; value < 90; value++) {  // şifre doğru ise yapılacaklar
         
           lcd.clear();
           lcd.begin(16, 2);
           lcd.print("**HOSGELDINIZ**");
           lcd.setCursor(0, 1);
           lcd.print("    MERHABA");
           Serial.write('k');
           Serial.write('d');
           digitalWrite(salon, HIGH);
         }
         delay(1000);
         sayfa=true;
         enter_password = "";
         control = 0;
         counter = 0;
    }
    sayfa=false;
     lcd.clear();
      lcd.begin(16, 2);
      lcd.print("**Sıfre**");
      lcd.setCursor(0, 1);
      lcd.print(enter_password);
    }
    if((enter_password != password) && (counter > 4) ) {  // şifre yanlış ise (  4 ten fazla giriş olduğunda)
       sayfa=false;
       lcd.clear();
       lcd.begin(16, 2);
       lcd.print("HATALI GIRISLER");
       lcd.setCursor(0, 1);
       lcd.print("  UYARI!!!");
    delay(10000);
    Serial.write('c');
    enter_password = "";
         control = 0;
         counter = 0;
    }
    }
}
void updateMenu() {    // menüler
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      while ( b <= 0) { //açılış mesajı için
        Serial.write('C');
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print(" ACILIYOR.");
        lcd.setCursor(0, 1);
        lcd.print(" MERHABA");
        delay(1000);
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print(" ACILIYOR..");
        lcd.setCursor(0, 1);
        lcd.print(" MERHABA");
        delay(1000);
        lcd.clear();
        lcd.begin(16, 2);
        lcd.print(" ACILIYOR...");
        lcd.setCursor(0, 1);
        lcd.print(" MERHABA");
        delay(1000);
        lcd.clear();
        digitalWrite(salon, HIGH);
        Serial.write('6');
        Serial.write('e');
        Serial.write('4');
        delay(3000);
        b = b + 1;
        sayfa = true;
      }
      delay(5);
      sayfa = true;
      break;
    case 2:
      lcd.clear();
      lcd.begin(16, 2);
      lcd.print("     MENU");    // 2. sayfa 1, seçenek
      lcd.setCursor(0, 1);
      lcd.print(">ELEKTRIK  HAVA");
      break;
    case 3:
      lcd.clear();             // 2. sayfa 2. seçenek
      lcd.begin(16, 2);
      lcd.print(" MENU");
      lcd.setCursor(0, 1);
      lcd.print(" ELEKTRIK >HAVA");
      break;
    case 4:
      lcd.clear();                // 3. sayfa 1. seçenek
      lcd.begin(16, 2);
      lcd.print(" SALON ISIGI ");
      lcd.setCursor(0, 1);
      lcd.print("  >AC   KAPA");
      break;
    case 5:
      lcd.clear();
      lcd.begin(16, 2);            // 3. sayfa 2. seçenek
      lcd.print(" SALON ISIGI");
      lcd.setCursor(0, 1);
      lcd.print("   AC  >KAPA");
      break;
    case 6:
      lcd.clear();
      lcd.begin(16, 2);                  // 4. sayfa 1. seçenek
      lcd.print(" BAHCE ISIGI");
      lcd.setCursor(0, 1);
      lcd.print("  >AC   KAPA");
      break;
    case 7:
      lcd.clear();
      lcd.begin(16, 2);                    // 4. sayfa 2. seçenek
      lcd.print(" BAHCE ISIGI");
      lcd.setCursor(0, 1);
      lcd.print("   AC  >KAPA");
      break;
    case 8:
      lcd.clear();
      lcd.begin(16, 2);
      lcd.print(" PRIZ");                    // 5. sayfa 1. seçenek
      lcd.setCursor(0, 1);
      lcd.print("  >AC   KAPA");
      break;
    case 9:
      lcd.clear();
      lcd.begin(16, 2);
      lcd.print(" PRIZ");
      lcd.setCursor(0, 1);                    // 5. sayfa 1. seçenek
      lcd.print("   AC  >KAPA");
      break;
    case 10:
      lcd.clear();
      lcd.begin(16, 2);
      lcd.print(" GARAJ KAPISI");              // 6. sayfa 2. seçenek
      lcd.setCursor(0, 1);
      lcd.print("  >AC   KAPA");
      break;
    case 11:
      lcd.clear();                         // 6. sayfa 1. seçenek
      lcd.begin(16, 2);
      lcd.print(" GARAJ KAPISI");
      lcd.setCursor(0, 1);
      lcd.print("   AC  >KAPA");
      break;
    case 12:
      lcd.clear();
      lcd.begin(16, 2);
      lcd.print(" SISTEMI");                   // 7. sayfa
      lcd.setCursor(0, 1);
      lcd.print(">YENIDEN BASLAT");
      break;
    case 13:
      lcd.clear();
      lcd.begin(16, 2);                        // 8. sayfa
      lcd.print(" SISTEM");
      lcd.setCursor(0, 1);
      lcd.print(">HATALARI ICIN");
      break;
    case 14:
      menu = 4;
      break;
  }
}

void executeAction() {
  switch (menu) {
    case 1:
      action1();            // menü oluşumu
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action4();
      break;
    case 5:
      action5();
      break;
    case 6:
      action6();
      break;
    case 7:
      action7();
      break;
    case 8:
      action8();
      break;
    case 9:
      action9();
      break;
    case 10:
      action10();
      break;
    case 11:
      action11();
      break;
    case 12:
      action12();
      break;
    case 13:
      action13();
      break;

  }
}

void action1() {
   
  lcd.clear();
  lcd.begin(16, 2);                // 1. sayfa içi
  lcd.print("  YARDIM");
  lcd.setCursor(0, 1);
  lcd.print("SEC/ASAGI/YUKARI");
  delay(1500);
   
   
}

void action2() {
  lcd.clear();                  
  lcd.begin(16, 2);             // 2. sayfa 1. seçenek içi
  lcd.print(" ELEKTRIK");
  lcd.setCursor(0, 1);
  lcd.print(elek);
  delay(1500);

  
}
void action3() {
  lcd.clear();
 if(veri == 'D'){
  lcd.clear();
  lcd.begin(16, 2);                // 2. sayfa 2. seçenek içi
  lcd.print("  HAVA");             // veriye göre yazdırma
  lcd.setCursor(0, 1);
  lcd.print("SICAK");
 }
  if(veri == 'C'){
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print("  HAVA");
  lcd.setCursor(0, 1);
  lcd.print("ILIK");
 }
  if(veri == 'B'){
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print("  HAVA");
  lcd.setCursor(0, 1);
  lcd.print("SOĞUK");
 }
 delay(1500); 

  
}
 
void action4() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" SALON ISIGI");
  lcd.setCursor(0, 1);                  // 3. sayfa 2. seçenek içi
  lcd.print(" ONAY VER");
  delay(1500);
   while ( a <= 50 && digitalRead(upButton) == 1){
   digitalWrite(salon, HIGH);
  if (digitalRead(downButton) == 1){
      a = a + 1; // a'yı 1 arttır.
  }
    }
    
}
void action5() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" SALON ISIGI");
  lcd.setCursor(0, 1);
  lcd.print(" ONAY VER");
  delay(1500);
   while (digitalRead(downButton) == 1){     // 3. sayfa 2. seçenek içi
  digitalWrite(salon, LOW);
  if (digitalRead(upButton) == 1){
      a = a + 1; // a'yı 1 arttır.
  }
   }
   
}
void action6() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" BAHCE ISIGI");
  lcd.setCursor(0, 1);
  lcd.print(" ONAY VER");                    // 4. sayfa 1. seçenek içi
  delay(150);
    if (!digitalRead(selectButton)) {
    ba = 1;
    delay(100);
    while (!digitalRead(selectButton));
  }
}
void action7() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" BAHCE ISIGI");
  lcd.setCursor(0, 1);
  lcd.print(" ONAY VER");
  delay(150);  // 4. sayfa 2. seçenek içi
    if (!digitalRead(selectButton)) {
    bc = 1;
    delay(100);
    while (!digitalRead(selectButton));
    }
}
void action8() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" PRIZ");
  lcd.setCursor(0, 1);
  lcd.print(" ONAY VER");                         // 5. sayfa 1. seçenek içi
  delay(150);
  if (!digitalRead(selectButton)) {
     pa = 1;
    delay(100);
    while (!digitalRead(selectButton));
  }
}
void action9() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" PRIZ");
  lcd.setCursor(0, 1);
  lcd.print(" ONAY VER");                            // 5. sayfa 2. seçenek içi
  delay(150);
   if (!digitalRead(selectButton)) {
     pc = 1;
    delay(100);
    while (!digitalRead(selectButton));
  }
}
void action10() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" GARAJ KAPISI");
  lcd.setCursor(0, 1);                                 // 6. sayfa 1. seçenek içi
  lcd.print(" ONAY VER");
  delay(150);
  if (!digitalRead(selectButton)) {
      ka = 1;
    delay(100);
    while (!digitalRead(selectButton));
  }
}
void action11() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" GARAJ KAPISI");                         // 6. sayfa 2. seçenek içi
  lcd.setCursor(0, 1);
  lcd.print(" ONAY VER");
  delay(150);
   if (!digitalRead(selectButton)) {
        kc = 1;
    delay(100);
    while (!digitalRead(selectButton));
  }
}

void action12() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" YENIDEN BASLATMA");
  lcd.setCursor(0, 1);
  lcd.print(" EMIN MISINIZ?");
  delay(1500);
   if (digitalRead(downButton) == 1){
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" KAPANIYOR.");
  lcd.setCursor(0, 1);
  lcd.print("GULE GULE");
  digitalWrite(salon, LOW);
  digitalWrite(yatak, LOW);
  Serial.write('5');
  Serial.write('7');                             // 7. sayfa  içi
  delay(300);
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" KAPANIYOR..");
  lcd.setCursor(0, 1);
  lcd.print("GULE GULE");
  Serial.write('b');
  Serial.write('f');
  Serial.write('h');
  Serial.write('g');
  delay(300);
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" KAPANIYOR...");
  lcd.setCursor(0, 1);
  lcd.print("GULE GULE");
  Serial.write('l');
  Serial.write('9');
  Serial.write('v');
  delay(1000);
  lcd.clear();
  delay(5000);
  lcd.begin(16, 2);
  lcd.print(" ACILIYOR.");
  lcd.setCursor(0, 1);
  lcd.print(" MERHABA");
  digitalWrite(salon, HIGH);
  Serial.write('M');
  delay(300);
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" ACILIYOR..");
  lcd.setCursor(0, 1);
  lcd.print(" MERHABA");
  Serial.write('e');
  Serial.write('G');
  Serial.write('C');
  delay(300);
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" ACILIYOR...");
  lcd.setCursor(0, 1);
  lcd.print(" MERHABA");
  Serial.write('6');
  delay(300);
}
}
void action13() {
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print("HATA VAR ISE");
  lcd.setCursor(0, 1);
  lcd.print("UYGULAMADA");
  delay(300);
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print("HATA VAR ISE");                   // 8. sayfa  içi
  lcd.setCursor(0, 1);
  lcd.print(" YARDIM");
  delay(300);
   lcd.clear();
  lcd.begin(16, 2);
  lcd.print("HATA VAR ISE");
  lcd.setCursor(0, 1);
  lcd.print("KISMINA");
  delay(300);
   lcd.clear();
  lcd.begin(16, 2);
  lcd.print("HATA VAR ISE");
  lcd.setCursor(0, 1);
  lcd.print("   BAK");
  delay(300);
  
}
