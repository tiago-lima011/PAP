#include <Password.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#define SS_PIN 9     //SDA Arduino
#define RST_PIN 8   //RST Arduino
#define rele 44    //rele ligado ao pino 4 Arduino
#define led 45    //led ligado ao pino 45 Arduino
#define interruptor 3
#define displaylight 5
int estadointerruptor = 0; //define o estado do interruptor em 0

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Criar instancia MFRC522

LiquidCrystal lcd(22, 23, 24, 25, 26, 27, 28); //Definir os pinos RS,RW,E,D4,D5,D6,D7 do display ao Arduino correspondentes

char newPassword[4]; //charater string of newPasswordString
byte maxPasswordLength = 4; //valor maximo de carateres PASSWORD
byte currentPasswordLength = 0;
byte data_count = 0;
char Data[4]; //numero de carateres PASSWORD

Password password = Password("4528"); //Password correspondente abrir porta
int dlugosc = 4; //Password com 4 carateres
int ilosc; //
//Definicao da quantidade de linhas e colunas
const byte ROWS = 4; //4 Linhas
const byte COLS = 3; //3 Colunas

//Matriz de carateres
char keys[ROWS][COLS] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

//Definicao dos pinos das linhas
byte rowPins[ROWS] = {34, 35, 36, 37}; //Linhas
byte colPins[COLS] = {38, 39, 40};  // Colunas
byte currentLength = 0;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  Serial.begin(9600);   // Inicia a serial comunicaçao
  lcd.begin(16, 2); //Inicio o display
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init(); //Inicia o RFID MFRC522
  keypad.addEventListener(keypadEvent);
  pinMode(led, OUTPUT); //led como saida
  pinMode(rele, OUTPUT); 
  pinMode(displaylight, OUTPUT);
  digitalWrite(displaylight, HIGH);
  digitalWrite(rele, LOW);
  digitalWrite(led, LOW);
  Serial.println("Aproxime o cartão...");
  Serial.println();
  digitalWrite(rele, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Aproxime cartao");
  lcd.setCursor(1, 1);
  lcd.print("Ou Digite PIN");
  digitalWrite(displaylight, LOW);
}

void loop()
{
  keypad.getKey();
  estadointerruptor = digitalRead(interruptor);
  if(estadointerruptor == 1){   
    lcd.clear();
    digitalWrite(displaylight, HIGH); 
    digitalWrite(rele, HIGH);
    Serial.println("Porta aberta");
    lcd.print("Porta aberta");
    delay(3000);
    digitalWrite(rele, LOW);
    setup();
  }
  else{
    digitalWrite(rele, LOW);
  } 
  lcd.setCursor(0, 0);
  lcd.leftToRight();
  // novos cartoes
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Mostrar UID no serial monitor
  Serial.print("UID tag : ");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "46 D6 C3 93" || content.substring(1) == "46 CD C7 93") //UID Tags de acesso para a fechadura eletrica magnetica
  {
    digitalWrite(displaylight, HIGH); 
    digitalWrite(rele, HIGH);
    digitalWrite(led, HIGH);
    Serial.println("Acesso Autorizado");
    Serial.println();
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Cartao");
    lcd.setCursor(5, 1);
    lcd.print("Valido");
    delay(2000);
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Acesso");
    lcd.setCursor(3, 1);
    lcd.print("Autorizado");
    delay(3000);
    lcd.clear();
    setup();
  }
  else   {
    digitalWrite(displaylight, HIGH); 
    Serial.println("Acesso Recusado");
    digitalWrite(rele, LOW);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Cartao");
    lcd.setCursor(3, 1);
    lcd.print("Invalido");
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Acesso");
    lcd.setCursor(4, 1);
    lcd.print("Recusado");
    delay(3000);
    lcd.clear();
  }
  //Para cartao  Master
  if (content.substring(1) == "59 BF 8B AB") //ID Cartao Master
  {
    digitalWrite(displaylight, HIGH); 
    Serial.println("Area de configuracao");
    Serial.println();
    lcd.setCursor(5, 0);
    lcd.print("Area de");
    lcd.setCursor(2, 1);
    lcd.print("Configuracao");
    menu();
  }
}
void menu()
{
  digitalWrite(displaylight, HIGH); 
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Menu de");
  lcd.setCursor(0, 1);
  lcd.print("Programacao");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Configuracao");
  lcd.clear();
}

void keypadEvent(KeypadEvent eKey)
{
  switch (keypad.getState())
  {
    case PRESSED:
      int i;
      for ( i = 1; i <= 1; i++ )
      {
        digitalWrite(led, HIGH);
        delay(100);
        digitalWrite(led, LOW);
      }
      Serial.print("Pressed: ");
      Serial.println(eKey);
      
      switch (eKey)
      {
          case '#':
          menu();
          break;

          case '*':
          setup();
          break;

        default:
          ilosc = ilosc + 1;
          password.append(eKey);
      }
      //Serial.println(ilosc);

      if (ilosc == 1)
      {
        digitalWrite(displaylight, HIGH); 
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("< PIN >");
        lcd.setCursor(0, 1);
        lcd.print("*_");        
      }
      if (ilosc == 2)
      {
        lcd.noBlink();
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("< PIN >");
        lcd.setCursor(0, 1);
        lcd.print("**_");
      }
      if (ilosc == 3)
      {
        lcd.noBlink();
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("< PIN >");
        lcd.setCursor(0, 1);
        lcd.print("***_");
      }
      if (ilosc == 4)
      {
        lcd.noBlink();
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("< PIN >");
        lcd.setCursor(0, 1);
        lcd.print("****");
      }

      if (ilosc == dlugosc)
      {
        delay(50);
        checkPassword();
        ilosc = 0;
      }
 }
}

void checkPassword()
{
  if (password.evaluate())
  {
    int i;
    for ( i = 1; i <= 3; i++ )
    {
      digitalWrite(led, HIGH);
      delay(100);
      digitalWrite(led, LOW);
    }
    ilosc = 0;
    password.reset();
    Serial.println("Correto");
    digitalWrite(displaylight, HIGH); 
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("<<PIN CORRETO>>");
    digitalWrite(rele, HIGH);
    delay(3000);
    digitalWrite(rele, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Bem vindo");
    lcd.setCursor(0, 1);
    lcd.print("FAVOR Digite PIN");
    setup();
  }
  else
  {
    int i;
    for ( i = 1; i <= 1; i++ )
    {
      digitalWrite(led, HIGH);
      delay(100);
      digitalWrite(led, LOW);
      delay(100);
    }
    ilosc = 0;
    password.reset();
    Serial.println("Error");
    digitalWrite(led, HIGH);
    delay(400);
    digitalWrite(led, LOW);
    digitalWrite(led, HIGH);
    delay(400);
    digitalWrite(led, LOW);
    digitalWrite(displaylight, HIGH); 
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("PIN INCORRETO");
    delay(2000);
    lcd.clear();
    setup();
  }
}
