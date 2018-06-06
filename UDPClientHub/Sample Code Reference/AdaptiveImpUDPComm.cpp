////////////////////////////////////////////////////////////////////////////////
//
// AdaptiveImpUDPComm.cpp
//
////////////////////////////////////////////////////////////////////////////////
//
// Author: Jason Fong (TBS, University of Alberta)
//	Email: jmfong@ualberta.ca
//
// Description: This code provides UDP communication services between the PC
//	controlling the robot (Yaskawa Motoman SIA5F) and any other client PC.
//
// Acknowledgements: Thanks to Ran Tao (formerly TBS, University of Alberta)
//	for creating the original code upon which this is based. Credit to Elena
//	Gribovskaya (LASA, EPFL) for the work "Motion Learning and Adaptive
//	Impedance for Robot Control during Physical Interaction with Humans" (ICRA,
//	2011) upon which the overarching work this code is a part of is based.
//
// Note: Threads are referred to as control (CON) and feedback (FB). Control
//	refers to control signals passed from a Simulink controller to the PC
//	controlling the Yaskawa Motoman SIA5F, while feedback refers to feedback
//	data passed from the robot-side PC back to the Simulink controller. This
//	code is run on the same PC as the Simulink controller, so the connection
//	is local. The controller could also be a robot though, which may cause
//	confusion. Quanser UDP communication defines sending and receiving relative
//	to the streaming server. The server is run through the Simulink code, so in
//	effect "sending" (e.g. pstream_send) from this script to the server = FB,
//	and "receiving" (e.g. pstream_receive) from the server to the script = CON.
//
// Date of Last Revision: 2017-07-20
//
////////////////////////////////////////////////////////////////////////////////
//
// Table of contents
//	1. Libraries, namespaces, and typedefs
//	2. Initialization
//	3. Function forward declarations
//	4. 
//
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//
// Libraries, namespaces, and typedefs
//
////////////////////////////////////////////////////////////////////////////////

#ifndef UNICODE
#define UNICODE
#endif
#pragma comment (lib, "winmm.lib")

// #define WIN32_LEAN_AND_MEAN /* windows.h includes winsock.h if you don't
// define that symbol; this creates havoc if you want to include winsock2.h, as
// everything gets redefined. */
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
#define STREAM_SIZE_CON 17 // Size of data for control
#define STREAM_SIZE_FB 18 // Size of data for feedback
// Data message storage variables
t_double msg_CON[STREAM_SIZE_CON] = {
	0, 0, 0, // posDes - desired position
	0, 0, 0, // velDes - desired velocitie
	0, 0, 0, // accDes - desired acceleration
	0, 0, 0, 0, // Mdel - desired change in inertia matrix
	0, 0, 0, 0}; // Sdel - desired change in spring matrix
t_double msg_FB[STREAM_SIZE_FB] = {
	0, 0, // posJoint
	0, 0, // velJoint
	0, 0, // accJoint
	0, 0, // T
	0, 0, // F
	0, 0, 0, 0, 0, 0, 0, 0}; // ControllerBreakdown

// Number of units per send/receive call - is this even used?!
#define NUM_UNIT_CON 1
#define NUM_UNIT_FB 1

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
#define THRESHOLD_TIMEOUTCOUNT (20/SIZE_STEP)


// Thread variables
///////////////////
HANDLE hCON = NULL;
HANDLE hFB = NULL;
HANDLE hProc = NULL;

bool bThreadCON = true;
bool bThreadFB = true;

// Variables for the stream client setup
t_pstream stream_CON; // Server to robot channel
t_pstream stream_FB; // Server to controller channel
t_pstream_options opt_stream_CON; // Server to robot channel options
t_pstream_options opt_stream_FB; // Server to controller channel options

/* Send/receive attributes for the communication channels. See
	"quanser_thread.h" for details. Note: All channels share the same
	attributes. */
qthread_attr_t CON_thread_attributes;
qthread_attr_t FB_thread_attributes;


// Prep work for TCP/IP Connection to Teleop Server in WinSock
//////////////////////////////////////////////////////////////
unsigned short TeleOpPortCON = 18000;
unsigned short TeleOpPortFB = 18001;
char* sIPAddrDest = "129.128.209.93"; /* IP Address of CEWin machine:
	142.244.62.59 for Robot 1, 142.244.62.60 for Robot 0 */
char* sIPAddrLocal = "129.128.209.241"; // IP Address of local machine (client)


// Miscellaneous synchronization diagnosis variables
////////////////////////////////////////////////////
// For counters
double PCFreq = 0.0;
_int64 CounterStart = 0;


// Communication data sharing variable types
////////////////////////////////////////////
// Impedance control breakdown and robot kinematics parameters
const int ControllerBreakdownNb = 4;
const int NbActuatedAxis = 2;
const int NbControlDOF = 3;
const double L1effec = 0.254;
const double initPos[3] = {L1effec, 0.0, 0.0};

struct SendData // Data to send from client PC to robot control PC
{
	double posDes[3];
	double velDes[3];
	double accDes[3];
	double Mdel[NbActuatedAxis][NbActuatedAxis];
	double Sdel[NbActuatedAxis][NbActuatedAxis];
};

struct RecvData // Data to be received from robot control PC by client PC
{
	double posJoint[NbActuatedAxis];
	double velJoint[NbActuatedAxis];
	double accJoint[NbActuatedAxis];
	double F[NbActuatedAxis];
	double T[NbActuatedAxis];
	double ControllerBreakdown[NbActuatedAxis][ControllerBreakdownNb];
};


// Initialize data variables
////////////////////////////
SendData CONData;
RecvData FBData;
RecvData FBBuff;


// Initialize UDP socket variables
//////////////////////////////////
// Controller to server socket
UDP* p_udpCON = new UDP(TeleOpPortCON, sIPAddrDest, sIPAddrLocal);
// Robot to server socket
UDP* p_udpFB = new UDP(TeleOpPortFB, sIPAddrDest, sIPAddrLocal);

int dataLoggingIndexCON = 0;
int dataLoggingIndexFB = 0;
int iFBBlock = 0; // Blocking counter UDP recv from CE

//Shared memeory address
const char uri_CON[] = "shmem://UDPmem_CON:2";
const char uri_FB[] = "shmem://UDPmem_FB:1";
	
// Local information for error messages
const char* localerr = NULL;

// Storage for error message
char err_message[SIZE_ERROR_MSG];

// Data structure for setting up the sleep timer
t_timeout sleeptime; // Data structure for setting up sleep timer

// Local timeout counter
int nCountTimeOutCON = 0;
int nCountTimeOutFB = 0;



////////////////////////////////////////////////////////////////////////////////
//
// Function forward declarations
//
////////////////////////////////////////////////////////////////////////////////

// Diagnosis counter functions
void StartCounter();
double GetCounter();

// UDP send and receive threads
DWORD ThreadCON(LPVOID param);
DWORD ThreadFB(LPVOID param);



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
		cout << "Thread priority setting failed for entire process: "
			<< GetLastError() << endl;
	}

	
	// Set sleep timer to sampling period
    timeout_get_timeout(&sleeptime, SIZE_STEP);

	// Registering the Ctrl-C handler
    signal(SIGINT, control_c_handler);
	

	// Quanser specific data stream setup
	/////////////////////////////////////
    // Initialize the CON/FB attributes to default values
    qthread_attr_init(&CON_thread_attributes);
    qthread_attr_init(&FB_thread_attributes);

	// CON/FB stream attributes
	opt_stream_CON.receive_thread_attributes = &CON_thread_attributes;
	opt_stream_FB.send_thread_attributes = &FB_thread_attributes;

	// Number of units (double) to be sent/received per send/receive call
    opt_stream_CON.num_receive_units = STREAM_SIZE_CON;
    opt_stream_FB.num_send_units = STREAM_SIZE_FB;

	// Size of each unit (double) to be sent/received per channel 
	opt_stream_CON.receive_unit_size = sizeof( double );
    opt_stream_FB.send_unit_size = sizeof( double );

	// Size of channel buffer (in bytes)
	opt_stream_CON.receive_buffer_size = SIZE_BUFFER;
    opt_stream_FB.send_buffer_size = SIZE_BUFFER;

	// Size of FIFO queue (used when multithreaded)
    opt_stream_CON.receive_fifo_size = SIZE_FIFO;
	opt_stream_FB.send_fifo_size = SIZE_FIFO;

	// Setting the persistent stream flags
	opt_stream_CON.flags = 0;
	opt_stream_FB.flags = 0;

	opt_stream_CON.flags	|=  PSTREAM_FLAG_MULTITHREADED
							// |   PSTREAM_FLAG_SWAP_BYTES
							|   PSTREAM_FLAG_MINIMIZE_LATENCY
							|   PSTREAM_FLAG_SEND_MOST_RECENT
							|   PSTREAM_FLAG_RECEIVE_MOST_RECENT;
	opt_stream_FB.flags		|=  PSTREAM_FLAG_MULTITHREADED
							// |   PSTREAM_FLAG_SWAP_BYTES
							|   PSTREAM_FLAG_MINIMIZE_LATENCY
							|   PSTREAM_FLAG_SEND_MOST_RECENT
							|   PSTREAM_FLAG_RECEIVE_MOST_RECENT;

	// Setting the size of the options instance
	opt_stream_CON.size = sizeof(opt_stream_CON);
	opt_stream_FB.size = sizeof(opt_stream_FB);


	// Making connection to WinCE
	/////////////////////////////
	cout << "Initializing UDP ports for CE connection..." << endl;
	
	if (p_udpCON->Initialize() == -1)
	{
		cout << "Could not create and bind CON socket!" << endl;
		delete p_udpCON;
		delete p_udpFB;
		return -2;
	}
	cout << "CON socket connected to teleop port." << endl;
	if (p_udpFB->Initialize() == -1)
	{
		cout << "Could not create and bind FB socket!" << endl;
		delete p_udpCON;
		delete p_udpFB;
		return -2;
	}
	cout << "FB socket connected to teleop port." << endl;

	t_error	result_stream_CON;
	t_error result_stream_FB;


	// Making connection to HD2 driver model
	////////////////////////////////////////
	result_stream_CON = pstream_connect(uri_CON, &opt_stream_CON, &stream_CON);
	result_stream_FB = pstream_connect(uri_FB, &opt_stream_FB, &stream_FB);
	
	bool Set1, Set2;
	
	// Checking the connection status
	if ((result_stream_CON == 0) && (result_stream_FB == 0))
    {
    	// Connected successfully
    	/////////////////////////
        cout << "Successfully Connected to Simulink servers." << endl;
		// StartCounter();
		
		HANDLE hCON = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadCON,
			NULL, 0, NULL);
		Set1 = SetThreadPriority(hCON, THREAD_PRIORITY_TIME_CRITICAL);
		if (!Set1)
		{
			cout << "Thread priority setting failed for ThreadCON: "
				<< GetLastError() << endl;
		}
		
		HANDLE hFB = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ThreadFB,
			NULL, 0, NULL);		
		Set2 = SetThreadPriority(hFB, THREAD_PRIORITY_TIME_CRITICAL);
		if (!Set2)
		{
			cout << "Thread priority setting failed for ThreadFB: "
				<< GetLastError() << endl;
		}
		

		// Run threads until Ctrl-C or other exit flag triggered
		////////////////////////////////////////////////////////
		while (!stop)
		{
			continue;
		}


		// Clean up threads
		///////////////////
		bThreadCON = false;
		bThreadFB = false;
	    CloseHandle(hCON);
		CloseHandle(hFB);
		cout << "Control and feedback threads closed." << endl;


		// Clean up server streaming channels
		/////////////////////////////////////
		pstream_close(stream_CON);
		pstream_close(stream_FB);
		cout << "Connection to Simulink servers closed." << endl;
		/*
		std::ofstream DataFile;
		// std::ofstream DataFileRecv;
		DataFile.open("Z:\\Data\\time trials HD2\\udpDataSend.txt");
		// DataFileRecv.open("C:\\Users\\user\\Desktop\\Communication\\udpDataRecv.txt");
		
		for (int s = 0; s < min(mlength, dataLoggingIndex); s++)
		{
			DataFile <<
				timeData[s] << '\t' <<
				pSend[s] << '\t' <<
				l1[s] << '\t' <<
				l2[s] << '\t' <<
				l3[s] << '\t' <<
				l4[s] << '\t' <<
				l5[s] << '\t' <<
				endl;
		}
		for (int j = 0; j < min(mlength, dataLoggingIndexRecv); j++)
		{
			DataFileRecv <<
				timeDataRecv[j] << '\t' <<
				pRecv[j] << '\t' <<
				endl;
		}
		DataFile.close();
		DataFileRecv.close();

		cout << "Data written to file." << endl;
		*/
		cout << "Connection to Simulink control server time out for a total "
			"number of " << nCountTimeOutCON << " times." << endl;
		cout << "Connection to Simulink feedback server time out for a total "
			"number of " << nCountTimeOutFB << " times." << endl;
		cout << "UDP recv blocking loops: " << iRecvBlock << endl;

	}
	else
	{
		// Shared memory connection error
		/////////////////////////////////
		if (result_stream_CON < 0)
		{
			msg_get_error_messageA(localerr, result_stream_CON, err_message, 
				sizeof(err_message));
			cout << "Error communication on URI " << uri_CON << ". "
				<< err_message << endl;
		}
		
		if (result_stream_FB < 0)
		{
			msg_get_error_messageA(localerr, result_stream_FB, err_message,
				sizeof(err_message));
			cout << "Error communication on URI " << uri_FB << ". "
				<< err_message << endl;
		}
	}


	// Cleanup timers and UDP objects
	/////////////////////////////////
	// qtimer_event_delete(qTimerEventSend);
	// qtimer_event_delete(qTimerEventRecv);
	// qtimer_end_resolution(qTimerSend);
	// qtimer_end_resolution(qTimerRecv);
	// qtimer_delete(qTimerSend);
	// qtimer_delete(qTimerRecv);
	
	delete p_udpCON;
	cout << "p_udpCON deleted." << endl;
	delete p_udpFB;
	cout << "p_udpFB deleted" << endl;
		
	cout << "Press enter to exit." << endl;
	getchar();

	return 0;
}



////////////////////////////////////////////////////////////////////////////////
//
// Control communication thread: Read desired control data from Simulink server
//	and send to robot control PC through UDP.
//
////////////////////////////////////////////////////////////////////////////////

DWORD ThreadCON(LPVOID param)
{
	t_error result_stream_CON; /* Return status for Quanser persistent stream
		communication */
	int written; // Return status for UDP communication
	// Initialize the CONData struct
	for (int i = 0; i < NbControlDOF; i++)
	{
		CONData.posDes[i] = initPos[i];
		CONData.velDes[i] = 0.0;
		CONData.accDes[i] = 0.0;	
	}
	for (int i = 0; i < NbActuatedAxis; i++)
	{
		for (int j = 0; j < NbActuatedAxis; j++)
		{
			CONData.Mdel[i][j] = 0.0;
			CONData.Sdel[i][j] = 0.0;
		}
	}
	
	while (bThreadCON)
	{
		// l1[dataLoggingIndex] = GetCounter();
		
		// Wait for qtimer_event notification
		// if ((result_stream_CON = qtimer_event_wait(qTimerEventSend)) < 0)
		// {
		// 	msg_get_error_messageA(localerr, result_stream_CON, err_message,
		// 		sizeof(err_message));
		// 	cout << "Error qtimer_event_wait in ThreadCON. Error: "
		// 		<< err_message << endl;
		// 	break;
		// }


		// Request control data from Simulink server
		////////////////////////////////////////////
		result_stream_CON = pstream_receive(stream_CON, &msg_CON);

		// l2[dataLoggingIndex] = GetCounter();

		if (result_stream_CON > 0)
		{
			// Update CONData
			CONData.posDes = {msg_CON[0], msg_CON[1], msg_CON[2]};
			CONData.velDes = {msg_CON[3], msg_CON[4], msg_CON[5]};
			CONData.accDes = {msg_CON[6], msg_CON[7], msg_CON[8]};
			CONData.Mdel = {{msg_CON[9], msg_CON[10]}, 
				{msg_CON[11], msg_CON[12]}};
			CONData.Sdel = {{msg_CON[13], msg_CON[14]}, 
				{msg_CON[15], msg_CON[16]}};


			// Attempt to send control data to robot control PC
			///////////////////////////////////////////////////
			// Blocking -> Might pose problem
			written = p_udpCON->Send((char*) &CONData, sizeof(CONData));

			// l3[dataLoggingIndex] = GetCounter();
			
			if (written == -1) 
			{
				cout << "Error in sending data through UDP. Written = "
					<< written << endl;
				break;
			} 
		}
		else if (result_stream_CON < 0)
		{
			// If pstream_receive fails, send the last CONData in memory
			nCountTimeOutCON++;
			// Blocking -> Might pose problem
			written = p_udpCON->Send((char*) &CONData, sizeof(CONData));

			// l3[dataLoggingIndex] = GetCounter();
			
			if (written == -1) 
			{
				cout << "Error in sending data through UDP. Written = "
					<< written << endl;
				break;
			} 

			// Check for timeout
			if (nCountTimeOutCON > THRESHOLD_TIMEOUTCOUNT)
			{
				stop = 1;
				cout << "Connection to Simulink Quarc send server lost!"
					<< endl;
				cout << "For Over "<< THRESHOLD_TIMEOUTCOUNT * SIZE_STEP
					<< " seconds." << endl;
			}
		}
		// if (dataLoggingIndex < mlength)
		// {
		// 	pSend[dataLoggingIndex] = commData.num;
		// 	timeData[dataLoggingIndex] = GetCounter();
		// }

		// l4 [dataLoggingIndex] = GetCounter();
		
		qtimer_sleep(&sleeptime); /* Timer mechanism not the best yet, can try
			to use the event mechanism */

		// l5 [dataLoggingIndex] = GetCounter();
		
		dataLoggingIndex++;
	}

	return 0;
}



////////////////////////////////////////////////////////////////////////////////
//
// Feedback communication thread: Read robot feedback data from robot control PC
//	through UDP and send to Simulink server.
//
////////////////////////////////////////////////////////////////////////////////

DWORD ThreadRecv(LPVOID param)
{
	t_error result_stream_FB; /* Return status for Quanser persistent stream
		communication */
	int written; // Return status for UDP communication
	// Initialize the FBBuff struct
	for (int i = 0; i < NbActuatedAxis; i++)
	{
		FBBuff.posJoint[i] = 0.0;
		FBBuff.velJoint[i] = 0.0;
		FBBuff.accJoint[i] = 0.0;
		FBBuff.T[i] = 0.0;
		FBBuff.F[i] = 0.0;
		for (int j = 0; j < ControllerBreakdownNb; j++)
		{
			FBBuff.ControllerBreakdown[i][j] = 0.0;
		}
	}
	
	p_udpFB->InitializeAsyncEventAndSelect();

	while (bThreadFB)
	{
		// Wait for qtimer_event notification
		// if ((result_stream_FB = qtimer_event_wait(qTimerEventRecv)) < 0)
		// {
		// 	msg_get_error_messageA(localerr, result_stream_FB, err_message,
		// 		sizeof(err_message));
		// 	cout << "Error qtimer_event_wait in ThreadFB. Error: "
		// 		<< err_message << endl;
		// 	break;
		// }
		
		// timeDataRecv[dataLoggingIndexRecv] = GetCounter();

		written = p_udpFB->RecvNonblocking((char*) &FBData, sizeof(FBData), 0);
		
		if (written == 0) // If would block, send most recent data in buffer
		{ 
			iRecvBlock++;
		}
		else if (written == -1) 
		{
			cout << "Error in receiving Motoman data through UDP. written = "
				<< written << endl;
			break;
		} 
		else
		{
			// UDP FB receive request ok -> Update buffer
			FBBuff = FBData;
		}

		msg_FB[0] = FBBuff.posJoint[0];
		msg_FB[1] = FBBuff.posJoint[1];
		msg_FB[2] = FBBuff.velJoint[0];
		msg_FB[3] = FBBuff.velJoint[1];
		msg_FB[4] = FBBuff.accJoint[0];
		msg_FB[5] = FBBuff.accJoint[1];
		msg_FB[6] = FBBuff.F[0];
		msg_FB[7] = FBBuff.F[1];
		msg_FB[8] = FBBuff.T[0];
		msg_FB[9] = FBBuff.T[1];
		msg_FB[10] = FBBuff.ControllerBreakdown[0][0];
		msg_FB[11] = FBBuff.ControllerBreakdown[0][1];
		msg_FB[12] = FBBuff.ControllerBreakdown[0][2];
		msg_FB[13] = FBBuff.ControllerBreakdown[0][3];
		msg_FB[14] = FBBuff.ControllerBreakdown[1][0];
		msg_FB[15] = FBBuff.ControllerBreakdown[1][1];
		msg_FB[16] = FBBuff.ControllerBreakdown[1][2];
		msg_FB[17] = FBBuff.ControllerBreakdown[1][3];
		
		result_stream_FB = pstream_send(stream_FB, &msg_FB);

		if (result_stream_FB < 0)
		{
			nCountTimeOutRecv++;
			if (nCountTimeOutRecv > THRESHOLD_TIMEOUTCOUNT)
			{
				stop = 1;
				cout << "Connection to Simulink Quarc recv server lost!"
					<< endl;
				cout << "For Over " << THRESHOLD_TIMEOUTCOUNT * SIZE_STEP
					<< " seconds." << endl;
			}
		}
		/* Record the joint position term sent to Simulink and compare with
			what Simulink receives */
		// pRecv[dataLoggingIndexRecv] = msg_SND[1];
		qtimer_sleep(&sleeptime);
		dataLoggingIndexRecv++;
	}

	return 0;
}



////////////////////////////////////////////////////////////////////////////////
//
// Start diagnostic counter
//
////////////////////////////////////////////////////////////////////////////////

void StartCounter()
{
	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed! \n";
	PCFreq = double(li.QuadPart) / 1000.0; /* Timer uses ms, if "1000.0"
		removed -> Uses s */
	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}



////////////////////////////////////////////////////////////////////////////////
//
// Get diagnostic counter current time
//
////////////////////////////////////////////////////////////////////////////////
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);

	return double(li.QuadPart - CounterStart) / PCFreq;
}