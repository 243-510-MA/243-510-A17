#include "mbed.h"
#include "nRF24L01P.h"

#define TxAddress       0x1                                     //Addresse du nRF
#define TxWidth         0x3                                     //Nombre de byte maximum de l'addresse

Serial pc(USBTX, USBRX); // tx, rx

nRF24L01P my_nrf24l01p(p5, p6, p7, p8, p9, p10);    // mosi, miso, sck, csn, ce, irq

AnalogIn vitIn(p16);           //potentiometre vitesse

int main()
{

    pc.baud(9600);
    pc.format();

    float vitesse;
    int vit;
    char txData[DEFAULT_NRF24L01P_TRANSFER_SIZE];

    // Initialisation
    my_nrf24l01p.powerUp();                                          // Power Up le nRF24L01P
    my_nrf24l01p.disableAutoAcknowledge();                           // Disable les "Auto Acknowledge", selon la datasheet
    my_nrf24l01p.setTransmitMode();                                   // Met le nRF24L01P en mode "Receive" pour my_nrf24l01p les trames
    my_nrf24l01p.setAirDataRate(250);                                // Set le data rate du Enhanced Shockburst à 250Kb/s     
    my_nrf24l01p.setRfFrequency(2400);                               // Set la Fréquence du NRF à 2,4Ghz au départ
    my_nrf24l01p.setRfOutputPower(0);                                // Set la puissance de sortie à 0dB
    my_nrf24l01p.setTxAddress(TxAddress,TxWidth,NRF24L01P_PIPE_P0);  // Set l'addresse du nRF en Rx
    my_nrf24l01p.setCrcWidth(DEFAULT_NRF24L01P_CRC);                 // Set la longueur du CRC
    my_nrf24l01p.setTransferSize(DEFAULT_NRF24L01P_TRANSFER_SIZE, NRF24L01P_PIPE_P0);
    my_nrf24l01p.enable();                                           // Permet au nRF de recevoir
    
    pc.printf( "nRF24L01+ Frequency    : %d MHz\r\n",  my_nrf24l01p.getRfFrequency() );
    pc.printf( "nRF24L01+ Output power : %d dBm\r\n",  my_nrf24l01p.getRfOutputPower() );
    pc.printf( "nRF24L01+ Data Rate    : %d kbps\r\n", my_nrf24l01p.getAirDataRate() );
    pc.printf( "nRF24L01+ TX Address   : 0x%010llX\r\n", my_nrf24l01p.getTxAddress() );
    pc.printf( "nRF24L01+ RX Address   : 0x%010llX\r\n", my_nrf24l01p.getRxAddress() );

    while (1) 
    {
		/*
        vitesse = vitIn.read();
        vitesse = vitesse * 100;
        vitesse = vitesse / 65536;
        vit = vitesse;
        */
		
        txData[0] = 0xAA;
        txData[1] = 0x11;
        txData[2] = 0x22;
        txData[3] = 0xBB;
        
        my_nrf24l01p.write( NRF24L01P_PIPE_P0, txData, txDataCnt );
       
        wait(1);
    }
}