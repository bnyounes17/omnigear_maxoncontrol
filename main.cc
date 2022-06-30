#include <stdio.h>
#include <iostream>  //
#include <conio.h>  
#include "cmaxonmotor.h"
#include <chrono>
#include <thread>

using namespace std; 

int main(int argc, char *argv[])
{
	CMaxonMotor motor;
	motor.initializeDevice(); // initialize EPOS4 device  

	using namespace std::this_thread;     // sleep_for, sleep_until
	using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
	using std::chrono::system_clock;
	//Specify the position below and move
	long CurrentPosition[2];
	CurrentPosition[0] = 0;
	CurrentPosition[1] = 0;

	long TargetPosition1 = 0;
	long TargetPosition2 = 0;
	
	/* Go to initial position : Position 0 */
	motor.GetCurrentPositionAllDevice(CurrentPosition);
	cout << "Position before motion : \n";
	cout << "Current Position M1: " << CurrentPosition[0] << endl;
	cout << "Current Position M2: " << CurrentPosition[1] << endl;
	motor.Move(0, 1);
	motor.Move(0, 2);


	motor.Sleep();
	motor.GetCurrentPositionAllDevice(CurrentPosition);
	cout << "Position after init : \n";
	cout << "Current Position M1: " << CurrentPosition[0] << endl;
	cout << "Current Position M2: " << CurrentPosition[1] << endl;

	/******************************************************/
	/*Pitch radius = 48 mm                                */
	/*D1 = 72  mm -> 72  / 48 = 1.5  [rad] -> 15238.29 [inc]  */
	/*D2 = 144 mm -> 144 / 48 = 3.0  [rad] -> 30476.58 [inc]  */
	/******************************************************/
	//BOOL* pTargetReached;DWORD errorCode = 0;
	/*State 1 : Forward Left (Diagonal) -> Move from 0 to A : Only M2 in forward direction */
	const double PI = 3.141592653589793;
	double t1 = 1.5 * (5621 / (361 * 2 * PI)) * 4096;//15225.72
	double t2 = 3.0 * (5621 / (361 * 2 * PI)) * 4096;//30451.43
	TargetPosition2 += (long)t1;
	motor.Move(TargetPosition2, 2);
	motor.Sleep();
	motor.GetCurrentPositionAllDevice(CurrentPosition);
	cout << "Current Position M1: " << CurrentPosition[0] << " | Expected : 0 "<< endl;
	cout << "Current Position M2: " << CurrentPosition[1] << " | Expected : 15225 " << endl;
	
	/*State 2 : Right -> Move from A to B : Motors in opposite directions : M1 positive/M2 Negative */
	TargetPosition1 += (long)t2;
	TargetPosition2 += -(long)t2;
	motor.Move(TargetPosition1, 1);
	motor.Move(TargetPosition2, 2);
	motor.Sleep();
	motor.GetCurrentPositionAllDevice(CurrentPosition);
	cout << "Current Position M1: " << CurrentPosition[0] << " | Expected : 30451 " << endl;
	cout << "Current Position M2: " << CurrentPosition[1] << " | Expected : -15225 " << endl;
	
	/*State 3 : Backward -> Move from Point B to Point C : Both Motors in the same backward direction */
	TargetPosition1 += -(long)t2;
	TargetPosition2 += -(long)t2;
	motor.Move(TargetPosition1, 1);
	motor.Move(TargetPosition2, 2);
	//sleep_for(10ns);
	//sleep_until(system_clock::now() + 40s);
	motor.Sleep();
	motor.GetCurrentPositionAllDevice(CurrentPosition);
	cout << "Current Position M1: " << CurrentPosition[0] << " | Expected : 0 " << endl;
	cout << "Current Position M2: " << CurrentPosition[1] << " | Expected : -45676 " << endl;

	/*State 4 : Left -> move from pont C to point D : Motors in opposite directions : M1 negative/M2 positive */
	TargetPosition1 += -(long)t2;
	TargetPosition2 += (long)t2;
	motor.Move(TargetPosition1, 1);
	motor.Move(TargetPosition2, 2);
	
	motor.Sleep();
	motor.GetCurrentPositionAllDevice(CurrentPosition);
	cout << "Current Position M1: " << CurrentPosition[0] << " | Expected : -30451 " << endl;
	cout << "Current Position M2: " << CurrentPosition[1] << " | Expected : -15225 " << endl;
	
	/*State 5 : Forward -> Move from Point D to Point A : Both Motors in the same forward direction */
	TargetPosition1 += (long)t2;
	TargetPosition2 += (long)t2;
	motor.Move(TargetPosition1, 1);
	motor.Move(TargetPosition2, 2);
	motor.Sleep();
	motor.GetCurrentPositionAllDevice(CurrentPosition);
	cout << "Current Position M1: " << CurrentPosition[0] << " | Expected : 0 " << endl;
	cout << "Current Position M2: " << CurrentPosition[1] << " | Expected : 15225 " << endl;

	/*State 6 : Backward Right (Diagonal Right Back) -> Move from A to 0 : Only M2 in backward direction */
	TargetPosition2 += -(long)t1;
	motor.Move(TargetPosition2, 2);
	motor.Sleep();
	motor.GetCurrentPositionAllDevice(CurrentPosition);
	cout << "Current Position M1: " << CurrentPosition[0] << " | Expected : 0 " << endl;
	cout << "Current Position M2: " << CurrentPosition[1] << " | Expected : 0 " << endl;
	
	motor.Sleep();
	motor.GetCurrentPositionAllDevice(CurrentPosition);
	cout << "Current Position M1: " << CurrentPosition[0] << endl;
	cout << "Current Position M2: " << CurrentPosition[1] << endl;

	motor.closeDevice(); // close EPOS4

	return 0;
}
