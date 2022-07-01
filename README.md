# omnigear_maxoncontrol

The research is about controlling helical gears meshed with an omnidirectional plate.
Each gear is activated with a brushless Maxon motor. Maxon EPOS4 motor driver are used with each motor for the control.
PID tuning was necessary before each experiment via the EPOS4 tuning interface.

In the table below, are listed parts that have been used. 
The code is mainly used for EPOS4 Maxon motors drivers. 
It can be adapted for other versions.




Motor	              :    Maxon ECX SPEED 22L brushless	       |    Maximum torque = 2.4 Nm
Planetary Gearhead	:    Maxon GPX 22	                         |    Gear Reduction =16
Encoder	            :    Maxon encoder “ENX 2EASY INT”	       |    Use SSI Absolute signal protocol
Motor Driver	      :    Maxon driver “EPOS4 Compact 50/8 CAN” |	  Use CAN protocol for signal transmission
Power supply 	      :    KIKUSUI PMC35-3	                     |    Maximum voltage 36 V, Maximum current 3.6 A
Adapter	            :    Adapter EASY Absolute	               |
Cables		          :                                          |    Different cables for : Power supply, CAN communication and data from encoder

