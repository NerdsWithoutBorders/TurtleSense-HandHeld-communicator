/*
 * ======== Standard MSP430 includes ========
 */
#include <msp430.h>

/*
 * ======== Grace related includes ========
 */

#include <ti/mcu/msp430/Grace.h>
		/// PROGRAM PARAMETERS (may vary for each unit)
#define COMM_SERIAL_NUMBER "H-AA0000"	// Unique serial number for each device.  Update before compiling for a new device
	// REMEMBER TO CHECK THE FTP LOG-IN CREDENTIALS IN THE FOLLOWING TWO LINES!!!!
#define ISP_PROVIDER "AT+CGDCONT=1,\"IP\",\"ISP-Provider.net\"\r"	// AT message to open internet connection UPDATE WITH YOUR CREDENTIALS
#define FTP_ACCOUNT	"AT#FTPOPEN=\"yourwebsite.org\",\"ftp-username\",\"ftp-pasword\"\r"	// AT messages to create FTP connection UPDATE WITH YOUR CREDENTIALS




/*
        ***************************************************************************************
        //
        // TURTLE SENSE HAND-HELD REGISTRATION COMMUNTICATION CONTROLLER
        // PHASE TWO -- Turtle M2M Interface --
        // Programmed By Samuel Wantman cc 4.0
		//
        //      VERSION HISTORY OF PHASE TWO
        // 		V0.01 2/21/14 First operational version for use with Phase II board version 0.14
        //			  3/11/14 Comm head master controller code split from smart sensor code
        //			  5/2/14 Revised to work with Phase II board version 0.25
        //			  5/22/14 Revised to work on hand held controller (extra button and LEDs)
		//	  V0.2502 6/23/14 Date registration fixed.  First unit resent to NPS in NC
        // some routines based on code logic from PHASE ONE --Turtle Dialer Phone Interface
        // Programmed by Thomas Zimmerman  cc 4.0
		****************************************************************************************


        // Logic for Phase Two -- COMM HEAD MASTER CONTROLLER
		// --------------------------------------------------
		 *
		 * 			INITIALIZATION for REGISTRATION
		 *
        // Initialize Microprocessor (settings using GRACE)
        // Initialize and test Sensor (create one record) if operating incorrectly indicate error with fast red blinks.
        // Check for sufficiently charged battery level. If insufficient, indicate error with slow red blinks, sleep.
        // Continue if there are no fatal problems with the battery or sensor
        //  Turn on phone.  // TODO //Record TELIT module ID.  Different modules will have different logic.
        //  If module is incompatible with the software, turn off phone and indicate with fast yellow binks.
        // 	Turn on the GPS if available.  While acquiring information slow green blinks.
        // 	When GPS location is found, indicate with a solid green LED.
        // 	Check for cell coverage.  The phone Status is shown by the LED controlled by the phone's logic
        //	The status logic is different for different TELIT modules.  This LED is only on or blinking
        //	when the phone module is powered up.  Otherwise it is always off.
		//  The red LED is for turtle sense status and also indicates the bars of coverage with number of LED blinks
		//	whenever the phone is on. (TODO: make it whatever is easy to report from phone card)
        // 		TODO: If the battery is strong, check for a parameter update file.  Load it in and change the parameters.
        //		If there is an update indicate with fast continuous blink of all 3 LEDs during the entire update process
        //		TODO: add verification of correct parameters.
        // 		Initialize sensor settings from the file if uploaded, otherwise use default settings.
		//  Create a new ID comprised of Serial number, last GPS location, and last installation date
        // 	Send the new ID to the sensor when the button is pushed.
        // 	Register the phone over the Internet by adding device to Log file.  Create new log file if one is missing.
        //	Report errors encountered.
        //


		// STATUS CODES
		 * ************			   TurtleSense			   Phone			  GPS
		// LED					LED 1 (Red)				LED 2 (Yellow)		LED 3 (Green)
		// Dead					never lit				never lit			never lit
		// Sensor testing				-- all three LEDs blink fast in cascading order --
        // Sensor Problem		Fast no pause			Off
        // GPS acquiring		Off						On					slow no pause
        // GPS Problem			Off						Off					(Fast no pause) not checked
        // GPS location set													On solid
        // Bad battery      	Slow, no pause			Off					Off
        // Phone problem	 	Off						never lit			Off
        // TODO // Multiple problem reporting
        // Phone Power									On
        // No cell coverage		Off						Constant On
        // Phone coverage								1-5 fast, Pause
		// Ready to register	OFF	  					1-5 fast, Pause		Constant On
        // After regisister		 -- all three colors blink in synch while sending log entries--
        // Fatal problem		Blinking and shutdown	Off					Off


		Timings
		ODR = Ouput Data Rate (eg. 50hz = 50 samples.  Each sample = 3 readings (x, y, z) each reading is 2 bytes)

		ODR		Max Seconds/Record		8K capacity		10 K capacity	16K capacity	20K capacity
		------	------------------		-----------		-------------	------------	------------
		50 Hz	1280 (~20 minutes)		~85 hrs			~4 days			~7 days			~9 days
		100 Hz	640  (~10 minutes)		~43 hrs			~2 days			~3 days			~4 days
		200 Hz	320	 (~5 minutes)		~21 hrs			~25 hrs			~43 hrs			~2 days
		400 Hz	160	 (~2.6 minutes)		~10 hrs			~13 hrs			~21 hrs			~25 hrs

		Default rates
		Slow days -- 1 upload/day		50Hz - 6min		50Hz - 5min		50Hz - 3min		50 Hz - 150sec
		Fast days -- 12 uploads/day		50Hz - 30sec	50 Hz -30 sec	50 Hz - 15sec	50 Hz - 12 sec

/////////////////////////////// Main logic for continuous operation /////////////////////////////////
        // Sensor buffers are read frequently (default is 8 times a second) in the smart sensor slave
        // Sensor reads are accumulated into records (8 K default is every 30 seconds or 6 minutes)
        // When a record is finished, the smart sensor slave signals to the comm tower that a record is complete
        //////////// Possible way to double memory since there is space in the comm tower (not yet implemented)
        // When there is room in memory in the comm tower CPU, the data is uploaded
        // If there is no more room, the record stays in the smart sensor and is just counted by the comm tower
        //
        //
        // TODO // Activate phone Between 3AM and 4AM (use the minutes reading of the activation
    	// 			time to randomize the reporting times)
    	// TODO // Store more data on phone, send out the data already uploaded from the sensor. Then upload the rest and send it
        // TODO // Change reporting frequency based on analysis of data. Send SMS if requested to registered phone.
         *
         *
         *
         *

        // All unused pins should be set to Output Low to minimize current drain
         *
         *
         * 			NEW SETTINGS version 0.25 (SMT)

           PIN HARDWARE USAGE			w/o phone				w/phone
           MSP430FR5739					-------------------     -------------------
           PIN CONNCECTION	    PORT	FUNC	HIGH	LOW		FUNC	HIGH	LOW		USAGE
        	1		XIN			J.4		---		---		---		---		---		---		Crystal
        	2		XOUT		J.5		---		---		---		---		---		---		Crystal
        	3		AVSS		---		---		---		---		---		---		---		Analog Ground
        	4		AVCC		---		---		---		---		---		---		---		Analog 3.3 V
        	5		RED LED		1.0		Hardware Status			Phone Reception 		Panel LED
        	6		YELLOW LED	1.1		Off		Off		Off		Phone Status			Panel LED
        	7		GREEN LED	1.2		Off		Off		Off		GPS Status				Panel LED
        	8		BattMon		3.0		OUT-HI	---		---		ADC A12 1.4V	0.9V	Battery Monitor
         	9		SnsrRst		3.1		OUT		Reset	Power	OUT		Reset	Power	Reset Power to smart sensor (powers up after reset)
        	10		TX/RX on	3.2		OUT		TX/RX	OFF		OUT		TX/RX	OFF		Sensor tx/rx enable
			11		Interupt	3.3		IN		COM-RQ	wait	OUT		COM-RQ	wait	Interrupt to request communication from sensor
        	12		Aux3		1.3		Opt		Opt		Opt		Opt		Opt		Opt		Optional (Digital I/O or ADC)
        	13		Aux4		1.4		Opt		Opt		Opt		Opt		Opt		Opt		Optional
        	14		Aux5		1.5		Opt		Opt		Opt		Opt		Opt		Opt		Optional
        	15		TDO			J.0		Opt		Opt		Opt		Opt		Opt		Opt		JTAG connector
        	16		TDI			J.1		Opt		Opt		Opt		Opt		Opt		Opt		JTAG connector
        	17		TMS			J.2   	Opt		Opt		Opt		Opt		Opt		Opt		JTAG connector
        	18		TCK			J.3  	Opt		Opt		Opt		Opt		Opt		Opt		JTAG connector
        	19		SensorTXD	2.5		UCA1TXD	DATA	DATA	UCA1TXD	DATA	DATA	Smart Sensor UART UCA1SIMO
        	20		SensorRXD	2.6 	UCA1RXD	DATA	DATA	UCA1RXD	DATA	DATA	Smart Sensor UART UCA1SOMI
        	21		TEST		TEST  	---		---		---		---		---		---		JTAG connector
        	22		#RST		#RST  	---		---		---		---		---		---		JTAG connector, Reset Button and Switch
        	23		U-OUT		2.0		OUT-HI	---		---		UCA0TXD	DATA	DATA	UART to Phone, UCA0TXD
        	24		U-IN		2.1		IN		---		Drain	UCA0RXD	DATA	DATA	UART from Phone, UCA0RXD
        	25		PhnMon		2.2		IN		???		OFF		IN		ON		OFF		Phone status indicator from phone board (can be monitored by LED)
        	26		TAMPER		3.4     IN		OK		Trouble	IN		OK		Trouble	Sensor cable trouble line (There is a problem if it goes low)
        	27		PhnOn/Off	3.5		OUT-HI	---		---		OUT		Hi-Z	LOW		Phone on/off request (hold low for 3 sec)
        	28		PhnPower	3.6		OUT-LO	ON		OFF		OUT-HI	ON		OFF		Turn on 5V Regulator to power phone card
        	29		LED			3.7		OUT		LED ON	LED	OFF	OUT		LED ON	LED	OFF	Phone power indicator LED (see individual board specs for meaning of blinks)
        	30		Uext6		1.6		Opt		Opt		Opt		Opt		Opt		Opt		Uext connector. UCB0SIMO
        	31		Uext5		1.7		Opt		Opt		Opt		Opt		Opt		Opt		Uext connector. UCB0SOMI
        	32		VCORE		VCORE	---		---		---		---		---		---		Internal power
        	33		DVSS		DVSS	---		---		---		---		---		---		Digital Ground
        	34		DVCC		DVCC	---		---		---		---		---		---		Digital Power
        	35		LED1		2.7		OUT		LED ON	LED OFF	OUT		LED ON	LED OFF	LED1
        	36		PhnReset	2.3		OUT-HI	---		---		OUT		Hi-Z	LOW		Phone Reset (hold low to reset)
        	37		PwrMon		2.4		IN		ON		OFF		IN		ON		OFF		Phone power indicator (Also UEXT pin 9)
        	38		AVSS		AVSS	---		---		---		---		---		---		Analog ground
         *
         *
         *
*/

	// Interrupt code resides in InterruptVectors_init.c

/////////// SMART SENSOR INSTRUCTION SET///////////////////////////////////
	#define SENSOR_OFF 0x00			// Hardware power cycle -- puts ADXL in low power standby with default settings
	#define SENSOR_ON 0x01			// turn on the ADXL sensor (does not change any settings)
	#define SENSOR_RESET			// does a hardware and software reset of the ADXL and puts it in standby and loads settings
	#define MSP_SLEEP 0x03			// put the smart sensor in a low power sleep mode
	#define CALIBRATE 0x04			// calibrate the ADXL chip
	#define REGISTER_SS 0x05		// send new GPS and time settings to register sensor
	#define NEW_PARAMETERS 0x06		// send new parameters for the ADXL chip to the smart sensor
	#define CLEAR_RECORD 0x07		// erase the last record on the smart sensor
	#define CLEAR_ALL 0x08			// clear the entire smart sensor memory
	#define UPLOAD_LAST 0x09		// upload the last record from the smart sensor
	#define UPLOAD_ALL 0x0A			// upload all records from the smart sensor
	#define MANUAL_OPERATION 0x0B	// Read or write data directly to and from the ADXL sensor without processing
	#define STREAM_DATA 0x0C		// Streams of data from the smart sensor
	#define CHECK_ID 0x0D			// upload the GPS and time settings stored in the sensor
	#define SEND_PARAMETERS 0x0E	// upload the old parameters from the smart sensor
	#define PROGRESS_REPORT 0x0F	// request the status of the sensor after the last command
	#define RESUME_RUN_BINS 0x10	// continues a run after an interruption
	#define START_RUN_BINS 0x11		// start up the smart sensor and collect data in bins

/////////////////////DATA RECORDING PARAMETERS//////////////////////////////////
	// SAMPLE_SPEED sets the OUTPUT DATA RATES (ODR) for the ADXL sensor (below)
	// The sample speed is the rate of sensor reads (12.5, 25, 50, 100, 200 or 400 Hz)
	// 0 = 12.5 Hz, 1 = 25 Hz, 2 = 50 Hz, 3 = 100 Hz,  4 = 200 Hz and 5 = 400 Hz
	// Bits 0, 1 and 2  of FILTER_CTL select the Output Data Rate.
	// which also configures internal filters to a bandwidth using the QUARTER_BW or the HALF_BW bit setting.
    #define ODR_12			0x00	// 12.5 Hz ODR
    #define ODR_25	 		BIT0	// 25 Hz ODR
    #define ODR_50 			BIT1	// 50 Hz ODR (reset default)
    #define ODR_100    (BIT1|BIT0)	// 100 Hz ODR
    #define ODR_200	   		BIT2	// 200 Hz ODR
    #define ODR_400    (BIT2|BIT0)	// 400 Hz ODR


#define SAMPLE_SPEED	ODR_100	// Output Data Rate (see above)
#define READ_SPEED		1		// the number of sets of buffer reads per second.
#define SLEEP_INTERVALS 8		// Must be a power of 2.  REED_SPEED X SLEEP_INTERVALS should equal 8
	// The timer is set to have an interrupt every 1/8 of a second, so SLEEP_INTERVALS
	// is number of interrupts to wait before emptying the buffer.
	// Each set of sensor readings is 6 bytes.  2 bytes for each sensor read of X, Y and Z acceleration
	// Temperature is only read once for each record.
	// The ADXL FIFO buffer is set to hold a maximum of 440 readings (a maximum of 100 sensor reading sets of 4)
	// If the speed is 100 Hz or less the buffer can be emptied once per second (SLEEP_INTERVALS = 8)
	// For 200 reads, this should be set to 2 buffer downloads (SLEEP_INTERVALS = 4)
	// For 400 reads, this should be set to 4 buffer downloads. (SLEEP_INTERVALS = 2)
	// the number of samples per second times SLOWHOURSBINSEC can not be more than 64K or bins might overflow


////////////// PROGRAM TIMINGS///////////////////////////
	// Default timings  (You can change these)
#define HOURS 4			// Number of hours for data collection per call-in period once motion detected.  This must be a factor of 120.
#define SLOWHOURS 24	// Number of hours in a slow day of data collection
#define SLOW_DAYS 40	// Number of days that should elapse until there is more frequent reporting

	// These are computed (don't change them)
#define BINSEC (15 * HOURS)		// The default is 30 // Number of seconds in each bin record.  Must be factor of 3600
#define SLOW_BINSEC (15 * SLOWHOURS)	// The default is 360 but it could be less if records are also stored in comm board or phone board.
								// Number of seconds in each bin record on a slow day (every 6 minutes)

	// These can be changed, but the program may need to be adjusted (change with caution)
#define MAX_BIN 10       		// number of histogram bins kept in each record (changing this might cause problems -- check thoroughly)
#define DATA_SIZE 16			// This is MAX_BIN plus other parameters (currently 6: Temp, X, Y, Z, Count, Max)
#define DATA_BYTES 32			// The number of bytes in one record (DATA_SIZE * 2)
#define MAX_RECORDS 240		// The default is 240 // The maximum number of Bin records assuming about 8 K and 32 bytes per record
#define CLOCKSPEED 8			// The number of  million cycles per second for the normal clockspeed

	// System parameters
#define REPORT_HEADINGS YES		// whether or not to print verbose report headings
#define CABLE_LENGTH 50			// the maximum cable length in feet (we roughly need 1 microsecond to discharge each foot of cat5e cable )
								// if you want to be more precise the actual value is .75 microsecond per foot
#define CABLE_DISCHARGE_TIME (50 + CABLE_LENGTH) // The capacitor in the circuit takes 50 microseconds to discharge
#define SHUT_OFF_LEVEL 480		// Battery capacity left at shut-off (2%)
								// 12V NiMH (8 AA * 1.2V) = 549
								// 9V  NiMH = ???

	//  Computed Default timings (Don't change these, change the ones above)
#define SECS_IN_DAY 	86400								// Number of seconds in a day
#define MAXRUN 			(SECS_IN_DAY-60)					// The maximum length of a run before an interrupt (a day minus a minute)
#define MAXRUN4H 		((MAXRUN & 0xFF000000) / 0x01000000)// The highest byte of MAXRUN
#define MAXRUN3 		((MAXRUN & 0x00FF0000) / 0x00010000)
#define MAXRUN2  		((MAXRUN & 0x0000FF00) / 0x00000100)
#define MAXRUN1L 		(MAXRUN & 0x000000FF)				// The lowest byte of MAXRUN
#define BINSEC_HI 		((BINSEC & 0xFF00) / 0x0100)		// The high byte of BIN_SEC
#define BINSEC_LO		(BINSEC & 0x00FF) 					// The low byte of BIN_SEC
#define SLOWBIN_HI 		((SLOW_BINSEC & 0xFF00) / 0x0100)	// The high byte of SLOW_BINSEC
#define SLOWBIN_LO 		(SLOW_BINSEC & 0x00FF)				// The low byte of SLOW_BINSEC
#define MAX_RECORDS_HI 	((MAX_RECORDS & 0xFF00) / 0x0100) 	// The high byte of MAX_RECORDS
#define MAX_RECORDS_LO  (MAX_RECORDS & 0x00FF) 				// The low byte of MAX_RECORDS

	// Temporary settings for sensor test (you can change these)
#define TEMP_SLOWBIN 	 10		// # of seconds for a test record in a test run
#define TEMP_MAXRUN 	 30	// maximum number of seconds to test the sensor
#define TEMP_MAX_RECORDS 2		// maximum number of records to create in a test run

	// Computed temporary settings for sensor test (Don't change these, change the ones above)
#define	TEMP_SLOWBIN_LO 	(TEMP_SLOWBIN & 0x00FF)
#define TEMP_SLOWBIN_HI 	((TEMP_SLOWBIN & 0xFF00) / 0x0100)			// parameters[16] and [17] - The number of seconds to accumulate readings in each record (set of bins)
#define TEMP_MAXRUN1L 		(TEMP_MAXRUN & 0x000000FF)
#define TEMP_MAXRUN2		((TEMP_MAXRUN & 0x0000FF00) / 0x00000100)
#define TEMP_MAXRUN3 		((TEMP_MAXRUN & 0x00FF0000) / 0x00010000)
#define TEMP_MAXRUN4H 		((TEMP_MAXRUN & 0xFF000000) / 0x01000000)	// parameters[18] to [21] - The maximum time the sensor can run without reporting back to comm head.
#define	TEMP_MAX_RECORDS_LO (TEMP_MAX_RECORDS & 0x00FF)
#define TEMP_MAX_RECORDS_HI ((TEMP_MAX_RECORDS & 0xFF00) / 0x0100) 			// parameters[22] and [23] - The maximum number of records to collect


    #define CR 13					// Carriage return
	#define LF	10					// Line feed
	#define COMMA 44				// comma
    #define SPACE 32				// space character
    #define DATA_BUFFER_SIZE 255	// The size of the data buffer for receiving data from UART or phone


    //	TSB = Turtle Sense Board (prefix)


    #define TRUE 1
    #define FALSE 0
    #define YES 1
    #define NO 0
    #define HIGH 1
    #define LOW 0
	#define RECEIVE 0
	#define SEND 1


    // Port 1
    #define TSB_AUX0 BIT0
    #define TSB_AUX1 BIT1
    #define TSB_AUX2 BIT2
    #define TSB_AUX3 BIT3
    #define TSB_AUX4 BIT4
    #define TSB_AUX5 BIT5
	#define TSB_UEXT6 BIT6
    #define TSB_UEXT5 BIT7


    // Port 2
    #define TSB_UCA0TXD BIT0
    #define TSB_UCA0RXD BIT1
    #define TSB_PHNMON BIT2
    #define TSB_PHNRESET BIT3
    #define TSB_PWRMON BIT4
    #define TSB_SENSOR_TXD BIT5
    #define TSB_SENSOR_RXD BIT6
 	#define TSB_LED BIT7			//  LEDs are connected to pin 2.7 and 3.7

    // Port 3
    #define TSB_BATTMON BIT0
    #define TSB_SNSRRST BIT1
    #define TSB_TX_RX_ON BIT2		// Enable communication with smart sensor
    #define TSB_INTERRUPT BIT3 		// Interrupt line from smart sensor
    #define TSB_TAMPER BIT4			// Sensor cable tamper line (should always be high)
    #define TSB_PHNONOFF BIT5
    #define TSB_PHNPOWER BIT6
//    #define TSB_LED2 BIT7			// Also LED2



    // ADXL362 Instruction set for MANUAL OPERATION (command 0x0B0)

// The first byte sent after the command should be the instruction code for the ADXL (un-comment if using)
//#define ADXL_WRITE 0x0A			// Instruction to write to a register
//#define ADXL_READ  0x0B			// Instruction to read from a register
// DO NOT SEND THE following instruction, it will return an error.  To stream directly use command (0x0C)
//#define ADXL_FIFO  0x0D			// Instruction to read the FIFO stack
//
//	//	//	//	//	ADXL362 Registers	//	//	//	//	//	//	//
/*

    Table 11. Register Summary

    Reg		Name			Bits	 Bit 7	 Bit 6	  Bit 5		 Bit 4	   Bit 3		   Bit 2		    Bit 1	   Bit 0	Reset	RW
    0x00	DEVID_AD		[7:0]								DEVID_AD[7:0]													0xAD	R
	0x01	DEVID_MST		[7:0]								DEVID_MST[7:0]													0x1D	R
	0x02	PARTID			[7:0]								PARTID[7:0]														0xF2	R
	0x03	REVID			[7:0]								REVID[7:0]														0x01	R
	0x08	XDATA			[7:0]								XDATA[7:0]														0x00	R
	0x09	YDATA			[7:0]								YDATA[7:0]														0x00	R
	0x0A	ZDATA			[7:0]								ZDATA[7:0]														0x00	R
	0x0B	STATUS			[7:0]  ERR_USER_ REGS|AWAKE  |INACT|ACT		|FIFO_OVER-RUN	|FIFO_WATER-MARK|FIFO_READY	|DATA_READY	0x40	R
	0x0C	FIFO_ENTRIES_L	[7:0]												FIFO_ENTRIES_L[7:0]								0x00	R
	0x0D	FIFO_ENTRIES_H	[7:0]									UNUSED								   |FIFO_ENTRIES_H[1:0]	0x00	R
	0x0E	XDATA_L			[7:0]								XDATA_L[7:0]													0x00	R
	0x0F	XDATA_H			[7:0]							SX			|						XDATA_H[3:0]					0x00	R
	0x10	YDATA_L			[7:0]								YDATA_L[7:0]													0x00	R
	0x11	YDATA_H			[7:0]							SX			|						YDATA_H[3:0]					0x00	R
	0x12	ZDATA_L			[7:0]								ZDATA_L[7:0]													0x00	R
	0x13	ZDATA_H			[7:0]							SX			|						ZDATA_H[3:0]					0x00	R
	0x14	TEMP_L			[7:0]								TEMP_L[7:0]														0x00	R
	0x15	TEMP_H			[7:0]							SX			|						TEMP_H[3:0]						0x00	R
	0x20	THRESH_ACT_L	[7:0]								THRESH_ACT_L[7:0]												0x00	RW
    0x21	THRESH_ACT_H	[7:0]				UNUSED									| 			 THRESH_ACT_H[2:0]			0x00	RW
    0x22	TIME_ACT		[7:0]								TIME_ACT[7:0]													0x00	RW
    0x23	THRESH_INACT_L	[7:0]								THRESH_INACT_L[7:0]												0x00	RW
    0x24	THRESH_INACT_H	[7:0]				UNUSED									|  			THRESH_INACT_H[2:0]			0x00	RW
    0x25	TIME_INACT_L	[7:0]								TIME_INACT_L[7:0]												0x00	RW
    0x26	TIME_INACT_H	[7:0]								TIME_INACT_H[7:0]												0x00	RW
    0x27	ACT_INACT_CTL	[7:0]	RES					  | LINKLOOP 	|  INACT_REF 	|INACT_EN 		|ACT_REF 	|ACT_EN		0x00	RW
    0x28	FIFO_CONTROL	[7:0]				UNUSED					|	AH			|FIFO_TEMP		|		FIFO_MODE		0x00	RW
    0x29	FIFO_SAMPLES	[7:0]								FIFO_SAMPLES[7:0]												0x80	RW
    0x2A	INTMAP1			[7:0]	INT_LOW 	  |AWAKE  |INACT|ACT	|FIFO_OVER-RUN 	|FIFO_WATER-MARK|FIFO_READY	|DATA_READY	0x00	RW
    0x2B	INTMAP2			[7:0]	INT_LOW 	  |AWAKE  |INACT|ACT	|FIFO_OVER-RUN	|FIFO_WATER-MARK|FIFO_READY	|DATA_READY	0x00	RW
    0x2C	FILTER_CTL		[7:0]	     RANGE			  |RES  |HALF_BW|	EXT_SAMPLE	|		 		  ODR					0x13	RW
    0x2D	POWER_CTL		[7:0]	RES			  |EXT_CLK|  LOW_NOISE	|		WAKEUP	|	AUTOSLEEP	|	MEASURE				0x00	RW
*/


//---- ADXL Defines  // Refer to ADXL362 documentation for complete descriptions:  http://dlnmh9ip6v2uc.cloudfront.net/datasheets/BreakoutBoards/ADXL362.pdf

// ADXL bit definitions

   //	FIFO_CONTROL BITS
#define FIFO_AH				BIT3 	// This bit is the MSB of the FIFO_SAMPLES register, allowing FIFO samples a range of 0 to 511 bytes.
#define FIFO_TEMP_ON 		BIT2	// Store Temperature Data to FIFO. 1 = temperature data is stored in the FIFO together with x-, y-, and z-axis acceleration data.
#define FIFO_OFF 			0x00	// BIT0 and BIT1 combinations to Enable FIFO and Mode Selection.
#define FIFO_OLDEST_SAVED 	BIT0	// Only the oldest samples are saved.  Once the FIFO fills, nothing else is stored until the data is read, then it empties
#define FIFO_STREAM			BIT1	// The FIFO is filled and then overwritten with new readings
#define FIFO_TRIGGERED		(BIT0|BIT1)	// b01 = Oldest saved mode.  b10 = Stream mode. b11=Triggered mode.

    //	FILTER CONTROL BITS (select one range, one BW, one ODR)
#define RANGE_2G		0x00	// Default sensitivity setting of +/- 2g
#define RANGE_4G		BIT6	// +/- 4G
#define RANGE_8G		BIT7	// +/- 8G   (use only one range)
#define HALF_BW			0x00	// Default setting of half bandwidth antialiasing filtering
#define QUARTER_BW		BIT4	// More conservative antialiasing of quarter bandwidth
#define EXT_SAMPLE 		BIT3	// External Sampling Trigger. 1 = the INT2 pin is used for external conversion timing control.



    //	POWER CONTROL BITS
#define EXT_CLK			BIT6 	//External Clock. 1 = the accelerometer runs off the external clock provided on the INT1 pin.
								// EXTERNAL CLOCK IS NOT SUPPORTED IN THE CURRENT VERSION OF THE SMART SENSOR
#define INT_CLK 		0x00 	//External Clock. 0 = the accelerometer runs off internal clock (default)
#define NORMAL_NOISE  	0x00	// Bits 4 and 5 select LOW_NOISE Selects Power vs. Noise Tradeoff: 00=Normal (default and lower power consumption)
#define LOW_NOISE 	 	BIT4	// 01 = Low noise mode.
#define ULTRA_LOW_NOISE BIT5	// 10 = Ultralow noise mode.
#define WAKEUP_MODE		BIT3	// Wake-Up Mode.  1 = the part operates in wake-up mode.
#define AUTOSLEEP 		BIT2	// Autosleep. Activity and inactivity detection must be in linked mode or
    							// loop mode (LINK/LOOP bits in ACT_INACT_CTL register) to enable
    							// autosleep; otherwise, the bit is ignored. 1 = autosleep is enabled, and
    							//the device enters wake-up mode automatically upon detection of inactivity.
#define ADXL_STANDBY	0x00	// Bit 0 and Bit1 Selects Measurement Mode or Standby. Start up the chip in Standby, and then switch to Measurement
#define ADXL_ON			BIT1 	// Start recording data in measurement mode


#define	THRESH_ACT_L	0x00 // low byte of the activity threshold (default is 0x00) The sensor only records data when over the threshold
#define	THRESH_ACT_H	0x00 // high byte (lowest three bits) of the activity threshold (default is 0x00)
#define	TIME_ACT		0x00 // When this timer is used, only sustained motion can trigger activity detection (default is 0x00)
#define	THRESH_INACT_L	0x00 // low byte of inactivity threshold (default is 0x00) The sensor stops recording data when under the threshold
#define	THRESH_INACT_H	0x00 // high byte (lowest three bits) of the inactivity threshold (default is 0x00)
#define TIME_INACT_L	0x00 // low byte of inactivity timer (default is 0x00) The sensor goes to sleep after reading this many samples of inactivity
#define	TIME_INACT_H	0x00 // high byte (lowest three bits) of the inactivity timer (default is 0x00)
#define	ACT_INACT_CTL	0x00 // activity/inactivity control byte.  (default is 0x00 -- not activated)
#define	FIFO_CONTROL	(FIFO_AH + FIFO_OLDEST_SAVED) // more than 256 bytes, and oldest save mode.  No temperature readings in the FIFO (once a record is enough)
#define	FIFO_SAMPLES	44 	 //  300 FIFO samples (3 X 100) // Along with AH, this is the number of two byte samples to save in the buffer (subtract 256 if AH is set).
    						 // Should be multiple of 3 unless FIFO_TEMP_ON is added to FIFO_CONTRO, then it should be a multiple of 4.  511 maximum.
							 // Optimizing this so it is not much bigger than needed will make the sensor run on a little less power.
#define	INTMAP1	 		0x00 // no interrupts for now.  This interrupt is not connected to the MSP430
#define	INTMAP2			0x00 // This interrupt is not currently used.  It can send interrupts to the MSP430, or used to sync a clock from the MSP430
#define	FILTER_CTL 		(RANGE_2G | HALF_BW | SAMPLE_SPEED)	  	// 2g sensitivity, normal anti-aliasing and 100 Hz sample rate
#define	POWER_CTL_OFF	(INT_CLK | ULTRA_LOW_NOISE | ADXL_STANDBY) // Startup in standby, and then change the ADXL_ON bit when ready
#define	POWER_CTL_ON	(INT_CLK | ULTRA_LOW_NOISE | ADXL_ON)	 // The byte to send when ready to read data

#define UART_TX_BUSY (!(UCA1IFG & UCTXIFG))    // Bit location to monitor while transmission is in process
#define UART_BUSY (UCA1STATW & UCBUSY)	 	 // Alternate bit location to monitor all UART activity
#define RX_BUFF_EMPTY (!(UCA1IFG & UCRXIFG))	 // Receive byte flag location
#define PHN_TX_BUSY	(!(UCA0IFG & UCTXIFG))    // Bit location to monitor while transmission is in process
#define PHN_RX_BUFF_EMPTY (!(UCA0IFG & UCRXIFG))	 // Receive byte flag location
#define WATCHDOG_RESET WDTCTL = ((WDTCTL & 0x00FF) | watchdog_password)	// mask off the high byte and put in the password



    // FRAM data storage

    #pragma SET_DATA_SECTION(".fram_vars")
    char data_buffer[DATA_BUFFER_SIZE]; 	//phone data buffer (incoming data)
    unsigned char records[TEMP_MAX_RECORDS][32];			// record data storage

    //	[MAX_RECORDS][0-1]   -- Temperature reading (the average of 16 readings)
    //	[MAX_RECORDS][2-3]   -- X Position
    //	[MAX_RECORDS][4-5]   -- Y Position
    //	[MAX_RECORDS][6-7]   -- Z Position
    //	[MAX_RECORDS][8-9]   -- Total number of accumulated readings for each record
    // 	[MAX_RECORDS][10-11] -- Highest Bin: the highest bin corresponds to bincount
    //							(plus one because zero readings are possible) of the highest bit
    //							triggered in any reading.  If there are high readings (>0x0A)
    //							the lowest bins are dropped.  EG:  0x0A = the highest is bin 0x0A,
    //							the lowest bin correspondsto the lowest readings;  0x10 = the highest is bin 0x10
    //							the lowest 6 bins are dropped.
    //  [MAX_RECORDS][22-31] --	The ten highest bin readings.  Bytes 30 and 31 are the highest bin.

    unsigned int battery_level;						// 0 = battery dead, 100 = fully charged
    unsigned int recCount = 0;						// The count of how many records have been collected since last upload
    unsigned char error_code = 0x00;				// for future error handling
    unsigned char count_GPS=0;						// count how long it has been since the last GPS reading
    unsigned char testing = NO;						// a flag for the WDT reset.  If set after restart, bad sensor
	// SENSOR VALUES

	// TEMPERATURE CALIBRATION SETTINGS
	// During Calibration, the reading at 0 degrees C becomes the temperature offset, and the ratio of 0x0200
	// divided by the reading at room temperature (minus the offset) becomes the temperature ratio.
#define CALIBRATE_TEMP YES					// The default is to calibrate the sensors
signed int temperature_offset = 0x00;		// Default setting does not change the reading
unsigned int temperature_ratio = 0x1000;	// The temperature reading adjusted by the offset times this number will be divided by 0x1000,


	// NEST IDENTIFICATION and SENSOR ID
	// This is the data that results from a GPS read from the phone boards
	// The first 32 bytes returned by the AT$GPSACP command is stored just as received
	// This has the UTC time, Latitude and longitude of the reading
	// This is followed by the date of activation and serial number
	// See the top of the program to set the serial number.
//	char temp_ID[] = "123456.890,2345.7890N,34567.9012W,567890,AB4567";
	#define NEST_ID "000000.000,0000.0000N,00000.0000W,000000,000000"
	char nest_ID[] = NEST_ID;			// The unique ID of each nest based on time, date, location and sensor ID
		// NEST_ID is defined as "122330.000,4542.8106N,01344.2720E,240613,AA001"
		// it breaks down as follows:
	#define NEST_ID_BYTES 47  	// the number of bytes in the nest_ID above
		// Starts with the UTC time, and GPS location copied directly from the GPS output.
		// The UTC is in the format: hhmmss.sss
	#define UTC 0				// nest_ID[UTC] - The offset for the GPS UTC time code
	#define UTC_BYTES 6			// The last byte is nest_ID[9], but the last 3 digits always seem to be zeros
	#define HOUR 0
	#define HOUR_BYTES 2
	#define MINS 2
	#define MINS_BYTES 2
	#define SECS 4
	#define SECS_BYTES 2
	// LATITUDE is in the format: ddmm.mmmm N/S  where:
		//	dd - degrees 00..90
		//	mm.mmmm - minutes 00.0000..59.9999
		//  N/S: North / South
	#define LATITUDE 11			// nest_ID[LATITUDE] - The offset for the GPS latitude
	#define LATITUDE_BYTES 10 	// The last byte is nest_ID[20]
		// LOGITUDE is similar: dddmm.mmmm E/W   with the differences:
			// ddd - degrees 000..180
		// E/W: East / West
	#define LONGITUDE  22		// nest_ID[LONGITUDE] - The offset for the GPS longitude
	#define LONGITUDE_BYTES 11 	// The last byte is nest_ID[32]
		// The DATE is the date the sensor was activated in the format: ddmmyy  where:
		// dd - day 01..31
		// mm - month 01..12
		// yy - year 00..99 - 2000 to 2099
	#define DATE 34				// nest_ID[DATE] - The date the sensor was activated
	#define DATE_BYTES 6		// The last byte is nest_ID[39]
	#define DAY 34
	#define DAY_BYTES 2
	#define MONTH 36
	#define MONTH_BYTES 2
	#define YEAR 38
	#define YEAR_BYTES 2
		// The SERIAL NUMBER is a unique id for each Smart Sensor produced
		// The first letter is the hardware version
		// The second letter is the software version
		// The next 4 digits is a unique production number for each device
	#define SERIAL 41		// nest_ID[SERIAL_START] - The offset for the sensor serial number nest_ID[SERIAL_START]
	#define SERIAL_BYTES 6		// The last byte is nest_ID[46]


	// Smart Sensor Interrupt codes
	// These codes are returned after a command 0x0F is received
	// thet explain the reason for generating an interrupt
	// All codes less than 0x80 are not errors.
#define NO_ERROR  0x00 				// = No interrupt was generated -- operation in progress
#define NO_ERROR_INT1 0x01 			// = INT1 generated an interrupt
#define NO_ERROR_INT2 0x02			// = INT2 generated an interrupt
#define NO_ERROR_DATA_READY 0x04 	// = operation completed successfully, data ready to report
#define NO_ERROR_COMPLETE 0x08		// = operation completed successfully, no data to report
		// >= 0x80 = an error condition exists.  The error code is returned:
#define ERROR_SENSOR_TIMEOUT  0x81	// = Sensor Start up failure (timed out)
#define ERROR_NO_COMMAND 0x82		// = Unknown command received
#define ERROR_BAD_DATA 0x84			// = Bad Data received
#define ERROR_TX_TIMEOUT 0x88		// = Data transmission timed out
#define ERROR_OUT_OF_RANGE 0x90		//F = Parameters out of range


	// SMART SENSOR PARAMETERS
	// These 40 bytes can be downloaded from the comm tower to change the behavior of the sensor.
 char parameters[40] = {
		THRESH_ACT_L, THRESH_ACT_H, TIME_ACT, THRESH_INACT_L,
		THRESH_INACT_H, TIME_INACT_L, TIME_INACT_H,
		ACT_INACT_CTL, FIFO_CONTROL, FIFO_SAMPLES,
		INTMAP1, INTMAP2, FILTER_CTL, POWER_CTL_OFF, // parameters[0] to [13] - the first 14 are all the ADXL reset parameters.
		SLEEP_INTERVALS,				// parameters[14] - The number of clock interrupts between each FIFO buffer read.
		READ_SPEED,						// parameters[15] - The number of interrupts per second (default is 8 which equals 1/8 second)
		SLOWBIN_LO, SLOWBIN_HI,			// parameters[16] and [17] - The current number of seconds to accumulate readings in each record (set of bins)
		MAXRUN1L, MAXRUN2, MAXRUN3, MAXRUN4H,	// parameters[18] to [21] - The maximum time the sensor can run without reporting back to comm head.
		MAX_RECORDS_LO, MAX_RECORDS_HI,	// parameters[22] and [23] - The maximum number of records to collect
		CALIBRATE_TEMP,					// parameters[24] - Is the temperature calibration active (YES=calibrate)
		REPORT_HEADINGS,				// parameters[25] - (comm board) Flag if we are sending headings in report (YES=headings)
		BINSEC_LO, BINSEC_HI,			// parameters[26] and [27] timing settings for active days
		SLOW_DAYS,						// parameters[28] -	Number of days of low activity
		SLOWBIN_LO, SLOWBIN_HI,			// parameters[29] and [30] - The number of seconds to accumulate readings in each record (set of bins)
		0,0,0,0,0,0,0,0,0 };			// parameters[31] to [39] reserved for future use
#define PARAM_BYTES 40 // the number of bytes in the parameters structure above

 char temp_parameters[8] = {				// Temporary settings for sensor test
		TEMP_SLOWBIN_LO, TEMP_SLOWBIN_HI,	// parameters[16] and [17] - The number of seconds to accumulate readings in each record (set of bins)
		TEMP_MAXRUN1L, TEMP_MAXRUN2,
		TEMP_MAXRUN3, TEMP_MAXRUN4H, 		// parameters[18] to [21] - The maximum time the sensor can run without reporting back to comm head.
		TEMP_MAX_RECORDS_LO, TEMP_MAX_RECORDS_HI }; //parameters[22] and [23] - The maximum number of records to collect


#pragma SET_DATA_SECTION()						// end of FRAM data section



    		// SRAM variables

    // Non persistant variables -- these will be reset after a powerup
	static const unsigned int watchdog_password = 0x5A00;	// password is 5A hex in the upper byte
    volatile unsigned char new_settings = NO;		// flag for uploading new settings to smart sensor
    volatile unsigned char Python_available = NO;	// flag for knowing if phone boardd has Python available for data storage
    volatile unsigned char sensor_ready = NO;		// flag for interrupt by smart sensor when a record is ready to read
    volatile unsigned char tamper = NO;				// flag for a power failure on the sensor power cable interlock line
    volatile unsigned char tamper_count = 10;		// the count down until it is time to check for a tamper condition
    volatile unsigned char starting_up = YES;		// "Yes" after a reset or power-up, "No" otherwise
    volatile unsigned char force_shut_down = NO;	// Set to "yes" after a bad battery reading
    volatile unsigned int adc_read;					// The data read in the adc interrupt routine
    volatile unsigned int script_counter = 0;
    volatile unsigned char sensor_plugged_in = NO;	// The pull-downs are set assuming that the sensor starts out in an unplugged state
    volatile unsigned char sensor_error = NO;		// The reply from the smart sensor after a command or interrupt request
    volatile unsigned char interruptCount=0;			// counts clock interrupts every eighth of a second, wakes up every 8 counts
    volatile unsigned char led[3] = {0, 0, 0};				// settings for LEDs
    volatile unsigned char led_old[3] = {0, 0, 0};				// settings for LEDs
    volatile unsigned char led_count[3] = {0, 0, 0};		// counters for LEDs
    volatile unsigned char led_blinks[3] = {0, 0, 0};		// blink counters for LEDs
    volatile const unsigned char speeds[4] = {1, 2, 3, 8};  // speeds in 16ths of a second (default of zero is fast)
    volatile unsigned char connected = FALSE;				// Flag for whether sensor is connected
    volatile unsigned char button_pushed = 0;				// counter for how long the button has been pushed
    volatile unsigned char location_found = NO;				// do we know where we are?
    volatile unsigned char red_count = 0;					// the number of short blinks to indicate cell reception


    // AT codes and strings for phone  -- the back slash is for adding quotes \r = CR

     char *messages[] = {
    		"\n",					// messages[0]  not used (marks end of script)
    		"ATE0 V0\r",			// messages[1] turn off echo, return error codes only 0=OK, 4=error
    		ISP_PROVIDER ,			// messages[2] Open Internet connection
    		"AT#SGACT=1,1\r",		// messages[3] Activate context
    		FTP_ACCOUNT ,			// messages[4] FTP login to website
    		"AT#FTPTYPE=0\r",		// messages[5] File type = Binary
    		"AT#FTPTYPE=1\r",		// messages[6] File type = ASCII
    		"AT$GPSACP\r",			// messages[7] Read GPS position
    		"AT#FTPAPP=\"SENSOR_LOG_AA0001.txt\"\r",
    								// messages[8] create or append log for sensor (sensor serial number must be modified)
    		"+++",					// messages[9] Upload escape sequence
    		"AT#FTPCLOSE\r",		// messages[10] Close FTP session
    		"AT#FTPGET=\"parameters.txt\"\r",
    								// messages[ 11] Read parameter list file
    		"AT#FTPAPP=\"ACTIVITY_LOG_2014-01-01.txt\"\r",
    								// messages[12] create or append log of activity (file date must be modified)
    		"AT+CIND=0,0,0,0,0,0,0,0,0\r ",
    								// messages[13] Turn off indicator monitoring
    		"AT+CIND?\r ",			// messages[14] query indicator monitoring (last number is bars of service)
    		"AT+CREG?\r",			// messages[15] Registration report (will return "+CREG: 0,1" when successful.)
    								// 		CREG codes
    								// 		0 - not registered, ME is not currently searching a new operator to
    								// 	   		register to (bad reception?  antenna disconnected?
    								// 		1 - registered, home network
    								// 		2 - not registered, but module is currently searching for an operator to
    								//    		 register to
    								// 		3 - registration denied
    								// 		4 - unknown
    								//		5 - registered, roaming
        	"AT#SERVINFO\r",		// messages[16]  get cellular service info
        	"AT#NITZ=1,0\r",		// messages[17]  set RTC from network time automatically
        	"AT#CCLK?\r",			// messages[18]  get time from RTC
        	"  ",					// messages[19]
        	"AT$GPSP=1\r",			// messages[20] power-up GPS
        	" ",					// messages[21]
        	" ",					// messages[22]
        	" ",					// messages[23]
        	" ",					// messages[24]
        	" ",					// messages[25] LAST AT command
    		"\"\r",					// messages[26] end quotes and CR (end of file name -- special case for error checking)
    		"\r",					// messages[27] CR
    		" ",					// messages[28]
    		"Sensor ID#: 000000 \r",// messages [29]The serial number of the smart sensor
    		" ",					// messages[30]
    		" ",					// messages[31]
        	" ",					// messages[32]
    		" ",					// messages[33]
        	" ",					// messages[34]
    		" ",					// messages[35]
        	"Registration Communicator ID#: ",	// messages[36]
        	COMM_SERIAL_NUMBER,		// messages[37]
    		"Nest GPS Location: ",	// messages[38]
        	"0000.0000N, ",  		// messages[39]  Nest GPS latitude
        	"00000.0000W\r",		// messages[40]  Nest GPS longitude
    		" ",					// messages[41]
        	" ",					// messages[42]
    		"",	// messages[43]
        	"REGISTRATION EVENT Date/Time: 2001/01/01, 01:01:00\r",	// messages[44]  Installation Date and Time
        	" ",					// messages[45]
    		" ",					// messages[46]
    		" ",					// messages[47]
    		"Battery level: ",		// messages[48]
    		" ",					// messages[49]
    		",",					// messages[50] just a comma
    		" ",					// messages[51]
    		" ",					// messages[52]
    		" ",					// messages[53]
    		" ",					// messages[54]
    		" ",					// messages[55]
    		",  ",					// messages[56] comma and a space
    		"_",					// messages[57] underscore
    		" ",					// messages[58]
    		"TurtleSense 0.25 -- CC 4.0 BY-SA NerdsWithoutBorders.Net\r",	// messages[59] COPYLEFT NOTICE
    		"LOW BATTERY -- SHUTTING DOWN\r",	// messages[60]
    		" ",					// messages[61]
    		" ",					// messages[62]
    		"  "					// messages[63]
    		};
    								// If the command is complete it ends with a CR (\r)



    /// SCRIPTS
    static const char sensor_log[] = {		// run this report first
    		 2,  3,  4,  5,  8,  0
    		} ; 				// Open ftp and append information into a sensor log
    static const char activity_log[] = {	// this closes the sensor log and opens the activity log
      		27, 27,  9, 27, 10,  4,  5, 12,  0
    		};  				// Open ftp and append information into an activity log
    static const char report_script[] = {
    		27,		// Start Report heading
    		44, 29, 36, 37, 27, 38, 39, 40, 48,	// Report data (IDs and Installation date)
    		0									//
    		};

    static const char after_report[] = {	// this closes the file and logs out
    		 27, 27, 9, 27, 10,  0};	// after upload, escape sequence and close ftp session

 //   static const char read_parameters[]={
 //   		2,  3,  4,  6, 11, 10,  0	// Open ftp and read in paramteters.txt
 //   		};

    // Error codes
    //------------
    // 	0 OK
    //	1 CONNECT
    //	2 RING
    //	3 NO CARRIER
    //	4 ERROR

    // Counters
    unsigned long int x;		//all purpose 4 byte
    unsigned int y;				//all purpose 2 byte
    unsigned int z;				//all purpose 2 byte


void watchdog_reset(void)
{
	WATCHDOG_RESET;			// reset the watchdog timer
}

    	// Wait a specified number of milliseconds (long int)
void wait(unsigned int wait_millisecs)
{ unsigned int cycles = CLOCKSPEED;		// be sure that CLOCKSPEED is set correctly for the timing of this to work
		while (wait_millisecs > 0) {	// count off the milliseconds
			for (cycles = CLOCKSPEED; cycles>0; cycles--) {  // wait about a millisecond
				__delay_cycles(982) ;
				}
			wait_millisecs-- ;
		}
}


void clear_data(void)
		// erase all data
{	for (y = 0; y < TEMP_MAX_RECORDS; y++) for (z = 0; z < 32; z++) records[y][z] = 0;  // Erase the entire array
	recCount = 0;					// The count of how many records have been collected since last upload
}


			// Sleep for a specified number of clock interrupt (they happen once a second)
void sleep(unsigned int secs)
{
	while (secs) {
		__bis_SR_register(LPM3_bits + GIE); // Enter low power mode until the next tick of the clock
		secs-- ;
	}
}

#define PAUSE 4
		// Set LED to turn on, off, or blink
		// pass the bits for port one and the setting
		// 0 = off
		// 0 - 3 = speed from speed table (see InterruptVectors_init.c)
		// 4  = monitor phone connection (only for LED2)
		// 8  = constantly on
		// 16 = one blink
		// 32 = two blinks
		// 48 = three blinks
		// 64 = four blinks
		// 80 = five blinks
		// 96 = six blinks
		// 112 = seven blinks
		// 128	= cascade (speed set by lower bits)
void blink (unsigned char port1_pins, unsigned char setting)
{	unsigned char led_num;
	unsigned char pin_test = 0x01;
	for (led_num = 0; led_num<3; led_num++) // do this for all three LEDs
	{	if (port1_pins & pin_test)			// check which pins were passed
		{	led[led_num] = setting;			// set the led to the passed setting
											// setting led_count and led_blinks primes the pump
			if (!led_count[led_num]) led_count[led_num] = speeds[led[led_num] & 0x03];
											//  if  the count is zero set the count to the speed bits
			if (!led_blinks[led_num]) 			// if the blink counter is zero
			{	P1OUT &= ~pin_test;				// switch the bit off
				led_blinks[led_num] = (led[led_num]>>3) + PAUSE;
			}								// reset the counter for twice the number of blinks plus the pause time
		}
		pin_test<<=1;	// try the next pin
	}
}
// These are the settings used by the blink routine
// blinking is interrupt driven
#define OFF 0
#define VERY_FAST 0
#define FAST 1
#define MEDIUM 2
#define SLOW 3
#define MONITOR_PHONE 4
#define CONSTANT_ON 8
#define ONE_BLINK 16
#define TWO_BLINKS 32
#define THREE_BLINKS 48
#define FOUR_BLINKS 64
#define FIVE_BLINKS 80
#define SIX_BLINKS 96
#define SEVEN_BLINKS 112
#define CASCADE 128
		// Speeds can be or'ed with other settings
		// speeds (except monitor phone and constant on)
		// speeds by themselves are constant blinking

#define RED    	 TSB_AUX0		// Red LED on Hand Held Unit
#define YELLOW 	 TSB_AUX1		// Yellow LED on Hand Held Unit
#define GREEN  	 TSB_AUX2		// Green LED on Hand Held Unit
#define BUTTON	 TSB_AUX3 		// Green button on Hand Held Unit
#define ALL_LEDS (RED|YELLOW|GREEN)	// all three LED bits



unsigned int readBattery(void)
{	unsigned int ADC_Result=0;
	REFCTL0 |= REFON; 			// Turn on 1.5V internal reference
	wait(10);					// Let ref voltage stabilize for 10 mSec
	ADC10CTL0 &= ~ADC10ENC ;  	// ADC10_B disabled
	ADC10CTL0 |= ADC10ON ;    	// ADC10_B module turned on
	ADC10CTL1 &= ~(ADC10SHS0 | ADC10SHS1) ;     // Trigger on ADC10SC

				//Enable and Start the conversion in Single-Channel, Single Conversion Mode
	for(x = 16; x>0; x--)
	{			// we'll take sixteen readings and average them
         ADC10CTL0 |= (ADC10ENC | ADC10SC) ;    // Enable ADC10_B and start sample and conversion
         while (!(ADC10IFG & ADC10IFG0) ); 		// wait until conversion is finished
                // Accumulate ADC10_B conversion results
         ADC_Result += ADC10MEM0;   			// add the 10 bit result into the total
    }
	ADC10CTL0 &= ~ADC10ON ; 	// ADC10_B module turned off
	REFCTL0 &= ~REFON;			// Turn off 1.5V reference
	battery_level = (ADC_Result >>4) ; 		// Make it into a 10 bit number again
	return (battery_level);
}


// TRANSFER A STRING
	// sends or receives a character array of known length using the UART
	// 1st parameter is the pointer to the origin or destination string,
	// the second is the length of that array
	// the third is starting byte offset of the first character to move
	// the fourth is the number of bytes to transfer
	// the fifth is whether sending or receiving 0 = RECEIVE, 1 = SEND
void transfer_string( char *string_ptr, unsigned char string_cnt, unsigned char start_byte, unsigned char bytes2move, unsigned char sending)
{	unsigned char temp_count;		// counter
	while (UART_TX_BUSY);			// wait for no UART activity
	UCA1TXBUF = start_byte;			// the first byte sent is the starting byte
	for (temp_count = 0; temp_count < start_byte; temp_count++) string_ptr++; // adjust pointer to correct location
	while (UART_TX_BUSY);			// wait for an empty buffer
	UCA1TXBUF = bytes2move;			// the second byte sent is how many bytes to transfer
	if (!sending)
	{	for (; bytes2move; bytes2move--)	// count off how many we've gotten
		{		while (RX_BUFF_EMPTY) ;		// wait for a byte
				*string_ptr  = UCA1RXBUF;	// store the parameter
				string_ptr++;				// increment the pointer
		}
		return;
	}
	for (; bytes2move; bytes2move--)// count off how many we're sending
	{	while (UART_TX_BUSY);		// wait for an empty buffer
		UCA1TXBUF= *string_ptr;		// send the parameter
		string_ptr++;				// increment the pointer
	}
	return;
}


	// MOVE_STRING
	// moves characters from one string (origin) to another (destination), overwriting the destination
	// the first parameter is the pointer to the string that is the origin of the characters
	// the second parameter  is the offset into the offset into that string for the first byte to move
	// the third parameter is the number of characters to move
	// the fourth parameter is the pointer to the string that is the destination of the characters
	// the fifth parameter is the offset into that string where the first byte gets put
	// CAUTION!  There is no error checking in this routine.  BE CAREFUL!

void move_string(char *origin_ptr,   unsigned char origin_offset,  unsigned char bytes2move,
									 char *destination_ptr,  unsigned char destination_offset)
{	unsigned char temp_count;		// counter
	for (temp_count=0; temp_count < destination_offset; temp_count++) destination_ptr++;
							// adjust the destination pointer to its actual destination
	for (temp_count=0; bytes2move; temp_count++)	// count through all of them
	{	if (temp_count >= origin_offset)			//only move the data once we're at the offset
		{	bytes2move--;							// decrement the number of bytes left to move
			*destination_ptr =  *origin_ptr;		// move a character from the origin string to the destination string
			destination_ptr++ ;						// increment the destination pointer only after a move
		}
		origin_ptr++;								// always increment the pointer to the origin
	}
}


////////// SS_command -- Send SMART SENSOR COMMANDS
		// send a command byte over the UART to the smart sensor
		// Smart Sensor Interrupt codes
		// These codes are returned after a command 0x0F is received
		// thet explain the reason for generating an interrupt
		// All codes less than 0x80 are not errors.
		#define NO_ERROR  0x00 				// = No interrupt was generated -- operation in progress
		#define NO_ERROR_INT1 0x01 			// = INT1 generated an interrupt
		#define NO_ERROR_INT2 0x02			// = INT2 generated an interrupt
		#define NO_ERROR_DATA_READY 0x04 	// = operation completed successfully, data ready to report
		#define NO_ERROR_COMPLETE 0x08		// = operation completed successfully, no data to report
		#define NO_ERROR_START_UP 0x10		// = Normal start-up
		#define NO_ERROR_SIGN_OFF 0x20		// = processing beginning, communication over
		#define NO_ERROR_READY_TO_RECEIVE 0x40 // = sensor is ready to receive data

				// >= 0x80 = an error condition exists.  The error code is returned:
		#define ERROR_SENSOR_TIMEOUT  0x81	// = Sensor Start up failure (timed out)
		#define ERROR_NO_COMMAND 0x82		// = Unknown command received
		#define ERROR_BAD_DATA 0x84			// = Bad Data received
		#define ERROR_TX_TIMEOUT 0x88		// = Data transmission timed out
		#define ERROR_OUT_OF_RANGE 0x90		// = Parameters out of range (bug in code)
void SS_command(char command2send)
{	unsigned char command_check=0xFF;
	wait(1);
	UCA1CTL1 |= UCSWRST;			// Reset the UART;
	__no_operation();
	UCA1CTL1 &= ~UCSWRST;			// Start the UART
	wait(2);						// give the sensor a chance to finish whatever it is doing
	while(UART_TX_BUSY);			// wait for the UART to finish
	UCA1TXBUF = command2send;		// Send the command byte
	if (command2send == 3) return;	// The sensor won't respond because it is asleep
	while (RX_BUFF_EMPTY) ;
	command_check = UCA1RXBUF;
	while (RX_BUFF_EMPTY) ;
	sensor_error = UCA1RXBUF;
	if (sensor_error == NO_ERROR_SIGN_OFF) P3OUT &= ~TSB_TX_RX_ON ;
											// Turn off RS485 -- communication is finished for the time being
	error_code = sensor_error & 0x80;		// for now just blink //TODO// more robust error handling
	if (command2send != command_check) error_code = 0x80;
}

////// SS_receive_records -- RECEIVE RECORDS FROM THE SMART SENSOR
	// receive data over the UART from the RS485
	// can receive 64K records of 255 bytes maximum
	// records[][] needs to be declared to be large enough to handle whatever comes in here.
	// probably no more than 240 records of 32 bytes (about 8K) on the MSP430FR5739
int SS_receive_records(void)
{	unsigned char data_count;
	unsigned int record_count = 0;
	unsigned char data_size;
	unsigned int records_coming;
	while(RX_BUFF_EMPTY);			// wait for the first byte
	records_coming = UCA1RXBUF;		// the first byte sent will be the high byte of the number of blocks to expect
	records_coming <<= 8;			// move it to the high byte
	while(RX_BUFF_EMPTY);			// wait for the second byte
	records_coming += UCA1RXBUF;	// the second byte sent will be the low byte of the number of blocks to expect
	if (records_coming)				// if no data, the reply is zero
	{	while(RX_BUFF_EMPTY);		// wait for another byte
		data_size = UCA1RXBUF;		// the third byte sent will be the count of how many bytes to receive in each record
		while (record_count < records_coming)		//loop for each record
		{	data_count = 0;							// start the data count at zero
			while (data_count < data_size)			//loop for each byte
			{	while(RX_BUFF_EMPTY);		// wait for a byte
				records[record_count][data_count] = UCA1RXBUF;	// Store the byte in the buffer
				data_count++;				// increment data count
			}
			record_count++;
		}
	}
	return(record_count);			// return the number of records received
}


void reset_phone_UART(void)
{		wait(10);
		UCA0CTL1 |= UCSWRST;			// Reset the UART for the phone
		__no_operation();
		UCA0CTL1 &= ~UCSWRST; 			// enable the UART for the phone
		wait(10);
}


	// Receive a response from the phone
	// The error code will be the penultimate character
unsigned int ATresponse(unsigned char CRs_expected)	// Pass the number of lines expected (additional lines are ignored)
{	unsigned int data_count = 0;				// how many bytes are received
	if(!CRs_expected) return(0);				// don't do anything if no response is expected
	while (CRs_expected)						// get whatever else comes in
	{ 	while(PHN_RX_BUFF_EMPTY);				// wait for a byte
		data_buffer[data_count] = UCA0RXBUF;	// store the byte received in a buffer
		if (data_buffer[data_count] == CR) CRs_expected--;	// count off the CRs received
		data_count++;							// increment the number of bytes received
	}
	if (data_count>1) return(data_buffer[data_count - 2]-0x30) ; // return error state:  0 = OK, 4=Error
	return(0);
}

		// sends a text message to the phone without expecting a response
		// returns the last character sent so we can check for a CR
unsigned char sendMessage(unsigned char message_code)	// the array number of the pointers is passed
{ 	unsigned char AT_tx_char;					// The character we will send out
	unsigned char last_char;					// used to check if the last char is a CR
	const char *string2send;					// we'll put the pointer here
	string2send = messages[message_code];		// get the pointer out of the array
	while (AT_tx_char = *string2send)			// get the next character and also continue looping if it is non-zero
	{ 	while(PHN_TX_BUSY);						// wait for the UART to finish
		last_char = AT_tx_char;					// save the last char
		string2send++;							// increment the pointer so it points to the next char
		UCA0TXBUF = AT_tx_char;					// Send the byte
	}
	return (last_char);
}


		// Sends an AT code or string to the phone gets a response and waits
void sendATcommand(unsigned char AT_message_code)	// the array number of the pointers is passed
{ 	error_code = 0;								// no error in the middle of lines
	if (sendMessage(AT_message_code) == CR)		// send the message and check for things to do at the end of lines
	{	error_code = ATresponse(1);				// wait for a reply for FTP commands
		wait(980); 								//wait a second between AT commands
	}
}


		// process a script for the phone
void do_script(const char *script)	//pass the pointer to the script array
{	unsigned char code;				// the command codes in the script
	const char *script_ptr;			// This pointer we can change
	script_ptr = script;			// set it to point to the script
	reset_phone_UART();				// clear out the buffers before starting
	while (code = *script_ptr)		// get the code pointed to in the script array and stop at the end of the string
	{ 	if (code == 9) wait(2000); 	// add 2 seconds before and after escape routine (code 9)
		if (code<27) sendATcommand(code);	// codes less than 27 are ATmessages
		else sendMessage(code);			// otherwise it is just text
		script_ptr++;				// point to the next character in the script
		if ((code == 26) && (error_code == 4)) script_ptr = script;	// reset the pointer and restart the script if there is a problem opening files
		if (code == 9) wait(2000); 	//  3 seconds total before and after escape routine
	}
}


		// get an internet connection with the phone board
void internet_connection(void)
{	reset_phone_UART();
	sendATcommand(1);							// Send the AT command to turn off echo and verbose responses
				// TODO // Modify so that this will work with all Janus boards automatically.
				// The following line is necessary for an HE910.
	while (data_buffer[1] != 0x2B ) error_code = ATresponse(1);  	// Wait for board to say "+PACSP0" (don't know why this happens)

	sendATcommand(20);			// Turn on GPS if hand-held unit
	sendATcommand(17);			// Set phones RTC from network time automatically
	sendATcommand(15);			// Send AT code to request a registration report
	while (data_buffer[9] != 0x31)	// Keep trying until there is a connection
	{	sleep(1);				// wait a second before trying again
		reset_phone_UART();
		sendATcommand(15);		// Send AT code to request a registration report
	}							// TODO // better error handling!
	sendATcommand(13);			// turn off unsolicited event monitoring
}


// turn off the phone
void phone_off(void)
{
P2OUT &= ~TSB_UCA0TXD;		// Set UART transmit bit low
P2OUT|= TSB_PHNRESET;		// Make certain that the reset bit is off
wait(1);					// wait for the chips to settle?
while (P2IN & TSB_PWRMON)	// keep doing this while the phone power monitor stays high
{	P3OUT &= ~TSB_PHNONOFF;	// switch the phone power on/off by driving the pin low by setting P3.5 low
	wait(3000);				// wait at least 3 seconds
	P3OUT |= TSB_PHNONOFF;	// return the pin to hi-z by setting 3.5 high
	wait(3000);				// wait at least 3 seconds
}
P3OUT &= ~TSB_PHNPOWER;				// Turn off the phone power
}



void shutdown(void)
{	testing = 0;			// reset for next time
	phone_off();				// turn the phone off it is on
	P3OUT &= ~TSB_PHNPOWER;		// Turn off the phone's power
	SS_command(MSP_SLEEP);		// send SS the command to go to sleep
	sleep(60);  // blink for a minute before powering down
	blink(ALL_LEDS, CASCADE);
	sleep(1);
	blink(ALL_LEDS, OFF);
	while (TRUE) __bis_SR_register(LPM4_bits + GIE); 	// Enter low power mode 4
}

	// set the RTC from the cell tower
void set_time(void)
{	reset_phone_UART();									// empty the buffers
	sendATcommand(18);									// request network time
	// store the date and time into the nest ID
	move_string(data_buffer, 8, YEAR_BYTES, nest_ID, YEAR);
	move_string(data_buffer, 14 , DAY_BYTES, nest_ID, DAY);
	move_string(data_buffer, 11, MONTH_BYTES, nest_ID, MONTH);
	move_string(data_buffer, 17, HOUR_BYTES, nest_ID, HOUR);
	move_string(data_buffer, 20, MINS_BYTES, nest_ID, MINS);
	move_string(data_buffer, 23, SECS_BYTES, nest_ID, SECS);
	// also send the date and time to the report field -- messages[44] --"REGISTRATION EVENT date/time: 2001/01/01, 01:01:00\r"
	move_string(nest_ID, YEAR, YEAR_BYTES, messages[44], 32);
	move_string(nest_ID, MONTH, MONTH_BYTES, messages[44], 35);
	move_string(nest_ID, DAY, DAY_BYTES, messages[44], 38);
	move_string(nest_ID, HOUR, HOUR_BYTES, messages[44], 42);
	move_string(nest_ID, MINS, MINS_BYTES, messages[44], 45);
	move_string(nest_ID, SECS, SECS_BYTES, messages[44], 48);
	// put the date in the file name for the activity log -- messages[12] -- "AT#FTPAPP=\"ACTIVITY_LOG_2014-01-01.txt\"\r",
	move_string(nest_ID, YEAR, YEAR_BYTES, messages[12], 26);
	move_string(nest_ID, MONTH, MONTH_BYTES, messages[12], 29);
	move_string( nest_ID, DAY, DAY_BYTES, messages[12], 32);
}


		// send out a nibble as an ASCII hexadecimal number from 0 to F
void send_nibble(unsigned char nibble)		// pass the nibble and we'll convert it
{	if (nibble > 0x09) nibble += 7;			// add 7 if the nibble is ten or more
	while(PHN_TX_BUSY);						// wait for the UART to finish
	UCA0TXBUF = (nibble + 0x30);			// Send the byte converted to ASCII
}

		// send out a byte in two nibbles
void send_byte(unsigned char data_byte)		// pass the byte to send
{		send_nibble(data_byte>>4);			// send high nibble of byte
		send_nibble(data_byte & 0x0F);		// send low nibble of byte
}

		// sends out an integer in four nibbles
void send_integer(unsigned int data_integer)
{
	send_byte((data_integer & 0xFF00)>>8); // print the high byte of recCount
	send_byte(data_integer & 0x00FF);	// print the low byte of recCount
}


void getGPSinfo(void)
{	unsigned char test_char, data_count = 0, commas = 0, countGPS = 0;
	reset_phone_UART();					// clear out buffers
	if (sendMessage(7)) error_code = ATresponse(2);		// send the message, get a two line response
	for (countGPS=0; (test_char = data_buffer[countGPS+9]); countGPS++)
									// go through all the chars received, stop at a zero
	{	if (test_char == COMMA) commas++;	// parse out fields based on finding commas
		if (commas < 3)	nest_ID[data_count++] = test_char;
					// we just want to save the 1st, 2nd and 3rd field
	}
//	temp_ID[data_count++] = 0; //end the string with a zero
	if (countGPS > 40)				// we should get more than this
	{	move_string(nest_ID,  LATITUDE, LATITUDE_BYTES, messages[39], 0);	// store the nest location
		move_string(nest_ID,  LONGITUDE, LONGITUDE_BYTES, messages[40], 0);
		location_found = YES;		// we know where we are
	}
	else location_found = NO;		// we don't know where we are
	if (location_found) blink(GREEN, CONSTANT_ON);  // indicate that  we know where we are
	else blink(GREEN, SLOW);						// indicate that we are still looking
}

	// returns the number of bars from 1 to 6
void cell_reception(void)
{	unsigned char blink_setting;
	reset_phone_UART();				// clear out buffers
	sendMessage(14);				// send the message
	blink_setting = ATresponse(1);
	if(blink_setting == 9) blink(YELLOW, CONSTANT_ON); 	// 99 means no reception
	else blink(YELLOW, FAST+((blink_setting+1)<<4)); 	// add one to the number and multiply by 16
}





		// Check for a sensor connection
		// The default setting before connection is for P3.3 (INTERRUPT) to have a pull-down on the input, and the default
		// for P.3.4 (TAMPER) is to have a pull-up so that the inputs do not float.
		// Once the sensor is connected the pull-up and pull-down is removed so there is no current drain.
		// sensor_plugged_in is set when the sensor is connected.
		// This routine polls these the TAMPER line to see if it is high.  If sensor_plugged_in is not set, then
		// a pull-down resistor is turned on before polling.  If the line is low,
unsigned char sensor_connection(void)
{	P3REN = (BIT3 | BIT4); // enable the pull down resistor on the TAMPER line
	if (sensor_plugged_in) wait(CABLE_DISCHARGE_TIME); // wait long enough to discharge the capacitors filtering the line
	if (P3IN & TSB_TAMPER)
	{	// there is no tamper condition if the pin is high
		if (!sensor_plugged_in)
		{	//  the pull-down resistors are no longer needed because the sensor was just plugged in
		    P3REN = 0;	 // Port 3 Resistor Enable Register
			sensor_plugged_in = YES;	// remember for next time
		}
		return(YES);
	}	// there is a tamper condition if the pin is low -- the pull-down down resistors are still needed
	sensor_plugged_in = NO;				// remember for next time
	return(NO);
}


	// Turn on the RS485 chips and confirm everything is alright
void rs485_on(void)
{	UCA1CTL1 |= UCSWRST;			// Reset the UART;
	__no_operation();
	UCA1CTL1 &= ~UCSWRST;			// Make sure the UART is on
	P3OUT |= TSB_TX_RX_ON ;			// Turn on RS485
	while (P3IN & TSB_INTERRUPT);	// wait for sensor to acknowledge RS485 is on by setting the interrupt line low
	wait(5);						// The interrupt line sometimes goes low even without the acknowledgment.
	SS_command(PROGRESS_REPORT);	// send SS the command to see if all is good
}

	// Wait for the sensor to finish whatever it was doing
void contact_sensor(void)
{	wait(1);						// allow RS485 to finish last send
	P3OUT &= ~TSB_TX_RX_ON ;		// Turn off RS485
	testing=1;						// start the testing counter limit testing to 40 seconds
	while (!(P3IN & TSB_INTERRUPT) && (testing<40));// Wait for Sensor to indicate that it is ready to talk
	if (testing>=40)			// we've timed out.  Something is wrong with the sensor
	{	blink(ALL_LEDS, OFF);	// show that there is a bad sensor
		blink(RED, FAST);		// with a fast red blink
		shutdown();				// blink a minute and power down
	}
	testing = 0;			// reset for next time
	rs485_on();						// establish communications
}

void power_monitor(void)
{	if (readBattery() < SHUT_OFF_LEVEL)	// if low battery, shut down
	{	blink(ALL_LEDS, OFF);
		blink(RED, SLOW);	// slow red blinking to indicate battery problem
		shutdown();			// blink a minute and power down
	}

}

		// Turn on the phone
void phone_on(void)
{		// Enable REF Module and prepare to turn on the phone
	unsigned char tries = 0;
	REFCTL0 |= REFON;				// Turn on 1.5V internal reference
	P2OUT |= TSB_PHNRESET;			// Set phone reset bit high  to set the phone pins to hi-z
	P3OUT|= TSB_PHNONOFF;			// Set on/off bit high to set the phone pins to hi-z
	P2OUT &= ~TSB_UCA0TXD;			// Set UART transmit bit low
	P3OUT |= TSB_PHNPOWER;			// Turn on the phone power
	wait(10);						// wait for 10 milliseconds
	while (P2IN & TSB_PWRMON) 		// check if the power monitor pin (P3.3) is high
	{	P3OUT &= ~TSB_PHNPOWER;		// while it is, turn off the phone board power supply
		while (P2IN & TSB_PWRMON) wait(1);
			// loop until the power monitor pin (P3.3) is low.  Pause a millisecond before checking again
		P3OUT |= TSB_PHNPOWER;		// Turn on the phone power
		wait(10);					// wait for 10 milliseconds for the capacitor to charge up
	}
		// actually turn on the phone
	while (!(P2IN & TSB_PWRMON)) 	// While the power monitor indicator bit is off
	{	P3OUT &= ~TSB_PHNONOFF;		// switch the phone power on/off by driving the pin low by setting P3.5 low
		force_shut_down = (readBattery() < SHUT_OFF_LEVEL) ;	// if the battery is low, set a flag to shut down after uploading data
		wait(3000);					// with the battery reads this will be over 3 seconds
		P3OUT |= TSB_PHNONOFF;		// return the pin to hi-z by setting 3.5 high
		wait(4000);
		tries++;
		if (tries >5)
		{	blink(RED, SLOW);	// slow red blinking to indicate battery problem
			shutdown();			// turn everything off
		}

	}
	watchdog_reset();  // reset watchdog timer
}



		// AWAIT CONNECTION
		// Check to see if the smart sensor is plugged in.
		// go to sleep when the sensor is disconnected to save power.
		// check for a connection every 10 seconds.
void await_sensor_connection()
{	while (!sensor_connection())
	{	sleep(1);
		cell_reception();
		sleep(1);
		if (++count_GPS > 3)						// check every 8 seconds
		{	count_GPS=0;
			getGPSinfo();  // update GPS location every loop
		}
		watchdog_reset();
	}
	sleep(1);			// let the smart sensor power up for a moment longer
}

			//  Create two reports
void phone_it_in(void)
{			blink(ALL_LEDS, OFF);
			wait(100);						// give the LEDs a chance to turn off
			blink(ALL_LEDS, FAST);
			do_script(sensor_log);			// send command to open FTP and create a file
			do_script(report_script);		// creates most of the report
			send_integer(battery_level ); 	// print the battery level
			if (force_shut_down) sendMessage(60); // send shutdown warning
			do_script(activity_log);		// end the ftp session and start another
			do_script(report_script);		// creates most of the report
			send_integer(battery_level ); 	// print the battery level
			if (force_shut_down) sendMessage(60); // send shutdown warning
			do_script(after_report);		// this closes the file and logs out
									// TODO // confirm transmission -- schedule resend if there's a problem.
									// TODO // upload updates
									// TODO // confirm updates
			watchdog_reset();   	// reset the watchdog timer
			tamper = NO;			// reset the tamper flag
			blink(ALL_LEDS, CASCADE);// show that we were successful
			wait(2000);
			blink(ALL_LEDS, OFF);
}



	// START UP routine
void start_up(void)
{	blink(GREEN, SLOW);
	blink(RED, CONSTANT_ON);		// indicate that the sensor is not connected
	// Check  for a sensor and test it
	clear_data();					// erase the previously uploaded data
	P3OUT &= ~TSB_SNSRRST;			// turn on power to the sensor
	await_sensor_connection();		// wait in low power mode until the smart sensor is plugged in.
	blink (ALL_LEDS, CASCADE|FAST);	// indicate that we are testing
	contact_sensor();				// wait for the sensor to respond and turn on RS485
	SS_command(PROGRESS_REPORT);	// send SS the command to see if all is good
	SS_command(CHECK_ID);			// ask sensor to send the nest_ID
	if (error_code & 0x80)			// check for a sensor problem
	{	blink(ALL_LEDS, OFF);
		blink(RED, FAST);			// blink constantly if there is one
		shutdown();					// blink a minute and power down
	}
	// Get the previous sensor ID and store it where it is needed
	transfer_string(nest_ID, NEST_ID_BYTES, 0, NEST_ID_BYTES, RECEIVE);	// receive the nest_ID from the sensor
	move_string(nest_ID,  SERIAL, SERIAL_BYTES, messages[29], 12);		// also store the serial number for the log
	move_string(nest_ID,  SERIAL, SERIAL_BYTES, messages[8], 23);		// and the sensor log file name

	watchdog_reset();  // reset watchdog timer
	if (readBattery() < SHUT_OFF_LEVEL)
	{	blink(ALL_LEDS, OFF);
		blink(RED, SLOW);
		shutdown();					// blink a minute and power down
	}
	// Temporarily change to two 30-second records
	move_string(parameters, 0, PARAM_BYTES, data_buffer, 0); 	// make a temporary copy of the paramters
	move_string(temp_parameters, 0, 8, data_buffer, 16);		// make some temporary changes
	SS_command(NEW_PARAMETERS);			// send the parameters to the sensor
	wait(1);							// don't get ahead of the sensor
	transfer_string(data_buffer, PARAM_BYTES, 0, PARAM_BYTES, SEND);	// send all the temporary parameters
	contact_sensor();					// wait for the sensor to respond and turn on RS485
	SS_command(START_RUN_BINS);			// send SS the command to collect data and process
	while (!(P3IN & TSB_INTERRUPT));	// The smart sensor will set this pin high when all recording is completed
	contact_sensor();					// set up communications
	SS_command(UPLOAD_ALL);				// send SS the command to upload all the records
	recCount = SS_receive_records();	// upload what is sent
	SS_command(NEW_PARAMETERS);			// send the parameters to the sensor
	wait(1);							// don't get ahead of the sensor
	transfer_string(parameters, PARAM_BYTES, 0, PARAM_BYTES, SEND);	// send all the parameters
	contact_sensor();					// wait for the sensor to respond and turn on RS485
	blink(ALL_LEDS, OFF);			// if we get this far the sensor is good!
	blink(GREEN, SLOW);			// indicate that we are still looking

}

void reset(void)
{	P3OUT |= TSB_SNSRRST;			// turn off power to the sensor
	P3OUT &= ~TSB_TX_RX_ON ;		// Turn off RS485
	REFCTL0 &= ~REFON; 				// Turn off 1.5V internal reference
	sleep(1);						// but just for a second
	blink(ALL_LEDS,OFF);			// turn them all off
	blink(RED, CONSTANT_ON);
	blink(YELLOW, CONSTANT_ON);
	power_monitor();				// check the battery
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// MAIN ROUTINE ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
		// sends parameters and registration information to the smart sensor and controls reporting via
		// Janus Plug-in terminus boards
void main(void)
{	Grace_init();                   // Activate Grace-generated configuration
	blink(ALL_LEDS,CASCADE);		// show that it is running
	reset();						// cold start or sensor unplugged
	phone_on();						// power up phone
	internet_connection();			// wait for the phone to establish communication

	while (TRUE)
	{	start_up();					// initialization after cold-start, reset, or disconnect
		connected = TRUE;
		while (connected)			// Main loop -- never stops
		{	__bis_SR_register(LPM3_bits + GIE); // Enter low power mode until the next tick of the clock (every second)
			power_monitor();					// check the battery
			cell_reception();					// the number of short red blinks is the number of bars of reception
			count_GPS++;
			if (button_pushed > 2)				// check counter set by interrupt
			{	set_time();						// reads RTC from the phone and saves it in messages
				// Store the updated sensor ID
				SS_command(REGISTER_SS);		// ask sensor to receive the nest_ID
				transfer_string(nest_ID, NEST_ID_BYTES, 0, NEST_ID_BYTES, SEND);	// receive the nest_ID from the sensor
				phone_it_in();					// turn on the phone and send a report with the uploaded data
				count_GPS = 8;					// force a GPS read to light up the indicator
				button_pushed = 0;				// reset the button counter
			}
			// Get the GPS location
			if (count_GPS > 7)						// check every 7 seconds
			{	count_GPS=0;
				getGPSinfo();  // update GPS location every loop
			}
			if (!sensor_connection())
			{	P3OUT |= TSB_SNSRRST;		// turn off power to the sensor
				tamper = YES;				// If it is not connected there is a tamper condition possible
				connected = FALSE;			// Await a reconnection and then resume operation
				blink(ALL_LEDS,FAST);
				reset();					// reset sensor, etc...
			}
			watchdog_reset();  // reset watchdog timer every second.  If something hangs, the timer will cause a reset in about 4 minutes.
		}
		P3OUT |= TSB_SNSRRST;			// turn off power to the sensor
	}
	// we'll never get here!
}

