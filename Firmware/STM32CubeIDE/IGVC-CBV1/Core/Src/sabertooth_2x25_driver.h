/**********************************************************************************************
 *
 * Sabertooth_2x25_Driver.h
 * Version 1
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

//    #define MOTOR_DRIVER_ADDRESS_1       128
    #define MOTOR_DRIVER_ADDRESS_2       129        //THIS IS ITTTTTTT!!!!!!!!!!!!!!
//    #define MOTOR_DRIVER_ADDRESS_3       130
//    #define MOTOR_DRIVER_ADDRESS_4       131
//    #define MOTOR_DRIVER_ADDRESS_5       132
//    #define MOTOR_DRIVER_ADDRESS_6       133
//    #define MOTOR_DRIVER_ADDRESS_7       134
//    #define MOTOR_DRIVER_ADDRESS_8       135


/**********************************************************************************************
 * COMMANDS
 *********************************************************************************************/


    #define COMMAND_LOW_LIMIT            0                            // For bounds checking purposes only

    #define DRIVE_FORWARD_1              0
    #define DRIVE_REVERSE_1              1

    #define DRIVE_FORWARD_2              4
    #define DRIVE_REVERSE_2              5

    #define DRIVE_MOTOR_1_7_BIT          6
    #define DRIVE_MOTOR_2_7_BIT          7

    #define DRIVE_FWD_REV_7_BIT          12
    #define DRIVE_TURN_7_BIT             13


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



/**********************************************************************************************
 * FUNCTION PROTOTYPES
 **********************************************************************************************
 *
 * *** NOTE *** If using the CCS Compiler, and only one single Sabertooth controller, you do not need the "address"
 *              variable. It is only needed if you are sending commands to more than one controller, or
 *              if you are using a compiler (such as Microchip's Cx/XCx series) which does not allow operator overloading.
 *
 *********************************************************************************************/

    static uint8_t set_baudrate ( uint8_t desired_baudrate, uint8_t address );
   
    static void send_command ( uint8_t command, uint8_t value, uint8_t address );
   
#endif

/**********************************************************************************************
 * END OF SABERTOOTH 2x25 HEADER
 *********************************************************************************************/
