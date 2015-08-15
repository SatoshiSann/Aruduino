#include<CapacitiveSensor.h>
#include<RollingAverage.h>
#include<MsTimer2.h>

#define MIC A0    //マイクピン番号
#define LIGHT A1  //光受信ピン番号
#define LED 10    //LEDピン番号
#define SPEAKER 9 //スピーカー番号
#define BAIO 11    //バイオメタルピン番号
#define BAIOTIME_HIGH 3000  //バイオメタルに電気を流す時間
#define BAIOTIME_LOW 10000  //バイオメタルに電気を流さない時間

RollingAverage <long , 16> cs_val_Avr;
RollingAverage <long , 16> MIC_val_Avr;

CapacitiveSensor cs = CapacitiveSensor(13, 12);// 静電容量取得用pinNo(SendPin RecivePin)
long cs_val = 0;      //静電容量値
long cs_val_bef = 0;
long cs_val_Last = 0;

int MIC_val = 0;     // マイクの値
int MIC_val_last = 0;
int MIC_val_bef = 0;

int LIGHT_val = 0;   // 光の値

int hikarikata = 0;  //デューティー比

float frq = 0;

boolean mainBool = true;
boolean hikarikataBool = true;
boolean baioBool = false;
int baioTime = 0;
int mainTime = 0;

void flash(){
  
  if(MIC_val_bef > 0){ 
    
   if(abs(MIC_val_bef - MIC_val) >= 40){
     mainBool = false;
     Serial.println("MIC = false");
   }else{
     Serial.println("MIC = true");
   }
   
  }
  
//   Serial.print("MIC = ");
//  Serial.print(abs(MIC_val_bef - MIC_val));
//  Serial.println(" <= 40");
  
  
  MIC_val_bef = MIC_val;
  
  if(LIGHT_val < 950){
   Serial.println("LIGHT = true");
  }else{
    mainBool = false;
    Serial.println("LIGHT = false");
  }
  
 
//  Serial.print("LIGHT = ");
//  Serial.print(LIGHT_val);
//  Serial.println(" <= 950");
  
  if(abs(cs_val - cs_val_bef) <= 100){
    Serial.println("CS = true");
  }else{
    Serial.println("CS = false");
  }
  
//  Serial.print("CS = ");
//  Serial.print(abs(cs_val - cs_val_bef) );
//  Serial.println(" <= 100");
  

}

void setup() {
  Serial.begin(9600);
  
  //////おまじない//////静電容量用
  cs.set_CS_AutocaL_Millis(0xFFFFFFFF);
  
  //////ピンモード設定///////
  pinMode(BAIO,OUTPUT);
  pinMode(8,OUTPUT);  
  digitalWrite(8,HIGH);
  MsTimer2::set(100,flash);
  MsTimer2::start();

}




void loop() {
  baioTime = millis() % (BAIOTIME_LOW + BAIOTIME_HIGH);
  //////入力系統//////
  cs_val_bef = cs_val_Last;
  cs_val = cs.capacitiveSensor(1000);//値を増やすとノイズが減る
  cs_val_Last = cs_val_Avr.next(cs_val);
  MIC_val = MIC_val_Avr.next(analogRead(MIC));
  LIGHT_val = analogRead(LIGHT);
  
  //////出力系統//////
  analogWrite(LED,hikarikata); //hikarikata=デューティー比　0-255 = 0% ~ 100%

  if( abs(cs_val - cs_val_bef) > 100){
    mainBool = false ; 
  }
  
  mainBool = true;
  //動きのメイン
  if(mainBool == true){
    
    ////////バイオメタル関係////////////
    if(baioBool == true){
      digitalWrite(BAIO,HIGH);
      Serial.println("BAIO = on");
    }else{
      digitalWrite(BAIO,LOW);
      Serial.println("BAIO = off");
    }
    
    
    if(baioBool == true && baioTime > BAIOTIME_HIGH){  
      baioBool = false;
    }
    
    if(baioBool == false){
  
      if(baioTime >= BAIOTIME_LOW + BAIOTIME_HIGH - 100 ){ //バイオメタルの現在の時間　==　バイオメタルの一周の時間 
        baioBool = true;
      }
      
    }
    
   
    ////////LED関係/////////
    
    if(hikarikata >= 251 && hikarikataBool){
      hikarikataBool = false;
    }
    
    if(hikarikata <= 50 && hikarikataBool == false){
      hikarikataBool = true;
    }
    
    if(hikarikataBool){
      hikarikata += 3;
    }else{
      hikarikata -= 3;
    }
    mainTime = 0;
  }else{
    hikarikataBool = false;
    hikarikata = 0;
    mainTime ++ ;
  }
  
  if(mainTime > 2000){
    mainBool = true;
  }
  
//  Serial.print("mainTime = ");
//  Serial.print(mainTime);
//  Serial.println(" > 2000 ");
//  
}
