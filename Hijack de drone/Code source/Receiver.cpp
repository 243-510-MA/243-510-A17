#include "mbed.h"
#include "nRF24L01P.h"

#define RxAddress       0x1                                     //Addresse du nRF
#define RxWidth         0x3                                     //Nombre de byte maximum de l'addresse

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX); // tx, rx

nRF24L01P my_nrf24l01p(p5, p6, p7, p8, p9, p10);    // mosi, miso, sck, csn, ce, irq

int freq,vitesse,i;
int RxDataCnt=0;
char RxData[4];
int RxDataStorage[4];

int main() 
{
    // Initialisation
    my_nrf24l01p.powerUp();                                          // Power Up le nRF24L01P
    my_nrf24l01p.disableAutoAcknowledge();                           // Disable les "Auto Acknowledge", selon la datasheet
    my_nrf24l01p.setReceiveMode();                                   // Met le nRF24L01P en mode "Receive" pour my_nrf24l01p les trames
    my_nrf24l01p.setAirDataRate(250);                                // Set le data rate du Enhanced Shockburst à 250Kb/s     
    my_nrf24l01p.setRfFrequency(2400);                               // Set la Fréquence du NRF à 2,4Ghz au départ
    my_nrf24l01p.setRfOutputPower(0);                                // Set la puissance de sortie à 0dB
    my_nrf24l01p.setRxAddress(RxAddress,RxWidth,NRF24L01P_PIPE_P0);  // Set l'addresse du nRF en Rx
    my_nrf24l01p.setCrcWidth(DEFAULT_NRF24L01P_CRC);                 // Set la longueur du CRC
    my_nrf24l01p.setTransferSize(DEFAULT_NRF24L01P_TRANSFER_SIZE, NRF24L01P_PIPE_P0);
    my_nrf24l01p.enable();                                           // Permet au nRF de recevoir
    
    while(1) 
    {
        freq = my_nrf24l01p.getRfFrequency();
        RxDataCnt = my_nrf24l01p.read(NRF24L01P_PIPE_P0, RxData, TRANSFER_SIZE);
        
        RxDataStorage[0] = RxData[0];
        RxDataStorage[1] = RxData[1];
        RxDataStorage[2] = RxData[2];
        RxDataStorage[3] = RxData[3];
        
        pc.printf("Receiver");
        pc.printf("data0: %d\n\r",data0);
        pc.printf("data1: %d\n\r",data1);
        pc.printf("data2: %d\n\r",data2);
        pc.printf("data3: %d\n\r",data3);
        pc.printf("Frequence: %d\n\r",freq);
        pc.printf("Stop Receive");
        wait(1);
    }
}

