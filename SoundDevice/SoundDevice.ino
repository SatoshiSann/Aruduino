#include <MozziGuts.h>
#include <Oscil.h> // オシレーターのテンプレート
#include <tables/cos8192_int8.h> // オシレーター用のコサインテーブル
#include <mozzi_midi.h> //Pdの[mtof]オブジェクトの代わり
#define pin 0
#define CONTROL_RATE 64 // コントロールレートの設定
 
// harmonics 加算するオシレーターの配列変数COS2048_DATA2048の8bitデータを格納
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos1(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos2(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos3(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCos4(COS8192_DATA);

 
// ベースとなるピッチの変数を用意
float f1,f2,f3,f4;
 
float val = 0.f;
int sensorValue;
 
void setup(){
  startMozzi(CONTROL_RATE); // Mozziの初期化と処理開始
}
 
void loop(){
  audioHook(); // 再生のために必要
}
 
void updateControl(){ // Mozziの音出力設定はこの領域に書き込む
 
  sensorValue = mozziAnalogRead(pin); //Mozzi処理のバックグラウンドでセンサ値を読み込む
  val = map(sensorValue, 200, 400, 0, 1000); //センサの値を0~127にマッピング
 
    // センサ値からピッチを決定 (MIDIノートナンバー)
    f1 = val;
    f2 = val*(5/4);
    f3 = val*(7/4);
    f4 = val*(9/4);
 
    // setFreqに格納
    aCos1.setFreq_Q16n16(f1);
    aCos2.setFreq_Q16n16(f2);
    aCos3.setFreq_Q16n16(f3);
    aCos4.setFreq_Q16n16(f4);

}
 
int updateAudio(){
  // オーディオの繰り返し出力処理
  int asig =
    aCos1.next() + aCos2.next() +
    aCos3.next() + aCos4.next() ;
 
  //return asig >> 1; ///それぞれの出力オーバーを防ぐために>>3
}
