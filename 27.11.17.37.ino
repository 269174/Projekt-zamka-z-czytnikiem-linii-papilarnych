//Biblioteki
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <LCDMenuLib.h>
#include <EEPROM.h>

//Definicje z WaveShare
#define ACK_SUCCESS     0x00
#define ACK_FAIL        0x01
#define ACK_FULL        0x04
#define ACK_NOUSER      0x05
#define ACK_USER_EXIST  0x06
#define ACK_FIN_EXIST   0x07
#define ACK_TIMEOUT     0x08
enum pageType {ROOT_MENU, SUB_MENU1, SUB_MENU2, SUB_MENU3};
enum pageType currPage = ROOT_MENU;

//Deklaracje
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // 0x27 - adres I2C, 16x2 - rozmiar wyświetlacza


// Definiowanie menu

bool isUserExisting[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
String UserNames[16] = {"ADMIN", "USER_A", "USER_B", "USER_C", "USER_D", "USER_E", "USER_F", "USER_G", "USER_H", "USER_I", "USER_J", "USER_K", "USER_L", "USER_M", "USER_N", "USER_O"};

uint8_t response[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const int buttonUp = 4;
const int buttonDown = 5;
const int buttonSelect = 6;
const int buttonUndo = 7;

int currentIndex = 0;
int number_of_tries = 0;

void setup()
{
  //Komunikacja serialowa
  delay(1500);            // arduino reset delay
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(19200);

  //Włączenie LCD
  delay(500);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(500);

  //Przyciski
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buttonSelect, INPUT_PULLUP);
  pinMode(buttonUndo, INPUT_PULLUP);

  //Dioda
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  //moduł sim900a mini 3.8.2
  Serial1.write("AT+CFUN=1,1\r");
  delay(1500);
  Serial1.write("AT+CREG=1\r");
  delay(1500);

  //zerowanie odpowiedzi
  clearResponse();
  loadFromEEPROM();
}

// ==============================================================================================
// ||                                   MAIN LOOP                                              ||
// ==============================================================================================
void loop()
{
  switch(currPage){
    case ROOT_MENU:   Serial.println("Wchodzimy w root menu");  break;
    case SUB_MENU1:   Serial.println("Wchodzimy submenu1");  break;
    case SUB_MENU2:   Serial.println("Wchodzimy submmenu2");  break;
    case SUB_MENU3:   Serial.println("Wchodzimy submenu3");  break;
  }
  switch(currPage){
    case ROOT_MENU:   page_RootMenu();  break;
    case SUB_MENU1:   page_SubMenu1();  break;
    case SUB_MENU2:   page_SubMenu2();  break;      //panel admina
    case SUB_MENU3:   page_SubMenu3();  break;      //przeglądanie użytkowników
  }
}
// ==============================================================================================
// ||                                   PAGE ROOT MENU                                            ||
// ==============================================================================================

void page_RootMenu(){
  bool flag = true;
  uint32_t loopStartMs;
  int RM_index = 0;
  unsigned long lastDebounceTimeUp = 0;
  unsigned long lastDebounceTimeDown = 0;
  unsigned long lastDebounceTimeSelect = 0;
  unsigned long debounceDelay = 200;
  int buttonUpint;
  int buttonDownint;
  int buttonSelectint;
  while(true){
    loopStartMs = millis();
    buttonUpint = digitalRead(buttonUp);
    buttonDownint = digitalRead(buttonDown);
    buttonSelectint = digitalRead(buttonSelect);
    updateLCD_RM(RM_index, flag);
    handleButtonNavigation(RM_index, lastDebounceTimeUp, lastDebounceTimeDown, lastDebounceTimeSelect, debounceDelay, buttonUpint, buttonDownint, buttonSelectint, flag);
    if(!currPage == ROOT_MENU) {break;}
    while(millis() - loopStartMs < 100);
  }
}



// ==============================================================================================
// ||                                   PAGE SUB MENU1                                          ||
// ==============================================================================================

void page_SubMenu1(){
  
}
// ==============================================================================================
// ||                                   PAGE SUB MENU2                                          ||
// ==============================================================================================


void page_SubMenu2(){
  Serial.print("Wszedlem w submenu2");
  bool flag_s2 = true;
  int S2_index = 0;
  int buttonUpint_s2;
  int buttonDownint_s2;
  int buttonSelectint_s2;
  int buttonUndoint_s2;
  unsigned long lastDebounceTimeUp_s2 = 0;
  unsigned long lastDebounceTimeDown_s2 = 0;
  unsigned long lastDebounceTimeSelect_s2 = 0;
  unsigned long debounceDelay_s2 = 200;
  uint32_t loopStartMs_s2;
  while(true){
    loopStartMs_s2 = millis();
    buttonUpint_s2 = digitalRead(buttonUp);
    buttonDownint_s2 = digitalRead(buttonDown);
    buttonSelectint_s2 = digitalRead(buttonSelect);
    buttonUndoint_s2 = digitalRead(buttonUndo);
    updateLCD_S2(S2_index, flag_s2);
    handleButtonNavigation_S2(S2_index, lastDebounceTimeUp_s2, lastDebounceTimeDown_s2, lastDebounceTimeSelect_s2, debounceDelay_s2, buttonUpint_s2, buttonDownint_s2, buttonSelectint_s2, buttonUndoint_s2, flag_s2);
    if(currPage != SUB_MENU2) {break;}
    while(millis() - loopStartMs_s2 < 100);
  }
}


// ==============================================================================================
// ||                                   PAGE SUB MENU3                                          ||
// ==============================================================================================

void page_SubMenu3(){
  bool flag_s1 = true;
  int S1_index = 0;
  int buttonUpint_s1;
  int buttonDownint_s1;
  int buttonSelectint_s1;
  int buttonUndoint_s1;
  unsigned long lastDebounceTimeUp_s1 = 0;
  unsigned long lastDebounceTimeDown_s1 = 0;
  unsigned long lastDebounceTimeSelect_s1 = 0;
  unsigned long debounceDelay_s1 = 200;
  uint32_t loopStartMs_s1;
  while(true){
    loopStartMs_s1 = millis();
    buttonUpint_s1 = digitalRead(buttonUp);
    buttonDownint_s1 = digitalRead(buttonDown);
    buttonSelectint_s1 = digitalRead(buttonSelect);
    buttonUndoint_s1 = digitalRead(buttonUndo);
    updateLCD_S1(S1_index, flag_s1);
    handleButtonNavigation_S1(S1_index, lastDebounceTimeUp_s1, lastDebounceTimeDown_s1, lastDebounceTimeSelect_s1, debounceDelay_s1, buttonUpint_s1, buttonDownint_s1, buttonSelectint_s1, buttonUndoint_s1, flag_s1);
    if(currPage != SUB_MENU3) {break;}
    while(millis() - loopStartMs_s1 < 100);
  }
}

// ==============================================================================================
// ||                                   FUNC DEFINITIONS                                        ||
// ==============================================================================================
void handleButtonNavigation(int &RM_index, unsigned long &lastDebounceTimeUp, unsigned long &lastDebounceTimeDown, unsigned long &lastDebounceTimeSelect, unsigned long debounceDelay, int &buttonUpint, int &buttonDownint, int &buttonSelectint, bool &flag){
  if (buttonUpint == LOW && (millis() - lastDebounceTimeUp) > debounceDelay) {
    lastDebounceTimeUp = millis();
    RM_index--;
    if (RM_index < 0) {
      RM_index = 3; 
    }
    flag = true;
    return;
  }
  if (buttonDownint == LOW && (millis() - lastDebounceTimeDown) > debounceDelay) {
    lastDebounceTimeDown = millis();
    RM_index++;
    if (RM_index > 3) {  
      RM_index = 0;
    }    
    flag = true;       
    return;
  }
  if(buttonSelectint == LOW && (millis() - lastDebounceTimeSelect) > debounceDelay){
    switch(RM_index){
      case 0: logging(); break;
      case 1: addingUserProcess();  Serial.println("Jesteśmy tu 1"); break;
      case 2: deletingProcess();    Serial.println("Jesteśmy tu 2"); break;
      case 3: currPage = SUB_MENU3; 
      Serial.println("Jesteśmy tu 3"); 

      
      if(isUserExisting[0] == true){
        lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan your finger");
        compareFingerprint();
        waitForCompleteResponse();
        Serial.println("pokazywanie uzytkownika debug2");
        if(response[3] != 0x0 || response[4] != 0x3){
          Serial.println("pokazywanie uzytkownika debug");
          clearResponse(); 
          currPage = ROOT_MENU;
          lcd.clear(); lcd.setCursor(0, 0); lcd.print("Wrong finger");
          delay(3000);
          break;
        }
      clearResponse(); 
      }
      else{
        lcd.clear(); lcd.setCursor(0, 0); lcd.print("Set admin first");
        currPage = ROOT_MENU; 
        delay(3000);
        break;
      }
      

      break;
    }
    flag = true;
  }
}

void updateLCD_RM(int &RM_index, bool &flag){
  if(!flag){
    return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  switch(RM_index){
        case 0:
          lcd.print("Unlock");
          break;
        case 1:
          lcd.print("Add new user"); 
          break;
        case 2:
          lcd.print("Delete user");
          break;
        case 3: 
          lcd.print("Show users");
          break;
      }
  lcd.setCursor(0, 1);
  lcd.print("<");
  flag = false;
}

void handleButtonNavigation_S1(int &S1_index, unsigned long &lastDebounceTimeUp_s1, unsigned long &lastDebounceTimeDown_s1, unsigned long &lastDebounceTimeSelect_s1, unsigned long debounceDelay_s1, int &buttonUpint_s1, int &buttonDownint_s1, int &buttonSelectint_s1, int &buttonUndoint_s1, bool &flag_s1){
  if (buttonUpint_s1 == LOW && (millis() - lastDebounceTimeUp_s1) > debounceDelay_s1) {
    lastDebounceTimeUp_s1 = millis();
    S1_index--;
    if (S1_index < 0) {
      S1_index = 15; 
    }
    flag_s1 = true;
    return;
  }
  if (buttonDownint_s1 == LOW && (millis() - lastDebounceTimeDown_s1) > debounceDelay_s1) {
    lastDebounceTimeDown_s1 = millis();
    S1_index++;
    if (S1_index > 15) {  
      S1_index = 0;
    }    
    flag_s1 = true;       
    return;
  }
  if (buttonUndoint_s1 == LOW && (millis() - lastDebounceTimeDown_s1) > debounceDelay_s1){
    lastDebounceTimeDown_s1 = millis();
    currPage = ROOT_MENU;
    flag_s1 = true;
    return;
  }
}

void handleButtonNavigation_S2(int &S2_index, unsigned long &lastDebounceTimeUp_s2, unsigned long &lastDebounceTimeDown_s2, unsigned long &lastDebounceTimeSelect_s2, unsigned long debounceDelay_s2, int &buttonUpint_s2, int &buttonDownint_s2, int &buttonSelectint_s2, int &buttonUndoint_s2, bool &flag_s2){
  if (buttonUpint_s2 == LOW && (millis() - lastDebounceTimeUp_s2) > debounceDelay_s2) {
    lastDebounceTimeUp_s2 = millis();
    S2_index--;
    if (S2_index < 0) {
      S2_index = 16; 
    }
    flag_s2 = true;
    return;
  }
  if (buttonDownint_s2 == LOW && (millis() - lastDebounceTimeDown_s2) > debounceDelay_s2) {
    lastDebounceTimeDown_s2 = millis();
    S2_index++;
    if (S2_index > 16) {  
      S2_index = 0;
    }    
    flag_s2 = true;       
    return;
  }
  if (buttonUndoint_s2 == LOW && (millis() - lastDebounceTimeDown_s2) > debounceDelay_s2){
    lastDebounceTimeDown_s2 = millis();
    currPage = ROOT_MENU;
    flag_s2 = true;
    return;
  }

  if(buttonSelectint_s2 == LOW && (millis() - lastDebounceTimeUp_s2) > debounceDelay_s2){
    lastDebounceTimeDown_s2 = millis();
    switch(S2_index){
      case 16:
        deleteAllFingerprints();
        waitForCompleteResponse();
        if(response[4] == ACK_SUCCESS){
          lcd.clear(); lcd.setCursor(0, 0); lcd.print("All users are"); lcd.setCursor(0, 1); lcd.print("deleted!"); 
          delay(1500);
          for(int i = 0; i < 16; i++){
            isUserExisting[i] = false;
          }
        }
        lcd.clear();
        clearResponse();
        saveToEEPROM();
        currPage = ROOT_MENU;
        break;
      default:
        if(isUserExisting[S2_index] == true){  
          uint8_t ID = S2_index;
          uint8_t ID2 = 0xF0 | S2_index;
          deleteFingerprint(ID);
          waitForCompleteResponse();
          deleteFingerprint(ID2);
          waitForCompleteResponse();
          lcd.clear(); lcd.setCursor(0, 0); lcd.print(UserNames[S2_index]); lcd.print(" is"); lcd.setCursor(0, 1); lcd.print("deleted!");
          isUserExisting[S2_index] = false;
          saveToEEPROM();
          currPage = ROOT_MENU;
          delay(3000);
        }
        break;
    }
    flag_s2 = true;
    }
  }


void addingUserProcess(){
 uint8_t CMD;
 uint8_t CHK;
 uint8_t ID;
 uint8_t ID2;
 uint8_t privilege;
 clearResponse(); 
  //sprawdzenie czy baza jest pusta, rozpoczęcie drzewka



  if(isUserExisting[0] == true){
    lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan your finger");
    compareFingerprint();
    waitForCompleteResponse();
    Serial.println("dodawanie uzytkownika debug2");
    if(response[3] != 0x0 || response[4] != 0x3){
      Serial.println("dodawanie uzytkownika debug");
      clearResponse(); 
      currPage = ROOT_MENU;
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("Wrong finger");
      delay(3000);
      lcd.clear();
      return;
    }
  clearResponse(); 
  }
  switch(isDatabaseEmpty()){
    case true:
      ID = 0x00;
      ID2 = 0xF0;
      privilege = 3;
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("Setting admin");
      delay(3000);
      break;

    case false:
      if(isUserExisting[0] == false){
        ID = 0x00;
        ID2 = 0xF0;
        privilege = 3;
        lcd.clear(); lcd.setCursor(0, 0); lcd.print("Setting admin");
        delay(3000);
        break;
      }
      switch(isDatabasefull()){
        case true:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Full Database");
          currPage = ROOT_MENU;
          delay(3000);
          return;
        case false:
          ID = 0x00 | firstFreeSlot();
          ID2 = 0xF0 | firstFreeSlot();
          Serial.print(ID); Serial.print(" "); Serial.print(ID2);
          privilege = 1;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Setting user");
          delay(3000);
          break;
      }
  }
  //wysłanie 1 (1/3)
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan 1st finger"); lcd.setCursor(0, 1); lcd.print("1/3"); 
  add_sendCMD(0x01, ID, privilege);
  waitForCompleteResponse();
  if(response[4] != ACK_SUCCESS){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Try again!");
    currPage = ROOT_MENU;
    delay(3000);
    return;
  }
  delay(3000);

  //wysłanie 1 (2/3)
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan 1st finger"); lcd.setCursor(0, 1); lcd.print("2/3"); 
  add_sendCMD(0x02, ID, privilege);
  waitForCompleteResponse();
  if(response[4] != ACK_SUCCESS){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Try again!");
    currPage = ROOT_MENU;
    delay(3000);
    return;
  }
  delay(3000);

  //wysłanie 1 (3/3)
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan 1st finger"); lcd.setCursor(0, 1); lcd.print("3/3"); 
  add_sendCMD(0x03, ID, privilege);
  waitForCompleteResponse();
  if(response[4] != ACK_SUCCESS){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Try again!");
    currPage = ROOT_MENU;
    delay(3000);
    return;
  }
  delay(3000);

  //wysłanie 2 (1/3)
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan 2nd finger"); lcd.setCursor(0, 1); lcd.print("1/3"); 
  add_sendCMD(0x01, ID2, privilege);
  waitForCompleteResponse();
  if(response[4] != ACK_SUCCESS){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Try again!");
    currPage = ROOT_MENU;
    deleteFingerprint(ID);
    delay(3000);
    return;
  }
  delay(3000);

  //wysłanie 2 (2/3)
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan 2nd finger"); lcd.setCursor(0, 1); lcd.print("2/3"); 
  add_sendCMD(0x02, ID2, privilege);
  waitForCompleteResponse();
  if(response[4] != ACK_SUCCESS){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Try again!");
    currPage = ROOT_MENU;
    deleteFingerprint(ID);
    delay(3000);
    return;
  }
  delay(3000);

  //wysłanie 2 (3/3)
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan 2nd finger"); lcd.setCursor(0, 1); lcd.print("3/3"); 
  add_sendCMD(0x03, ID2, privilege);
  waitForCompleteResponse();
  if(response[4] != ACK_SUCCESS){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Try again!");
    currPage = ROOT_MENU;
    deleteFingerprint(ID);
    delay(3000);
    return;
  }
  delay(3000);


  lcd.clear(); lcd.setCursor(0, 0);
  if(ID == 0x00){
    lcd.print("Admin added");
    lcd.setCursor(0, 1);
    lcd.print("Number: "); lcd.print(firstFreeSlot() + 1); 
  } 
  else{
    lcd.print(UserNames[firstFreeSlot()]); lcd.print(" added");
    lcd.setCursor(0, 1);
    lcd.print("Number: "); lcd.print(firstFreeSlot() + 1);
  }
  isUserExisting[firstFreeSlot()] = true;
  saveToEEPROM();
  currPage = ROOT_MENU;
  clearResponse();
  delay(3000);
  return;

}

void deletingProcess(){
  clearResponse();
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan your finger"); lcd.setCursor(0, 1); lcd.print(">");
  switch(isDatabaseEmpty()){
    case true:
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("Nothing to"); lcd.setCursor(0, 1); lcd.print("delete!");
      currPage = ROOT_MENU;
      delay(3000);
      return;
    case false:
      break;
  }
  compareFingerprint();
  waitForCompleteResponse();
  if(response[4] == ACK_NOUSER || response[4] == ACK_TIMEOUT){
    lcd.clear(); lcd.setCursor(0, 0); lcd.print("Try again!");
    currPage = ROOT_MENU;
    delay(3000);
    for(int i = 0; i < 8; i++){
    response[i] = 0x00;
    }
    return;
  }
  switch(response[3]){
    case 0x00:
      currPage = SUB_MENU2;
      return;
      break;
    default:
      uint8_t ID = response[3];
      uint8_t ID2 = 0xF0 | response[3];
      deleteFingerprint(ID);
      waitForCompleteResponse();
      deleteFingerprint(ID2);
      waitForCompleteResponse();
      isUserExisting[ID] = false;
      saveToEEPROM();
      lcd.clear(); lcd.setCursor(0, 0); lcd.print("ID: 0x0"); lcd.print(ID); lcd.print(" ("); lcd.print(ID+1); lcd.print(")"); lcd.setCursor(0, 1); lcd.print("Deleted!");
      delay(3000);
      lcd.clear();
      currPage = ROOT_MENU;
      return;
  }
}

void updateLCD_S2(int &S2_index, bool &flag_s2){
    if(!flag_s2){
    return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  switch(S2_index){
    case 16:
      lcd.print("Delete all"); lcd.setCursor(0, 1); lcd.print(">          users");
      break;
    default:
      printInS1Names(S2_index);
  }
  flag_s2 = false;
}

void updateLCD_S1(int &S1_index, bool &flag_s1){
  if(!flag_s1){
    return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  printInS1Names(S1_index);
  flag_s1 = false;
}

void printInS1Names(int S1_index){
          lcd.print(S1_index + 1);
          lcd.print(". "); 
          if(isUserExisting[S1_index] == false){       
            lcd.print("<free slot>");
          }
          else{
            lcd.print(UserNames[S1_index]);
          }
          lcd.setCursor(0, 1);
          lcd.print(">");
}

void add_sendCMD(uint8_t CMD, uint8_t ID, uint8_t privilege){
  uint8_t CHK = CMD ^ ID ^ privilege;
  Serial2.write(0xF5);
  Serial2.write(CMD);
  Serial2.write(0x00);
  Serial2.write(ID);
  Serial2.write(privilege);
  Serial2.write(0x00);
  Serial2.write(CHK);
  Serial2.write(0xF5);
}


void waitForCompleteResponse() {
  while (Serial2.available() < 8) {  // Czekamy, aż będzie dostępnych 8 bajtów
    delay(10);  // Czekaj chwilę, aby nie blokować programu
  }
  
  // Teraz odczytujemy pełną odpowiedź
  while (Serial2.available() >= 8) {
    for (int i = 0; i < 8; i++) {
      uint8_t byteReceived = Serial2.read();
      response[i] = byteReceived;
      Serial.print("Otrzymano: 0x");
      Serial.println(byteReceived, HEX);
    }
  }
}

bool isDatabaseEmpty(){
  uint8_t test = 0;
  for(int i = 0; i < 16; i++){
    if (isUserExisting[i] == false){
      test++;
    }
  }
  if(test == 16){
    return true;
  }
  else{
    return false;
  }
}

bool isDatabasefull(){
  uint8_t test = 0;
  for(int i = 0; i < 16; i++){
    if (isUserExisting[i] == true){
      test++;
    }
  }
  if(test == 16){
    return true;
  }
  else{
    return false;
  }
}

uint8_t firstFreeSlot(){
  for(int i = 0; i < 16; i++){
    if (isUserExisting[i] == false){
      return i;
    }
  }
}

void deleteFingerprint(uint8_t ID){
  uint8_t CHK = 0x04 ^ ID; 
  Serial2.write(0xF5);
  Serial2.write(0x04);
  Serial2.write(0x00);
  Serial2.write(ID);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(CHK);
  Serial2.write(0xF5);
}

void deleteAllFingerprints(){
  Serial2.write(0xF5);
  Serial2.write(0x05);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(0x00);
  Serial2.write(0x05);
  Serial2.write(0xF5);
}

void compareFingerprint(){
    Serial2.write(0xF5);
    Serial2.write(0x0C);
    Serial2.write(0x00);
    Serial2.write(0x00);
    Serial2.write(0x00);
    Serial2.write(0x00);
    Serial2.write(0x0C);
    Serial2.write(0xF5);
}

void logging(){
  clearResponse();
  if(isDatabaseEmpty() == true){
    lcd.clear(); lcd.setCursor(0, 0); lcd.print("No user in"); lcd.setCursor(0, 1); lcd.print("Database!"); 
    delay(3000);
    return;
  }
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Scan your finger"); 
  compareFingerprint(); 
  waitForCompleteResponse();
  if(response[3] <= 0x0F && (response[4] == 1 || response[4] == 3)){
    openLock();
    number_of_tries = 0;
  }
  else if(response[3] >= 0xF0 && (response[4] == 1 || response[4] == 3)){
    openLock();
    sendSMS(response[3]);
    number_of_tries = 0;
  }
  else if(response[4] == ACK_NOUSER){
    number_of_tries++;
    lcd.clear(); lcd.setCursor(0, 0); lcd.print("No of tries: "); lcd.print(number_of_tries);
    Serial.println(number_of_tries);
    if(number_of_tries >= 3){
      sendSMS("Wystapil incydent, ktos nieautoryzowany probuje otworzyc zamek");
    }
  }
  else if(response[4] == ACK_TIMEOUT){
    lcd.clear(); lcd.setCursor(0, 0); lcd.print("Timeout");
  }
  delay(3000);
  lcd.clear();
  clearResponse();
  digitalWrite(13, LOW);
}

void clearResponse(){
  for(int i = 0; i < 8; i++){
    response[i] = 0x00;
  }
}

void openLock(){
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("UNLOCKED"); 
  digitalWrite(13, HIGH);
  delay(3000);
}

void sendSMS(){
  lcd.clear(); lcd.setCursor(0, 1); lcd.print("SMS SENT!"); 
  delay(3000);
}


void saveToEEPROM(){
  for(int i = 0; i < 16; i++){
    EEPROM.update(i, isUserExisting[i]);
  }
}


void loadFromEEPROM(){
  for(int i = 0; i < 16; i++){
    isUserExisting[i] = EEPROM.read(i);
  }
}

void sendSMS(String number, String message) {
  Serial1.write("AT+CMGS=\"+48515001644\"\r");
  delay(1000);
  Serial1.write("Incydent"); 
  Serial1.write(26);          
  delay(3000);
  readMessage();
}

void sendSMS(String message) {
  String mess = message;
  Serial1.write("AT+CMGS=\"+48515001644\"\r");
  delay(1000);
  Serial1.write(mess.c_str()); 
  Serial1.write(26);          
  delay(3000);
  readMessage();
}

void sendSMS(uint8_t hexValue) {
  uint8_t Value = hexValue - 0xF0;
  String string = UserNames[Value];


  Serial1.write("AT+CMGS=\"+48515001644\"\r");
  delay(1000);
  Serial1.write("Wystapil incydent, ");  Serial1.write(string.c_str()); Serial1.write(" uzyl alarmowego palca."); 
  Serial1.write(26);          
  delay(3000);
  readMessage();
}

void readMessage() {
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}


// void switchsendSMS(int number){
//   switch(number)

// }
