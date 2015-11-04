#include "../lib/chronos.h"	// Gerado pelo RPCgen
#include <stdio.h>			// IO
#include <string.h>			// Manipular string
#include <stdlib.h> 		// Alocação dinâmica
#include <time.h> 			// Obter timestamp
#include <unistd.h> 		// Executar comandos no sistema


// Functions Headers
struct tm * getTimeFromServer ();
void setTimeOnMachine (struct tm * tm);
int setTimeBasicMode ();
int setTimeChristianMode ();
int setTimeNTPMode ();



// Global variables
char SERVER_IP[100];
int  MODE;
struct tm * TIME_FROM_SERVER;



int main(int argc, char **argv) {
	// Verifica se o usuário envio todos os argumentos
	// caso falte algum, finaliza o cliente
	// 		argv[1] : IP do servidor
	// ===========================================
	if (argc != 3) {
		printf("How to use: ./client <server_address> <method[0-2]>\n");
		printf("   @method: [0-Basic] [1-Christian] [2-NTP]\n\n");
		printf("   Example: ./client 192.168.1.100 1\n");
		return 1;
	}


	// Getting Server IP and set to Global Var
	// ===========================================
	strcpy (SERVER_IP, argv[1]);


	// Getting Algorithm Mode and set to Global Var
	// ===========================================
	MODE = ((int) argv[2][0]) - 48;



	// Selecting Algorithm Mode
	// ===========================================
	if(MODE != 0 && MODE != 1 && MODE != 2) {
		printf("ERROR! Invalid mode!\n");
		printf("How to use: ./client <server_address> <method[0-2]>\n");
		printf("   @method: [0-Basic] [1-Christian] [2-NTP]\n\n");
		printf("   Example: ./client 192.168.1.100 1\n");
		exit(1);
	}
	
	// Basic Mode
	if(MODE == 0)
		setTimeBasicMode();

	// Christian Mode
	if(MODE == 1)
		setTimeChristianMode();

	// NTP Mode
	if(MODE == 2)
		setTimeNTPMode();


	return 0;
}



int setTimeBasicMode () {
	// Getting time from server
	// ===========================================
	struct tm * time_server;
	time_server = (struct tm *) malloc(sizeof(struct tm));
	time_server = getTimeFromServer();


	// Set time on local machine
	// ===========================================
	setTimeOnMachine(time_server);


	// Console output:
	// 		Server Timestamp
	// ===========================================
	printf("%s", asctime( time_server ) );
}



int setTimeChristianMode () {
	time_t rawtime;

	// Getting local time before request
	// ===========================================
	struct tm * time_before_request;
	time ( &rawtime );
	time_before_request = localtime ( &rawtime );


	// Getting time from server
	// ===========================================
	struct tm * time_server;
	time_server = (struct tm *) malloc(sizeof(struct tm));
	time_server = getTimeFromServer();


	// Getting local time after request
	// ===========================================
	struct tm * time_after_request;
	time ( &rawtime );
	time_after_request = localtime ( &rawtime );
}



int setTimeNTPMode () {
	printf("ERROR! NTP Mode not implemented!\n");
	exit(1);
}




/*
	setTimeOnMachine (struct tm)
	@description:	Recebe uma struct de data e hora,
					usada para definir data e hora atual
					no máquina.
	===========================================
*/
void setTimeOnMachine (struct tm * tm) {
	// Formate datetime input
	int LEN=30;
	char dt_input[LEN];

	strcpy(dt_input,"sudo date -s '");							// Limpando
	sprintf(dt_input, "%s%d-", dt_input, (tm->tm_year+1900));	// Ano
	sprintf(dt_input, "%s%d-", dt_input, (tm->tm_mon+1));		// Mês
	sprintf(dt_input, "%s%d ", dt_input, (tm->tm_mday));		// Dia

	sprintf(dt_input, "%s%d:", dt_input, (tm->tm_hour));		// Hora
	sprintf(dt_input, "%s%d:", dt_input, (tm->tm_min));			// Minuto
	sprintf(dt_input, "%s%d'", dt_input, (tm->tm_sec));			// Segundo

	//printf("%s\n", dt_input); // DEBUG


	// Executa comando externo e define data e hora usando "date"
	printf("| Date/time setted to: \n");
    char *name[] = {
        "/bin/bash",
        "-c",
        dt_input, // 2004-02-29  16:21:42
        NULL
    };
    execvp(name[0], name);
}



/*
	getTimeFromServer ()
	@description:	Redireciona o processamento para o servidor.
					O servidor retorna uma struct com todas as
					informações de data e hora.

	@return (struct tm*) struct tm obtida da "time.h"
	===========================================
*/
struct tm * getTimeFromServer () {
	// Criando conexão TCP com o servidor
	// ===========================================
	CLIENT *cl; /* definido em rpc.h */
	chronos_in in;
	chronos_out *outp;
	cl = clnt_create(SERVER_IP, chronos_PROG, chronos_VERS, "udp");


	// Get current timestamp
	// ===========================================
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	//printf ( "Current local time and date: %s\n", asctime (timeinfo) );


	// Gerando struct de envio
	// ===========================================
	in.tm_sec	= timeinfo->tm_sec;
	in.tm_min	= timeinfo->tm_min;
	in.tm_hour	= timeinfo->tm_hour;
	in.tm_mday	= timeinfo->tm_mday;
	in.tm_mon	= timeinfo->tm_mon;
	in.tm_year	= timeinfo->tm_year;
	in.tm_wday	= timeinfo->tm_wday;
	in.tm_yday	= timeinfo->tm_yday;
	in.tm_isdst	= timeinfo->tm_isdst;
	

	// Chamada do RPC a partir do seu identificador
	// ===========================================
	if ((outp = chronosproc_1(&in, cl)) == NULL) {
		perror(clnt_sperror(cl, SERVER_IP)); exit(1); 
	}




	// Gerando struct tm a ser retornada
	// ===========================================
	struct tm * server_time;
	server_time = (struct tm*) malloc(sizeof(struct tm));

	server_time->tm_sec		= outp->tm_sec;
	server_time->tm_min		= outp->tm_min;
	server_time->tm_hour	= outp->tm_hour;
	server_time->tm_mday	= outp->tm_mday;
	server_time->tm_mon		= outp->tm_mon;
	server_time->tm_year	= outp->tm_year;
	server_time->tm_wday	= outp->tm_wday;
	server_time->tm_yday	= outp->tm_yday;
	server_time->tm_isdst	= outp->tm_isdst;

	return server_time;
}

