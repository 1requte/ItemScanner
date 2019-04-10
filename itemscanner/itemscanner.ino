#include "SPI.h"
#include "MFRC522.h"

#define SS_PIN 10
#define RST_PIN 9
#define SP_PIN 8

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

String items[12][3] = {
  "C0:04:A3:83",    "Test", 
                    "",
                    
  "26:68:4B:12",    "Appelsin", 
                    "5kr",
                    
  "7F:EB:4C:29",    "Æble", 
                    "10kr",
                    
  "4F:6A:D1:29",    "Pære", 
                    "12,5kr",
                    
  "3F:C8:CE:29",    "Agurk", 
                    "5kr",
                    
  "4F:0E:B2:29",    "Ananas", 
                    "20kr",
                    
  "3F:B3:F2:29",    "Mælk", 
                    "21kr",
                    
  "4F:14:FC:29",    "Øko mælk", 
                    "40kr",
                    
  "4F:13:C9:29",    "", 
                    "",
                    
  "4F:07:B3:29",    "", 
                    "",
                    
  "3F:9C:43:29",    "", 
                    "",
                    
  "3F:F3:B2:29",    "", 
                    ""
};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Error: Not the right type"));
    return;
  }

  String tagID = "";
  for (byte i = 0; i < 4; i++) {
    tagID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  tagID.toUpperCase();
  for(int i = 0; i < 12; i++) {
    if(tagID.equals(items[i][0])) {
      String itmName = "";
      String itmPrice = "";
      if(items[i][1].equals("")) {
        itmName = "Unknown item";
      } else {
        itmName = items[i][1];
      }
      if(items[i][2].equals("")) {
        itmPrice = "No defined price";
      } else {
        itmPrice = items[i][2];
      }
      Serial.println(itmName + " - " + itmPrice);
    }
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
