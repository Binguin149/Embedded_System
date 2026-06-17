/* 
 * File:   
 * Author: Phuoc / bin
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stddef.h>
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

#define EXMEM_ADD 0x1100
// CS 
#define CS0 0
#define CS1 1
#define CS2 2
#define CS3 3
#define CS4 4
#define CS5 5
#define CS6 6
//define LED 7 doan
#include <util/delay.h>
#define LED7D1 *(unsigned char*)(EXMEM_ADD+CS0)
#define LED7D2 *(unsigned char*)(EXMEM_ADD+CS1)
#define LED7D3 *(unsigned char*)(EXMEM_ADD+CS2)
unsigned char SegCode[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0XC6,0xA1,0x86,0x8E};
//define LCD
#define LCDE_H (PORTG|=(1<<3))
#define LCDE_L (PORTG&=~(1<<3))
#define LCD_DATA *(unsigned char *) (EXMEM_ADD+CS3)
#define LCD_INS *(unsigned char *) (EXMEM_ADD+CS4) 
//define dong co motor
#define MOTOR_ACTIVATE (*(volatile unsigned char *)(EXMEM_ADD+CS5) = *(unsigned char *)(&Motors))
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
//define nut nhan
#define Readkey (*(unsigned char*)(&Buttons) = *(unsigned char *)(EXMEM_ADD+CS6))

#define CU 3
#define CD 4
#define SET255 5
unsigned char count = 0;
#define Device_LED7 0
#define Device_Motor1 1
#define Device_Motor2 2
/*----------------------------------------------------------------------------*/
void SevenSegDisplay(unsigned int value)
{
    LED7D1 = SegCode[value%10];
    _delay_ms(2);
    LED7D2 = SegCode[(value/10)%10];
    _delay_ms(2);
    LED7D3 = SegCode[(value/100)%10];
    _delay_ms(2);
}
/*----------------------------------------------------------------------------*/
struct Motor
{
    unsigned char DC1 : 2;
    unsigned char DC2 : 2;
    unsigned char DCfree : 8;
};
struct Motor Motors;
struct Button
{
    unsigned char NEXT : 1;
    unsigned char BACK : 1;
    unsigned char SET : 1;
    unsigned char UP : 1;
    unsigned char DOWN : 1;
};
struct Button Buttons;
/*----------------------------------------------------------------------------*/
void Activation(char Device, char Status)
{
    switch(Device)
    {
        case Device_LED7: 
        if(Status == CU) {count++; SevenSegDisplay(count); }
        else if(Status == CD) {count--; SevenSegDisplay(count);}
        else count = 255; SevenSegDisplay(count);
        
        case Device_Motor1: 
        if (Status == FORWARD) Motors.DC1 = FORWARD; 
        else if (Status == BACKWARD )Motors.DC1 = BACKWARD;
        else Motors.DC1 = STOP;
        MOTOR_ACTIVATE;
        break;
        
        case Device_Motor2: 
        if (Status == FORWARD) Motors.DC2 = FORWARD; 
        else if (Status == BACKWARD)Motors.DC2 = BACKWARD;
        else Motors.DC2 = STOP;
        MOTOR_ACTIVATE;
        break;
    }
}
/*----------------------------------------------------------------------------*/
typedef struct Linker
{
    char MenuID;
    struct Linker *PRE;    
    char Title[20];
    char List1[20];    
    struct Linker *Menulist1; void (*Activate_1)(char, char); 
    char List2[20];    
    struct Linker *Menulist2; void (*Activate_2)(char, char);  
    char List3[20];    
    struct Linker *Menulist3; void (*Activate_3)(char, char);
}
Menu ;
Menu MainMenu, Devices, Counter, Motor1, Motor2;
/*----------------------------------------------------------------------------*/
Menu MainMenu = 
{
    0,
    NULL,
    " Embedded system    ",
    " Huynh Huu Phuoc    ",&Devices,NULL,
    " Stu.iD : 22161069  ",&Devices,NULL,
    " PRESS NEXT TO BEGIN",&Devices,NULL,
};
/*----------------------------------------------------------------------------*/
Menu Devices =
{
    0,
    &MainMenu,
    " Devices            ",
    " Counter            ",&Counter,NULL,
    " Motor 1            ",&Motor1,NULL,
    " Motor 2            ",&Motor2,NULL,
};
/*----------------------------------------------------------------------------*/
Menu Counter =
{
    Device_LED7,
    &Devices,
    " Counter            ",
    " Count Up           ",NULL,&Activation,
    " Count Down         ",NULL,&Activation,
    " Set to 255         ",NULL,&Activation,
};
/*----------------------------------------------------------------------------*/
Menu Motor1 =
{
    Device_Motor1,
    &Devices,
    " Motor 1            ",
    " Forward            ",NULL,&Activation,
    " Backward           ",NULL,&Activation,
    " Turn Off Motor     ",NULL,&Activation,
};
/*----------------------------------------------------------------------------*/
Menu Motor2 =
{
    Device_Motor2,
    &Devices,
    " MOTOR 2            ",
    " Forward            ",NULL,&Activation,
    " Backward           ",NULL,&Activation,
    " Turn Off Motor     ",NULL,&Activation,
};
/*----------------------------------------------------------------------------*/