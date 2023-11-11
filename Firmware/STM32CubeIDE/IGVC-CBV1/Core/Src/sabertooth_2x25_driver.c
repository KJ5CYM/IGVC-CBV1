/**********************************************************************************************
 *
 * Sabertooth_2x25_Driver.c
 * Version 1
*/


#include "sabertooth_2x25_driver.h"


/**********************************************************************************************
 * Function:        uint8_t set_baudrate ( uint8_t desired_baudrate, \
 *                                         uint8_t address = MOTOR_DRIVER_ADDRESS_1 )
 * Pre-Condition:   None
 * Input:           Receives command data from the application program
 * Output:          Sends the command to the send_command function,
 *                      and then to the serial port; returns true or false to calling function, to
 *                      verify valid commands received and processed
 * Side Effects:    None
 * Overview:        Checks command for validity, and passes it to the serial port
 *                  set_baudrate accepts baud values between 2,400 and 38,400 (commands 1-4),
 *                      with a default value of 9,600 baud (command 2)
 * Notes:           This function is valid for Sabertooth Command 15
 *********************************************************************************************/

static uint8_t set_baudrate ( uint8_t desired_baudrate, uint8_t address ) {

    static uint8_t new_baudrate = DEFAULT_BAUDRATE;

    if ( desired_baudrate < BAUDRATE_2400 || \
         desired_baudrate > BAUDRATE_38400 ) {

            new_baudrate = DEFAULT_BAUDRATE;
            return FALSE;                                   // Set error code for error
    }

    else {
        new_baudrate = desired_baudrate;
    }

    send_command ( SET_BAUD_RATE, new_baudrate, address );
    return TRUE;                                            // Set error code for no error
}


/**********************************************************************************************
 * Function:        static void send_command ( uint8_t command, uint8_t value, uint8_t address )
 *
 * Pre-Condition:   None
 * Input:           Receives the command data from the driver functions
 * Output:          Sends the three commands plus their checksum to the serial port, and through
 *                      that to the Sabertooth Motor Controller
 * Side Effects:    None
 * Overview:        None
 * Notes:           Static helper function, for use only by driver functions in this file
 *********************************************************************************************/

    /* Helper Command, for internal driver use only
     * Defining it here, in the .c file, instead of in the .h file, to prevent
     * compiler warning about it being declared "static", but never defined
     * This is correct, since it is not for the user, only the user's functions

    assert  ( command < COMMAND_HIGH_LIMIT);
    putchar ( address );
    putchar ( command );
    putchar ( value );
    putchar ( ( address + command + value ) & CRC_MASK );*/

//static void send_command ( uint8_t command, uint8_t value, uint8_t address );

/*********************************************************************************************/
static void send_command(uint8_t command, uint8_t value, uint8_t address)
{
	assert(command< COMMAND_HIGH_LIMIT);
	putchar(address);
	putchar(command);
	putchar(value);
	putchar((address+command+value)& CRC_MASK);
}





/**********************************************************************************************
 * END OF SABERTOOTH 2x25 DRIVER
 *********************************************************************************************/
