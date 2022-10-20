/*

   All the resources for this project: https://www.hackster.io/Aritro
   Modified by Aritro Mukherjee


*/

#include <SPI.h>
#include <MFRC522.h>


#define SS_1_PIN 53
#define SS_2_PIN 7
#define RST_PIN 5  // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1

MFRC522 mfrc522A(SS_1_PIN, RST_PIN), mfrc522B(SS_2_PIN, RST_PIN);  // Create MFRC522 instance.

String saved[] = {
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
};
String x = "";


void setup() {
  Serial.begin(9600);  // Initiate a serial communication
  while (!Serial)
    ;  // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();  // Init SPI bus
  // READER A
  mfrc522A.PCD_Init();
  Serial.print(F("Reader A "));
  Serial.print(F(": "));
  mfrc522A.PCD_DumpVersionToSerial();
  // READER B
  mfrc522B.PCD_Init();
  Serial.print(F("Reader B "));
  Serial.print(F(": "));
  mfrc522B.PCD_DumpVersionToSerial();

  delay(100);
}


void loop() {
  // Look for new cards
  if (mfrc522A.PICC_IsNewCardPresent() && mfrc522A.PICC_ReadCardSerial()) {
    String x = "";
    //Jika reader A mendeteksi
    Serial.print(F("Reader A"));
    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F(": Card UID: "));

    x = getHex(mfrc522A.uid.uidByte, mfrc522A.uid.size);
    Serial.println(x.substring(1));
    bool sudahAda = false;
    // Cek apakah ada kartu yg sama di sepanjang array
    for (int a = 0; a < 16;) {
      if (saved[a] == x.substring(1)) {
        Serial.println("Gausah Serakah lah ya...");
        sudahAda = true;
      }
      a++;
    }
    if (!sudahAda) {
      // Cari slot apakah ada yg kosong
      for (int a = 0; a < 16;) {
        if (saved[a] == "") {
          // Masukkan ID ke array
          saved[a] = x.substring(1);
          Serial.println("Masuk");
          // Untuk berhenti dari perulangan
          a = 99;
        } else {
          a++;
        }
      }
    }


    for (int a = 0; a < 16;) {
      Serial.print(a);
      Serial.print(" ");
      Serial.println(saved[a]);
      a++;
    }

    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522A.PICC_GetType(mfrc522A.uid.sak);
    Serial.println(mfrc522A.PICC_GetTypeName(piccType));
    // Halt PICC
    mfrc522A.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522A.PCD_StopCrypto1();
  }  //if (mfrc522A.PICC_IsNewC

  // Look for new cards
  if (mfrc522B.PICC_IsNewCardPresent() && mfrc522B.PICC_ReadCardSerial()) {
    String x = "";
    //Jika reader A mendeteksi
    Serial.print(F("Reader B"));
    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F(": Card UID: "));

    x = getHex(mfrc522B.uid.uidByte, mfrc522B.uid.size);
    Serial.println(x.substring(1));
    bool bukanCircle = true;
    for (int a = 0; a < 16;) {
      if (saved[a] == x.substring(1)) {
        saved[a] = "";
        Serial.println("Keluar");
        bukanCircle = false;
      }
      a++;
    }

    // Action Bukan Circle
    if (bukanCircle) {
      Serial.println("Kamu bukan circle sini ya...");
    }


    for (int a = 0; a < 16;) {
      Serial.print(a);
      Serial.print(" ");
      Serial.println(saved[a]);
      a++;
    }

    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522B.PICC_GetType(mfrc522B.uid.sak);
    Serial.println(mfrc522A.PICC_GetTypeName(piccType));
    // Halt PICC
    mfrc522B.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522B.PCD_StopCrypto1();
  }  //if (mfrc522A.PICC_IsNewC
}


String getHex(byte* buffer, byte bufferSize) {
  String content = "";
  for (byte i = 0; i < bufferSize; i++) {
    content.concat(String(buffer[i] < 0x10 ? " 0" : " "));
    content.concat(String(buffer[i], HEX));
  }
  content.toUpperCase();
  return content;
}