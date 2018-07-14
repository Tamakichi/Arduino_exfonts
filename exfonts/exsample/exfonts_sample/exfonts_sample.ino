// フォントライブラリ利用サンプル
// 作成 2014/10/20
// 修正 2015/01/28
//

#include <SPI.h>
#include <W25Q64.h>
#include <exfonts.h>
#if defined (__STM32F1__)   
#define CSPIN PB0
#else
#define CSPIN 10
#endif

exfonts ft;  // フォント管理オブジェクト

// ビットパターン表示
// d: 8ビットパターンデータ
//
void bitdisp(uint8_t d) {
  for (byte i=0; i<8;i++) {
    if (d & 0x80>>i) 
      Serial.print("#");
    else
      Serial.print(".");
  }
}

// フォントデータの表示
// c(in) : フォントコード(UTF16コード)
//
void fontdisp(uint16_t c) {
  byte buf[MAXFONTLEN]; 
  ft.getFontData(buf,c);        // フォントデータの取得
  byte bn= ft.getRowLength();   // 1行当たりのバイト数取得
  
  Serial.print(ft.getWidth(),DEC);
  Serial.print("x");      
  Serial.print(ft.getHeight(),DEC);      
  Serial.print(" ");      
  Serial.println((unsigned short)c, HEX);  // UTF16コード表示

  for (byte i = 0; i < ft.getLength(); i += bn ) {
      for (byte j = 0; j < bn; j++) {
        bitdisp(buf[i+j]);
      }
      Serial.println("");
  }
  Serial.println("");
} 

void test(char* pUTF8) {
  int n;
  uint16_t pUTF16[100];
  n = exfonts::Utf8ToUtf16(pUTF16, pUTF8);  // UTF8からUTF16に変換する

  // 7種類フォントサイズで表示
  for (byte i=0; i < MAXSIZETYPE; i++) {
    ft.setFontSize(i);      // フォントサイズの設定
    for (byte j=0; j < n; j++) {
      fontdisp(pUTF16[j]);  // フォントパターンの表示
    }
  }
}

void test02() {
  
}

void setup() {
  Serial.begin(115200);
#if defined (__STM32F1__)   
  while(!Serial.isConnected());
#endif  
  ft.init(CSPIN);  // フォント管理の初期化
  Serial.println("exfont liblary");
  char *pUTF8 = "AＡｱｲｳｴｵ１２Ａ＼￠￡§¨￢°±´¶×÷埼玉さいたま";
  //char *pUTF8 = "1aA!#/";
  //char *pUTF8 = "あ";
  test(pUTF8);
}

void loop() {
}

