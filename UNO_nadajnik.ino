/*  Joystick + nRF24L01+  ‑‑‑ Nadajnik
 *  ───────────────────────────────────
 *  Joystick:
 *     X  → A0
 *     Y  → A1
 *     SW → D7  (INPUT_PULLUP)
 *
 *  nRF24L01+ (zasilanie 3 V – 3,6 V!):
 *     GND  → GND
 *     VCC  → 3.3 V  (nie 5 V)
 *     CE   → D9
 *     CSN  → D10   (SS)
 *     SCK  → D13   (SCK)
 *     MOSI → D11   (MOSI)
 *     MISO → D12   (MISO)
 *  Zalecany kondensator 4 7 µF–10 µF między 3.3 V a GND przy samym module.
 */

#include <SPI.h>
#include <RF24.h>          // Biblioteka TMRh20

// ─── Piny ──────────────────────────────────────────────────────────
const int pinX       = A0;   // Oś X
const int pinY       = A1;   // Oś Y
const int pinButton  = 7;    // Przycisk (SW)

#define CE_PIN   9          // CE dla nRF24L01+
#define CSN_PIN 10          // CSN (CS) dla nRF24L01+

RF24 radio(CE_PIN, CSN_PIN);
const byte address[5] = "0002";     // 5‑znakowy identyfikator rury

// Struktura pakietu do wysłania
struct Payload {
  uint16_t x;   // 0‑1023
  uint16_t y;   // 0‑1023
};

void setup() {
  // ─── Joystick ───────────────────────────────────────────
  Serial.begin(9600);
  pinMode(pinButton, INPUT_PULLUP);

  // ─── nRF24L01+ ─────────────────────────────────────────
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);       // LOW lub MIN – mniej zakłóceń z bliska
  radio.setDataRate(RF24_250KBPS);     // 250 kb/s – największy zasięg
  radio.setChannel(2);               // np. 2.508 GHz (zakres 0‑125)
  radio.openWritingPipe(address);
  radio.enableDynamicPayloads();
  radio.stopListening();               // Nadajnik
}

void loop() {
  // Odczyt joysticka
  //int xValue , yValue;
  int xValue = analogRead(pinX);   // 0‑1023
  int yValue = analogRead(pinY);   // 0‑1023
  static int xValueLast = 0;
  static int yValueLast = 0;
  bool buttonPressed = (digitalRead(pinButton) == LOW); 

  xValue = (xValue - 520)/2;
  yValue = (yValue - 511)/2;
  if (xValue > 249)
    {
      xValue = 255;
    }
  else if (xValue < -255)
    xValue = -255;


  if(xValue < 80)
    xValue = 2*xValue;
  else
    xValue = 160 + (xValue-80)/3;
  
  if(yValue < 40)
    yValue = 5*yValue;
  else
    yValue = 200 + (yValue-80)/3;
  
  xValue = -xValue/3;
  yValue = -yValue/8+xValue/5;

  // Pakiet do wysłania
  Payload data = { (uint16_t)xValue, (uint16_t)yValue };
  radio.write(&data, sizeof(data));    // Wyślij 4 bajty

  // Logujemy na Serial, żeby łatwo podejrzeć wartości
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" | Y: ");
  Serial.print(yValue);
  Serial.print(" | Button: ");
  Serial.println(buttonPressed ? "Pressed" : "Released");

  delay(5);   // 10 Hz – możesz zmienić wg potrzeby
}
