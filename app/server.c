#include "../lib/chronos.h" /* gerado pelo RPCgen */
#include <stdio.h>
#include <time.h>
#include <math.h>


/* servidor sempre possui a seguinte forma:
NOME_VERSAO_SVC e um dos parâmetros é do tipo svc_req.
Além disso, retorna sempre um ponteiro*/
chronos_out * chronosproc_1_svc (chronos_in *in, struct svc_req *rqstp) {

	// Obtendo dados do client
	static chronos_out out;

	// Calculando tempo atual no servidor
	// =============================================
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );


	
	// DEBUG
	int debug = 1;
	if(debug) {

		printf("\e[1;1H\e[2J");
		printf("================ DEBUG ================\n");
		printf("\n");
		printf("| Starting...\n");
	}



	// Cliente
	// ========================================================
	struct tm * time_client;
	time_client = (struct tm *) malloc(sizeof(struct tm));
	time_client->tm_sec  	= in->tm_sec;
	time_client->tm_min  	= in->tm_min;
	time_client->tm_hour  	= in->tm_hour;
	time_client->tm_mday  	= in->tm_mday;
	time_client->tm_mon  	= in->tm_mon;
	time_client->tm_year  	= in->tm_year;
	time_client->tm_wday  	= in->tm_wday;
	time_client->tm_yday  	= in->tm_yday;
	time_client->tm_isdst  	= in->tm_isdst;
	printf("\n| Hora no cliente: %s", asctime( time_client ) );




	// Adicionando ao output a hora atual do servidor
	// ========================================================
	out.tm_sec 		= timeinfo->tm_sec;
	out.tm_min 		= timeinfo->tm_min;
	out.tm_hour 	= timeinfo->tm_hour;
	out.tm_mday 	= timeinfo->tm_mday;
	out.tm_mon 		= timeinfo->tm_mon;
	out.tm_year 	= timeinfo->tm_year;
	out.tm_wday 	= timeinfo->tm_wday;
	out.tm_yday 	= timeinfo->tm_yday;
	out.tm_isdst 	= timeinfo->tm_isdst;


	if(debug) {
		//printf("| Time Client: %s\n", asctime(in->time_client));
		printf("| Time Server: %s\n", asctime (timeinfo));

		printf("| Closing connection...\n");
		printf("================ DEBUG ================\n");
	}

	// Retornando saída ao cliente
	return(&out);
}