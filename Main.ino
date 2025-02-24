//created : Nana Perm12
//Created: 25/02/2025
//Program: Doorlock Keypad & Fingerprint

#include <Wire.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SPI.h>

const byte baris = 4;
const byte kolom = 4;
char Hexa[baris][kolom] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//--Pin yang digunakan--//
byte pinBaris [baris] = {10, 9, 8, 7};
byte pinKolom [kolom] = {6, 5, A2, A3};
int relay1 = A0;
int bt_dalam = 11;

//--Variable tipe data--//
char password[4];
char pass[4], pass1[4];
int i = 0;
int addrpassawal = 5;
byte passawal;
char customKey;
int eadd = 0;                             //eeprom addres
int getFingerprintIDez();                 //fungsi check sidikjari
uint8_t getFingerprintEnroll(int id);     //fungsi buat ngerekan atau menambhkan sidikjari
int access = 3;
bool menu_utama = true;

//initialize an instance of class NewKeypad
//Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS)
//------------------------------//
Keypad customKeypad = Keypad( makeKeymap(Hexa), pinBaris, pinKolom, baris, kolom);
LiquidCrystal_I2C lcd(0x27, 16, 2);             //alamat lcd
SoftwareSerial mySerial(2, 3);                  //pin sidikjari
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  pinMode(bt_dalam, INPUT_PULLUP);
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, LOW);

  finger.begin(57600);
  if (finger.verifyPassword()) {

  } else {
    while (1);
  }

  eadd = EEPROM.read(10);                 //ketika menekan tombol A maka sidikjari akan bertambah dan akan disimpan di eeprom
  if (eadd > 200)EEPROM.write(0, 0);      //jika pennyimpanan eepromnya udah lebih dari 200 maka akan mengulang ke 0 lagi.
  passawal = EEPROM.read(addrpassawal);
  delay(500);

  if (passawal == 0) {
    for (int j = 0; j < 4; j++) {
      EEPROM.write(j, j + 49);
      EEPROM.write(addrpassawal, 1);
    }
  }
  else {
    for (int j = 0; j < 4; j++) {
      pass[j] = EEPROM.read(j);
    }
  }
  delay(200);

  lcd.setCursor (0, 0);
  lcd.print("    Doorlock    ");
  lcd.setCursor (0, 1);
  lcd.print("By Nana Perm12");
  delay(1000);
  lcd.setCursor (0, 0);
  lcd.print(" Finger/Keypad  ");
  delay(1200);
  lcd.clear();
  delay(15);
}

void loop() {
  customKey = customKeypad.getKey();

  if (menu_utama == true) {
    lcd.setCursor (0, 0);
    lcd.print(F("Finger/Password"));
awal:
    getFingerprintIDez();
    password_enter();
    delay(75);
  }
  else {
    change();
  }

  if (digitalRead(bt_dalam) == LOW) {
    lcd.clear();
    delay(15);
    digitalWrite(relay1, HIGH);
    lcd.setCursor(1, 0);
    lcd.print("Akses Diterima");
    lcd.setCursor (0, 1);
    lcd.print(F("AutoLock after "));
    for (int i = 5; i > 0; i--) {
      lcd.setCursor (15, 1); lcd.print(i);
      delay (1000);
    }
    digitalWrite(relay1, LOW);
    lcd.clear();
    delay(15);
    menu_utama = true;
  }

  if (customKey == 'A') {
    //password_enter();
    lcd.clear();
    delay(15);
    tambah_sidikjari();
  }

  if (customKey == 'B') {
    //password_enter();
    lcd.clear();
    delay(15);
    hapus_sidikjari();
  }

  if (customKey == 'C') {
    //password_enter();
    lcd.clear();
    delay(15);
    menu_utama = false;
  }

  if (customKey == '#') {
    //password_enter();
    lcd.clear();
    delay(15);
    i = 0;
    menu_utama = true;
  }
}

void tambah_sidikjari() {
  eadd = EEPROM.read(10);
  delay(200);
  eadd += 1;
  if (eadd > 50) {
    eadd = 0;
  }
  EEPROM.write(10, eadd);
  getFingerprintEnroll(eadd);
}

void hapus_sidikjari() {
  finger.emptyDatabase();
  eadd = 0;
  EEPROM.write(10, eadd);
  lcd.clear();
  delay(15);
  lcd.setCursor(3, 0);
  lcd.print("Sidik Jari");
  lcd.setCursor(1, 1);
  lcd.print("Telah Dihapus");
  delay(2500);
  lcd.clear();
  delay (15);
  menu_utama = true;
}
