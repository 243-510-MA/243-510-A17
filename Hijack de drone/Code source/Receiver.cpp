#include "mbed.h"
#include "nRF24L01P.h"

#define TxAddress       0x1                                     //Addresse du nRF
#define TxWidth         0x3                                     //Nombre de byte maximum de l'addresse

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX); // tx, rx

nRF24L01P my_nrf24l01p(p5, p6, p7, p8, p9, p10);    // mosi, miso, sck, csn, ce, irq

int freq,vitesse,i;
int rxDataCnt=0;
char RxData[4];
int RxDataStorage[4];

int main() 
{
    // Initialisation
    my_nrf24l01p.powerUp();                                          // Power Up le nRF24L01P
    my_nrf24l01p.setReceiveMode();                                   // Met le nRF24L01P en mode "Receive" pour my_nrf24l01p les trames
    my_nrf24l01p.setAirDataRate(250);                                // Set le data rate du Enhanced Shockburst à 250Kb/s     
    my_nrf24l01p.setRfFrequency(2400);                               // Set la Fréquence du NRF à 2,4Ghz au départ
    my_nrf24l01p.setRfOutputPower(0);                                // Set la puissance de sortie à 0dB
    my_nrf24l01p.enable();                                           // Permet au nRF de recevoir
    
    pc.printf( "nRF24L01+ Frequency    : %d MHz\r\n",  my_nrf24l01p.getRfFrequency() );
    pc.printf( "nRF24L01+ Output power : %d dBm\r\n",  my_nrf24l01p.getRfOutputPower() );
    pc.printf( "nRF24L01+ Data Rate    : %d kbps\r\n", my_nrf24l01p.getAirDataRate() );
    pc.printf( "nRF24L01+ RX Address   : 00x%010llX\r\n", my_nrf24l01p.getRxAddress() );
    
    while(1) 
    {
        if(my_nrf24l01p.readable(NRF24L01P_PIPE_P0))
        {
            rxDataCnt = my_nrf24l01p.read(NRF24L01P_PIPE_P0, RxData, DEFAULT_NRF24L01P_TRANSFER_SIZE);
        
            RxDataStorage[0] = RxData[0];
            RxDataStorage[1] = RxData[1];
            RxDataStorage[2] = RxData[2];
            RxDataStorage[3] = RxData[3];
            
            pc.printf("Receiver\n\r");
            pc.printf("RxData_Cnt\n\r", rxDataCnt);
            pc.printf("data0: %d\n\r",RxDataStorage[0]);
            pc.printf("data1: %d\n\r",RxDataStorage[1]);
            pc.printf("data2: %d\n\r",RxDataStorage[2]);
            pc.printf("data3: %d\n\r",RxDataStorage[3]);
        }
    }
}