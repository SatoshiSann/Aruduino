//ライブラリを読み込み
#include<CapacitiveSensor.h>
 
CapacitiveSensor cs = CapacitiveSensor(13, 12); // inputは13番ピン, outputを12番ピン
 
void setup() {
  // おまじないと思ってよさそうです。
  // キャリブレーションするための時間とかそういうのだと思います。
  cs.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);
}
 
void loop() {
  // ここの30はノイズをとる強さと考えればいいと思います。
  // 大きくすればノイズはとれますが、遅くなり、
  // 小さくすれば速くなりますが、あまりノイズはとれません。
  
  long val = cs.capacitiveSensor(30);
 
  // 値をシリアルに書き出し
  //Serial.print("value:tt");
  //Serial.println(val);
 
  // 放電
  delay(10);
}
