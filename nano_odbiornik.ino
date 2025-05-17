/*  Joystick RX – Arduino Nano + nRF24L01+
 *  Kanał: 2 (2 402 MHz), adres: "CTRL1"
 *
 *  ─── Podłączenie nRF24L01+ ─────────────────────────────────────────────
 *     VCC  → 3.3 V  (nie 5 V!)   + kondensator 4 7 µF–10 µF do GND
 *     GND  → GND
 *     CE   → D9
 *     CSN  → D10
 *     SCK  → D13
 *     MOSI → D11
 *     MISO → D12
 *     IRQ  → (opcjonalnie) D2  – tu nieużywane
 *
 *  SPI piny na Arduino Nano są takie same jak w Uno, ale leżą na krawędzi
 *  obudowy: D11/D12/D13 (MOSI/MISO/SCK).
 */

#include <SPI.h>
#include <RF24.h>

#define silnikPrawyDir1 6
#define silnikPrawyDir2 7 
#define silnikPrawyPWM 3
#define silnikLewyDir1 8
#define silnikLewyDir2 9
#define silnikLewyPWM 5
#define PWM_max 160

#define CE_PIN  4
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);

const byte address[5] = "0002";   // musi być identyczny z nadajnikiem

struct Payload {
  uint16_t x;      // 0‑1023
  uint16_t y;      // 0‑1023
};

void setup() {
  //Serial.begin(9600);
  pinMode(silnikPrawyDir1, OUTPUT);
  pinMode(silnikPrawyDir2, OUTPUT);
  pinMode(silnikPrawyPWM, OUTPUT);
  pinMode(silnikLewyDir1, OUTPUT);
  pinMode(silnikLewyDir2, OUTPUT);
  pinMode(silnikLewyPWM, OUTPUT);
  Prawy(0);
  Lewy(0);

  while (!Serial) ;                // dla klonów Nano z CH340

  // ─── Konfiguracja nRF24L01+ ─────────────────────────────
  radio.begin();                   // włącza PWR_UP = 1
  radio.setPALevel(RF24_PA_LOW);   // mała moc = mniejsze zakłócenia
  radio.setDataRate(RF24_250KBPS); // jak w nadajniku
  radio.setChannel(2);             // kanał 0 … 125  →  2400 MHz + kanał
  radio.openReadingPipe(0, address);
  radio.enableDynamicPayloads();   // niekonieczne, ale przydatne
  radio.startListening();          // PRIM_RX = 1, CE = 1  →  RX mode
}

void Prawy(int spd)
{
  if(spd<0)
  {
    digitalWrite(silnikPrawyDir1, HIGH);
    digitalWrite(silnikPrawyDir2, LOW);
    if(spd<PWM_max)
    {
      analogWrite(silnikPrawyPWM,PWM_max);
    }
    else
      analogWrite(silnikPrawyPWM,-spd);
  }
  else
  {
    digitalWrite(silnikPrawyDir2, HIGH);
    digitalWrite(silnikPrawyDir1, LOW);
    if(spd>PWM_max)
    {
      analogWrite(silnikPrawyPWM,PWM_max);
    }
    else
      analogWrite(silnikPrawyPWM,spd);
  }
}

void Lewy(int spd)
{
  if(spd<0)
  {
    digitalWrite(silnikLewyDir1, HIGH);
    digitalWrite(silnikLewyDir2, LOW);
    if(spd<PWM_max)
    {
      analogWrite(silnikLewyPWM,PWM_max);
    }
    else
      analogWrite(silnikLewyPWM,-spd);
  }
  else
  {
    digitalWrite(silnikLewyDir2, HIGH);
    digitalWrite(silnikLewyDir1, LOW);
    if(spd>PWM_max)
    {
      analogWrite(silnikLewyPWM,PWM_max);
    }
    else
      analogWrite(silnikLewyPWM,spd);
  }
}


void loop() {
  if (radio.available()) {         // pakiet czeka w RX‑FIFO?
  
    Payload data;
    radio.read(&data, sizeof(data));

  int x_ruch = data.x;
  int y_ruch = data.y;

if(y_ruch<0)
{
  Lewy(x_ruch + y_ruch);
  Prawy(x_ruch);
}
else
{
  Lewy(x_ruch);
  Prawy(x_ruch - y_ruch);
}

    // — Wyświetlenie —
    //Serial.print("X: ");
    //Serial.print(data.x);
    //Serial.print(" | Y: ");
    //Serial.print(data.y);

    // RPD (Received Power Detector) – poziom sygnału > ‑64 dBm w kanale
    bool rpd = radio.testRPD();    // zwraca true, jeżeli RPD high
    //Serial.print(" | RPD: ");
    //Serial.println(rpd ? "High" : "Low");
  }


}
