#include "data_transmission.h"

void data_transmission::InitWinsock()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("\nWSAStartup failed with error: %d\n", iResult);
	}
}

int data_transmission::init_transmission(
	char* ip_local_scp, short port_local_ss){

	int error = 0;
	InitWinsock();
	remotelen = sizeof(remote);
	local.sin_family = AF_INET;
	local.sin_port = htons(port_local_ss);
	local.sin_addr.s_addr = inet_addr(ip_local_scp);

	socketS = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketS == INVALID_SOCKET) {
		error = WSAGetLastError();
		WSACleanup();
	}
	else{
		bind(socketS, (sockaddr*)&local, sizeof(local));
	}

	return error;
}

int data_transmission::init_transmission(
	char* ip_local_scp, short port_local_ss,
	char* ip_remote_scp, short port_remote_ss){

	int error = 0;
	InitWinsock();
	remotelen = sizeof(remote);
	local.sin_family = AF_INET;
	local.sin_port = htons(port_local_ss);
	local.sin_addr.s_addr = inet_addr(ip_local_scp);
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port_remote_ss);
	remote.sin_addr.s_addr = inet_addr(ip_remote_scp);

	socketS = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketS == INVALID_SOCKET) {
		error = WSAGetLastError();
		WSACleanup();
	}

	return error;
}

int data_transmission::init_async_eventandselect(){
	int error = 0;
	hWSAEvent = WSACreateEvent();
	if (hWSAEvent == NULL){
		error = WSAGetLastError();
	}
	else{
		if (WSAEventSelect(socketS, hWSAEvent, FD_WRITE | FD_READ | FD_CLOSE)){
			error = WSAGetLastError();
			WSACloseEvent(hWSAEvent);
			hWSAEvent = NULL;
		}
		else{
			printf("MTC: Async socket ops ready.\n");
		}
	}
	return error;
}

int data_transmission::do_send(char* buffer_scp, int len){
	int error = 0;
	if (sendto(
		socketS, buffer_scp, len, 0, (sockaddr*)&remote, remotelen)
		== SOCKET_ERROR){
		error = WSAGetLastError();
	}
	return error;
}

int data_transmission::send(char* array_scp, int len){
	return do_send((char*)array_scp, len);
}

int data_transmission::send(double* array_dbp, int len){
	return do_send((char*)array_dbp, len * 8);
}

int data_transmission::send(float* array_flp, int len){
	return do_send((char*)array_flp, len * 4);
}

int data_transmission::send(int* array_sip, int len){
	return do_send((char*)array_sip, len * 4);
}

int data_transmission::send(short* array_ssp, int len){
	return do_send((char*)array_ssp, len * 2);
}

int data_transmission::listen(char* buffer_scp, int len){
	int error = 0;
	if (recvfrom(
		socketS, buffer_scp, len, 0, (sockaddr*)&remote, &remotelen)
		== SOCKET_ERROR){
		error = WSAGetLastError();
	}
	return error;
}

int data_transmission::listen(char* buffer_scp, int len, int timeout){
	int nrec = 0;
	int error = 0;
	DWORD waitRes = 0;
	if (hWSAEvent != NULL){
		waitRes = WaitForSingleObject(hWSAEvent, timeout);
		if (waitRes == WAIT_TIMEOUT){
			printf("\nWait for socket read timed out!\n");
			nrec = SOCKET_ERROR;
			return nrec;
		}
		else if (waitRes == WAIT_FAILED){
			printf("\nWait for socket read event failed! Err = %d\n", 
				GetLastError());
			nrec = SOCKET_ERROR;
			return nrec;
		}
		else{
			// Fall through to regular recv in case this version used
		}
	}
	nrec = recvfrom(socketS, buffer_scp, len, 0, (sockaddr*)&remote, &remotelen);
	error = WSAGetLastError();
	if (nrec == SOCKET_ERROR){
		if (error == WSAEWOULDBLOCK){
			return 0; // Don't want to stop thread on blocking calls
		}
		else{
			return nrec;
		}
	}
	else if (nrec == 0){
		return -1;
	}
	return 0;
}

void data_transmission::num2charray(short in_ss, char* out_scp){
	out_scp = (char*)&in_ss;
}

void data_transmission::num2charray(
	short in_ss, char* out_scp, int* len_out_sip){
	out_scp = (char*)&in_ss;
	*len_out_sip = 2;
}

void data_transmission::num2charray(unsigned short in_us, char* out_scp){
	out_scp = (char*)&in_us;
}

void data_transmission::num2charray(
	unsigned short in_us, char* out_scp, int* len_out_sip){
	out_scp = (char*)&in_us;
	*len_out_sip = 2;
}

void data_transmission::charray2num(char* in_scp, short* out_ssp){
	out_ssp = (short*)in_scp;
}

short data_transmission::chararray2short(unsigned char *in_ch){
	short temp = in_ch[0];
	temp = temp << 8;
	return temp | in_ch[1];
}

unsigned char* data_transmission::short2chararray(short in_sh){
	unsigned char temp[2];
	temp[0] = (in_sh >> 8) & 0xFF;
	temp[1] = in_sh & 0x00FF;
	return temp;
}

unsigned char* data_transmission::short2chararray(unsigned short in_sh){
	unsigned char temp[2];
	temp[0] = (in_sh >> 8) & 0xFF;
	temp[1] = in_sh & 0x00FF;
	return temp;
}