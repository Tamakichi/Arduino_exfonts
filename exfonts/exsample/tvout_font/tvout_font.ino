//
// TVOut�Ń}���`�t�H���g�\�� by ���܋g���� 2016/03/30
//  

#include <SPI.h>
#include <W25Q64.h>
#include <exfonts.h>
#include <TVout.h>

// TVout�̔h���N���X�̒�`���A
// bitmap���\�b�h���t���b�V���������ł͂Ȃ��ASRAM�Ή�������
class exTVout : public TVout {
  public:
    void bitmap(uint8_t x, uint8_t y, const uint8_t* bmp, 
                uint16_t i, uint8_t width, uint8_t lines);
};

// bitmap���\�b�h
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

exTVout TV;  // TV�o�̓I�u�W�F�N�g
exfonts ft;  // �t�H���g�Ǘ��I�u�W�F�N�g

// �w��ʒu�ɂP�����\��
void mputc(uint8_t x, uint8_t y, uint16_t code) {
  byte buf[MAXFONTLEN]; 
  ft.getFontData(buf,code); // �t�H���g�f�[�^�̎擾  
  TV.bitmap(x,y, buf ,0,ft.getWidth(), ft.getHeight());
}

// �w��ʒu�ɕ�����\��
void mprint(uint8_t x, uint8_t y, char* str) {
  
  uint16_t wstr[64];
  int8_t   len;
  
  len = exfonts::Utf8ToUtf16(wstr, str);  // UTF8����UTF16�ɕϊ�����
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
  ft.init();  // �t�H���g�Ǘ��̏�����
  TV.begin(NTSC, 120, 80); // TVOut�J�n
  
  //7��ނ̃t�H���g�T�C�Y�ŕ�����\��
  while(1)
    for (uint8_t i =0 ; i <7; i++) {
      TV.fill(0);
      ft.setFontSize(i);
      mprint(0,0,"�t�͂�͂����ڂ̂₤�₤�����Ȃ�䂭�R�ہA����������āA����������_�ׂ̍����Ȃт�����");
      delay(1000*10);
    }
}

void loop () {

}