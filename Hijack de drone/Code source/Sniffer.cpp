//----------------------------------------//
//          Laurent Tremblay              //
//       Sniffer pour nRF24L01P           //
//          Cours 243-510-MA              //
//----------------------------------------//
#include "mbed.h"
#include "nRF24L01P.h"

#define RxAddress       0x1                                     //Addresse du nRF
#define RxWidth         0x3                                     //Nombre de byte maximum de l'addresse

// Objets
nRF24L01P sniffer(P5,P6,P7,P8,P9,P10);
Serial pc(USBTX,USBRX);

//Variables globales
int lenght = 1;                                                 //Lenght de données de la trame Enhanced Shockburst

//Programme Principal
int main() 
{
    //Variables locales
    long Freq = 2400;                                               //Initialise Freq à 2,4Ghz
    char ESdata[32];                                                //Enhanced Shockburst payload
    float floatfreq;
    
    // Initialisation
    sniffer.powerUp();                                          // Power Up le nRF24L01P
    sniffer.disableAutoAcknowledge();                           // Disable les "Auto Acknowledge", selon la datasheet
    sniffer.setReceiveMode();                                   // Met le nRF24L01P en mode "Receive" pour sniffer les trames
    sniffer.setAirDataRate(250);                                // Set le data rate du Enhanced Shockburst à 250Kb/s     
    sniffer.setRfFrequency(Freq);                               // Set la Fréquence du NRF à 2,4Ghz au départ
    sniffer.setRfOutputPower(0);                                // Set la puissance de sortie à 0dB
    sniffer.setRxAddress(RxAddress,RxWidth,NRF24L01P_PIPE_P0);  // Set l'addresse du nRF en Rx
    sniffer.setCrcWidth(DEFAULT_NRF24L01P_CRC);                 // Set la longueur du CRC
    sniffer.setTransferSize(DEFAULT_NRF24L01P_TRANSFER_SIZE, NRF24L01P_PIPE_P0);
    sniffer.enable();                                           // Permet au nRF de recevoir
    
    while(1) 
    {
        if(sniffer.Readable(NRF24L01P_PIPE_P0)                      //Si on est capapable de lire des données sur le channel actuel, sur le Pipe P0
        {
            sniffer.read(NRF24L01P_PIPE_P0, ESdata, sniffer.getTransferSize);
            
            floatfreq = freq/1000;
            
            pc.printf("Channel: %1.3f Ghz\n\r", floatfreqreq);
            pc.printf("Output Power: %d dB\n\r", sniffer.getRFOutputPower());
            pc.printf("Data Rate: %L kBPS\n\r", sniffer.getAirDataRate());
            pc.printf("CRC width: %d\n\r", sniffer.getCrcWidth());
            pc.printf("RxAddressL %d\n\r", sniffer.getRxAddress());   
                     
            pc.printf("Trame:");                                    //Impression de la trame
            for(int i=0; i<lenght; i++) pc.printf(ESdata[i]);
            pc.printf("\r\n");
            
        }
        else                                                    //Sinon changer de channel en changant la fréquence de 1Mhz
        {
            Freq = Freq + 1;
            if(Freq >= 2526) Freq = 2400;                       // Assure que la fréquence ne dépasse pas 2,525Ghz (Maximum)
            sniffer.setRfFrequency(Freq);
        }
    }
}
