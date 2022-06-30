#ifndef CMAXONMOTOR_H
#define CMAXONMOTOR_H  
#include "Definitions.h" // Maxon Motor Header file

#define TRUE 1
#define FALSE 0

class CMaxonMotor
{
private:							
   	 char* PortName;
  	  DWORD ErrorCode;
	//unsigned short nodeID[2]; //unsigned short 
    	void *keyHandle;

void GetCurrentPosition(long& CurrentPosition, unsigned short nodeID);
	
public:								
	unsigned short nodeID[2]; //can be private
	CMaxonMotor();
         CMaxonMotor(char[], unsigned int ); //CONSTRUCTOR
void initializeDevice();                                  
    	void closeDevice();
	void EnableDevice(unsigned short nodeID);
    	void DisableDevice(unsigned short nodeID);
   
	void Move(long TargetPosition, unsigned short nodeID); //can be private
	void MoveAllDevice(long TargetPosition);
    	void GetCurrentPositionAllDevice(long* CurrentPosition);
         void activate_device();
	void GetAbsoluteData(unsigned short nodeID);
};
#endif // CMAXONMOTOR_H
