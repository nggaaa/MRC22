/*                                                                                       
                                                 ~~                                                 
                                           .?7. .@&. .7?.                                           
                                       .   .Y&#?^&&:?#&J    .                                       
                                      ?@7    .J&&@@&#?.    ?@7                                      
                                  ~7: :&&.     .?@@?.     :&&. :7~                                  
                                  7G#GJP@Y      .&&.      5@PJG#G7                                  
                                   .!5@@@@J^.   :&&.   .~Y@@@@5~.                                   
                                 ?BBBP5J77P##P7::&&.:?P##P77J5PBBB?                                 
                                 :^.       .~JG#B@@B#P?^.       :^:                                 
                                             ^J#@@@@BJ^                                             
                                 JBG5J7~:^?P##P7~&&^7P##P7::~7J5GB?                                 
                                 .^~?P@@&@GJ^.  .&&.  .~JG@&@@57~^.                                 
                                  ^JG#GB@G      .&&.      G@GGBGJ^                                  
                                  ?57:.B@^      ~@&^      ~@B.:757                                  
                                      ?@Y     ^P&@@&5^     5@7                                      
                                      :~.   !G@P!&&!G@P~   .~:                                      
                                           .P5^ .@&. ^PP.                                           
                                                 JJ                                                 
                                                                                                  
                     .GB#~ .J##P^ 5##?   P##7 ###BBB### ?##5   J##Y 7#BB###BP!                      
                     .&@@!^B@@5:  B@@J   B@@? ?7Y@@@J7? Y@@G   5@@P ?@@#   @@@:                     
                     .&@@B@@#^    B@@J   B@@?   ^@@&:   Y@@G   5@@P ?@@&55B@&?                      
                     .&@@P&@&?    G@@Y  .#@@?   ~@@&:   J@@B   P@@P ?@@#555@@G:                     
                     .&@@!:P@@#!  !@@@GP#@@B:   ~@@@:   ^#@@BPB@@&~ ?@@&   @@@!                     
                     .PGG~  7GGG!  ^JPBBG5!.    ^GGP:    .75GBG5?:  !GGGGGGPY~                  
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// RFID
#define SS_A_PIN 53  // SDA PIN MASUK
#define SS_B_PIN 7   // SDA PIN KELUAR
#define RST_PIN 5    // RST PIN
// OUTPUT
#define BUZZER_PIN 9        // BUZZER PIN
#define SERVO_MASUK_PIN 8   // SERVO MASUK PIN
#define SERVO_KELUAR_PIN 2  // SERVO MASUK PIN

// VARIABEL

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

// INIT LIBRARY
MFRC522 mfrc522A(SS_A_PIN, RST_PIN), mfrc522B(SS_B_PIN, RST_PIN);  // Create MFRC522 instance.
Servo masuk, keluar;

void setup() {
  Serial.begin(9600);  // Initiate a serial communication
  while (!Serial)
    ;           // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();  // Init SPI bus

  // SERVO
  masuk.attach(SERVO_MASUK_PIN);
  keluar.attach(SERVO_KELUAR_PIN);

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