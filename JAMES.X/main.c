#include <18F4550.h>
#include <xc.h>
#include <stdio.h>  //libreria de c
#include "config.h" //configuraciones de los fuses
#include "LIBRERIA_LCD.h"  //librer?a LCD
#include "LIBRERIA_ADC.h"  //librer?a conversor ADC
#include "UART_LIB.h"  //libreria uart

#define ACEPTAR PORTCbits.RC0 // Pin RC0 del puerto C 
#define ARRIBA PORTCbits.RC1 // Pin RC1 del puerto C 
// Definicion de variables
int opcion=1,conver;
char txt[10]=" ";
char aux[20]=" ";
float hum;
float temp,tempi;
int conver1,conver2,conver3, conver4;
float peso, oxigeno;
char t;

void MenuP(){
     LCD_Cmd(1); //Limpiar display
     // Secuencia de texto en el LCD
     LCD_Out(1,1,"   USER INTERFACE");
     LCD_Out(2,1,"1.Heater and Fan");
     LCD_Out(3,1,"2.Humidity");
     LCD_Out(4,1,"3.O2 Level");
     LCD_Out(4,13,"opcion: ");
     sprintf(txt,"%i",opcion);
     LCD_Out(4,20,txt);
     
       do{
         //Cambio de Opcion en el LCD  
        if (ARRIBA==1){
           opcion++;
           if (opcion>3){
              opcion=1;
           }
           while(ARRIBA==1);
        }
        sprintf(txt,"%i",opcion);
        LCD_Out(4,20,txt);
     }while(ACEPTAR==0);
     //Despliegue de las opciones en el menu
     switch (opcion){
         case 1: LCD_Cmd(1);
                 LCD_Out(1,1,"REGULATE TEMPERATURE");
                 LCD_Out(4,1,"1.VOLVER");
                 LCD_Out(4,13,"opcion: ");
                 sprintf(txt,"%i",opcion);
                 LCD_Out(4,20,txt);
                 do{
                   
                     conver1 = ADC_Read(0); //canal 0 del ADC
                     temp = conver1*0.4887; // Se linealiza el ADC con el rango deseado 
                     sprintf (aux,"TEMP(INC):%3.1f",temp) ; // 3 enteros y 1 decimal
                     LCD_Out(2,1,aux); //Mostrar el valor convertido
                     
                     conver2 = ADC_Read(1); //canal 1 del ADC
                     tempi = conver2*0.0391; // Se linealiza el ADC con el rango deseado 
                     sprintf (aux,"TEMP(NEO):%3.1f",tempi) ; // 3 enteros y 1 decimal
                     LCD_Out(3,1,aux);//Mostrar el valor convertido
                     //Delays para calentar la resistencia segun sea el valor del LM35
                     if(temp<29){
                      PORTD=1;
                      __delay_ms(10);
                      PORTD=0;
                      __delay_ms(350);
                     }
                      if(temp>29&&temp<31){
                      PORTD=1;
                      __delay_ms(5);
                      PORTD=0;
                      __delay_ms(350);
                     }
                      if(temp>31&&temp<33){
                      PORTD=1;
                      __delay_ms(3);
                      PORTD=0;
                      __delay_ms(350);
                     }
                      if(temp>33&&temp<37){
                      PORTD=1;
                      __delay_ms(1);
                      PORTD=0;
                      __delay_ms(350);
                     }
                     
                     if(temp>39){
                         PORTD=2;
                     }else{PORTD=0;
                     }
                     
                
                 }while(ACEPTAR==0);
                 break;
                 
        case 2:  LCD_Cmd(1);
                 LCD_Out(1,1,"   USER INTERFACE");
                 LCD_Out(4,1,"2.VOLVER");
                 LCD_Out(4,13,"opcion: ");
                 sprintf(txt,"%i",opcion);
                 LCD_Out(4,20,txt);
                 do{
                     conver = ADC_Read(2);
                     hum = conver*0.097;
                     sprintf (aux,"hum:%3.0f",hum) ; 
                     LCD_Out(2,1,aux);
                     LCD_Chr(2,10,'%');
                 
                 }while(ACEPTAR==0);
                 break;
                 
        case 3:  LCD_Cmd(1);
                 LCD_Out(1,1,"   USER INTERFACE");
                 LCD_Out(4,1,"3.VOLVER");
                 LCD_Out(4,13,"opcion: ");
                 sprintf(txt,"%i",opcion);
                 LCD_Out(4,20,txt);
                 do{
                     conver3 = ADC_Read(3);// canal 3
                     peso = conver3*0.00488;// Linealiza con el rango deseado
                     sprintf (aux,"Peso(Kg):%1.2f",peso); // convierte
                     LCD_Out(2,1,aux); // se muestra en el LCD
                     
                     conver4 = ADC_Read(4); // canal 4
                     ox?geno = conver4*0.0977;// Linealiza con el rango deseado
                     sprintf (aux,"Ox?geno:%3.1f",ox?geno);// convierte
                     LCD_Out(3,1,aux); // se muestra en el LCD
                      
                     if(oxigeno<21){
                         PORTD=2;
                     }else{PORTD=0;
                     }
                   
                 }while(ACEPTAR==0);
                 break;
                
                 
     }


}
//Interrupcion de seleccion 
void __interrupt()nombre(){
    if(PIR1bits.RCIF==1){ // Bandera de la interrupcion 
        t=UART_Read(); // Lectura de un dato ingresado
        PIR1bits.RCIF=0; // Reseteo de la bandera para una posterior lectura
        
        // Condici?n if para las diferentes opciones
        if(t=='1'||t=='2'||t=='3'||t=='4'||t=='5'||t=='6'){
        UART_Wrrite_Chr(' ');
        UART_Wrrite_Chr('O');
        UART_Wrrite_Chr('K');
        UART_Wrrite_Chr(10);// salto de l?nea
        UART_Wrrite_Chr(13);// Ubicar el cursor en la ?ltima posici?n
        }
        else{
        UART_Wrrite_Chr(' ');
        UART_Wrrite_Chr('E');
        UART_Wrrite_Chr('R');
        UART_Wrrite_Chr('R');
        UART_Wrrite_Chr('O');
        UART_Wrrite_Chr('R');
        UART_Wrrite_Chr(10);// salto de l?nea
        UART_Wrrite_Chr(13);// Ubicar el cursor en la ?ltima posici?n
        
        }
    }
}

void serial(){
    //Menu del puerto Virtual
        UART_Wrrite_Chr('O');
        UART_Wrrite_Chr('P');
        UART_Wrrite_Chr('C');
        UART_Wrrite_Chr('I');
        UART_Wrrite_Chr('O');
        UART_Wrrite_Chr('N');
        UART_Wrrite_Chr('E');
        UART_Wrrite_Chr('S');
        UART_Wrrite_Chr(':');
        UART_Wrrite_Chr(10);
        UART_Wrrite_Chr(13);
        UART_Wrrite_Chr('1');
        UART_Wrrite_Chr('.');
        UART_Wrrite_Chr('3');
        UART_Wrrite_Chr('4');
        UART_Wrrite_Chr(10);
        UART_Wrrite_Chr(13);
        UART_Wrrite_Chr('2');
        UART_Wrrite_Chr('.');
        UART_Wrrite_Chr('3');
        UART_Wrrite_Chr('5');
        UART_Wrrite_Chr(10);
        UART_Wrrite_Chr(13);
        UART_Wrrite_Chr('3');
        UART_Wrrite_Chr('.');
        UART_Wrrite_Chr('3');
        UART_Wrrite_Chr('6');
        UART_Wrrite_Chr(10);
        UART_Wrrite_Chr(13);
        UART_Wrrite_Chr('4');
        UART_Wrrite_Chr('.');
        UART_Wrrite_Chr('3');
        UART_Wrrite_Chr('7');
        UART_Wrrite_Chr(10);
        UART_Wrrite_Chr(13);
        UART_Wrrite_Chr('5');
        UART_Wrrite_Chr('.');
        UART_Wrrite_Chr('3');
        UART_Wrrite_Chr('8');
        UART_Wrrite_Chr(10);
        UART_Wrrite_Chr(13);
        UART_Wrrite_Chr('6');
        UART_Wrrite_Chr('.');
        UART_Wrrite_Chr('3');
        UART_Wrrite_Chr('9');
        UART_Wrrite_Chr(10);
        UART_Wrrite_Chr(13);


}

void main(void) {
    ADCON1=11; // 4 canales ADC y el resto digital
    TRISC=3; // RC0 Y RC1 datos de entrada
    TRISDbits.RD0=0; // pines de salida
    TRISDbits.RD1=0; //pines de salida
   
    LCD_Init(); //inicializa la LCD
    LCD_Cmd(12); //comando para borrar el cursor LCD
    ADC_Init(_TAD_4,_TOSC_4); // inicializa ADC y ajusta tiempos de adquisici?n y oscilaci?n
    
    INTCONbits.GIE=1; // configuraci?n interrupciones globales
    INTCONbits.PEIE=1; //configuraci?n perif?ricos
    
    PIR1bits.RCIF=0; //configuraci?n bandera
    PIE1bits.RCIE=1; //configuraci?n bandera
    
    UART_Init(9600); // inicializa UART y configura baudeaje
    
    serial(); // se incluye el m?todo
    
    while(1){
        
        MenuP(); // se incluye el m?todo
    }
    
    
    return;
}

