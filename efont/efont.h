#ifndef __EFONT_H__
#define __EFONT_H__

#include <arduino.h>
#include "efontFontData.h"

char* efontUFT8toUTF16( uint16_t *pUTF16, char *pUTF8 ) {

  // 1Byte Code
  if ( pUTF8[0] < 0x80 ) {
    // < 0x80 : ASCII Code
    *pUTF16 = pUTF8[0];
    return pUTF8 + 1;
  }

  // 2Byte Code
  if ( pUTF8[0] < 0xE0 )  {
    *pUTF16 = ( ( pUTF8[0] & 0x1F ) << 6 ) + ( pUTF8[1] & 0x3F );
    return pUTF8 + 2;
  }

  // 3Byte Code
  if ( pUTF8[0] < 0xF0 ) {
    *pUTF16 = ( ( pUTF8[0] & 0x0F ) << 12 ) + ( ( pUTF8[1] & 0x3F ) << 6 ) + ( pUTF8[2] & 0x3F );
    return pUTF8 + 3;
  } else {
    // 4byte Code
    *pUTF16 = 0;
    return pUTF8 + 4;
  }
}

void getefontData( uint8_t *font, uint16_t fontUTF16 ) {
  for( int i = 0 ; i < sizeof( efontFontList ) / 2 ; i++ ){
    if( fontUTF16 <= efontFontList[i] ){
      if( efontFontList[i] == fontUTF16 ){
        // copy
        memcpy( font, efontFontData + i*32, 32 );
        return;
      } else {
        // No Font
        memset( font, 0, 32 );
        return;
      }
    }
  }

  // No Font
  memset( font, 0, 32 );
}

#endif
