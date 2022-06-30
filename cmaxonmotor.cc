//.cc or .cpp are similar
#include "cmaxonmotor.h"
#include <string.h>  
#include <iostream>
#include <stdlib.h> 
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

CMaxonMotor::CMaxonMotor()
{
	PortName = "USB0";
	ErrorCode = 0x00;
	nodeID[0] = 1; //right motor
	nodeID[1] = 2; //left motor
}

void CMaxonMotor::closeDevice()
{
	DisableDevice(nodeID[0]);
	DisableDevice(nodeID[1]);
       DWORD ErrorCode = 0;

    	cout<<"Closing Device!"<<endl;

    	if(keyHandle != 0)
            VCS_CloseDevice(keyHandle, &ErrorCode);
    	VCS_CloseAllDevices(&ErrorCode);
}

void CMaxonMotor::EnableDevice(unsigned short nodeID)
{
DWORD ErrorCode = 0;
    int IsInFault = FALSE;

    if( VCS_GetFaultState(keyHandle, nodeID, &IsInFault, &ErrorCode) )
    {
        if( IsInFault && !VCS_ClearFault(keyHandle, nodeID, &ErrorCode) )
        {
            cout << "Clear fault failed! , error code="<<ErrorCode<<endl;
            return;
        }

        int IsEnabled = FALSE;
        if( VCS_GetEnableState(keyHandle, nodeID, &IsEnabled, &ErrorCode) )
        {
            if( !IsEnabled && !VCS_SetEnableState(keyHandle, nodeID, &ErrorCode) )
            {
                cout << "Set enable state failed!, error code="<<ErrorCode<<endl;
            }
            else
            {
                cout << "Enable succeeded!" << endl;
		            }
        }
    }
    else
    {
        cout << "Get fault state failed!, error code, error code="<<ErrorCode<<endl;
		
    }

}
 
void CMaxonMotor::DisableDevice(unsigned short nodeID)
{

DWORD ErrorCode = 0;
    int IsInFault = FALSE;

    if( VCS_GetFaultState(keyHandle, nodeID, &IsInFault, &ErrorCode) )
    {
        if( IsInFault && !VCS_ClearFault(keyHandle, nodeID, &ErrorCode) )
        {
            cout<<"Clear fault failed!, error code="<<ErrorCode<<endl;
            return;
        }

        int IsEnabled = FALSE;
        if( VCS_GetEnableState(keyHandle, nodeID, &IsEnabled, &ErrorCode) )
        {
            if( IsEnabled && !VCS_SetDisableState(keyHandle, nodeID, &ErrorCode) )
            {
                cout<<"Set disable state failed!, error code=" <<ErrorCode<<endl;
            }
            else
            {
                cout<<"Set disable state succeeded!"<<endl;
            }
        }
    }
    else
    {
        cout<<"Get fault state failed!, error code="<<ErrorCode<<endl;
    }
}

void CMaxonMotor::Move(long TargetPosition, unsigned short nodeID)
{

DWORD errorCode = 0;

    if( VCS_ActivateProfilePositionMode(keyHandle, nodeID, &errorCode) )
    {

        int Absolute = TRUE; // FALSE;
        int Immediately = FALSE;
		
	DWORD ProfileVelocity = 0;	//159 //Velocity Motors (output motors, not output Gearhead) = output
// velocity GH * Gear reduction
	DWORD ProfileAcceleration = 0;//159 //Acceleration Motors (output motors, not output Gearhead)
	DWORD ProfileDeceleration = 0;//159 //Decceleration Motors (output motors, not output Gearhead)
       // if( !Absolute )
       //{
       //    long PositionIs = 0;
       //    if( VCS_GetPositionIs(keyHandle, nodeID, &PositionIs, &errorCode) );
       //}

	ProfileVelocity = 159*2;	//159 //Velocity Motors (output motors, not output Gearhead) = output 
  //velocity GH * Gear reduction
	ProfileAcceleration = 159*2;//159	//Acceleration Motors (output motors, not output Gearhead)
	ProfileDeceleration = 159*2;//159	//Deceleration Motors (output motors, not output Gearhead)
		
	cout << "Velocity : " << ProfileVelocity << endl;
	VCS_SetPositionProfile(keyHandle, nodeID, ProfileVelocity, ProfileAcceleration, ProfileDeceleration, &errorCode);

	VCS_GetPositionProfile(keyHandle, nodeID, &ProfileVelocity, &ProfileAcceleration, &ProfileDeceleration, &errorCode);

        if( !VCS_MoveToPosition(keyHandle, nodeID, TargetPosition, Absolute, Immediately, &errorCode) )
        {
            cout << "Move to position failed!, error code="<<errorCode<<endl;
        }
    }
    else
    {
        cout << "Activate profile position mode failed!" << endl;
    }
}

void CMaxonMotor::GetCurrentPosition(long& CurrentPosition, unsigned short nodeID)
{

DWORD errorCode = 0;

    if( !VCS_GetPositionIs(keyHandle, nodeID, &CurrentPosition, &errorCode) ){
        cout << " error while getting current position , error code="<<errorCode<<endl;
    }
}

void CMaxonMotor::GetCurrentPositionAllDevice(long* CurrentPosition)
{
	long Pos;
	GetCurrentPosition(Pos, nodeID[0]);
	CurrentPosition[0] = Pos;
	GetCurrentPosition(Pos, nodeID[1]);
	CurrentPosition[1] = Pos;
}

void CMaxonMotor::activate_device()
{
  // Configuring EPOS for analog motor control
  char DeviceName[]="EPOS4";
  char ProtocolStackName[] = "MAXON SERIAL V2";
  char InterfaceName[] = "USB";
  DWORD ErrorCode = 0x00;
  unsigned long timeout_ = 500;
  unsigned long baudrate_ = 1000000;
  const unsigned short MotorType = 10;
	//Subdevice com
	HANDLE subkeyHandle = 0;
	char subdeviceName[] = "EPOS4";
	char subProtocolStackName[] = "CANopen";

	//Motors parameters
	unsigned short NominalCurrent[] = { 337 , 337}; //337
	unsigned short MaxOutputCurrent[] = { 1800 ,1800 }; //1800
	unsigned short ThermalTimeConstant[] = { 1950 , 1950 }; //1950 
	unsigned short NbOfPolePairs[] = { 1, 1 }; //1

	//Encoder parameters
	const unsigned short SensorType = 5;
	unsigned short DataRate = 2000;
	unsigned short NbOfMultiTurnDataBits = 0;
	unsigned short NbOfSingleTurnDataBits = 12;
	unsigned short NbOfSpecialDataBits = 0;
	int InvertedPolarity = 0;	//polarity 0or1
	unsigned short Timeout = 13;
	unsigned short PowerupTime = 200;

        keyHandle=VCS_OpenDevice(DeviceName,ProtocolStackName,
InterfaceName,PortName,&ErrorCode);
	cout << "KeyH : " << keyHandle << endl;
    if( keyHandle == 0 )
    {
        cout<<"Open device failure, error code="<<ErrorCode<<endl;
    }
    else
    {
        cout<<"Open device success!"<<endl;

	subkeyHandle = VCS_OpenSubDevice(keyHandle, subdeviceName, subProtocolStackName, &ErrorCode);
	cout << "SubKeyH : " << subkeyHandle << endl;
	if (subkeyHandle == 0)
	{
		cout << "Open sub device failure, error code=" << ErrorCode << endl;
	}
	else
	{
		cout << "Open sub device success!" << endl;
	}
    }


    if( !VCS_SetProtocolStackSettings(keyHandle, baudrate_, timeout_, &ErrorCode) )
    {
        cout<<"Set protocol stack settings failed!, error code="<<ErrorCode<<endl;
        closeDevice();
    }

	VCS_SetMotorType(keyHandle, nodeID[0], MotorType, &ErrorCode);

	VCS_SetEcMotorParameter(keyHandle, nodeID[0], NominalCurrent[0], MaxOutputCurrent[0], ThermalTimeConstant[0], NbOfPolePairs[0], &ErrorCode);

	VCS_SetSensorType(keyHandle, nodeID[0], SensorType, &ErrorCode);

  VCS_SetSsiAbsEncoderParameterEx(keyHandle, nodeID[0], DataRate, NbOfMultiTurnDataBits, NbOfSingleTurnDataBits, NbOfSpecialDataBits, InvertedPolarity, Timeout, PowerupTime, &ErrorCode);


	VCS_SetMotorType(keyHandle, nodeID[1], MotorType, &ErrorCode);

	VCS_SetEcMotorParameter(keyHandle, nodeID[1], NominalCurrent[1], MaxOutputCurrent[1], ThermalTimeConstant[1], NbOfPolePairs[1], &ErrorCode);//nbre pole to change

	VCS_SetSensorType(keyHandle, nodeID[1], SensorType, &ErrorCode);

	VCS_SetSsiAbsEncoderParameterEx(keyHandle, nodeID[1], DataRate, NbOfMultiTurnDataBits,
  NbOfSingleTurnDataBits, NbOfSpecialDataBits,
  InvertedPolarity, Timeout, PowerupTime, &ErrorCode);
	
  EnableDevice(nodeID[0]);
	EnableDevice(nodeID[1]);

}


void CMaxonMotor::initializeDevice(){
    closeDevice(); // To close if opend
    activate_device();
}

void CMaxonMotor::MoveAllDevice(long TargetPosition) {
	Move(TargetPosition, nodeID[0]);
	Move(TargetPosition, nodeID[1]);
}

void CMaxonMotor::Sleep() {

	DWORD errorCode1 = 0x00;
	DWORD errorCode2 = 0x00;
	BOOL TargetReached1 = 0;
	BOOL TargetReached2 = 0;
	
	while ( !TargetReached1 || !TargetReached2 ) {
		
		sleep_for(10ms);
		VCS_GetMovementState(keyHandle, 1, &TargetReached1, &errorCode1);
		VCS_GetMovementState(keyHandle, 2, &TargetReached2, &errorCode2);
	}
}

void CMaxonMotor::GetAbsoluteData(unsigned short nodeID) {

	WORD DataRate;
	WORD NbOfMultiTurnDataBits;
	WORD NbOfSingleTurnDataBits;
	WORD NbOfSpecialDataBits;
	BOOL InvertedPolarity;
	WORD Timeout;
	WORD PowerupTime;
	DWORD ErrorCode;

	VCS_GetSsiAbsEncoderParameterEx(keyHandle, nodeID, &DataRate, &NbOfMultiTurnDataBits, &NbOfSingleTurnDataBits, &NbOfSpecialDataBits, &InvertedPolarity, &Timeout, &PowerupTime, &ErrorCode);

	cout << "DataRate : " << DataRate << endl;
	cout << "NbOfMultiTurnDataBits : " << NbOfMultiTurnDataBits << endl;
	cout << "NbOfSingleTurnDataBit : "s << NbOfSingleTurnDataBits << endl;
	cout << "NbOfSpecialDataBits : " << NbOfSpecialDataBits << endl;
	cout << "InvertedPolarity : " << InvertedPolarity << endl;
	cout << "Timeout : " << Timeout << endl;
	cout << "PowerupTime : " << PowerupTime << endl;
	cout << "ErrorCode : " << ErrorCode << endl;
}
