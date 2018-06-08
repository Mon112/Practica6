#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#include <DHT11.h>
#use fast_io(a)
#use fast_io(d)
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)

//contador de anillo respecto a los activadores de los displays para el control del uso y mostrado de los displays
//timer0 con el despliegue de la memoria de video(displays)

int cambio=0;
int contador=1;
int bcdSin[10]={126, 48,109,121, 51, 91, 95,112,127,123};
int bcdCon[10]={254,176,237,249,179,219,223,240,255,251};
unsigned int8 dato[9] ={0};
#INT_TIMER0
void sti_TIMER0(){
   switch(contador){
      case 1:
         output_a(contador);
         output_d(bcdSin[dato[contador]]);
      break;
      case 2:
         output_a(contador);
         output_d(bcdCon[dato[contador]]);
      break;
      case 4:
         output_a(contador);
         output_d(bcdSin[dato[contador]]);
      break;
      case 8:
         output_a(contador);
         output_d(bcdSin[dato[contador]]);
      break;
   }
   contador*=2;
   if(contador>8){
      contador=1;
   } 
}

#INT_RB
void isr_RB(){
   if(1==input(PIN_B4)){
      cambio++;
      if(cambio==2){
         cambio=0;
      }
   }
}

void main(void) {
   set_tris_d(0x00);
   set_tris_a(0x00);
   set_tris_e(0x00); 
   set_tris_b(0xff);
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_128 | RTCC_8_BIT);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_RB);
   enable_interrupts(GLOBAL);
   
   while (1) { 
      Start_signal();
      if(check_response()){                    
         RhByte1 = Read_Data();          
         RhByte2 = Read_Data();                 
         TemByte1 = Read_Data();                   
         TemByte2 = Read_Data();                     
         CheckSum = Read_Data();                       
         if(CheckSum == ((RhByte1 + RhByte2 + TemByte1 + TemByte2) )){
            switch(cambio){
               case 0:
                  output_e(0x02);
                  dato[1]=(int)TemByte1/10;
                  dato[2]=(int)TemByte1%10;
                  dato[4]=(int)TemByte2/10;
                  dato[8]=(int)TemByte2%10;
               break;
               case 1:
                  output_e(0x01);
                  dato[1]=(int)RhByte1/10;
                  dato[2]=(int)RhByte1%10;
                  dato[4]=(int)RhByte2/10;
                  dato[8]=(int)RhByte2%10;
               break;
            }
         }
      } 
   }

