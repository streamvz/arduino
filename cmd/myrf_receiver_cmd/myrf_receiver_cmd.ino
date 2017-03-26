#include <SPI.h>                                          // Подключаем библиотеку  для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку  для работы с nRF24L01+
// Подключаем библиотеку  для работы с сервоприводами
RF24           radio(48, 53);                              // Создаём объект radio   для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
// Создаём объект myservo для работы с функциями библиотеки Servo
int            data[2];                                   // Создаём массив для приёма данных

byte addresses[][6] = {"0x1234567890LL", "0x1234567891LL"};
byte cmd;

int receiveBuffer;

void setup() {
  Serial.begin(9600);

  delay(1000);

  radio.begin();                                        // Инициируем работу nRF24L01+
  radio.setChannel(5);                                  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_250KBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
//  radio.openReadingPipe (1, 0x1234567890LL);            // Открываем 1 трубу с идентификатором 0x1234567890 для приема данных (на ожном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
  //radio.setAutoAck(false);
//  radio.startListening  ();                             // Включаем приемник, начинаем прослушивать открытую трубу
  //  radio.stopListening   ();    
  // Выключаем приёмник, если потребуется передать данные

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);

  Serial.println("Ready!");
}
void loop() {
Serial.println("Read ");
  while (!Serial.available());
  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    Serial.println(c); 
    
    if (c == 'T') {
      cmd = 2;
    } else if (c == 'H') {
      cmd = 1;
    } else {
      cmd = 0;
    }
    radio.stopListening(); 
    Serial.println(cmd); 
    radio.write(&cmd, sizeof(byte));  
    
  }
  radio.startListening();
  while(!radio.available());
  if (radio.available()) {                              // Если в буфере имеются принятые данные
    radio.read(&receiveBuffer, sizeof(int));                  // Читаем данные в массив data и указываем сколько байт читать
    Serial.print(c);
    Serial.println(receiveBuffer);    
//    Serial.println(data[1]);
    //   dispLED.print(data[0]);                           // Выводим показания Trema слайдера на индикатор
    //   myservo.write(map(data[1],0,1023,0,180));         // Поворачиваем сервопривод на угол заданный Trema потенциометром
  }

//  delay(1000);
}
