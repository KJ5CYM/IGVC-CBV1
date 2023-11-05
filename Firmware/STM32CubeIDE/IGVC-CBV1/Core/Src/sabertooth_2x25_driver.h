/**********************************************************************************************
 *
 * Sabertooth_2x25_Driver.h
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
 * ____________________________+______________________________This driver works in the "Packetized Serial" mode, which must be set with the DIP_______________________
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


#ifndef sabertooth_2x25_driver_h
    #define sabertooth_2x25_driver_h

#include <stdio.h>                  // Need to be able to use standard output "putchar" command
#include <stdint.h>                 // Include your standard types definition file to accomodate the C99 uint_x types

#define FALSE   0
#define TRUE    1


/**********************************************************************************************
 **********************************************************************************************
 * This is the only section that should require modification for non-PIC/non-CCS Compiler usage
 *********************************************************************************************/

 /* In this section you must make modifications to suit both your hardware, and your compiler
  *
  * Assign name to a MOTOR_STOP output pin for emergency stop routines
  *
  * The two macros are for use in your application code to halt all movement or
  * re-enable it, using the MOTOR_STOP pin defined above.
  *
  * The Sabertooth requires only that a control pin be pulled low to completely disable
  * the two motor outputs, and if unused, it can be left floating (you don't have to use that pin)
  *
  * Possible uses are bumper switches or other sensors, or during startup when you don't
  * want the motors enabled until all systems are properly initialized */


/* TODO: Create a driver-wide #define file to include from a system hardware file, rather than assigning hardware
 *       in this header file. The intent is to assign all hardware I/O in "hardware.h", rather than
 *       in separate/distributed files....  */

#ifdef __PICC__                                                                                                         /** CCS Compiler */

    #define MOTOR_STOP                   PIN_D5

    #define mSTOP_ALL_MOTORS             output_low(MOTOR_STOP);
    #define mRESTART_ALL_STOPPED_MOTORS  output_high(MOTOR_STOP);

#endif

#ifdef __C32__                                                                                                          /** Microchip PIC 32-Bit Compiler */

    #define MOTOR_STOP                   1 << 10                        //PORTBbits.RB10

    #define mSTOP_ALL_MOTORS             mPORTBClearBits( MOTOR_STOP ) // output_low(MOTOR_STOP);
    #define mRESTART_ALL_STOPPED_MOTORS  mPORTBSetBits( MOTOR_STOP )   // output_high(MOTOR_STOP);

#endif


/**********************************************************************************************
 * End of the only section that should require modification for non-PIC/non-CCS Compiler usage
 **********************************************************************************************
 *********************************************************************************************/





/**********************************************************************************************
 * SETUP INFORMATION
 *********************************************************************************************/

    /* You must send the autobauding character to your Sabertooth during program initialization,
     * before sending any commands to the device.
     *
     * This must be done each time the device powers up. */

    #define mSEND_AUTOBAUD               putchar(0xAA)


    /* Each serial command to the hardware is checked for validity by the on-board processor */

    #define CRC_MASK                     0b01111111


    /* Use desired hardware address, per hardware DIP switch settings */

    #define MOTOR_DRIVER_ADDRESS_1       128
    #define MOTOR_DRIVER_ADDRESS_2       129        //THIS IS ITTTTTTT!!!!!!!!!!!!!!
    #define MOTOR_DRIVER_ADDRESS_3       130
    #define MOTOR_DRIVER_ADDRESS_4       131
    #define MOTOR_DRIVER_ADDRESS_5       132
    #define MOTOR_DRIVER_ADDRESS_6       133
    #define MOTOR_DRIVER_ADDRESS_7       134
    #define MOTOR_DRIVER_ADDRESS_8       135


/**********************************************************************************************
 * COMMANDS
 *********************************************************************************************/


    #define COMMAND_LOW_LIMIT            0                            // For bounds checking purposes only

    #define DRIVE_FORWARD_1              0
    #define DRIVE_REVERSE_1              1

    #define SET_MIN_VOLTAGE              2
    #define SET_MAX_VOLTAGE              3

    #define DRIVE_FORWARD_2              4
    #define DRIVE_REVERSE_2              5

    #define DRIVE_MOTOR_1_7_BIT          6
    #define DRIVE_MOTOR_2_7_BIT          7

    #define DRIVE_FORWARD_MIXED          8
    #define DRIVE_REVERSE_MIXED          9
    #define DRIVE_TURN_RIGHT_MIXED       10
    #define DRIVE_TURN_LEFT_MIXED        11

    #define DRIVE_FWD_REV_7_BIT          12
    #define DRIVE_TURN_7_BIT             13

    #define SET_SERIAL_TIMEOUT           14
    #define SET_BAUD_RATE                15

    #define SET_RAMPING_RATE             16
    #define SET_DEADBAND                 17

    /* If any more commands are added by the manufacturer in the future, increase
     * COMMAND_HIGH_LIMIT to the highest command number in that new command set.
     * As with the COMMAND_LOW_LIMIT, this is only used for bounds checking in a function.
     *
     * Yes, we could enumerate the commands, and that works better in MPLAB 8 debugging,
     * but the macro viewer capability in the new MPLABX IDE that I am using is so neat
     * that I changed it back to separate #defines so I could use it.
     *
     * Your call. 8^)  */

    #define COMMAND_HIGH_LIMIT           17


/**********************************************************************************************
 * COMMAND DATA
 *********************************************************************************************/

    #define BAUDRATE_2400                1
    #define BAUDRATE_9600                2
    #define BAUDRATE_19200               3
    #define BAUDRATE_38400               4
    #define DEFAULT_BAUDRATE             BAUDRATE_9600

    #define MAX_MINIMUM_VOLTAGE          120
    #define MIN_MINIMUM_VOLTAGE          0
    #define DEFAULT_MINIMUM_VOLTAGE      0

    #define MAX_MAXIMUM_VOLTAGE          128
    #define MIN_MAXIMUM_VOLTAGE          0
    #define DEFAULT_MAXIMUM_VOLTAGE      0

    #define MAX_SERIAL_TIMEOUT           50
    #define MIN_SERIAL_TIMEOUT           0
    #define DEFAULT_SERIAL_TIMEOUT       0

    #define MAX_RAMPING_VALUE            80
    #define MIN_RAMPING_VALUE            0
    #define DEFAULT_RAMPING_VALUE        1

    #define MAX_DEADBAND                 127
    #define MIN_DEADBAND                 0
    #define DEFAULT_DEADBAND             3


/**********************************************************************************************
 * FUNCTION PROTOTYPES
 **********************************************************************************************
 *
 * *** NOTE *** If using the CCS Compiler, and only one single Sabertooth controller, you do not need the "address"
 *              variable. It is only needed if you are sending commands to more than one controller, or
 *              if you are using a compiler (such as Microchip's Cx/XCx series) which does not allow operator overloading.
 *
 *********************************************************************************************/


    static uint8_t control_motors_sep ( uint8_t command1, uint8_t speed1, uint8_t command2, uint8_t speed2, uint8_t address );
   
    static uint8_t control_motors_mixed ( uint8_t command, uint8_t speed, uint8_t address );

    static uint8_t set_minimum_controller_voltage ( uint8_t desired_minimum_voltage, uint8_t address );

    static uint8_t set_maximum_controller_voltage ( uint8_t desired_maximum_voltage, uint8_t address );

    static uint8_t set_serial_timeout ( uint8_t desired_timeout_period, uint8_t address );

    static uint8_t set_baudrate ( uint8_t desired_baudrate, uint8_t address );

    static uint8_t set_ramping_rate ( uint8_t desired_ramping_rate, uint8_t address );

    static uint8_t set_deadband_range ( uint8_t desired_deadband, uint8_t address );
   
    static void send_command ( uint8_t command, uint8_t value, uint8_t address );
   
#endif

/**********************************************************************************************
 * END OF SABERTOOTH 2x25 HEADER
 *********************************************************************************************/
