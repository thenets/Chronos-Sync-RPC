/*
	Parâmentros de entrada

	Tudo o que será obtido no cliente repassado para o servidor
*/
struct chronos_in {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

/*
	Parâmentros de saída

	O que será retornado para o cliente
*/
struct chronos_out {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;

	long tm_ms; /* milliseconds */
	float process_delay;
};

/*
	Configuração RPC

	Configuração Básica e Obrigatório do RPC
*/
program chronos_PROG {
	version chronos_VERS {
		chronos_out chronosPROC(chronos_in) = 1;
	} = 1; /* versao numero 1 */
} = 0x31230000; /* id do programa*/