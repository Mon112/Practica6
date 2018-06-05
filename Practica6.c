#include <18F4620.h>
#include <stdio.h>
#include <stdlib.h>
#include <DHT11.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)


unsigned int unidad=0,decena =0, unidadHR=0, decenaHR = 0;
unsigned int8 T_byte1, T_byte2, RH_byte1, RH_byte2;
unsigned int8 Sum;
//#define Dht11 PIN_c4 


void main(void) {
   setup_oscillator(OSC_16MHZ);
   set_tris_a(0x00);
   set_tris_b(0x00);
   set_tris_e(0x00);
   
   
while (true) {
   
   Check_response();
   output_a(0x0F);
   Start_Signal();
   if(Check_response())
   {
      //output_a(0x01);
      RH_byte1 = Read_Data();
      RH_byte2 = Read_Data();
      T_byte1 = Read_Data();
      T_byte2 = Read_Data();
      Sum= Read_Data();
      if(Sum == ((RH_byte1+RH_byte2+T_byte1+T_byte2))){
         decenaHR = RH_byte1/10;
         unidadHR = RH_byte1%10;
         decena = t_byte1/10;
         unidad = T_byte1%10;
         output_d(decenaHR);   
         output_a(0x01);
      }
   }
      
   
   
}


}
