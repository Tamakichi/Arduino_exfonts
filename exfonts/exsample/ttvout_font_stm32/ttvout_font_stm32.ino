//
// TTVOutでマルチフォント表示 by たま吉さん 2017/11/18
//  

#include <SPI.h>
#include <W25Q64.h>
#include <exfonts.h>
#include <TTVout.h>

TTVout TV;
exfonts ft;  // フォント管理オブジェクト

// 指定位置に１文字表示
void mputc(uint16_t x, uint16_t y, uint16_t code) {
  byte buf[MAXFONTLEN]; 
  ft.getFontData(buf,code); // フォントデータの取得  
  TV.bitmap(x,y, buf ,0,ft.getWidth(), ft.getHeight());
}

// 指定位置に文字列表示
void mprint(uint16_t x, uint16_t y, char* str) {
  uint16_t wstr[1024];
  int16_t   len;
  
  len = exfonts::Utf8ToUtf16(wstr, str);  // UTF8からUTF16に変換する
  for (uint16_t i=0; i <len; i++) {
    mputc(x, y, wstr[i]);
    if (x + ft.getWidth()*2 < TV.hres()) {
      x += ft.getWidth();  
    } else {  
      x = 0;
      if (y+ft.getHeight()*2 < TV.vres()) {
        y+=ft.getHeight();
      } else {  
        break;
      }
    }
  }      
}

void setup () {
  static const char* text=
   "吾輩は猫である。名前はまだ無い。"
   "どこで生れたかとんと見当けんとうがつかぬ。"
   "何でも薄暗いじめじめした所でニャーニャー泣いていた事だけは記憶している。"
   "吾輩はここで始めて人間というものを見た。"
   "しかもあとで聞くとそれは書生という人間中で一番獰悪な種族であったそうだ。"
   "というのは時々我々を捕つかまえて煮にて食うという話である。"
   "しかしその当時は何という考もなかったから別段恐しいとも思わなかった。"
   "ただ彼の掌のひらに載せられてスーと持ち上げられた時何だかフワフワした感じがあったばかりである。"
   "掌の上で少し落ちついて書生の顔を見たのがいわゆる人間というものの見始めであろう。";
   
  TV.begin(SC_448x216,2); // 第2引数でSPI 1,2を指定(デフォルト 1))
  TV.adjust(2);           // 垂直同期信号補正(デフォルト 0)
  ft.init(PB0);           // フォント管理の初期化
      
  //7種類のフォントサイズで文字列表示
  while(1)
    for (uint8_t i =0 ; i <7; i++) {
      TV.fill(0);
      ft.setFontSize(i);
      mprint(0,0,(char*)text);
      delay(1000*3);
    }
}

void loop () {

}

