#include "FastLED.h"             // 此示例程序需要使用FastLED库
#define NUM_LEDS 15              // LED灯珠数量
#define LED_DT 2                 // LED燈信号引脚 D2

#include "pitches.h"  // 蜂鳴器 音階

#define LED_TYPE WS2812          // LED灯带型号
#define COLOR_ORDER GRB          // RGB灯珠中红色、绿色、蓝色LED的排列顺序
CRGB leds[NUM_LEDS];             // 建立光带leds
CRGB ledback[8]; 

#define Ball_in_seneor  12        //進球感測器接腳 D12
#define BTN  9                  // 開始按鍵   的接腳D11
#define RELAY_Pin  10            // 繼電器的接腳   D10
#define Ball_out_seneor  11        //退球感測器接腳 D9

#include <TM1637Display.h>   //數碼管庫     https://github.com/avishorp/TM1637
#define CLK 3              //數碼管  CLK 接  D3
#define DIO 4              //數碼管  DIO 接  D4
#define BUZZER_PIN 5       //蜂鳴器 D5

TM1637Display display(CLK, DIO);

unsigned long prevTime = 0;
unsigned long currentMillis = 0;


boolean CustomMode = false;

int old_BTN_State = HIGH;
int old_BTN1_State = HIGH;
int old_BTN2_State = HIGH;
int old_BTN3_State = HIGH;
int old_BTN4_State = HIGH;
int old_BTN5_State = HIGH;
int old_BTN6_State = HIGH;
int old_BTN7_State = HIGH;
int old_BTN8_State = HIGH;

boolean BTN_S = true ;                 //按鍵狀態
boolean SENSOR_S = false ;            // 球道的光敏感測 關
boolean RELAY_Sensor_S = false ;     //出球感測  開/關
boolean RELAY_S = false ;           //繼電器  開/關

int ball_in_sensor ;              //進球感應器變數
int ball_out_sensor;             //退球感應器變數
int ball_num =0;                //球的數量
int Mode;                      //隨機選擇備率的模式
int checker;                  //

int D1 = 0;          
int D2 = 0;
int D3 = 0;
int D4 = 0;
int D5 = 0;
int D6 = 0;
int D7 = 0;
int D8 = 0;



void setup() {
     Serial.begin(115200);
     LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);    // 初始化光带
   
     FastLED.setBrightness(20);          // 0 ～ 255， 数值越大则光带亮度越高
     display.setBrightness(10);         //4位數管亮度




     pinMode(BTN, INPUT_PULLUP);
     pinMode(A0, INPUT_PULLUP);
     pinMode(A1, INPUT_PULLUP);
     pinMode(A2, INPUT_PULLUP);
     pinMode(A3, INPUT_PULLUP);
     pinMode(A4, INPUT_PULLUP);
     pinMode(A5, INPUT_PULLUP);
     pinMode(7, INPUT_PULLUP);  //D7
     pinMode(8, INPUT_PULLUP);  //D8
     
     pinMode(RELAY_Pin,OUTPUT);
     digitalWrite(RELAY_Pin,LOW);
    
     Blink_ALL();    //初始化  全藍燈閃爍
     FastLED.show();
  
            }


void loop() {
        //循環流程
        Easy_Mode();
        in_count();                //入球感應記數
        show_4digital_num();       //顯示球數
        button();                  //開始按鈕
        sensor();                  //光敏感應球道
        RELAY_Sensor();            //退球感應記數
        RELAY();                  //繼電器控制馬達運轉
        wait();
}




//入球感應記數
void in_count(){
  
     ball_in_sensor = digitalRead(Ball_in_seneor);  //讀取D12
     
     if(ball_in_sensor==0) {
          ball_num ++ ;
          Serial.println("進球");
          delay(100);              //感測器延遲頻率
                          }
              }


//顯示4段數字
void show_4digital_num(){ 
   display.showNumberDec(ball_num, false);
  }




//按鍵功能
void button(){
 
        int now_BTN_State = digitalRead(BTN);      //讀取 即時 按鍵狀態

        if (now_BTN_State != old_BTN_State && BTN_S && ball_num > 0){        //按鍵啟動條件 按鈕狀態不同 /按鈕狀態(BTN_S開啟)  / 入球數>0
                coin();
                LED_Random();                   //隨機閃爍LED
                BTN_S = false;                  //按鈕狀態(BTN_S 關)
                SENSOR_S = true;                // 球道的光敏感測 開
                Serial.println("按鍵啟動");
                delay(10);
                
                }
 now_BTN_State = old_BTN_State;
 
}



//球道的感測功能
void sensor(){

   int now_BTN1_State = digitalRead(A0);  //  A0   讀取 即時 按鍵狀態
   int now_BTN2_State = digitalRead(A1);  //  A1  
   int now_BTN3_State = digitalRead(A2);  //  A2  
   int now_BTN4_State = digitalRead(A3);  //  A3  
   int now_BTN5_State = digitalRead(A4);  //  A4  
   int now_BTN6_State = digitalRead(A5);  //  A5  
   int now_BTN7_State = digitalRead(7);  //  D7  
   int now_BTN8_State = digitalRead(8);  //  D8  


       //接收到感測  後 流程  /Blink_LED閃爍LED   /球道變數 開   //count計算倍率
if(SENSOR_S == true){
    if (now_BTN1_State != old_BTN1_State){ Serial.println("D1"); Blink_LED(0); D1=1; delay(1000); count();}
    if (now_BTN2_State != old_BTN2_State){ Serial.println("D2"); Blink_LED(1); D2=1; delay(1000); count();}
    if (now_BTN3_State != old_BTN3_State){ Serial.println("D3"); Blink_LED(2); D3=1; delay(1000); count();}
    if (now_BTN4_State != old_BTN4_State){ Serial.println("D4"); Blink_LED(3); D4=1; delay(1000); count();}
    if (now_BTN5_State != old_BTN5_State){ Serial.println("D5"); Blink_LED(4); D5=1; delay(1000); count();}
    if (now_BTN6_State != old_BTN6_State){ Serial.println("D6"); Blink_LED(5); D6=1; delay(1000); count();}
    if (now_BTN7_State != old_BTN7_State){ Serial.println("D7"); Blink_LED(6); D7=1; delay(1000); count();}
    if (now_BTN8_State != old_BTN8_State){ Serial.println("D8"); Blink_LED(7); D8=1; delay(1000); count();}
                   } 

 now_BTN1_State = old_BTN1_State;
  now_BTN2_State = old_BTN2_State;
   now_BTN3_State = old_BTN3_State;
    now_BTN4_State = old_BTN4_State;
    now_BTN5_State = old_BTN5_State;
     now_BTN7_State = old_BTN7_State;
     now_BTN8_State = old_BTN8_State;
                   
           }    //end sensor



//固定閃爍 選定第幾顆LED 效果   
void Blink_LED(int num){

   for (int i = 0; i < 4; i++) {         // x 4次
            leds[num] = CRGB::Green;
            FastLED.show();
            delay(300); 
            leds[num] = CRGB::Black;
            FastLED.show();
            delay(300);
           // FastLED.clear(); 
            FastLED.show();
                               }
                     }
 
//閃爍全部 LED 效果
void Blink_ALL(){
        for (int i = 0; i < 5; i++) { 
               fill_solid(leds, 15, CRGB::Blue);   //選取瑱滿 0-15顆LED 顏色
               FastLED.show();
               delay(200);
               fill_solid(leds, 15, CRGB::Black);   //選取瑱滿 0-15顆LED 顏色
               FastLED.show();
               delay(200);
                                   }
            }


//隨機閃爍LED 效果
 void LED_Random(){ 

       //大量亂數  快速閃爍Led
       for (int i = 0; i < 50; i++) { 
              int no = random(0, 8);    //隨機選擇範圍 0-7
              leds[no] = CRGB::Red;
              FastLED.show();
              delay(10); 
              leds[no] = CRGB::Black;
                                    }
      if(CustomMode == true)  Mode =random(1, 3);         //隨機選擇模式 1-3                             
      if(CustomMode == false) Mode =random(1, 6);         //隨機選擇模式 1-5
      Serial.print("選定模式:");
      Serial.println(Mode);
      

      fill_solid(leds, 15, CRGB::Black);   //選取瑱滿 0-15顆LED 顏色
      
      if(Mode==1){
            leds[10] = CRGB::Red;   //顯示倍率x2 的燈
    
            leds[1] = CRGB::Red;
            leds[3] = CRGB::Red;
            leds[5] = CRGB::Red;
            leds[7] = CRGB::Red;
            FastLED.show();
            delay(1000);
                 }

     if(Mode==2){
           leds[11] = CRGB::Red;   //顯示倍率x4 的燈
     
           leds[2] = CRGB::Red;
           leds[4] = CRGB::Red;
           leds[6] = CRGB::Red;
           FastLED.show();
           delay(1000);
                }

    if(Mode==3){
           leds[12] = CRGB::Red;  //顯示倍率x6 的燈
         
           leds[0] = CRGB::Red;
           leds[2] = CRGB::Red;
           leds[4] = CRGB::Red;
           FastLED.show();
           delay(1000);
            }

    if(Mode==4){
           leds[13] = CRGB::Red;   //顯示倍率x8 的燈
     
           leds[0] = CRGB::Red;
           leds[2] = CRGB::Red;
           FastLED.show();
           delay(1000);
              }

    if(Mode==5){
          leds[14] = CRGB::Red;   //顯示倍率x10 的燈
          
          checker=random(0, 3);   //隨機選擇LED  0-2         
          leds[checker] = CRGB::Red;
          FastLED.show();
          delay(1000);
          }

       
 }

 
 



//count計算倍率
void count(){

   if(Mode==1){
               if(D2==1){ball_num = ball_num*2 ; win(); }
           else if(D4==1){ball_num = ball_num*2 ; win();}
           else if(D6==1){ball_num = ball_num*2 ; win();}
           else if(D8==1){ball_num = ball_num*2 ; win();}
            else { delay(300); ball_num =0; Lose(); }    //沒有中獎 歸零
             }
              
   if(Mode==2){
                 if(D3==1){ball_num = ball_num*4 ; win();}
            else if(D5==1){ball_num = ball_num*4 ; win();}
            else if(D7==1){ball_num = ball_num*4 ; win();}
            else { delay(300); ball_num =0; Lose();}
             }
              
   if(Mode==3){
                if(D1==1){ball_num = ball_num*6 ;  win();}
           else if(D3==1 ){ball_num = ball_num*6 ; win();}
           else if(D5==1 ){ball_num = ball_num*6 ; win();}
           else{ delay(300); ball_num =0; Lose(); }
             }


   if(Mode==4){
                  if(D1==1){ball_num = ball_num*8 ;  win();}
            else  if(D3==1){ball_num = ball_num*8 ; win(); }
            else{ delay(300); ball_num =0; Lose(); }
             }

   if(Mode==5){
                  if(checker==0  && D1==1 ){ball_num = ball_num*10 ;  win(); }
            else  if(checker==1  && D2==1 ){ball_num = ball_num*10 ;  win(); }
            else  if(checker==2  && D3==1 ){ball_num = ball_num*10 ; win();  }
            else{ delay(300); ball_num =0; Lose(); }
             }
 display.showNumberDec(ball_num, false);
 data_reset();
}


//球道感應 狀態料重設
void data_reset(){


SENSOR_S = false ;          //球道感測變數 關
RELAY_Sensor_S=true;        //繼電器感測變數  開
RELAY_S=true;               //繼電器狀態     開
BTN_S=true;                 //按鈕狀態      開

Mode=0;
D1 = 0;
D2 = 0;
D3 = 0;
D4 = 0;
D5 = 0;
D6 = 0;
D7 = 0;
D8 = 0;
delay(1000);
//prevTime = currentMillis;
}


//繼電器感測功能
void RELAY_Sensor(){
  
      if(RELAY_Sensor_S){
             ball_out_sensor = digitalRead(Ball_out_seneor);  //讀取D9
             if(ball_out_sensor==0) {
                       ball_num = ball_num -1 ;
                       Serial.print("出球:");
                       Serial.println(ball_num);
                       delay(100);
                                     }
                      }
                   if(ball_num<0) ball_num=0;  
                    }

//繼電器功能
void RELAY(){
   if(RELAY_S){
       if(ball_num > 0) digitalWrite(RELAY_Pin,HIGH);    //球數>0 啟動繼電器
       
       
       if(ball_num <=0) {
               digitalWrite(RELAY_Pin,LOW);
               RELAY_Sensor_S =false;      //繼電器感測變數 關
               RELAY_S = false;            //繼電器   關
                       }
             }
         }



void wait(){

if (ball_num > 0){ 
   currentMillis = millis();  
   
  if (currentMillis - prevTime >= 20000) { 
      ball_num =0;
      prevTime = currentMillis; 
     }
    }
  }


void Easy_Mode(){
            int now_BTN1_State = digitalRead(A0);  //  A0 
            int now_BTN2_State = digitalRead(A1);  //  A1 
            int now_BTN_State = digitalRead(BTN);      //讀取 即時 按鍵狀態

        if (now_BTN_State ==0 && now_BTN1_State ==0 ){   
                coin(); 
                display.showNumberDec(9999, false);
                CustomMode = true ;
                Serial.println("EASY MODE -- ON");            
                delay(1000);
                }

        if (now_BTN_State ==0 && now_BTN2_State ==0 ){  
                coin();
                display.showNumberDec(5555, false);
                CustomMode = false ;
                Serial.println("EASY MODE -- OFF");            
                delay(1000);
                }
}




void win() {

  tone(BUZZER_PIN,NOTE_F5,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_A5,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_C6,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_A5,250);
  delay(250);
  tone(BUZZER_PIN,NOTE_C6,500);
  delay(500);
  tone(BUZZER_PIN,REST,500);
}


void Lose() {
  tone(BUZZER_PIN,NOTE_C7,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_B6,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_AS6,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_A6,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_GS6,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_G6,250);
  delay(250);
  tone(BUZZER_PIN,REST,125);
}



void level_up() {


  // Play 1-up sound
  tone(BUZZER_PIN,NOTE_E5,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_G5,125);
 delay(125);
  tone(BUZZER_PIN,NOTE_E6,125);
 delay(125);
  tone(BUZZER_PIN,NOTE_C6,125);
 delay(125);
  tone(BUZZER_PIN,NOTE_D6,125);
  delay(125);
  tone(BUZZER_PIN,NOTE_G6,125);
  delay(125);
  tone(BUZZER_PIN,REST,125);
  
}

void coin(){
  tone(BUZZER_PIN,NOTE_B6,125);    
  delay(130);
  tone(BUZZER_PIN,NOTE_E7,255);
  delay(130);
  tone(BUZZER_PIN,REST,125);
}
