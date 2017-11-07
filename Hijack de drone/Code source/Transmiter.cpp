#include "mbed.h"
#include "nRF24L01P.h"
 
Serial pc(USBTX, USBRX); // tx, rx
 
nRF24L01P my_nrf24l01p(p5,p6,p7,p8,p9,p10); //mosi, miso, sck, csn, ce, irq

AnalogIn VitIn(p20);

int vitesse;

int main()
{
    char Vitesse[1];
    char TxDataCnt;
    char temp;
 
 
    my_nrf24l01p.powerUp();
    my_nrf24l01p.setRfOutputPower(0);
    my_nrf24l01p.setRfFrequency(2410);
 
    //Display the (default) setup of the nRF24L01+ chip
    pc.printf( "nRF24L01+ Frequency    : %d MHz\r\n",  my_nrf24l01p.getRfFrequency() );
    pc.printf( "nRF24L01+ Output power : %d dBm\r\n",  my_nrf24l01p.getRfOutputPower() );
    pc.printf( "nRF24L01+ Data Rate    : %d kbps\r\n", my_nrf24l01p.getAirDataRate() );
    pc.printf( "nRF24L01+ TX Address   : 0x%010llX\r\n", my_nrf24l01p.getTxAddress() );
    pc.printf( "nRF24L01+ RX Address   : 0x%010llX\r\n", my_nrf24l01p.getRxAddress() );
    
 
    TxDataCnt = 1;
    my_nrf24l01p.setTransferSize(TxDataCnt);
 
    my_nrf24l01p.enable();
 
    while (1) {
 
        
        vitesse = VitIn.read_u16();
        Vitesse[0] = vitesse / 256;				//Envoi la donnée du ADC pour controler le moteur
        
        // Send the Transmit buffer via the nRF24L01
        temp = my_nrf24l01p.write( NRF24L01P_PIPE_P0,Vitesse, TxDataCnt );
 
        pc.printf( "Sending %d data - %d\r\n",temp,Vitesse[0]);
            
        wait_ms(250);
    }
}
 
            