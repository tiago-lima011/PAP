#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#define SS_PIN 9
#define RST_PIN 8
#define rele 44
#define interruptor 3
#define displaylight 5
int estadorele = 0;
int estadointerruptor = 0;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Criar instancia MFRC522

LiquidCrystal lcd(22,23,24,25,26,27);

void setup() 
{
  pinMode(rele, OUTPUT);
  pinMode(interruptor, INPUT);
  pinMode(displaylight, OUTPUT);
  digitalWrite(displaylight, HIGH);
  digitalWrite(rele, LOW);
  Serial.begin(9600);   // Inicia a serial comunicaçao
  lcd.begin(16,2);
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  lcd.clear();
  Serial.println("Aproxime o cartão...");
  Serial.println();
  digitalWrite(rele, LOW);
  lcd.setCursor(0,0);
  lcd.print("Aproxime cartao");
  digitalWrite(displaylight, LOW);
}

void loop() 
{
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
  lcd.setCursor(0,0);
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
  String content= "";
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
  if (content.substring(1) == "96 E3 C8 73") //UID Tags de acesso
  {
    digitalWrite(displaylight, HIGH); 
    digitalWrite(rele, HIGH);
    Serial.println("Acesso Autorizado");
    Serial.println();
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Cartao");
    lcd.setCursor(5,1);
    lcd.print("Valido");
    delay(1000);
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Acesso");
    lcd.setCursor(3,1);
    lcd.print("Autorizado");
    delay(3000);
    lcd.clear();
    digitalWrite(rele, LOW);
    setup();
    
  }
 else   {
    digitalWrite(displaylight, HIGH); 
    Serial.println("Acesso Recusado");
    digitalWrite(rele, LOW);
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Cartao");
    lcd.setCursor(3,1);
    lcd.print("Invalido");
    delay(1000);
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Acesso");
    lcd.setCursor(4,1);
    lcd.print("Recusado");
    delay(3000);
    lcd.clear();
    setup();
  }
}
