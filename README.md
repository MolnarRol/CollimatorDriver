
[TOC]

# Project details
## CollimatorDriver
X-Ray systems are among the basic diagnostic devices in medicine. However, their disadvantage is that it uses ionizing radiation for its activity. For this reason, radiation is minimized during the examination area e.g. by using lead shields that are placed on the patient. Another alternative is to minimize radiation by directing or blocking radiation at the radiation source. It was developed for this purpose the Collimator unit, which positions the lead shielding slats just before the radiation source so that it is irradiated only the desired part of the patient (Fig. 5, Fig. 6). The positioning of the shading slats must be very precise and smooth and dynamic. During the time when the device is not in use, the slats must be closed, the slats to the given position
they only open when a snapshot is requested and then close again. From the point of view of security, it is therefore it is necessary to limit the force of the slats during their movement / closing. The task is to design a control system for the unit
  

## Project requirements

- Setting:
	- maximum force to position blades
	- limit requested position
	- limit movement speed
- PC application communicating using serial communication interface
- Information about position and status available directly from LCD
- Mechanism to signal errors to operator

## Tasks
- - [x] Serial communication (4. week):
	-  [x] Hardware setup for communication (Initialization and interface functions)
	-  [x] Communication protocol design
	-  [x] Implementation of protocol
- - [x] PC application (5. week):
	- [x] Communication protocol implementation 
	- [x] GUI design
- - [ ] Direct control (6. week):
	- [ ] Buttons
	- [ ] LCD 
- - [x] Motor control (6. - 8. week):
	- [x] Scalar control
	- [x] FOC control
	-  [x] Regulator tuning:
		-  [x] Current regulation 
		-  [x] Speed regulation
		-  [x] Position regulator 
	-  [ ] Safety and error checks:
		-  [ ] Maximum torque limit and error
- - [ ] Testing and tuning (9. week)

# Software team
- Bc. Roland Molnar, roland.molnar.3@student.tuke.sk (Team leader)
- Bc. Samuel Fertal, samuel.fertal@student.tuke.sk (Developer)
- Bc. Vadym Holysh, vadym.holysh@student.tuke.sk (Developer)

# Programming manual
[Programming manual](doc/ProgrammingManual.md)


