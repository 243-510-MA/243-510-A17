#ifdef __PIC32MX__
	#include <xc.h>
	#include <plib.h>
	#include <stdint.h>
	#include <stdbool.h>

	#include "system.h"
	#include "spi.h"

	/************************ FUNCTIONS ********************************/

	/*********************************************************************
	* Function:         void SPIPut(uint8_t v)
	*
	* PreCondition:     SPI has been configured
	*
	* Input:		    v - is the uint8_t that needs to be transfered
	*
	* Output:		    none
	*
	* Side Effects:	    SPI transmits the uint8_t
	*
	* Overview:		    This function will send a uint8_t over the SPI
	*
	* Note:			    None
	********************************************************************/
	void SPIPut(uint8_t v)
	{
	    uint8_t i;

	    #if !defined(HARDWARE_SPI)

	        SPI_SDO = 0;
	        SPI_SCK = 0;

	        for(i = 0; i < 8; i++)
	        {
	            SPI_SDO = (v >> (7-i));
	            SPI_SCK = 1;
	            SPI_SCK = 0;
	        }
	        SPI_SDO = 0;
	    #else

	        #if defined(__PIC32MX__)
	            putcSPI2(v);
	            i = (uint8_t)getcSPI2();
	        #else
	            IFS0bits.SPI1IF = 0;
	            i = SPI1BUF;
	            SPI1BUF = v;
	            while(IFS0bits.SPI1IF == 0){}
	        #endif
	    #endif
	}


	/*********************************************************************
	* Function:         uint8_t SPIGet(void)
	*
	* PreCondition:     SPI has been configured
	*
	* Input:		    none
	*
	* Output:		    uint8_t - the uint8_t that was last received by the SPI
	*
	* Side Effects:	    none
	*
	* Overview:		    This function will read a uint8_t over the SPI
	*
	* Note:			    None
	********************************************************************/
	uint8_t SPIGet(void)
	{
	    #if !defined(HARDWARE_SPI)
	        uint8_t i;
	        uint8_t spidata = 0;


	        SPI_SDO = 0;
	        SPI_SCK = 0;

	        for(i = 0; i < 8; i++)
	        {
	            spidata = (spidata << 1) | SPI_SDI;
	            SPI_SCK = 1;
	            SPI_SCK = 0;
	        }

	        return spidata;
	    #else
	        #if defined(__PIC32MX__)
	            uint8_t dummy;

	            putcSPI2(0x00);
	            dummy = (uint8_t)getcSPI2();
	            return(dummy);
	        #else
	            SPIPut(0x00);
	            return SPI1BUF;
	        #endif
	    #endif
	}

	#if defined(SUPPORT_TWO_SPI)
	    /*********************************************************************
	    * Function:         void SPIPut2(uint8_t v)
	    *
	    * PreCondition:     SPI has been configured
	    *
	    * Input:		    v - is the uint8_t that needs to be transfered
	    *
	    * Output:		    none
	    *
	    * Side Effects:	    SPI transmits the uint8_t
	    *
	    * Overview:		    This function will send a uint8_t over the SPI
	    *
	    * Note:			    None
	    ********************************************************************/
	    void SPIPut2(uint8_t v)
	    {
	        uint8_t i;

	        #if !defined(HARDWARE_SPI)

	            SPI_SDO2 = 0;
	            SPI_SCK2 = 0;

	            for(i = 0; i < 8; i++)
	            {
	                SPI_SDO2 = (v >> (7-i));
	                SPI_SCK2 = 1;
	                SPI_SCK2 = 0;
	            }
	            SPI_SDO2 = 0;
	        #else

	            #if defined(__PIC32MX__)
	                putcSPI2(v);
	                i = (uint8_t)getcSPI2();
	            #else
	                IFS2bits.SPI2IF = 0;
	                i = SPI2BUF;
	                SPI2BUF = v;
	                while(IFS2bits.SPI2IF == 0){}
	            #endif
	        #endif
	    }


	    /*********************************************************************
	    * Function:         uint8_t SPIGet2(void)
	    *
	    * PreCondition:     SPI has been configured
	    *
	    * Input:		    none
	    *
	    * Output:		    uint8_t - the uint8_t that was last received by the SPI
	    *
	    * Side Effects:	    none
	    *
	    * Overview:		    This function will read a uint8_t over the SPI
	    *
	    * Note:			    None
	    ********************************************************************/
	    uint8_t SPIGet2(void)
	    {
	        #if !defined(HARDWARE_SPI)
	            uint8_t i;
	            uint8_t spidata = 0;

	            SPI_SDO2 = 0;
	            SPI_SCK2 = 0;

	            for(i = 0; i < 8; i++)
	            {
	                spidata = (spidata << 1) | SPI_SDI2;
	                SPI_SCK2 = 1;
	                SPI_SCK2 = 0;
	            }

	            return spidata;
	        #else
	            #if defined(__PIC32MX__)
	                uint8_t dummy;

	                putcSPI2(0x00);
	                dummy = (uint8_t)getcSPI2();
	                return(dummy);
	            #else
	                SPIPut2(0x00);
	                return SPI2BUF;
	            #endif
	        #endif
	    }
	#endif

#else
	//PIC18
#include <xc.h>
//#include <plib.h>
#include <stdint.h>
#include <stdbool.h>

#include "system.h"
#include "spi.h"

/************************ FUNCTIONS ********************************/

/*********************************************************************
* Function:         void SPIPut(uint8_t v)
*
* PreCondition:     SPI has been configured
*
* Input:		    v - is the uint8_t that needs to be transfered
*
* Output:		    none
*
* Side Effects:	    SPI transmits the uint8_t
*
* Overview:		    This function will send a uint8_t over the SPI
*
* Note:			    None
********************************************************************/
void SPIPut(uint8_t v)
{
    volatile uint8_t i;

    #if !defined(HARDWARE_SPI)

        SPI_SDO = 0;
        SPI_SCK = 0;

        for(i = 0; i < 8; i++)
        {
            SPI_SDO = (v >> (7-i));
            SPI_SCK = 1;
            SPI_SCK = 0;
        }
        SPI_SDO = 0;
    #else

        #if defined(__PIC32MX__)
            putcSPI2(v);
            i = (uint8_t)getcSPI2();
        #else
            /*IFS0bits.SPI1IF = 0;
            i = SPI1BUF;
            SPI1BUF = v;
            while(IFS0bits.SPI1IF == 0){}*/

        PIR1bits.SSPIF = 0;
        i = SSPBUF;
        do
        {
            SSPCON1bits.WCOL = 0;
            SSPBUF = v;
        } while( SSPCON1bits.WCOL );
    
        while( PIR1bits.SSPIF == 0 );
        #endif
    #endif
}


/*********************************************************************
* Function:         uint8_t SPIGet(void)
*
* PreCondition:     SPI has been configured
*
* Input:		    none
*
* Output:		    uint8_t - the uint8_t that was last received by the SPI
*
* Side Effects:	    none
*
* Overview:		    This function will read a uint8_t over the SPI
*
* Note:			    None
********************************************************************/
uint8_t SPIGet(void)
{
    #if !defined(HARDWARE_SPI)
        uint8_t i;
        uint8_t spidata = 0;


        SPI_SDO = 0;
        SPI_SCK = 0;

        for(i = 0; i < 8; i++)
        {
            spidata = (spidata << 1) | SPI_SDI;
            SPI_SCK = 1;
            SPI_SCK = 0;
        }

        return spidata;
    #else
        #if defined(__PIC32MX__)
            uint8_t dummy;

            putcSPI2(0x00);
            dummy = (uint8_t)getcSPI2();
            return(dummy);
        #else
            /*SPIPut(0x00);
            return SPI1BUF;*/
            SPIPut(0x00);
            return SSPBUF;
        #endif
    #endif
}

#if defined(SUPPORT_TWO_SPI)
    /*********************************************************************
    * Function:         void SPIPut2(uint8_t v)
    *
    * PreCondition:     SPI has been configured
    *
    * Input:		    v - is the uint8_t that needs to be transfered
    *
    * Output:		    none
    *
    * Side Effects:	    SPI transmits the uint8_t
    *
    * Overview:		    This function will send a uint8_t over the SPI
    *
    * Note:			    None
    ********************************************************************/
    void SPIPut2(uint8_t v)
    {
        volatile uint8_t i;

        #if !defined(HARDWARE_SPI)

            SPI_SDO2 = 0;
            SPI_SCK2 = 0;

            for(i = 0; i < 8; i++)
            {
                SPI_SDO2 = (v >> (7-i));
                SPI_SCK2 = 1;
                SPI_SCK2 = 0;
            }
            SPI_SDO2 = 0;
        #else

            #if defined(__PIC32MX__)
                putcSPI2(v);
                i = (uint8_t)getcSPI2();
            #else
                PIR3bits.SSP2IF = 0;
                i = SSP2BUF;
                do
                {
                    SSP2CON1bits.WCOL = 0;
                    SSP2BUF = v;
                } while( SSP2CON1bits.WCOL );

                while( PIR3bits.SSP2IF == 0 );
            #endif
        #endif
    }


    /*********************************************************************
    * Function:         uint8_t SPIGet2(void)
    *
    * PreCondition:     SPI has been configured
    *
    * Input:		    none
    *
    * Output:		    uint8_t - the uint8_t that was last received by the SPI
    *
    * Side Effects:	    none
    *
    * Overview:		    This function will read a uint8_t over the SPI
    *
    * Note:			    None
    ********************************************************************/
    uint8_t SPIGet2(void)
    {
        #if !defined(HARDWARE_SPI)
            uint8_t i;
            uint8_t spidata = 0;

            SPI_SDO2 = 0;
            SPI_SCK2 = 0;

            for(i = 0; i < 8; i++)
            {
                spidata = (spidata << 1) | SPI_SDI2;
                SPI_SCK2 = 1;
                SPI_SCK2 = 0;
            }

            return spidata;
        #else
            #if defined(__PIC32MX__)
                uint8_t dummy;

                putcSPI2(0x00);
                dummy = (uint8_t)getcSPI2();
                return(dummy);
            #else
                SPIPut2(0x00);
                return SSP2BUF;
            #endif
        #endif
    }
#endif


#endif