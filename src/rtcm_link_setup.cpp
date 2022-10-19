
#include "system.h"
#include "rtcm_link.h"
#include "rtcm_link_settings.h"

//RTCM Radio Link
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
RTCM_Link radioLink;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//Radio Link ISRs
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void DIO0ISR(){
  radioLink.RxTxDoneISR();
}
void DIO1ISR(){
  radioLink.HopISR();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

//RTCM Radio Connections
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//radio TXEN = 41;
//radio RXEN = 40;
//radio MOSI = 11
//radio MISO = 12
//radio SCK  = 13
//radio CS   = 10;
//radio DIO0 = 3;   Triggered at the end of receive/transmit
//radio DIO1 = 4;   Triggered when it's time to hop frequencies
//radio RST  = 255;
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void beginRTCMLink(RTCMSettings settings) {
  Serial.println("Begining RTCM Link Initialization...");
  radioLink.attachPetWDT(petWDT);
  Serial.println("WDT attached");
  radioLink.setDIO0ISR(DIO0ISR);
  Serial.println("DIO0 ISR attached");
  radioLink.setDIO1ISR(DIO1ISR);
  Serial.println("DIO1 ISR attached");
  radioLink.begin(&settings, 41, 40, 10, 3, 4, 255);
  Serial.println("RTCM Link Initialized");
}

