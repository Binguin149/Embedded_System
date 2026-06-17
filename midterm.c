/*
 * File:   midterm.c
 * Author: Huu Phuoc
 *
 * Created on June 6, 2026, 9:30 AM
 */
#include <xc.h>
#include "userdef.h"
#define F_CPU 1000000
/*----------------------------------------------------------------------------*/
void LCDInit()
{
    LCD_INS =0x38 ;LCDE_H ;_delay_us(50); LCDE_L;    _delay_us(50);
    _delay_us(200) ;
    LCD_INS =0x0c ; LCDE_H ;_delay_us(50); LCDE_L;   _delay_us(50);
    _delay_us(200) ;
    LCD_INS =0x06 ; LCDE_H ;_delay_us(50); LCDE_L;   _delay_us(50);
    _delay_us(200) ;
    LCD_INS =0x01 ;LCDE_H ;_delay_us(50); LCDE_L;    _delay_us(50);
    _delay_ms(20) ;
}
/*----------------------------------------------------------------------------*/
void PrintL (const char *str, unsigned line, unsigned col)
{
    unsigned char add;
    switch (line)
    {
            case 0: add = 0x80; break;
            case 1: add = 0xC0; break;
            case 2: add = 0x94; break;
            case 3: add = 0xD4; break; 
    }
    LCD_INS = add + col; 
    LCDE_H; _delay_us(50); 
    LCDE_L; _delay_us (50);
    while (*(str)!='\0')
    {
        LCD_DATA = *str++; 
        LCDE_H; _delay_us(50); 
        LCDE_L; _delay_us(50);
    }
}
/*----------------------------------------------------------------------------*/
void MenuDisplay(Menu *menu, unsigned char select)
{
    PrintL(menu -> Title,0,0);
    PrintL(menu -> List1,1,0);
    PrintL(menu -> List2,2,0);
    PrintL(menu -> List3,3,0);
    PrintL(">",select,0);    
}
/*----------------------------------------------------------------------------*/
void main(void) 
{
    unsigned char select = 1;
    Menu *menu;
    MCUCR |= 0x80;
    XMCRA = 0;
    DDRG = 0x0f;
    menu = &MainMenu;
    LCDInit();
    MenuDisplay(menu, select);
    while(1)
    {
        Readkey;
        if (!Buttons.UP)
        {
            while(!Buttons.UP) Readkey;
            select = (select==1)?3:select-1;
            MenuDisplay(menu, select);
        }
        if (!Buttons.DOWN)
        {
            while(!Buttons.DOWN) Readkey;
            select = (select==3)?1:select+1;
            MenuDisplay(menu, select);
        }
        if (!Buttons.NEXT)
        {
            while(!Buttons.NEXT) Readkey;
            switch (select)
            {
                case 1 :menu= (menu->Menulist1 == NULL)? menu: menu->Menulist1; 
                break;
                case 2 :menu= (menu->Menulist2 == NULL)? menu: menu->Menulist2; 
                break;
                case 3 :menu= (menu->Menulist3 == NULL)? menu: menu->Menulist3; 
                break;
            }
            MenuDisplay(menu, select);
        }
        if (!Buttons.BACK)
        {
            while(!Buttons.BACK) Readkey;
            menu= (menu->PRE == NULL)? menu: menu->PRE;
            MenuDisplay(menu, select);
        }
        if (!Buttons.SET)
        {
            while(!Buttons.SET) Readkey;
            switch (select)
            {
                case 1: if (menu->Activate_1!= NULL) 
                {
                    if (menu->MenuID==Device_LED7) menu->Activate_1(menu->MenuID,CU);
                    else menu->Activate_1(menu->MenuID,FORWARD);
                }                   
                break; 
                case 2: if (menu->Activate_2!= NULL) 
                {
                    if (menu->MenuID==Device_LED7) menu->Activate_2(menu->MenuID,CD);
                    else menu->Activate_2(menu->MenuID, BACKWARD);
                }
                break;
                case 3: if (menu->Activate_3!= NULL) 
                {
                    if (menu->MenuID==Device_LED7) menu->Activate_3(menu->MenuID,SET255);
                    else menu->Activate_3(menu->MenuID, STOP);
                }
                break;
            }
        }
    }
    return;
}
