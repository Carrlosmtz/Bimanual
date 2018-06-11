///////////////////////////////////////////////////////////////////////////////
//
// UDPClientHub.cpp
//
///////////////////////////////////////////////////////////////////////////////
//
// Author: Jason Fong (TBS, University of Alberta)
//	Email: jmfong@ualberta.ca
//
// Description: 
//
// Acknowledgements: 
//
// Note: 
//
// Date of Last Revision: 2017-09-19
//
///////////////////////////////////////////////////////////////////////////////
//
// Table of contents
//	1. Libraries, namespaces, and typedefs - ln. 
//	2. Initialization - ln. 
//	3. Function forward declarations - ln. 
//	4. Main function - ln. 
//	5. Control communication thread - ln. 
//	6. Feedback communication thread - ln. 
//
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//
// Libraries, namespaces, and typedefs
//
///////////////////////////////////////////////////////////////////////////////

#ifndef UNICODE
#define UNICODE
#endif
#pragma comment (lib, "winmm.lib")

//#define WIN32_LEAN_AND_MEAN /* windows.h includes winsock.h if you don't
//	define that symbol; this creates havoc if you want to include winsock2.h,
//	as everything gets redefined. */
#include <winsock2.h>
#include <windows.h>
#include <WinBase.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h> 
#include "UDP.h"

// Include Quarc-needed header files
#include <signal.h>
#include <tchar.h>

#include "quanser_messages.h"
#include "quanser_thread.h"
#include "quanser_persistent_stream.h"

// Quanser's timer header file for controlling the sampling period
#include "quanser_timer.h"

// UDP data transmission specific to motion tracker camera server
#include <tchar.h>
#include "targetver.h"
#include "data_transmission.h"

using namespace std;

typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char byte;



////////////////////////////////////////////////////////////////////////////////
//
// Initialization
//
////////////////////////////////////////////////////////////////////////////////


// Prep work for TCP/IP Connection to Teleop Server in Quarc shared memory
//////////////////////////////////////////////////////////////////////////

/* Stream data transmission sizes (in number of doubles) -> to change when data
dimension changes */
#define STREAM_SIZE_RCV 4 // Size of data for control (from Simulink)
#define STREAM_SIZE_SND 12 // Size of data for feedback (to Simulink)
// Data message storage variables
t_double msg_RCV[STREAM_SIZE_RCV] = {
	0, 0, 0, // dirDes - desired direction
	0 }; // spdDes - desired speed
t_double msg_SND[STREAM_SIZE_SND] = {
	0, 0, 0, // posCur - current position
	0, 0, 0, // F - force from sensor
	0, 0, 0, // T - torque from sensor
	0, 0, 0 }; // posKinect - selected wrist position tracked by Kinect

// Number of units per send/receive call - is this even used?!
#define NUM_UNIT_RCV 1
#define NUM_UNIT_SND 1

// Communication channel buffer size (in bytes)
#define SIZE_BUFFER 1460

// Communication FIFO queue size (in bytes)
// Used when multithreaded (blocking IO)
#define SIZE_FIFO 1000

// Error message buffer size (in bytes)
#define SIZE_ERROR_MSG 255

// Sample Step (in seconds)
#define SIZE_STEP 0.001

// Define and initalize global variables
static int stop = 0;

// Custom handler for the ctrl-c exit command
static void control_c_handler(int signum)
{
	stop = 1;
} // End control_c_handler()

// Time out threshold (in number of sample step)
#define THRESHOLD_TIMEOUTCOUNT (10/SIZE_STEP)


// Thread variables
///////////////////
HANDLE hRCV = NULL;
HANDLE hSND = NULL;
HANDLE hProc = NULL;

bool bThreadRCV = true;
bool bThreadSND = true;

// Variables for the stream client setup
t_pstream stream_RCV; // Data channel from Simulink
t_pstream stream_SND; // Data channel to Simulink
t_pstream_options opt_stream_RCV; // Server to robot channel options
t_pstream_options opt_stream_SND; // Server to controller channel options

/* Send/receive attributes for the communication channels. See
"quanser_thread.h" for details. Note: All channels share the same
attributes. */
qthread_attr_t RCV_thread_attributes;
qthread_attr_t SND_thread_attributes;


// Prep work for TCP/IP Connection to Teleop Server in WinSock
//////////////////////////////////////////////////////////////
unsigned short TeleOpPortRCV = 18000;
unsigned short TeleOpPortSND = 18001;
char* sIPAddrLocal = "142.244.62.8"; // IP Address of local machine (client)
char* sIPAddrDest = "142.244.62.60"; /* IP Address of CEWin machine:
	142.244.62.59 for Robot 1, 142.244.62.60 for Robot 0 */


// Miscellaneous synchronization diagnosis variables
////////////////////////////////////////////////////
// For counters
double PCFreq = 0.0;
_int64 CounterStart = 0;


// Communication data sharing variable types
////////////////////////////////////////////
// Robot kinematics parameters
const int NbActuatedAxes = 3;
const double initPos[3] = { 100.0000, -524.2110, -32.5507 };

struct SendData // Data to send from Simulink PC to robot control PC
{
	double dirDes[NbActuatedAxes];
	double spdDes;
};

struct RecvData // Data received from robot control PC by Simulink PC
{
	double posCur[NbActuatedAxes];
	double F[NbActuatedAxes];
	double T[NbActuatedAxes];
};


// Initialize data variables
////////////////////////////
SendData SNDData; // Receive from Simulink, will be sent (confusing I know!)
RecvData RCVData; // Sent from UDP, will be received (confusing I know!)
RecvData RCVBuff;


// Initialize UDP socket variables
//////////////////////////////////
// Local host to CEWIN socket (received from Simulink)
UDP* p_udpRCV = new UDP(TeleOpPortRCV, sIPAddrDest, sIPAddrLocal);
// CEWin to local host socket (will be sent to Simulink)
UDP* p_udpSND = new UDP(TeleOpPortSND, sIPAddrDest, sIPAddrLocal);

int dataLoggingIndexRCV = 0;
int dataLoggingIndexSND = 0;
int iRecvBlock = 0; // Blocking counter UDP recv from CE

// Shared memeory address (defined in Simulink QUARC stream blocks)
const char uri_RCV[] = "shmem://UDPmem_recv:2";
const char uri_SND[] = "shmem://UDPmem_send:1";

// Local information for error messages
const char* localerr = NULL;

// Storage for error message
char err_message[SIZE_ERROR_MSG];

// Data structure for setting up the sleep timer
t_timeout sleeptime; // Data structure for setting up sleep timer

// Local timeout counter
int nCountTimeOutRCV = 0;
int nCountTimeOutSND = 0;

// Motion tracker camera UDP socket variables
/////////////////////////////////////////////
// Data transmission class object
data_transmission transmission;
// IP addresses and ports
char* ip_local_scp = "142.244.62.8";
short port_local_ss = 12001;
char* ip_remote_scp = "142.244.62.89";
short port_remote_ss = 12000;



////////////////////////////////////////////////////////////////////////////////
//
// Function forward declarations
//
////////////////////////////////////////////////////////////////////////////////

// Diagnosis counter functions
void StartCounter();
double GetCounter();

// UDP send and receive threads
DWORD ThreadRCV(LPVOID param);
DWORD ThreadSND(LPVOID param);



////////////////////////////////////////////////////////////////////////////////
//
// Main function
//
////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	bool Set;
	hProc = GetCurrentProcess();
	Set = SetPriorityClass(hProc, REALTIME_PRIORITY_CLASS);
	if (!Set)
	{
		cout << "Thread priority setting failed for entire process: " <<
			GetLastError() << endl;
	}


	// Set sleep timer to sampling period
	timeout_get_timeout(&sleeptime, SIZE_STEP);

	// Registering the Ctrl-C handler
	signal(SIGINT, control_c_handler);


	// Quanser specific data stream setup
	/////////////////////////////////////
	// Initialize the RCV/SND attributes to default values
	qthread_attr_init(&RCV_thread_attributes);
	qthread_attr_init(&SND_thread_attributes);

	// RCV/SND stream attributes
	opt_stream_RCV.receive_thread_attributes	= &RCV_thread_attributes;
	opt_stream_RCV.send_thread_attributes		= &SND_thread_attributes;
	opt_stream_SND.receive_thread_attributes	= &RCV_thread_attributes;
	opt_stream_SND.send_thread_attributes		= &SND_thread_attributes;

	// Number of units (double) to be sent/received per send/receive call
	opt_stream_RCV.num_receive_units	= STREAM_SIZE_RCV;
	opt_stream_RCV.num_send_units		= STREAM_SIZE_SND;
	opt_stream_SND.num_receive_units	= STREAM_SIZE_RCV;
	opt_stream_SND.num_send_units		= STREAM_SIZE_SND;

	// Size of each unit (double) to be sent/received per channel
	opt_stream_RCV.receive_unit_size	= sizeof(double);
	opt_stream_RCV.send_unit_size		= sizeof(double);
	opt_stream_SND.receive_unit_size	= sizeof(double);
	opt_stream_SND.send_unit_size		= sizeof(double);

	// Size of channel buffer (in bytes)
	opt_stream_RCV.receive_buffer_size	= SIZE_BUFFER;
	opt_stream_RCV.send_buffer_size		= SIZE_BUFFER;
	opt_stream_SND.receive_buffer_size	= SIZE_BUFFER;
	opt_stream_SND.send_buffer_size		= SIZE_BUFFER;

	// Size of FIFO queue (used when multithreaded)
	opt_stream_RCV.receive_fifo_size	= SIZE_FIFO;
	opt_stream_RCV.send_fifo_size		= SIZE_FIFO;
	opt_stream_SND.receive_fifo_size	= SIZE_FIFO;
	opt_stream_SND.send_fifo_size		= SIZE_FIFO;

	// Setting the persistent stream flags
	opt_stream_RCV.flags = 0;
	opt_stream_SND.flags = 0;

	opt_stream_RCV.flags	|= PSTREAM_FLAG_MULTITHREADED
							//|   PSTREAM_FLAG_SWAP_BYTES
							| PSTREAM_FLAG_MINIMIZE_LATENCY
							| PSTREAM_FLAG_SEND_MOST_RECENT
							| PSTREAM_FLAG_RECEIVE_MOST_RECENT;
	opt_stream_SND.flags	|= PSTREAM_FLAG_MULTITHREADED
							//|   PSTREAM_FLAG_SWAP_BYTES
							| PSTREAM_FLAG_MINIMIZE_LATENCY
							| PSTREAM_FLAG_SEND_MOST_RECENT
							| PSTREAM_FLAG_RECEIVE_MOST_RECENT;

	// Setting the size of the options instance
	opt_stream_RCV.size = sizeof(opt_stream_RCV);
	opt_stream_SND.size = sizeof(opt_stream_SND);


	// Making connection to WinCE
	/////////////////////////////
	cout << "Initializing UDP ports for CE connection..." << endl;

	if (p_udpRCV->Initialize() == -1)
	{
		cout << "Could not create and bind RCV socket!" << endl;
		delete p_udpRCV;
		delete p_udpSND;
		return -2;
	}
	cout << "RCV socket connected to teleop port." << endl;
	if (p_udpSND->Initialize() == -1)
	{
		cout << "Could not create and bind SND socket!" << endl;
		delete p_udpRCV;
		delete p_udpSND;
		return -2;
	}
	cout << "SND socket connected to teleop port." << endl;
	

	// Initialize motion tracker socket
	///////////////////////////////////
	int error = transmission.init_transmission(ip_local_scp, port_local_ss);
	if (error)
	{
		cout << "Kinect socket init failed with error: " << error << "!" << 
			endl;
	}
	else
	{
		cout << "Kinect socket initialized on " << ip_local_scp << ":" <<
			port_local_ss << "." << endl;


		// Making connection to HD2 driver model
		////////////////////////////////////////
		t_error	result_stream_RCV;
		t_error result_stream_SND;

		result_stream_RCV = pstream_connect(uri_RCV, &opt_stream_RCV, &stream_RCV);
		result_stream_SND = pstream_connect(uri_SND, &opt_stream_SND, &stream_SND);

		bool Set1, Set2;

		// Checking the connection status
		if ((result_stream_RCV == 0) && (result_stream_SND == 0))
		{
			// Connected successfully
			/////////////////////////
			cout << "Successfully Connected to Simulink servers." << endl;
			// StartCounter();

			HANDLE hRCV = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadRCV,
				NULL, 0, NULL);
			Set1 = SetThreadPriority(hRCV, THREAD_PRIORITY_TIME_CRITICAL);
			if (!Set1)
			{
				cout << "Thread priority setting failed for ThreadRCV: " <<
					GetLastError() << endl;
			}

			HANDLE hSND = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadSND,
				NULL, 0, NULL);
			Set2 = SetThreadPriority(hSND, THREAD_PRIORITY_TIME_CRITICAL);
			if (!Set2)
			{
				cout << "Thread priority setting failed for ThreadSND: " <<
					GetLastError() << endl;
			}


			// Run threads until Ctrl-C or other exit flag triggered
			////////////////////////////////////////////////////////
			while (!stop)
			{
				continue;
			}


			// Clean up threads
			///////////////////
			bThreadRCV = false;
			bThreadSND = false;
			CloseHandle(hRCV);
			CloseHandle(hSND);
			cout << "Control and feedback threads closed." << endl;


			// Clean up server streaming channels
			/////////////////////////////////////
			pstream_close(stream_RCV);
			pstream_close(stream_SND);
			cout << "Connection to Simulink servers closed." << endl;
			cout << "Connection to Simulink control server time out for a total "
				"number of " << nCountTimeOutRCV << " times." << endl;
			cout << "Connection to Simulink feedback server time out for a total "
				"number of " << nCountTimeOutSND << " times." << endl;
			cout << "UDP recv blocking loops: " << iRecvBlock << endl;
		}
		else
		{
			// Shared memory connection error
			/////////////////////////////////
			if (result_stream_RCV < 0)
			{
				msg_get_error_messageA(localerr, result_stream_RCV, err_message,
					sizeof(err_message));
				cout << "Error communication on URI " << uri_RCV << ". " <<
					err_message << endl;
			}

			if (result_stream_SND < 0)
			{
				msg_get_error_messageA(localerr, result_stream_SND, err_message,
					sizeof(err_message));
				cout << "Error communication on URI " << uri_SND << ". " <<
					err_message << endl;
			}
		}
	}


	// Cleanup UDP objects
	//////////////////////
	delete p_udpRCV;
	cout << "p_udpRCV deleted." << endl;
	delete p_udpSND;
	cout << "p_udpSND deleted" << endl;

	cout << "Press enter to exit." << endl;
	getchar();

	return 0;
}



////////////////////////////////////////////////////////////////////////////////
//
// Feedback communication thread: Read robot feedback data from robot control PC
//	through UDP and send to Simulink server.
//
////////////////////////////////////////////////////////////////////////////////

DWORD ThreadRCV(LPVOID param)
{
	Sleep(100);

	t_error result_stream_RCV; /* Return status for Quanser persistent stream
							   communication */
	int written; // Return status for UDP communication
	// Initialize the RCVBuff struct
	for (int i = 0; i < NbActuatedAxes; i++)
	{
		RCVBuff.posCur[i] = initPos[i];
		RCVBuff.F[i] = 0.0;
		RCVBuff.T[i] = 0.0;
	}
	// Initialize Kinect data buffers
	char KinectData[1024];
	double KinectBuff[3] = { 0.0, 0.0, 0.0 };
	// Kinect transmission error flag
	int comm_error = 0;

	p_udpRCV->InitializeAsyncEventAndSelect();
	transmission.init_async_eventandselect();

	while (bThreadRCV)
	{
		// Wait for qtimer_event notification
		// if ((result_stream_RCV = qtimer_event_wait(qTimerEventRecv)) < 0)
		// {
		// 	msg_get_error_messageA(localerr, result_stream_RCV, err_message,
		// 		sizeof(err_message));
		// 	cout << "Error qtimer_event_wait in ThreadRCV. Error: "
		// 		<< err_message << endl;
		// 	break;
		// }


		// Attempt to receive feedback data from robot control PC
		/////////////////////////////////////////////////////////
		written = p_udpRCV->RecvNonblocking((char*)&RCVData, sizeof(RCVData), 0);

		if (written == 0) // If would block, send most recent data in buffer
		{
			iRecvBlock++;
			//// For debugging
			//cout << "UDP (from Motoman) blocked!" << endl;
		}
		else if (written == -1)
		{
			cout << "Error in receiving Motoman data through UDP. written = " <<
				written << endl;
			break;
		}
		else
		{
			// UDP RCV receive request ok -> Update buffer
			RCVBuff = RCVData;
		}


		// Attempt to read MTC data // NEED TO MAKE THIS WORK WITH MULTIPLE MARKERS
		///////////////////////////
		comm_error = transmission.listen(KinectData, sizeof(KinectData), 0);
		if (comm_error)
		{
			cout << "Listening failed with error: " << comm_error << endl;
		}
		else
		{
			double* tmp = (double*)KinectData;
			for (int i = 0; i < 3; i++)
			{
				KinectBuff[i] = tmp[i];
			}
			//// For debugging
			//cout << '\r' << "Kinect data: x = " <<
			//	KinectBuff[0] << ", y = " <<
			//	KinectBuff[1] << ", z = " <<
			//	KinectBuff[2];
		}


		// Update message to send to Simulink
		/////////////////////////////////////
		for (int i = 0; i < NbActuatedAxes; i++)
		{
			msg_SND[i] = RCVBuff.posCur[i];
			msg_SND[i + NbActuatedAxes] = RCVBuff.F[i];
			msg_SND[i + 2 * NbActuatedAxes] = RCVBuff.T[i];
			msg_SND[i + 3 * NbActuatedAxes] = KinectBuff[i];
		}


		// Attempt to send feedback data to Simulink
		////////////////////////////////////////////
		result_stream_RCV = pstream_send(stream_RCV, &msg_SND);

		if (result_stream_RCV < 0)
		{
			nCountTimeOutRCV++;
			if (nCountTimeOutRCV > THRESHOLD_TIMEOUTCOUNT)
			{
				stop = 1;
				cout << "Connection to Simulink Quarc recv server lost for "
					"over " << THRESHOLD_TIMEOUTCOUNT * SIZE_STEP <<
					" seconds!" << endl;
			}
		}
		else
		{
			// Reset timeout counter
			nCountTimeOutRCV = 0;
		}

		qtimer_sleep(&sleeptime);
	}
	
	return 0;
}



////////////////////////////////////////////////////////////////////////////////
//
// Control communication thread: Read desired control data from Simulink server
//	and send to robot control PC through UDP.
//
////////////////////////////////////////////////////////////////////////////////

DWORD ThreadSND(LPVOID param)
{
	Sleep(200);

	t_error result_stream_SND; /* Return status for Quanser persistent stream
							   communication */
	int written; // Return status for UDP communication
	// Initialize the SNDData struct
	for (int i = 0; i < NbActuatedAxes; i++)
	{
		SNDData.dirDes[i] = 0.0;
	}
	SNDData.spdDes = 0.0;

	while (bThreadSND)
	{
		//// Wait for qtimer_event notification
		// if ((result_stream_SND = qtimer_event_wait(qTimerEventSend)) < 0)
		// {
		// 	msg_get_error_messageA(localerr, result_stream_SND, err_message,
		// 		sizeof(err_message));
		// 	cout << "Error qtimer_event_wait in ThreadSND. Error: "
		// 		<< err_message << endl;
		// 	break;
		// }


		// Request control data from Simulink server
		////////////////////////////////////////////
		result_stream_SND = pstream_receive(stream_SND, &msg_RCV);

		if (result_stream_SND > 0)
		{
			// Reset timeout counter
			nCountTimeOutSND = 0;
			// Update SNDData
			for (int i = 0; i < NbActuatedAxes; i++)
			{
				SNDData.dirDes[i] = msg_RCV[i];
			}
			SNDData.spdDes = msg_RCV[NbActuatedAxes];
			//// For debugging (output data from Simulink)
			//cout << '\r' << "Received from Simulink: " <<
			//	SNDData.dirDes[0] << '\t' <<
			//	SNDData.dirDes[1] << '\t' <<
			//	SNDData.dirDes[2] << '\t' <<
			//	SNDData.spdDes;


			// Attempt to send control data to robot control PC
			///////////////////////////////////////////////////
			// Blocking -> Might pose problem
			written = p_udpSND->Send((char*)&SNDData, sizeof(SNDData));

			if (written == -1)
			{
				cout << "Error in sending data through UDP. Written = "	<<
					written << endl;
				break;
			}
		}
		else if (result_stream_SND < 0)
		{
			// If pstream_receive fails, send the last SNDData in memory
			nCountTimeOutSND++;

			//// For debugging
			//cout << "QUARC stream (from Simulink) failed!" << endl;

			// Blocking -> Might pose problem
			written = p_udpSND->Send((char*)&SNDData, sizeof(SNDData));

			if (written == -1)
			{
				cout << "Error in sending data through UDP. Written = " <<
					written << endl;
				break;
			}

			// Check for timeout
			if (nCountTimeOutSND > THRESHOLD_TIMEOUTCOUNT)
			{
				stop = 1;
				cout << "Connection to Simulink Quarc send server lost for "
					"over " << THRESHOLD_TIMEOUTCOUNT * SIZE_STEP <<
					" seconds!" << endl;
			}
		}

		qtimer_sleep(&sleeptime); /* Timer mechanism not the best yet, can try
								  to use the event mechanism */
	}

	return 0;
}