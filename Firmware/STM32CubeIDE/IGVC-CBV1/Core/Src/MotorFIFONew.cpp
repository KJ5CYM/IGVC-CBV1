
/* This ifdef allows the header to be used from both C and C++. */
extern "C"
{
	//This allows the C driver to be implemented.
	#include "sabertooth_2x25_driver.h"
}

// Joysticks
#define CH2 5
#define CH3 6
// Switches, could select switches through the RC Settings menu. 
#define CH5 9 // SWA
#define CH6 11 // SWD

#define BIAS 10 // Noise bias rejection 
#define P_S_R 2 // pin select receive 0 for left vcc rail for right
#define BUFFER_SIZE 10
typedef struct RECEIVER
{
  int LEFT = 0;
  int RIGHT = 0;

}RECEIVER;

/*
FIFO implementation for smoothing motor commands.
*/

class MotorFIFONew
{  
    private:
	  int leftMotorBuffer[8]; // CREATE BUFFER_SIZE Variable in order to ensure readability.
	  int rightMotorBuffer[8];//BUFFER_SIZE
	  int currentIndex = 0;
	  int itemCount = 0;
    
    public:
        MotorFIFONew()
        {
            for (int i = 0; i < 8; i++) {//BUFFER_SIZE
            leftMotorBuffer[i] = 0;
            rightMotorBuffer[i] = 0;
            }
        } 

        void insertVelocities(int leftVelocity, int rightVelocity) {
		if (itemCount < 8) { //BUFFER_SIZE
		  leftMotorBuffer[currentIndex] = leftVelocity;
		  rightMotorBuffer[currentIndex] = rightVelocity;
		  currentIndex = (currentIndex + 1) % 8;//BUFFER_SIZE
		  itemCount++;
		} else {
		  // Buffer is full, shift the data
		  for (int i = 1; i < 8; i++) {//BUFFER_SIZE
			leftMotorBuffer[i - 1] = leftMotorBuffer[i];
			rightMotorBuffer[i - 1] = rightMotorBuffer[i];
		  }
		  leftMotorBuffer[8 - 1] = leftVelocity;//BUFFER_SIZE
		  rightMotorBuffer[8 - 1] = rightVelocity;//BUFFER_SIZE
		}
	  }  

      float getLeftMotorAverage() {
		int sum = 0;

		for (int i = 0; i < itemCount; i++) {
		  sum += leftMotorBuffer[i];
		}

		return static_cast<float>(sum) / itemCount;
	  }

	  float getRightMotorAverage() {
		int sum = 0;

		for (int i = 0; i < itemCount; i++) {
		  sum += rightMotorBuffer[i];
		}

		return static_cast<float>(sum) / itemCount;
	  }   
};

RECEIVER R_V; // received values
MotorFIFONew S_MOTOR; // Smoothing motor
const int receiverAddress = 129;

int motor1_speed = 0;
int motor2_speed = 0;


//There were originally other methods here, but they are for a joystick, so no use until then.
void Send_To_Motors(int LEFT, int RIGHT); // sends values to motors using FIFO buffer
void setup() 
{
	
}

