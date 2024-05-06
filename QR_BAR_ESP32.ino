#include <GxEPD2_BW.h>
#include <qrcode.h>
#include "GxEPD2_display_selection_new_style.h"

#define MAX_DIGITS 12  
#define X_START 100 
#define X_START93 200
#define DEBUG 1

#if DEBUG
#define PRINTS(s)   do { Serial.print(F(s)); } while (false)
#define PRINT(s, v)  do { Serial.print(F(s)); Serial.print(v); } while (false)
#else
#define PRINTS(s)
#define PRINT(s, v)
#endif

// EPaper Display Pins (Adapt to your display)
const uint8_t CLK_PIN = 4;
const uint8_t DATA_PIN = 16;
const uint8_t CS_PIN = 17;
const uint8_t DC_PIN = 5;
const uint8_t RST_PIN = 23;

// QR Code Settings
QRCode QR;
const uint16_t MSG_BUF_SIZE = 500;
char mesg[MSG_BUF_SIZE] = { "000201010212021644038463016688030415522024063016688061661000306301668850825HDFC00000015020005953521626460010A0000005240128sgbipvtltd.63016688@hdfcbank27380010A00000052401203171449169711253331752045732530335654041.005802IN5912SGBI PVT LTD6009ERNAKULAM6106682022623605203171449169711253331707086301668863049e55____________________\0" };
const uint8_t QR_VERSION = 17;
const uint8_t QR_QUIET_ZONE = 80; 

// Barcode Variables
static int X_Position = X_START;
static int X_Position93 = X_START93;

// Function Prototypes (barcode functions)
static void drawCode_EAN13(char barCodeValue[MAX_DIGITS]);
static void drawCode_UPC(char barCodeValue[MAX_DIGITS]);
static void drawCode93(int c93);
static void drawStrip93(int StripType93);
static void drawStrip(int StripType);
void intr2(char *c);
void str2(char *s);
void check_sum2(int y);



enum {
  SPACE,
  SPACE2,
  SPACE3,
  BAR,
  SME_BAR
};

  char StartA[]="11010000100";                   // binary representation of numbers and alphabets of code 128
  char StartB[]="11010010000";                  
  char StartC[]="11010011100";
  char Stop[]="110001110101";
  char _00[]=" 11011001100";
  char _01[]="11001101100";
  char _02[]="11001100110";
  char _03[]="10010011000";
  char _04[]="10010001100";
  char _05[]="10001001100";
  char _06[]="10011001000";
  char _07[]="10011000100";
  char _08[]="10001100100";
  char _09[]="11001001000";
  char _10[]="11001000100";
  char _11[]="11000100100";
  char _12[]="10110011100";
  char _13[]="10011011100";
  char _14[]="10011001110";
  char _15[]="10111001100";
  char _16[]="10011101100";
  char _17[]="10011100110";
  char _18[]="11001110010";
  char _19[]="11001011100";
  char _20[]="11001001110";
  char _21[]="11011100100";
  char _22[]="11001110100";
  char _23[]="11101101110";
  char _24[]="11101001100";
  char _25[]="11100101100";
  char _26[]="11100100110";
  char _27[]="11101100100";
  char _28[]="11100110100";
  char _29[]="11100110010";
  char _30[]="11011011000";
  char _31[]="11011000110";
  char _32[]="11000110110";
  char _33[]="10100011000";
  char _34[]="10001011000";
  char _35[]="10001000110";
  char _36[]="10110001000";
  char _37[]="10001101000";
  char _38[]="10001100010";
  char _39[]="11010001000";
  char _40[]="11000101000";
  char _41[]="11000100010";
  char _42[]="10110111000";
  char _43[]="10110001110";
  char _44[]="10001101110";
  char _45[]="10111011000";
  char _46[]="10111000110";
  char _47[]="10001110110";
  char _48[]="11101110110";
  char _49[]="11010001110";
  char _50[]="11000101110";
  char _51[]="11011101000";
  char _52[]="11011100010";
  char _53[]="11011101110";
  char _54[]="11101011000";
  char _55[]="11101000110";
  char _56[]="11100010110";
  char _57[]="11101101000";
  char _58[]="11101100010";
  char _59[]="11100011010";
  char _60[]="11101111010";
  char _61[]="11001000010";
  char _62[]="11110001010";
  char _63[]="10100110000";
  char _64[]="10100001100";
  char _65[]="10010110000";
  char _66[]="10010000110";
  char _67[]="10000101100";
  char _68[]="10000100110";
  char _69[]="10110010000";
  char _70[]="10110000100";
  char _71[]="10011010000";
  char _72[]="10011000010";
  char _73[]="10000110100";
  char _74[]="10000110010";
  char _75[]="11000010010";
  char _76[]="11001010000";
  char _77[]="11110111010";
  char _78[]="11000010100";
  char _79[]="10001111010";
  char _80[]="10100111100";
  char _81[]="10010111100";
  char _82[]="10010011110";
  char _83[]="10111100100";
  char _84[]="10011110100";
  char _85[]="10011110010";
  char _86[]="11110100100";
  char _87[]="11110010100";
  char _88[]="11110010010";
  char _89[]="11011011110";
  char _90[]="11011110110";
  char _91[]="11110110110";
  char _92[]="10101111000";
  char _93[]="10100011110";
  char _94[]="10001011110";
  char _95[]="10111101000";
  char _96[]="10111100010";
  char _97[]="11110101000";
  char _98[]="11110100010";
  char _99[]="10111011110";
  char _100[]="10111101110";
  char _101[]="11101011110";
  char _102[]="11110101110";
  char _103[]="11010000100";
  char _104[]="11010010000";
  char _105[]="11010011100";

/***********************************************************************************************************/

 char Start[]="101011110";                    // binary representation of numbers and alphabets of code 93
  char zero[]="100010100";
  char one[]="101001000";
  char two[]="101000100";
  char three[]="101000010";
  char four[]="100101000";
  char five[]="100100100";
  char six[]="100100010";
  char seven[]="101010000";
  char eight[]="100010010";
  char nine[]="100001010";
  char A[]="110101000";
  char B[]="110100100";
  char C[]="110100010";
  char D[]="110010100";
  char E[]="110010010";
  char F[]="110001010";
  char G[]="101101000";
  char H[]="101100100";
  char I[]="101100010";
  char J[]="100110100";
  char K[]="100011010";
  char L[]="101011000";
  char M[]="101001100";
  char N[]="101000110";
  char O[]="100101100";
  char P[]="100010110";
  char Q[]="110110100";
  char R[]="110110010";
  char S[]="110101100";
  char T[]="110100110";
  char U[]="110010110";
  char V[]="110011010";
  char W[]="101101100";
  char X[]="101100110";
  char Y[]="100110110";
  char Z[]="100111010";
  char us[]="100101110";
  char dot[]="111010100";
  char sp[]="111010010";
  char dr[]="111001010";
  char bs[]="101101110";
  char pls[]="101110110";
  char mod[]="110101110";

  char sft1[]="100100110";
  char sft2[]="111011010";
  char sft3[]="111010110";
  char sft4[]="100110010";
 
enum {
  SPACE93,
  SPACE193,
  SPACE293,
  SPACE393,
  BAR93
};

/********************************Read Serial Function***************************************/

bool readSerial(void) {
  static char* cp = mesg;
  bool gotNewData = false;

  while (Serial.available()) {
    *cp = (char)Serial.read();

    if ((*cp == '\n') || (cp - mesg >= MSG_BUF_SIZE - 2)) { 
      *cp = '\0'; 
      cp = mesg; 
      gotNewData = true;
    } else { 
      cp++; 
    }
  }

  return gotNewData;
}



/*******************************Draw Code Function for EAN13*******************************************/

static void drawCode_EAN13( char barCodeValue[MAX_DIGITS] ) 
{
  X_Position = X_START;
  /* Draw Start Bars */
  char zero_EAN[]="LLLLLL";
  char one_EAN[]="LLGLGG";
  char two_EAN[]="LLGGLG";
  char three_EAN[]="LLGGGL";
  char four_EAN[]="LGLLGG";
  char five_EAN[]="LGGLLG";
  char six_EAN[]="LGGGLL";
  char seven_EAN[]="LGLGLG";
  char eight_EAN[]="LGLGGL";
  char nine_EAN[]="LGGLGL";

  char *x;
  int num = barCodeValue[0] - 48;
  switch (num){
    case 0:x=zero_EAN;
           break;
    case 1:x=one_EAN;
           break;
    case 2:x=two_EAN;
           break;
    case 3:x=three_EAN;
           break;
    case 4:x=four_EAN;
           break;
    case 5:x=five_EAN;
           break;
    case 6:x=six_EAN;
           break;
    case 7:x=seven_EAN;
           break;
    case 8:x=eight_EAN;
           break;
    case 9:x=nine_EAN;
           break;   
           
  }
  drawStrip(SPACE3);drawStrip(SME_BAR); drawStrip(SPACE); drawStrip(SME_BAR);
  /* Draw Left Numerical Digit */
  for (int i = 1; i < 7; i++)
  {
    int digit = barCodeValue[i] - 48;
      if(x[i-1]=='L'){
        switch (digit)
        {
          case 0:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 1:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 2:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
          case 3:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 4:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
          case 5:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 6:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR);
            break;
          case 7:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
          case 8:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR);
            break;
          case 9:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
        }
       }
       else if(x[i-1]=='G'){
        switch (digit)
        {
          case 0:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR);
            break;
          case 1:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
          case 2:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
          case 3:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 4:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 5:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 6:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 7:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 8:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 9:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR);
            break;
        }
       }
  }

 /* Draw Middle Bars */
 drawStrip(SPACE); drawStrip(SME_BAR); drawStrip(SPACE); drawStrip(SME_BAR); drawStrip(SPACE);

  /* Draw Right Numerical Digit Bars */
  for (int i = 7; i < 13; i++)
  {
    int digit = barCodeValue[i] - 48;
        switch (digit)
        {
          case 0:
            drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE);
            break;
          case 1:
            drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE);
            break;
          case 2:
            drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 3:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE);
            break;
          case 4:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 5:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE);
            break;
          case 6:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 7:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 8:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 9:
            drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE);
            break;
        }
  }
  
  /* Draw End Bars*/
  drawStrip(SME_BAR); drawStrip(SPACE); drawStrip(SME_BAR);drawStrip(SPACE2);
  
}

/**************************************Draw code function for UPC A*************************************************************************/

static void drawCode_UPC( char barCodeValue[MAX_DIGITS] ) 
{
  X_Position = X_START;
  /* Draw Start Bars */
  drawStrip(SPACE3);drawStrip(SME_BAR); drawStrip(SPACE); drawStrip(SME_BAR);
  /* Draw Left Numerical Digit */
  for (int i = 0; i < 6; i++)
  {
    int digit = barCodeValue[i] - 48;
        switch (digit)
        {
          case 0:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 1:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 2:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
          case 3:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 4:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
          case 5:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR);
            break;
          case 6:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR);
            break;
          case 7:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
          case 8:
            drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR);
            break;
          case 9:
            drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR);
            break;
        }
  }

 /* Draw Middle Bars */
 drawStrip(SPACE); drawStrip(SME_BAR); drawStrip(SPACE); drawStrip(SME_BAR); drawStrip(SPACE);

  /* Draw Right Numerical Digit Bars */
  for (int i = 6; i < 12; i++)
  {
    int digit = barCodeValue[i] - 48;
        switch (digit)
        {
          case 0:
            drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE);
            break;
          case 1:
            drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE);
            break;
          case 2:
            drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 3:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE);
            break;
          case 4:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 5:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE);
            break;
          case 6:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 7:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 8:
            drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE); drawStrip(SPACE);
            break;
          case 9:
            drawStrip(BAR); drawStrip(BAR); drawStrip(BAR); drawStrip(SPACE); drawStrip(BAR); drawStrip(SPACE); drawStrip(SPACE);
            break;
        }
  }
  
  /* Draw End Bars*/
  drawStrip(SME_BAR); drawStrip(SPACE); drawStrip(SME_BAR);drawStrip(SPACE2);
  
}


/***********************************Draw Strip Function for UPC A and EAN13********************************************/

static void drawStrip( int StripType )
{
#define Y_START     100              //Starting pixel of barcode(Y coordinate)
#define HEIGHT      100            // Barcode height
int BAR_WIDTH = 2;                 //How many pixels wide each bar/space will be

 
  for (int numBars = 0; numBars < BAR_WIDTH; numBars++ )     
  {
    
        switch ( StripType )      //Drawing lines for Barcode generation
        {
          case SPACE:
            display.drawLine( X_Position, Y_START, X_Position, Y_START + HEIGHT, 1 );  //display.drawLine( X0, Y0, X1, Y1, Color);   
            break;      
          case SPACE2:
            display.fillRect( X_Position, Y_START, X_Position+30, Y_START + HEIGHT, 1 );
            break;  
          case SPACE3:
            display.fillRect( 0, 0, 103, Y_START + HEIGHT, 1 );
            break;
          case BAR:
            display.drawLine( X_Position, Y_START, X_Position, Y_START + HEIGHT, 0 );
            break;
          case SME_BAR:
            display.drawLine( X_Position, Y_START, X_Position, Y_START + HEIGHT, 0 );
            break;
        }
    
        X_Position=X_Position+1;

  }


}
/******************************************code128***************************************************/

void intr(char *c){
    int n;
    for(n = 0;n < 11;n++){
      switch(c[n]){
        case '1':drawStrip93(BAR93); break;
        case '0':drawStrip93(SPACE93); break;
        
      }
  }
}
void str(char *s){
    display.fillScreen(GxEPD_WHITE);
    drawStrip93(SPACE393);
    char a;
    char *x;
    int v=0;
    int w=1;
    int sum=0;
    int mod=0;
    intr(StartB);
    for(a = 0;a < strlen(s);a++){
       switch(tolower(s[a])){
         case '0': x=_16; v=16; break;
         case '1': x=_17; v=17; break;
         case '2': x=_18; v=18; break;
         case '3': x=_19; v=19; break;
         case '4': x=_20; v=20; break;
         case '5': x=_21; v=21; break;
         case '6': x=_22; v=22; break;
         case '7': x=_23; v=23; break;
         case '8': x=_24; v=24; break;
         case '9': x=_25; v=25; break; 
       }
      //Serial.println(x);
      intr(x);
      sum=sum+(v*w);
      w=w+1;
    }
    mod=sum%103;
    //Serial.println(mod);
    //Serial.println(sum);
    check_sum((mod+1));
    x=Stop;
    for(int m = 0;m < 12;m++){
      switch(x[m]){
        case '1':drawStrip93(BAR93); break;
        case '0':drawStrip93(SPACE93); break;
       }
     }  
    drawStrip93(SPACE193);
    drawStrip93(SPACE293);
    
  }

void check_sum(int y){
  char *I;
  switch(y){
         case 0: I=_00;intr(I); break;
         case 1: I=_01;intr(I); break;
         case 2: I=_02;intr(I); break;
         case 3: I=_03;intr(I); break;
         case 4: I=_04;intr(I); break;
         case 5: I=_05;intr(I); break;
         case 6: I=_06;intr(I); break;
         case 7: I=_07;intr(I); break;
         case 8: I=_08;intr(I); break;
         case 9: I=_09;intr(I); break;
         case 10: I=_10;intr(I); break;
         case 11: I=_11;intr(I); break;
         case 12: I=_12;intr(I); break;
         case 13: I=_13;intr(I); break;
         case 14: I=_14;intr(I); break;
         case 15: I=_15;intr(I); break;
         case 16: I=_16;intr(I); break;
         case 17: I=_17;intr(I); break;
         case 18: I=_18;intr(I); break;
         case 19: I=_19;intr(I); break;
         case 20: I=_20;intr(I); break;
         case 21: I=_21;intr(I); break;
         case 22: I=_22;intr(I); break;
         case 23: I=_23;intr(I); break;
         case 24: I=_24;intr(I); break;
         case 25: I=_25;intr(I); break;
         case 26: I=_26;intr(I); break;
         case 27: I=_27;intr(I); break;
         case 28: I=_28;intr(I); break;
         case 29: I=_29;intr(I); break;
         case 30: I=_30;intr(I); break;
         case 31: I=_31;intr(I); break;
         case 32: I=_32;intr(I); break;
         case 33: I=_33;intr(I); break;
         case 34: I=_34;intr(I); break;
         case 35: I=_35;intr(I); break;
         case 36: I=_36;intr(I); break;
         case 37: I=_37;intr(I); break;
         case 38: I=_38;intr(I); break;
         case 39: I=_39;intr(I); break;
         case 40: I=_40;intr(I); break;
         case 41: I=_41;intr(I); break;
         case 42: I=_42;intr(I); break;
         case 43: I=_43;intr(I); break;
         case 44: I=_44;intr(I); break;
         case 45: I=_45;intr(I); break;
         case 46: I=_46;intr(I); break;
         case 47: I=_47;intr(I); break;
         case 48: I=_48;intr(I); break;
         case 49: I=_49;intr(I); break;
         case 50: I=_50;intr(I); break;
         case 51: I=_51;intr(I); break;
         case 52: I=_52;intr(I); break;
         case 53: I=_53;intr(I); break;
         case 54: I=_54;intr(I); break;
         case 55: I=_55;intr(I); break;
         case 56: I=_56;intr(I); break;
         case 57: I=_57;intr(I); break;
         case 58: I=_58;intr(I); break;
         case 59: I=_59;intr(I); break;
         case 60: I=_60;intr(I); break;
         case 61: I=_61;intr(I); break;
         case 62: I=_62;intr(I); break;
         case 63: I=_63;intr(I); break;
         case 64: I=_64;intr(I); break;
         case 65: I=_65;intr(I); break;
         case 66: I=_66;intr(I); break;
         case 67: I=_67;intr(I); break;
         case 68: I=_68;intr(I); break;
         case 69: I=_69;intr(I); break;
         case 70: I=_70;intr(I); break;
         case 71: I=_71;intr(I); break;
         case 72: I=_72;intr(I); break;
         case 73: I=_73;intr(I); break;
         case 74: I=_74;intr(I); break;
         case 75: I=_75;intr(I); break;
         case 76: I=_76;intr(I); break;
         case 77: I=_77;intr(I); break;
         case 78: I=_78;intr(I); break;
         case 79: I=_79;intr(I); break;
         case 80: I=_80;intr(I); break;
         case 81: I=_81;intr(I); break;
         case 82: I=_82;intr(I); break;
         case 83: I=_83;intr(I); break;
         case 84: I=_84;intr(I); break;
         case 85: I=_85;intr(I); break;
         case 86: I=_86;intr(I); break;
         case 87: I=_87;intr(I); break;
         case 88: I=_88;intr(I); break;
         case 89: I=_89;intr(I); break;
         case 90: I=_90;intr(I); break;
         case 91: I=_91;intr(I); break;
         case 92: I=_92;intr(I); break;
         case 93: I=_93;intr(I); break;
         case 94: I=_94;intr(I); break;
         case 95: I=_95;intr(I); break;
         case 96: I=_96;intr(I); break;
         case 97: I=_97;intr(I); break;
         case 98: I=_98;intr(I); break;
         case 99: I=_99;intr(I); break;
         case 100: I=_100;intr(I); break;
         case 101: I=_101;intr(I); break;
         case 102: I=_102;intr(I); break;
         case 103: I=_103;intr(I); break;
         case 104: I=_104;intr(I); break;
         case 105: I=_105;intr(I); break;
        
  }
 // Serial.println(I);
}


/*****************************************Function for drawing space  and bars for code93 and code128****************************************************************/
static void drawStrip93( int StripType93 )
{

#define Y_START     100
#define HEIGHT      100
#define SME_HEIGHT  HEIGHT
int BAR_WIDTH = 1;   //How many pixels wide each bar/space will be

  for (int numBars = 0; numBars < BAR_WIDTH; numBars++ )
  {

    switch ( StripType93 )
    {
      case SPACE193:
        display.drawLine( X_Position93, Y_START, X_Position93, Y_START + HEIGHT, 0 );
        break;       
      case SPACE293:
        display.fillRect( X_Position93+3, Y_START, X_Position93+200, Y_START + HEIGHT, 1 );
        break;  
      case SPACE393:
        display.fillRect( 0, 0, 103, Y_START + HEIGHT, 1 );
        break;
      case SPACE93:
        display.drawLine( X_Position93, Y_START, X_Position93, Y_START + HEIGHT, 1 );
        break; 
      case BAR93:
        display.drawLine( X_Position93, Y_START, X_Position93, Y_START + HEIGHT, 0 );
        break;
      
    }

    X_Position93=X_Position93+1;

  }
}

/****************************************************Code93**************************************************************************************/



void intr2(char *c){
    int n;
    for(n = 0;n < 9;n++){
      switch(c[n]){
        case '1':drawStrip93(BAR93); break;
        case '0':drawStrip93(SPACE93); break;
        
      }
  }
}
void str2(char *s){
    display.fillScreen(GxEPD_WHITE);
    drawStrip93(SPACE393);
    char a;
    char *x;
    int cw=0;
    int cv=0;
    int kw=0;
    int kv=0;
    int c_check=0;
    int k_check=0;
    int l=0;
    l=strlen(s);
    int L0=20;
    int L2 = l-20;
    int L3=15;
    int L4=l-14;
    intr2(Start);
    for(a = 0;a < strlen(s);a++){
       switch(tolower(s[a])){
         case '0': x=zero;cw=0; break;
         case '1': x=one;cw=1; break;
         case '2': x=two;cw=2; break;
         case '3': x=three;cw=3; break;
         case '4': x=four;cw=4; break;
         case '5': x=five;cw=5; break;
         case '6': x=six;cw=6; break;
         case '7': x=seven;cw=7;break;
         case '8': x=eight;cw=8; break;
         case '9': x=nine;cw=9; break;
         case 'a': x=A;cw=10; break;
         case 'b': x=B;cw=11; break;
         case 'c': x=C;cw=12; break;
         case 'd': x=D;cw=13; break;
         case 'e': x=E;cw=14; break;
         case 'f': x=F;cw=15; break;
         case 'g': x=G;cw=16; break;
         case 'h': x=H;cw=17; break;
         case 'i': x=I;cw=18; break;
         case 'j': x=J;cw=19; break;
         case 'k': x=K;cw=20; break;
         case 'l': x=L;cw=21; break;
         case 'm': x=M;cw=22; break;
         case 'n': x=N;cw=23; break;
         case 'o': x=O;cw=24; break;
         case 'p': x=P;cw=25; break;
         case 'q': x=Q;cw=26; break;
         case 'r': x=R;cw=27; break;
         case 's': x=S;cw=28; break;
         case 't': x=T;cw=29; break;
         case 'u': x=U;cw=30; break;
         case 'v': x=V;cw=31; break;
         case 'w': x=W;cw=32; break;
         case 'x': x=X;cw=33; break;
         case 'y': x=Y;cw=34; break;
         case 'z': x=Z;cw=35; break;
         case '_': x=us;cw=36; break;
         case '.': x=dot;cw=37; break;
         case ' ': x=sp;cw=38; break;
         case '$': x=dr;cw=39; break;
         case '/': x=bs;cw=40; break;
         case '+': x=pls;cw=41; break;
         case '%': x=mod;cw=42; break;
         
       }


       if(l>15 && l<=20){
              
         cv=cw*l+cv;
         l=l-1;
         
         if(L4==0){
          kw=cw*(L3)+kw;
          L3=L3-1;
         }
         else{
          kw=cw*L4+kw;
          L4=L4-1;
          //Serial.print(kw);
         }
        
        intr2(x);
       }

       else if(l>20){
             if(L2==0){   
             cv=(cw*L0)+cv;
             L0=L0-1;
             }
             else{
             cv=(cw*L2)+cv;
             L2=L2-1;
             }
             if(L4==0){
              kw=cw*(L3)+kw;
              L3=L3-1;
             }
             else{
              kw=cw*L4+kw;
              L4=L4-1;
              //Serial.print(kw);
             }
            
            intr2(x);
           
        
       }
       
       
       else{
        cv=cw*l+cv;
        kw=cw*(l+1)+kw;
        l=l-1;
        intr2(x);
      }
    }
    //Serial.println(cv);
   // Serial.println(kw);
    c_check=cv%47;
    kv=c_check+kw;
    k_check=kv%47;
   // Serial.println(c_check);
   // Serial.println(k_check);
    check_sum2(c_check);
    check_sum2(k_check);
    intr2(Start);
    drawStrip93(SPACE193);
    drawStrip93(SPACE293);
    
  }

void check_sum2(int y){
  char *I;
  switch(y){
         case 0: I=zero;intr2(I); break;
         case 1: I=one;intr2(I); break;
         case 2: I=two;intr2(I); break;
         case 3: I=three;intr2(I); break;
         case 4: I=four;intr2(I); break;
         case 5: I=five;intr2(I); break;
         case 6: I=six;intr2(I); break;
         case 7: I=seven;intr2(I);break;
         case 8: I=eight;intr2(I);break;
         case 9: I=nine;intr2(I); break;
         case 10: I=A;intr2(I); break;
         case 11: I=B;intr2(I); break;
         case 12: I=C;intr2(I); break;
         case 13: I=D;intr2(I); break;
         case 14: I=E;intr2(I); break;
         case 15: I=F;intr2(I); break;
         case 16: I=G;intr2(I); break;
         case 17: I=H;intr2(I); break;
         case 18: I=I;intr2(I); break;
         case 19: I=J;intr2(I); break;
         case 20: I=K;intr2(I); break;
         case 21: I=L;intr2(I); break;
         case 22: I=M;intr2(I); break;
         case 23: I=N;intr2(I); break;
         case 24: I=O;intr2(I); break;
         case 25: I=P;intr2(I); break;
         case 26: I=Q;intr2(I); break;
         case 27: I=R;intr2(I); break;
         case 28: I=S;intr2(I); break;
         case 29: I=T;intr2(I); break;
         case 30: I=U;intr2(I); break;
         case 31: I=V;intr2(I); break;
         case 32: I=W;intr2(I); break;
         case 33: I=X;intr2(I); break;
         case 34: I=Y;intr2(I); break;
         case 35: I=Z;intr2(I); break;
         case 36: I=us;intr2(I); break;
         case 37: I=dot;intr2(I); break;
         case 38: I=sp;intr2(I); break;
         case 39: I=dr;intr2(I); break;
         case 40: I=bs;intr2(I); break;
         case 41: I=pls;intr2(I); break;
         case 42: I=mod;intr2(I); break;
         case 43: I=sft1;intr2(I); break;
         case 44: I=sft2;intr2(I); break;
         case 45: I=sft3;intr2(I); break;
         case 46: I=sft4;intr2(I); break;
  }
}


void printQRBlock(uint16_t x, uint16_t y, uint8_t size, uint16_t col)
// Draw a square block of size pixels. Drawing individual pixels as this is faster
// that line segments and much faster that a filled rectangle.
{
  for (uint8_t i = 0; i < size; i++)
    for (uint8_t j = 0; j < size; j++)
      display.drawPixel(x + i, y + j, col);
}

void displayQRCode(char *msg, uint16_t x0, uint16_t y0)
// Paged display of the QR code with top left corner at (x0,y0)
// on the display
{
  uint8_t QRData[qrcode_getBufferSize(QR_VERSION)];
  uint8_t blockSize;
  uint8_t page = 0;
//  PRINT("\n\nQR buffer Size: ", qrcode_getBufferSize(QR_VERSION));
  qrcode_initText(&QR, QRData, QR_VERSION, ECC_LOW, msg);
  //  PRINT("\n\nQR Size: ", QR.size);
  //  PRINT("\nLocation: ", x0);
  //  PRINT(",", y0);
  //  PRINT("\nDisplay Height: ", display.height());

  blockSize = (display.height() - (1.5 * QR_QUIET_ZONE)) / QR.size;
  //  PRINT("\nBlock Size: ", blockSize);


//  Serial.print(msg);//mesg
//  Serial.print(F("\nsizeof - "));
//  Serial.print(sizeof(msg));//mesg
  display.firstPage();
  do
  {
    page++;
    // Space all around
    display.fillRect(x0, y0,
                     x0 + QR.size + (2 * QR_QUIET_ZONE), y0 + QR.size + (2 * QR_QUIET_ZONE),
                     GxEPD_WHITE);

    // For each vertical module
    for (uint8_t y = 0; y < QR.size; y++)
    {
      // Eor each horizontal module
      for (uint8_t x = 0; x < QR.size; x++)
      {
        if (qrcode_getModule(&QR, x, y))
          printQRBlock(x0 + (x * blockSize) + QR_QUIET_ZONE + 40,
                       y0 + (y * blockSize) + QR_QUIET_ZONE - 10,
                       blockSize,
                       (qrcode_getModule(&QR, x, y)) ? GxEPD_BLACK : GxEPD_WHITE);
      }
    }
  } while (display.nextPage());

  //Serial.print(F("OK\n"));
//  PRINT(" @ page ", page);
}





void setup(void) {
  Serial.begin(9600); 

  display.init();

  // Choose the rotation that works best for both barcodes and QR codes:
  display.setRotation(0); // Or use display.setRotation(1);
}

void loop(void)
{
  if (readSerial()) {
    // Check for clear command
    if (strncmp(mesg, "clear", 5) == 0) {
      display.fillRect(0, 0, display.width(), display.height(), GxEPD_WHITE); // Clear screen
      display.display();
      Serial.println("0"); // Send confirmation
    }
    // Check for QR code command
    else if (strncmp(mesg, "QR:", 3) == 0) { 
      char *qrMsg = mesg + 3; // Extract QR message (skip the 'QR:' prefix)
      display.fillRect(0, 0, display.width(), display.height(), GxEPD_WHITE); // Clear screen
      displayQRCode(qrMsg, 0, 0);
      Serial.println("0"); // Send confirmation
    }
    // Check for barcode command
    else if (strncmp(mesg, "BR:", 3) == 0) {
      char *barcodeMsg = mesg + 3; // Extract barcode message (skip the 'BR:' prefix)
      display.fillRect(0, 0, display.width(), display.height(), GxEPD_WHITE); // Clear screen

      int len = strlen(barcodeMsg);
      int q;
      if(len <= 20) q = 2;
      else q = 3;

      String s = String(barcodeMsg); 

      if(len == 12) {
        drawCode_UPC(barcodeMsg);
        display.display();
        Serial.println("0");
      } else if(len == 13) {
        drawCode_EAN13(barcodeMsg);
        display.display();
        Serial.println("0");
      } else if(len > 20) {
        X_Position93 = X_START93 - (15 * (len / q)); 
        str2(barcodeMsg);
        display.display();
        Serial.println("0");
      } else { 
        X_Position93 = X_START93 - (15 * (len / q));
        str(barcodeMsg);
        display.display();
        Serial.println("0");
      }
    } else {
      Serial.println("13"); //unrecognized commmand
    }
  }
}
