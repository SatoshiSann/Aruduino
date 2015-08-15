#include <MozziGuts.h>
#include <Oscil.h> // オシレーター
#include <tables/triangle2048_int8.h> // 三角波テーブルのロード
#include <tables/cos2048_int8.h> // サイン波テーブル
#include <mozzi_midi.h>
#include <RollingAverage.h>
#include <ControlDelay.h>

#define PinX A0
#define PinY A1
#define PinZ A2
#define CONTROL_RATE 128

#define delayBuff 16
#define delayBuff2 30
#define delayBuff3 127

ControlDelay <128, int> kDelay; // 2seconds


Oscil <TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aTri(TRIANGLE2048_DATA );
Oscil <TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aTri3(TRIANGLE2048_DATA );//5/4 7/4 9/4 
Oscil <TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aTri9(TRIANGLE2048_DATA );

Oscil <TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aTri_1(TRIANGLE2048_DATA );
Oscil <TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aTri_2(TRIANGLE2048_DATA );//5/4 7/4 9/4 
Oscil <TRIANGLE2048_NUM_CELLS, AUDIO_RATE> aTri_3(TRIANGLE2048_DATA );

Oscil <COS2048_NUM_CELLS, AUDIO_RATE> aVibrato(COS2048_DATA);

RollingAverage <int, 16> AvrX;
RollingAverage <int, 16> AvrY;
RollingAverage <int, 16> AvrZ;

byte gain = 255;
byte herasu = 0;

float vib = 0;

float frq = 440;
float frq2 , frq3;

void setup(){
  startMozzi(CONTROL_RATE); // Mozziの開始
  kDelay.set(delayBuff);
  aVibrato.setFreq(6.f);
}

void updateControl(){
  int valX = mozziAnalogRead(PinX);
  int valY = mozziAnalogRead(PinY);
  int valZ = mozziAnalogRead(PinZ);
  
  int avgX = AvrX.next(valX);
  int avgY = AvrY.next(valY);
  int avgZ = AvrZ.next(valZ);
  
  herasu = constrain(map(avgX, 270, 780, 0, 180), 0, 180);
  
  // マイナスにいくと一周してByteのMAXになる
  gain = herasu ; 
  if(avgY > 0 && avgY < 800){
  //midiNo = map(avgY, 0, 800, 0, 116);
  frq = map(avgY, 0, 800, 200 + vib, 2000 + vib);
  }
  
  frq2=frq*5/4;
  frq3=frq*7/4;
  
  aTri.setFreq(frq); // aTriの周波数の設定
  aTri3.setFreq(frq2);
  aTri9.setFreq(frq3);

  
  aTri_1.setFreq(kDelay.next(frq));
  aTri_2.setFreq(kDelay.read(delayBuff2));
  aTri_3.setFreq(kDelay.read(delayBuff3));
  
//  Serial.print("X = ");
//  Serial.println(avgX);
  
//  Serial.print("Y = ");
//  Serial.println(avgY);
//  
//  Serial.print("Z = ");
//  Serial.println(avgZ);

}


int updateAudio(){
  vib = 1 * aVibrato.next();
   
  int vol1 = aTri.next() + aTri3.next() >> 2 + aTri9.next() >> 2;
  int vol2 = aTri_1.next() >> 1;
  int vol3 = aTri_2.next() >> 2;
  int vol4 = aTri_3.next() >> 3;

  int vol = vol1 + vol2 + vol3 + vol4;
  vol = vol / 3;
    //int vol = vol2 + vol3 + vol4;

  return   (vol * gain) >> 5 ; // shift back to STANDARD audio range, like /256 but faster
}


void loop(){
  audioHook(); 
}

