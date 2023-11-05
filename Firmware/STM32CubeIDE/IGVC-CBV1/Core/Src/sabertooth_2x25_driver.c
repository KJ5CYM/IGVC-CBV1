/**********************************************************************************************
 *
 * Sabertooth_2x25_Driver.c
 * Version 2.0.2
 * 07 July, 2013
 *
 * Brad Hedges
 * H & H Enterprises
 * Stratford, Oklahoma
 *
 **********************************************************************************************
 **********************************************************************************************
 *
 * This is a multi-purpose device driver to operate the Dimension Engineering Sabertooth series
 * of H-Bridge motor controllers. These can be used to control permanent magnet DC motors
 * between 6 & 30 volts, up to 25 amps continuous and 50 amps surge.
 *
 * This driver works in the "Packetized Serial" mode, which must be set with the DIP
 * switches on the controller hardware. The driver defaults to operating only one
 * Sabertooth, using the lowest physical address of 128. The address is configurable in
 * the driver by assigning the address value to the constant "MOTOR_DRIVER_ADDRESS_x".
 *
 * The driver is written in the C language for Microchip PIC microcontrollers using CCS's
 * PIC-C Compiler, and was developed using version 4.120 of that compiler. I have also used it
 * with 32-bit PICs, using Microchip's C32 compiler.
 *
 * It should be easily convertible to other microcontrollers and compilers with minimal effort. I have
 * annotated the three lines which would have to be modified for a different compiler or
 * processor architecture.
 *
 * Dimension Engineering's web site is at:      www.dimensionengineering.com
 * CCS's web site is:                           www.ccsinfo.com
 * Microchip's web site:                        www.microchip.com
 *
 *********************************************************************************************/


#include "sabertooth_2x25_driver.h"



/**********************************************************************************************
 * Function:        uint8_t control_motors_sep ( uint8_t command1, uint8_t speed1, \
 *                                               uint8_t command2, uint8_t speed2, \
 *                                               uint8_t address )
 *
 * Pre-Condition:   None
 * Input:           Receives command data from the application program
 * Output:          Sends the commands to the send_command function,
 *                      and then to the serial port
 * Side Effects:    None
 * Overview:        Checks commands for validity, and passes them to the serial port
 * Notes:           This function is valid for Sabertooth Commands 0, 1, 4 - 7
 *                  These commands are for controlling two motors with individual settings, a single motor at a time
 *
 *                      Individual Motor Commands:
 *                          0:  Drive Forward      Motor 1
 *                          1:  Drive Reverse      Motor 1
 *                          4:  Drive Forward      Motor 2
 *                          5:  Drive Reverse      Motor 2
 *                          6:  Drive 7-Bit        Motor 1
 *                          7:  Drive 7-Bit        Motor 2
 *
 *********************************************************************************************/

static uint8_t control_motors_sep ( uint8_t command1, uint8_t speed1, \
                             uint8_t command2, uint8_t speed2, \
                             uint8_t address ) {
// = MOTOR_DRIVER_ADDRESS_1 ) { // If your compiler allows overloading, feel free to un-comment the assignment
// and move it immediately after the "address" variable
    if ( ( command1 < COMMAND_LOW_LIMIT || command1 > DRIVE_MOTOR_2_7_BIT ) || \
         ( command2 < COMMAND_LOW_LIMIT || command2 > DRIVE_MOTOR_2_7_BIT ) ) {

   /*     Set error code for invalid command
    *     Call a user error function to do whatever your application requires, such as:
    *         mSTOP_ALL_MOTORS;  */
        return FALSE;
    }

    else {
        send_command ( command1, speed1, address );
        send_command ( command2, speed2, address );
//      Set error code for no error
        return TRUE;
    }
}



/**********************************************************************************************
 * Function:        uint8_t control_motors_mixed ( uint8_t command, uint8_t speed1, uint8_t address )
 *
 * Pre-Condition:   None
 * Input:           Receives command data from the application program
 * Output:          Sends the commands to the send_command function,
 *                      and then to the serial port
 * Side Effects:    None
 * Overview:        Checks commands for validity, and passes them to the serial port
 * Notes:           This function is valid for Sabertooth Commands 8 - 13
 *                  These commands are for controlling two motors simultaneously
 *
 *                      Mixed Mode Commands:
 *                          8:  Drive Forward      Mixed Mode
 *                          9:  Drive Reverse      Mixed Mode
 *                          10: Turn Right         Mixed Mode
 *                          11: Turn Left          Mixed Mode
 *                          12: Drive Forwards/Reverse     7 bit mode
 *                          13: Turn Left/Right            7 bit mode
 *
 *********************************************************************************************/

static uint8_t control_motors_mixed ( uint8_t command, uint8_t speed, uint8_t address ) {

    if ( command < DRIVE_FORWARD_MIXED || command > DRIVE_TURN_7_BIT ) {

   /*     Set error code for invalid command
    *     Call a user error function to do whatever your application requires, such as:
    *         mSTOP_ALL_MOTORS;
    *         return;   */
        return FALSE;
    }

    else {
        send_command ( command, speed, address );
//      Set error code for no error
        return TRUE;
    }
}


/**********************************************************************************************
 * Function:        uint8_t set_minimum_controller_voltage ( uint8_t desired_minimum_voltage, \
 *                                                           uint8_t address = MOTOR_DRIVER_ADDRESS_1 )
 * Pre-Condition:   None
 * Input:           Receives command data from the application program
 * Output:          Sends the command to the send_command function,
 *                      and then to the serial port; returns true or false to calling function, to
 *                      verify valid commands received and processed
 * Side Effects:    None
 * Overview:        Checks command for validity, and passes it to the serial port
 * Notes:           This function is valid for Sabertooth Command 2
 *********************************************************************************************/

atatic uint8_t set_minimum_controller_voltage ( uint8_t desired_minimum_voltage, uint8_t address ) {

    uint8_t new_min_voltage = DEFAULT_MINIMUM_VOLTAGE;

    if ( desired_minimum_voltage < MIN_MINIMUM_VOLTAGE || \
         desired_minimum_voltage > MAX_MINIMUM_VOLTAGE ) {

            new_min_voltage = DEFAULT_MINIMUM_VOLTAGE;
            return FALSE;                                    // Set error code for error
    }

    else {
        new_min_voltage = desired_minimum_voltage;
    }

    send_command ( SET_MIN_VOLTAGE, new_min_voltage, address );
    return TRUE;                                             // Set error code for no error
}


/**********************************************************************************************
 * Function:        uint8_t set_maximum_controller_voltage ( uint8_t desired_maximum_voltage, \
 *                                                           uint8_t address = MOTOR_DRIVER_ADDRESS_1 )
 *
 * Pre-Condition:   Do not use if powering Sabertooth from batteries, only needed for use with
 *                      power supplies
 * Input:           Receives command data from the application program
 * Output:          Sends the command to the send_command function,
 *                      and then to the serial port; returns true or false to calling function, to
 *                      verify valid commands received and processed
 * Side Effects:    If you change this from the default value of 30 Volts, you can not set it
 *                      for more than 25 Volts. In order to reset it to a higher value, you must
 *                      use the DEScribe software available from Dimension Engineering
 * Overview:        Checks command for validity, and passes it to the serial port
 * Notes:           This function is valid for Sabertooth Command 3
 *********************************************************************************************/

static uint8_t set_maximum_controller_voltage ( uint8_t desired_maximum_voltage, uint8_t address ) {

    uint8_t new_max_voltage = DEFAULT_MAXIMUM_VOLTAGE;

    if ( desired_maximum_voltage < MIN_MAXIMUM_VOLTAGE ||  \
         desired_maximum_voltage > MAX_MAXIMUM_VOLTAGE ) {

            new_max_voltage = DEFAULT_MAXIMUM_VOLTAGE;
        return FALSE;                                       // Set error code for error
    }

    else {
        new_max_voltage = desired_maximum_voltage;
    }

    send_command ( SET_MAX_VOLTAGE, new_max_voltage, address );
    return TRUE;                                            // Set error code for no error
}


/**********************************************************************************************
 * Function:        uint8_t set_serial_timeout ( uint8_t desired_timeout_period, \
 *                                               uint8_t address = MOTOR_DRIVER_ADDRESS_1 )
 * Pre-Condition:   None
 * Input:           Receives command data from the application program
 * Output:          Sends the command to the send_command function,
 *                      and then to the serial port; returns true or false to calling function, to
 *                      verify valid commands received and processed
 * Side Effects:    None
 * Overview:        Checks command for validity, and passes it to the serial port
 *                  Value received by the Sabertooth is in multiples of 100 milli-seconds
 *                      EXAMPLE: '1' = 100 ms
 *                  I set the max timeout value at a discretionary value of 5 seconds; you can
 *                      set it as desired by changing the "MAX_SERIAL_TIMEOUT" constant #define,
 *                      in the header file under "Command Data"
 * Notes:           This function is valid for Sabertooth Command 14
 *********************************************************************************************/

static uint8_t set_serial_timeout ( uint8_t desired_timeout_period, uint8_t address ) {

    uint8_t new_timeout_period = DEFAULT_SERIAL_TIMEOUT;

    if ( desired_timeout_period < MIN_SERIAL_TIMEOUT || \
         desired_timeout_period > MAX_SERIAL_TIMEOUT ) {

            new_timeout_period = DEFAULT_SERIAL_TIMEOUT;
        return FALSE;                                       // Set error code for error
    }

    else {
        new_timeout_period = desired_timeout_period;
    }

   send_command ( SET_SERIAL_TIMEOUT, new_timeout_period, address );
   return TRUE;                                             // Set error code for no error
}


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
 * Function:        uint8_t set_ramping_rate ( uint8_t desired_ramping_rate, \
 *                                             uint8_t address = MOTOR_DRIVER_ADDRESS_1 )
 * Pre-Condition:   None
 * Input:           Receives command data from the application program
 * Output:          Sends the command to the send_command function,
 *                      and then to the serial port; returns true or false to calling function, to
 *                      verify valid commands received and processed
 * Side Effects:    None
 * Overview:        Checks command for validity, and passes it to the serial port
 *                  set_ramping_rate accepts the desired ramping rate, which must be a value between
 *                      zero and eighty ( 0 - 80 )
 * Notes:           This function is valid for Sabertooth Command 16
 *********************************************************************************************/

static uint8_t set_ramping_rate ( uint8_t desired_ramping_rate, uint8_t address ) {

    static uint8_t new_ramping_rate = DEFAULT_RAMPING_VALUE;

    if ( desired_ramping_rate < MIN_RAMPING_VALUE || \
         desired_ramping_rate > MAX_RAMPING_VALUE ) {

            new_ramping_rate = DEFAULT_RAMPING_VALUE;
        return FALSE;                                       // Set error code for error
    }

    else {
        new_ramping_rate = desired_ramping_rate;
    }

    send_command ( SET_RAMPING_RATE, new_ramping_rate, address );
    return TRUE;                                            // Set error code for no error
}


/**********************************************************************************************
 * Function:        uint8_t set_deadband_range ( uint8_t desired_deadband, \
 *                                               uint8_t address = MOTOR_DRIVER_ADDRESS_1 )
 * Pre-Condition:   None
 * Input:           Receives command data from the application program
 * Output:          Sends the command to the send_command function,
 *                      and then to the serial port; returns true or false to calling function, to
 *                      verify valid commands received and processed
 * Side Effects:    None
 * Overview:        Checks command for validity, and passes it to the serial port
 *                  set_deadband_range accepts the desired deadband, which must be a value
 *                      between zero and 127
 *                  Value is set as follows:
 *                      ( 127 - command value ) < motors_off < ( 128 + command value )
 * Notes:           This function is valid for Sabertooth Command 17
 *********************************************************************************************/

static uint8_t set_deadband_range ( uint8_t desired_deadband, uint8_t address ) {

    static uint8_t new_deadband = DEFAULT_DEADBAND;

    if ( desired_deadband < MIN_DEADBAND || desired_deadband > MAX_DEADBAND ) {

         new_deadband = DEFAULT_DEADBAND;
         return FALSE;                                      // Set error code for error
    }

    else {
        new_deadband = desired_deadband;
    }

    send_command ( SET_DEADBAND, new_deadband, address );
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
