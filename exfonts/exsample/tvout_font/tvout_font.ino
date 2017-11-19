//
// TVOutでマルチフォント表示 by たま吉さん 2016/03/30
//  

#include <SPI.h>
#include <W25Q64.h>
#include <exfonts.h>
#include <TVout.h>

// TVoutの派生クラスの定義し、
// bitmapメソッドをフラッシュメモリではなく、SRAM対応させる
class exTVout : public TVout {
  public:
    void bitmap(uint8_t x, uint8_t y, const uint8_t* bmp, 
                uint16_t i, uint8_t width, uint8_t lines);
};

// bitmapメソッド
void exTVout::bitmap(uint8_t x, uint8_t y, const uint8_t* bmp,
                     uint16_t i, uint8_t width, uint8_t lines) {

  uint8_t temp, lshift, rshift, save, xtra;
  uint16_t si = 0;
	
  rshift = x&7;
  lshift = 8-rshift;		
  if ( width & 7 ) {
    xtra = width & 7; width = width / 8; width++;
  } else {
    xtra = 8; width = width/8;
  }	
  for ( uint8_t l = 0; l < lines; l++ ) {
    si = (y + l)*display.hres + x/8;
    if (width == 1)
      temp = 0xff >> rshift + xtra;
    else
      temp = 0;
    save = screen[si];
    screen[si] &= ((0xff << lshift) | temp);
    temp = *(bmp+ i++); screen[si++] |= temp >> rshift;
    for ( uint16_t b = i + width-1; i < b; i++ ) {
      save = screen[si]; screen[si] = temp << lshift;
      temp = *(bmp + i); screen[si++] |= temp >> rshift;
    }
    if ( rshift + xtra < 8 )
      screen[si-1] |= (save & (0xff >> rshift + xtra));
    if ( rshift + xtra - 8 > 0 )
      screen[si] &= (0xff >> rshift + xtra - 8);
      screen[si] |= temp << lshift;
    }
}

exTVout TV;  // TV出力オブジェクト
exfonts ft;  // フォント管理オブジェクト

// 指定位置に１文字表示
void mputc(uint8_t x, uint8_t y, uint16_t code) {
  byte buf[MAXFONTLEN]; 
  ft.getFontData(buf,code); // フォントデータの取得  
  TV.bitmap(x,y, buf ,0,ft.getWidth(), ft.getHeight());
}

// 指定位置に文字列表示
void mprint(uint8_t x, uint8_t y, char* str) {
  
  uint16_t wstr[64];
  int8_t   len;
  
  len = exfonts::Utf8ToUtf16(wstr, str);  // UTF8からUTF16に変換する
  for (uint8_t i=0; i <len; i++) {
    mputc(x, y, wstr[i]);
    if (x + ft.getWidth()*2 < 120) {
      x += ft.getWidth();  
    } else {  
      x = 0;
      if (y+ft.getHeight()*2 < 80) {
        y+=ft.getHeight();
      } else {  
        break;
      }
    }
  }      
}

void setup () {
  ft.init();  // フォント管理の初期化
  TV.begin(NTSC, 120, 80); // TVOut開始
  
  //7種類のフォントサイズで文字列表示
  while(1)
    for (uint8_t i =0 ; i <7; i++) {
      TV.fill(0);
      ft.setFontSize(i);
      mprint(0,0,"春はるはあけぼのやうやう白くなりゆく山際、少し明かりて、紫だちたる雲の細くたなびきたる");
      delay(1000*10);
    }
}

void loop () {

}