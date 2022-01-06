// definicja pinów
int latchPin = 4; // pis connected to shift registors
int clockPin = 5;
int dataPin = 3;
int pins [8] = {6, 7, 8, 9, 10, 11, 12, 13}; // common cathode pins

#define XPIN A4
#define YPIN A5
#define SW 2
#define LED 1
#define POT A0


// zadeklarowane obrazki do wyświetlania
byte UARROW[] = {B00011000, B00111100, B01111110, B11011011, B00011000, B00011000, B00011000, B00011000};
byte DARROW[] = {B00011000, B00011000, B00011000, B00011000, B11011011, B01111110, B00111100, B00011000};
byte RARROW[] = {B00001000, B00001100, B00000110, B11111111, B11111111, B00000110, B00001100, B00001000};
byte LARROW[] = {B00010000, B00110000, B01100000, B11111111, B11111111, B01100000, B00110000, B00010000};

byte MAP[] = {B01010101, B10101010, B01010101, B10101010, B01010101, B10101010, B01010101, B10101010};
byte HEART[] = {B00000000, B01100110, B11111111, B11111111, B11111111, B01111110, B00111100, B00011000};
byte PACMAN[] = {B00111000, B01000100, B10001000, B10010001, B10001000, B01000100, B00111000, B00000000};
byte GHOST[] = {B00000000, B00111000, B01111100, B11010110, B11111110, B11000110, B11111110, B01010100};

byte SMILEH[] = {B11111111, B10000001, B10100101, B10000001, B10100101, B10011001, B10000001, B11111111};
byte SMILEU[] = {B11111111, B10000001, B10100101, B10000001, B10011001, B10100101, B10000001, B11111111};
byte SMILEO[] = {B11111111, B10000001, B10100101, B10000001, B10011001, B10100101, B10011001, B11111111};
byte SMILEC[] = {B11111111, B10000001, B10100101, B10000001, B10000001, B10111101, B10000001, B11111111};

byte ALL[] = {B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111};

byte col0[] = {B10000000,B10000000,B10000000,B10000000,B10000000,B10000000,B10000000,B10000000};
byte col1[] = {B11000000,B11000000,B11000000,B11000000,B11000000,B11000000,B11000000,B11000000};
byte col2[] = {B11100000,B11100000,B11100000,B11100000,B11100000,B11100000,B11100000,B11100000};
byte col3[] = {B11110000,B11110000,B11110000,B11110000,B11110000,B11110000,B11110000,B11110000};
byte col4[] = {B11111000,B11111000,B11111000,B11111000,B11111000,B11111000,B11111000,B11111000};
byte col5[] = {B11111100,B11111100,B11111100,B11111100,B11111100,B11111100,B11111100,B11111100};
byte col6[] = {B11111110,B11111110,B11111110,B11111110,B11111110,B11111110,B11111110,B11111110};
byte col7[] = {B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};

byte* tab[8]={col0,col1,col2,col3,col4,col5,col6,col7};

// zmienne globalne - do odczytu wartości analogowych i zmiany progamu

uint16_t x = 0;
uint16_t y = 0;
uint16_t p = 0;
int xMap, yMap,pMap;
volatile uint8_t iterator = 0;


void setup() {
  Serial.begin(9600); // Serial begin
  pinMode(latchPin, OUTPUT); // Pin configuration
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for (int i = 0; i < 8; i++) { // for loop is used to configure common cathodes
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);
  }
  pinMode(POT,INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(LED,OUTPUT);

//attachInterrupt(digitalPinToInterrupt(SW), change,RISING); // przerwanie od przycisku
}

void loop() {
  
  // wybór programu odbywa się na podstawie wartości iteratora 
  // zadeklarowano 5 programow (3 programy z obsluga joysticka, 1 program wyswietlajacy całą tablice, 1 program wyswietlający kolumny na podstawie suwaka)
  if(digitalRead(SW)==0)
  {
    delay(200);
    iterator++;
    Serial.print(digitalRead(SW));
Serial.print('\n');
  }
  
  if (iterator > 4)
  {
    iterator = 0;
  }
Serial.print(iterator);
Serial.print('\n');


// odczyt wartości z joysticka i mapowanie 
  x = analogRead(XPIN);
  y = analogRead(YPIN);
  xMap = map(x, 0, 1023, 0, 10);
  yMap = map(y, 0, 1023, 10, 0);

  if (iterator == 1)
  {
    digitalWrite(LED,LOW);
    sterowanie(xMap, yMap, UARROW, DARROW, RARROW, LARROW);
  }
  else if (iterator == 2)
  {
    digitalWrite(LED,LOW);
    sterowanie(xMap, yMap, MAP, HEART, PACMAN, GHOST);
  }
  else if (iterator == 3)
  {
    digitalWrite(LED,LOW);
    sterowanie(xMap, yMap, SMILEH, SMILEU, SMILEO, SMILEC);
  }
  else if (iterator == 4)
  {
    digitalWrite(LED,LOW);
    drawScreen(ALL);
  }
    else if (iterator == 0)
  { 
     digitalWrite(LED,HIGH);
    p = analogRead(POT);
    pMap = map(p, 0, 920, 0, 7);
    linijka(pMap);

  }



}
//////////////////////////////////////////////////////////////////////////////////////////////////////
/*void linijka(int pMap)
 * funkcja zapalająca liczbę kolumn na wyświetlaczu w zależności od wartości pMap 1-8
 */
void linijka(int pMap)
{ 
drawScreen(tab[pMap]);
  
}
/*void drawScreen(byte ch[8])
 * funkcja wyswietlajaca obrazek na wyswietlaczu na podstawie podanej tablicy bitów - ch[8]
 * do wyswietlania wykorzystano rejestr przesuwny 74HC595N
 */
void drawScreen(byte ch[8]) { // Method do the multiplexing
  for (int j = 0; j < 8; j++) {
    digitalWrite(latchPin, LOW);
    digitalWrite(pins[j], LOW);
    
    shiftOut(dataPin, clockPin, LSBFIRST, ch[j]);
    digitalWrite(latchPin, HIGH);
    digitalWrite(latchPin, LOW);
     
    shiftOut(dataPin, clockPin, LSBFIRST, B00000000); 
    digitalWrite(latchPin, HIGH);
    digitalWrite(pins[j], HIGH);

  }


}
/*void sterowanie(uint16_t x, uint16_t y, byte* U, byte* D, byte* R, byte* L)
 * funkcja wyswietlająca obrazek na matrycy na podstawie wychylenia gałki joysticka
 * 
 */
void sterowanie(uint16_t x, uint16_t y, byte* U, byte* D, byte* R, byte* L)
{
  
  if (x < 2 && (y > 3 && y < 8))
  {
    drawScreen(U);
  }
  else if (x > 8 && (y > 3 && y < 8))
  {
    drawScreen(D);
  }
  if ( y > 8 )
  {
    drawScreen(R);
  }
  else if (y < 2 )
  {
    drawScreen(L);
  }
  else 
  {
    drawScreen(ALL);
  }
  

}
/*void change()
 * inkrementuje wartosc iteratora podczas wcisnięcia przycisku
 */
void change()
{
iterator++;
Serial.print(digitalRead(SW));
Serial.print('\n');
}
