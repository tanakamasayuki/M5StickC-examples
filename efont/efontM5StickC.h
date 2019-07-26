#ifndef __EFONT_M5STICKC_H__
#define __EFONT_M5STICKC_H__

#include <M5StickC.h>
#include "efont.h"

//#define EFONT_DEBUG

void printEfont(char *str) {
  int posX = M5.Lcd.getCursorX();
  int posY = M5.Lcd.getCursorY();
  uint8_t textsize = M5.Lcd.textsize;
  uint32_t textcolor = M5.Lcd.textcolor;
  uint32_t textbgcolor = M5.Lcd.textbgcolor;
  
  byte font[32];
  
  while( *str != 0x00 ){
    // 改行処理
    if( *str == '\n' ){
      // 改行
      posY += 16 * textsize;
      posX = M5.Lcd.getCursorX();
      str++;
      continue;
    }

    // フォント取得
    uint16_t strUTF16;
    str = efontUFT8toUTF16( &strUTF16, str );
    getefontData( font, strUTF16 );

    // 文字横幅
    int width = 16 * textsize;
    if( strUTF16 < 0x0100 ){
      // 半角
      width = 8 * textsize;
    }


#ifdef EFONT_DEBUG
    Serial.printf( "str : U+%04X\n", strUTF16 );
#endif


    // 背景塗りつぶし
    M5.Lcd.fillRect(posX, posY, width, 16 * textsize, textbgcolor);

    // 取得フォントの確認
    for (uint8_t row = 0; row < 16; row++) {
      word fontdata = font[row*2] * 256 + font[row*2+1];
      for (uint8_t col = 0; col < 16; col++) {
#ifdef EFONT_DEBUG
        Serial.write( ( (0x8000 >> col) & fontdata ) ? "#" : " " );
#endif
        if( (0x8000 >> col) & fontdata ){
          int drawX = posX + col * textsize;
          int drawY = posY + row * textsize;
          if( textsize == 1 ){
            M5.Lcd.drawPixel(drawX, drawY, textcolor);
          } else {
            M5.Lcd.fillRect(drawX, drawY, textsize, textsize, textcolor);
          }
        }
      }
#ifdef EFONT_DEBUG
        Serial.write( "\n" );
#endif
    }

    // 描画カーソルを進める
    posX += width;

    // 折返し処理
    if( M5.Lcd.width() <= posX ){
      posX = 0;
      posY += 16 * textsize;
    }
  }

  // カーソルを更新
  M5.Lcd.setCursor(posX, posY);
}

void printEfont(char *str, int x, int y) {
  M5.Lcd.setCursor(x, y);
  printEfont(str);
}

void printEfont(char *str, int x, int y, int textsize) {
  M5.Lcd.setCursor(x, y);
  M5.Lcd.textsize = textsize;
  printEfont(str);
}

#endif
