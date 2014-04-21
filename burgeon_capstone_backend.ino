#include <EEPROM.h>
#include <SPI.h>
#include <bitlash.h>
#include <RGBTile.h>

/*******************************************************************************
 * globals
 ******************************************************************************/
RGBTile myTile;

// buffer and required setup for saving programs to Arduino
#if defined(EEPROM_MICROCHIP_24XX32A)
	#define ENDDB 4095
	#define ENDEEPROM 4095
#else
	#define ENDDB E2END
	#define ENDEEPROM E2END
#endif
#define SIZE_PRG_BUFFER 512
#define ENDDB (E2END - SIZE_PRG_BUFFER) // reserve space in EEPROM
char prg_buffer[SIZE_PRG_BUFFER] = "twinkle()";

/*******************************************************************************
 * bitlash functions
 ******************************************************************************/
// twinkle() : defualt non-epileptic speed and using current board colors
// twinkle(color) : defualt speed but board colored with color first 
numvar bitlash_twinkle() {
    if(getarg(0) == 1) {
        numvar color = getarg(1);
        for(int i = 0; i < 5; i++)
            for(int j = 0; j < 5; j++)
                myTile.colorPixel(i, j, (uint32_t)color);
    }
    
    while(1) {
        myTile.twinkle();
        delay(250);
    }
    
    return (numvar) 0;
}

// draw(x, y) : color pixel white, no draw_all required
// draw(x, y, color) : color pixel with color
numvar bitlash_draw() {
    if(getarg(0) == 2) { // number of args actually passed is 5
        myTile.colorPixel(getarg(1), getarg(2), 0xffffff);
        myTile.drawAll();
    } else if(getarg(0) == 3) {
        myTile.colorPixel(getarg(1), getarg(2), getarg(3));
        myTile.drawAll();
    }

    // do nothing if arg count incorrect
    return (numvar) 1;
}

// write( str) # display string of characters
// write(str, color)
numvar bitlash_write() {
    //myTile.drawLetter('m');

    if(getarg(0) == 1 && isstringarg(1)) {
        char *s = (char *) getstringarg(1);
        
        for(int i = 0; s[i] != '\0'; i++) {
            myTile.drawLetter(s[i]);
            delay(500);
        }
    }
    
    return (numvar) 1;
}

// clear () # clear whole tile
// clear(x,y) # single led
numvar bitlash_clear() {
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            myTile.colorPixel(i, j, 0);
        }
    }
    myTile.drawAll();
}

/*******************************************************************************
 * main program
 ******************************************************************************/
void setup() {
    
    /* diable serial load
    Serial.begin(57600);
    int bytes_read = Serial.readBytes(prg_buffer, SIZE_PRG_BUFFER - 1);
    
    if(bytes_read) {
        prg_buffer[bytes_read] = '\0';
        for(int i = ENDDB; i < ENDEEPROM; i++) {
            EEPROM.write(i, prg_buffer[i - ENDDB]);    
        }
    } else {
        for(int i = ENDDB; i < ENDEEPROM; i++) {
            prg_buffer[i - ENDDB] = EEPROM.read(i);    
        }
    } // end disable serial load */ 
    
    initBitlash(57600);
    // all new function names MUST be lower case
    addBitlashFunction("twinkle", (bitlash_function) bitlash_twinkle);
    addBitlashFunction("draw", (bitlash_function) bitlash_draw);
    addBitlashFunction("write", (bitlash_function) bitlash_write);
    addBitlashFunction("clear", (bitlash_function) bitlash_clear);

    //doCommand(prg_buffer);
}

void loop() {
    runBitlash();
}
