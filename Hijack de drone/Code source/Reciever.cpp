#include "mbed.h"
#include "nRF24L01P.h"
 
Serial pc(USBTX, USBRX); // tx, rx
PwmOut pwm(p26);
 
nRF24L01P my_nrf24l01p(p5, p6, p7, p8, p9, p10);    // mosi, miso, sck, csn, ce, irq
 
 char temp;
 
int main() {
    
    char count[1];
    char RxDataCnt;
    float vit = 0.05;
 
    my_nrf24l01p.powerUp();
    my_nrf24l01p.setRfFrequency(2410);
 
    // Display the (default) setup of the nRF24L01+ chip
    pc.printf( "nRF24L01+ Frequency    : %d MHz\r\n",  my_nrf24l01p.getRfFrequency() );
    pc.printf( "nRF24L01+ Output power : %d dBm\r\n",  my_nrf24l01p.getRfOutputPower() );
    pc.printf( "nRF24L01+ Data Rate    : %d kbps\r\n", my_nrf24l01p.getAirDataRate() );
    pc.printf( "nRF24L01+ TX Address   : 0x%010llX\r\n", my_nrf24l01p.getTxAddress() );
    pc.printf( "nRF24L01+ RX Address   : 0x%010llX\r\n", my_nrf24l01p.getRxAddress() );
 
    pc.printf( "Simple 2 Byte Receiver\r\n" );
    
    RxDataCnt = 1;
    my_nrf24l01p.setTransferSize( RxDataCnt );
    
    my_nrf24l01p.setReceiveMode();
    my_nrf24l01p.enable();
    pwm.write(vit);
 
    while (1) {
 
        // If we've received anything in the nRF24L01+...
        if ( my_nrf24l01p.readable() ) 
        {
 
            // ...read the data into the receive buffer
            temp = my_nrf24l01p.read( NRF24L01P_PIPE_P0, count, RxDataCnt );
            
            //calcul pour le pwm
            vit = (float) (count[0]*0.00019607);
            vit = vit + 0.05;
            pc.printf( "Cnt %d = %d \r\n",temp,count[0]);
            pc.printf("%1.3f", vit);
            
            //Write pwm
            pwm.write(vit);
        }
    }
}