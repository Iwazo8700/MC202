#include "derivative.h" /* include peripheral declarations */
#include "clkConfig.h"

/******************************************************************************/
/*********************** Definições  de  constantes ***************************/
/******************************************************************************/

/*** Configurações e máscaras em baixo nível: ***/
// Mascaras de bits relevantes para barra de sensores em GPIOA e GPIOB:
#define GPIOA_PINOS_BARRA_MASK 0xFF000000
#define GPIOB_PINOS_BARRA_MASK 0xFF
// Valor de recarga do contador do módulo pit, no caso, o maior possível:
#define PIT0_MOD_VALUE 0xFFFFFFFF
#define PIT1_MOD_VALUE 0xFFFFFFFF


/*** Configurações para leitura dos sensores:***/
// Tempo a se esperar para carregar as barras de sensores:
#define CAPACITOR_CHARGE_TIME 4000


// Leitura dos sensores será mapeada para um numero entre essas constantes dependendo da calibração:
// ifdef para troca de leitura entre estilos de pista
#define TipoPista 1   /* (linha branca -> 1 / if commented ->  0) */

#ifdef TipoPista
#define SENSOR_MAX_VALUE 1
#define SENSOR_MIN_VALUE 0
#endif
#ifndef TipoPista
#define SENSOR_MAX_VALUE 0
#define SENSOR_MIN_VALUE 1
#endif
// Quantidades e amostras a serem tiradas para executar o filtro de mediana:
#define MEDIAN_SAMPLE_COUNT 3


/*** Configurações para operação do servo: ***/
// Valores máximos e mínimos a serem enviados para o servo:
// Essas constantes não devem ser utilizadas em nenhum outro lugar além da função servo
#define SERVO_MIN_PULSE_OUT 780
#define SERVO_MAX_PULSE_OUT 1800
#define SERVO_MID_PULSE_OUT (SERVO_MIN_PULSE_OUT+SERVO_MAX_PULSE_OUT)/2

// Valores de entrada para a função servo que devem ser usadas no restante do código:
#define SERVO_MIN_PULSE_IN -200000
#define SERVO_MAX_PULSE_IN 200000
#define SERVO_MID_PULSE_IN (SERVO_MIN_PULSE_IN+SERVO_MAX_PULSE_IN)/2

// Valores máximo e mínimo da velocidade
#define MOTOR_MAX_SPEED 1300 //1200
#define MOTOR_MIN_SPEED 1150

#define FTM0_MOTOR_MOD_VALUE 1599
#define FTM1_SERVO_MOD_VALUE 60000 // (MOD - CNTIN + 0x01)*(PS/fclk) = Tcontagem

// Calibracao
#define CALIBRATION_TIMES 1000

#define Square(x) ((x)*(x))
#define CHECK_STOP_DELAY 5
#define RUN_TIME 2500

/******************************************************************************/
/*************************** Protótipos de Funções ****************************/
/******************************************************************************/

void delay(unsigned int time);
void init_GPIO();
void init_PIT();
void init_FMT();
void acenderLed();
void apagarLed();
void piscaLed(int vezes, int periodo);
void servo(int Valor_Pulso);
void motorPot(int Valor);
void aciona_servo(int valor);
void lerSensores(unsigned int* clock_count);
void calibrarSensores();
void ajustaSensores(unsigned int* clock_count);
int mediaSensores();
void insertionSort(unsigned int samples[MEDIAN_SAMPLE_COUNT][16]);
void mediana(unsigned int *resposta, unsigned int samples[MEDIAN_SAMPLE_COUNT][16]);
void updateState();
void calibra_servo();



/******************************************************************************/
/***************************** Variáveis Globais ******************************/
/******************************************************************************/


// Veja a explicação dessa variável no README.md:
const char read_gpio_offset[16] = {5,31,30,29,4,28,27,26,3,2,1,0,25,24,7,6};

// Vetor de struct simples com min e max para guardar a calibração de cada sensor:
struct sensor_calibration{
	unsigned int min; ; //maximo do branco
	unsigned int med;
	unsigned int max; //minimo do preto
} calibration_vector[16] =
{
{0,4000,999999},
{0,6000,999999},
{0,4000,999999},
{0,6000,999999},
{0,7000,999999},
{0,4000,999999},
{0,4000,999999},
{0,4000,999999},
{0,3000,999999},
{0,4000,999999},
{0,4000,999999},
{0,5000,999999},
{0,4000,999999},
{0,6000,999999},
{0,6000,999999},
{0,4000,999999}
};


int calibrar = 0;
int parada = 0, possivel_parada = 0, falsa_parada = 0;
// Delay para parar
int parada_count = CHECK_STOP_DELAY;
int PIT1v_OVFcount = 0;


/******************************************************************************/
/********************************** Funções ***********************************/
/******************************************************************************/

#pragma GCC push_options
#pragma GCC optimize(0)
/* Função para delay com busy waiting.
 */
void delay(unsigned int time){
	while(time > 0)
		time -= 1;
}
#pragma GCC pop_options



/* Função para inicializar todos os pinos que utilizam GPIO
 *
 */
void init_GPIO(){
	// Configurar os pinos como entradas:
	GPIOA_PDDR &= ~GPIOA_PINOS_BARRA_MASK;
	GPIOB_PDDR &= ~GPIOB_PINOS_BARRA_MASK;
	// Habilitar input nesses pinos:
	GPIOA_PIDR &= ~GPIOA_PINOS_BARRA_MASK;
	GPIOB_PIDR &= ~GPIOB_PINOS_BARRA_MASK;
	// Configurar pino do mosfet como saída:
	GPIOB_PDDR |= (1 << 14);
	GPIOB_PIDR |= (1 << 14);
	// Configurar LED PTH6 como saída
	GPIOB_PDDR |= (1 << 30);
	GPIOB_PIDR |= (1 << 30);
	// Configurar pinos de entrada da ponte H como saídas do micro:
	GPIOA_PDDR |= (1 << 22) | (1 << 23);
	GPIOA_PIDR |= (1 << 22) | (1 << 23);
	GPIOB_PDDR |= (1 << 8) | (1 << 9);
	GPIOB_PIDR |= (1 << 8) | (1 << 9);
	// Aproveitar para já deixar esse pinos nos níveis corretos para os motores
	// irem para frente, afinal nunca precisaremos ir para trás com o seguidor
	// PTC7 e PTF0 altos:
	GPIOA_PSOR |= (1 << 23);
	GPIOB_PSOR |= (1 << 8);
	// PTC6 e PTF1 baixos:
	GPIOA_PCOR |= (1 << 22);
	GPIOB_PCOR |= (1 << 9);
}

//PTH6
void acenderLed(){
	GPIOB_PSOR |= (1 << 30);
}

void apagarLed(){
	GPIOB_PCOR |= (1 << 30);
}

// Pisca o led PTH6 tantas vezes e por um periodo em milissegundos
void piscaLed(int vezes, int periodo){
	while(vezes--){
		acenderLed();
		delay(3200*periodo);
		apagarLed();
		delay(3200*periodo);
	}
}

/* Função para inicalização do módulo PIT e de seus contadores
 * Autor: Arthur Moraes do lago <arthurmoraeslago@gmail.com>
 */
void init_PIT(){
	// Habilitar o clock para o módulo PIT:
	SIM_SCGC |= SIM_SCGC_PIT_MASK;
	// Habilitar o módulo para poder configurar:
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;

	// Configurar valor de recarga do timer para ser o maior possível:
	PIT_LDVAL0 = PIT0_MOD_VALUE;
	// Não ligaremos interrupções para o canal 0, pois o contador tem 32 bits, no clock máximo de
	// 24MHz, isso equivale a 178 segundos, muito mais que qualquer tempo descarga

	// Configurar valor de recarga do timer para ser o maior possível:
	PIT_LDVAL1 = PIT1_MOD_VALUE;
	// Já para o canal 1, que mantém o tempo global, ligaremos a interrupção de overflow:
	PIT_TCTRL1 |= PIT_TCTRL_TIE_MASK;
	// E já podemos começar a rodar o timer:
	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK;
}


/*
 *
 */
void init_FMT(){

	// Para os motores, vamos chutar uma frequência de PWM de 30kHz.
	// O motivo para isso é que é a frequiencia usada no combate, que eu suponho
	// que tenha um motivo para ser essa(provavelmente depende dos motores, mas estamos sem base aqui)
	// Outro motivo é que nessa frequência, não conseguiremos ouvir o som do PWM,
	// deixando o robô menos chato

	SIM_SCGC |= SIM_SCGC_FTM0_MASK;

	// Definir prescaler para dividir por 1:
	FTM0_SC |= FTM_SC_PS(0x00);
	// Definir fonte do clock como a do sistema:
	FTM0_SC |= FTM_SC_CLKS(0x01);

	// Definir módulo do contador para o suficiente para gerar um periodo de 33.3uS
	FTM0_MOD = FTM0_MOTOR_MOD_VALUE;
	// Configurar PWM edge-aligned, setando a saida no overflow, e limpando quando o valor do canal for igual:
	FTM0_C0SC |= FTM_CnSC_MSB_MASK;
	FTM0_C0SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C0SC &= ~FTM_CnSC_ELSA_MASK;
	// Mapeia a saída o canal 0 para o pino PTA0:
	SIM_PINSEL0 &= ~SIM_PINSEL_FTM0PS0_MASK;

	// Configurar PWM edge-aligned, setando a saida no overflow, e limpando quando o valor do canal for igual:
	FTM0_C1SC |= FTM_CnSC_MSB_MASK;
	FTM0_C1SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C1SC &= ~FTM_CnSC_ELSA_MASK;
	// Mapeia a saída o canal 1 para o pino PTA1:
	SIM_PINSEL0 &= ~SIM_PINSEL_FTM0PS1_MASK;

	// De inicio, deixar os 2 PWMs desligados:
	FTM0_C0V = FTM_CnV_VAL(0);
	FTM0_C1V = FTM_CnV_VAL(0);

	// O FTM 1 é usado para gerar PWM para o servo no canal 1, que vai para o PTC5:
	SIM_SCGC |= SIM_SCGC_FTM1_MASK ; //Ativa clk para FTM1

	FTM1_SC |= FTM_SC_PS(4); //Prescaler: fator que divide o clock por 1
	FTM1_SC |= FTM_SC_CLKS(0x01); //Escolhe o clock do sistema para alimentar o FTM

	FTM1_MOD = FTM1_SERVO_MOD_VALUE; //Valor maximo do contador
	FTM1_C1SC |= FTM_CnSC_MSB_MASK;  //PWM Edge-Aligned
	FTM1_C1SC |= FTM_CnSC_ELSB_MASK; //Pulso nivel alto verdadeiro
	FTM1_C1SC &= ~FTM_CnSC_ELSA_MASK; //Pulso nivel alto verdadeiro
	// De inicio, deixar desligado:
	FTM1_C1V = FTM_CnV_VAL(0);

	SIM_PINSEL0 &= ~SIM_PINSEL_FTM1PS1_MASK; //Seleciona em qual pino a saída do canal deve ser mapeada, no caso PTC5
}

void init_NVIC(){
	// Habilitar NVIC para as interrupções do PIT1:
	NVIC_ISER |= (1 << 23);
}




/* Envia o sinal PWM para o motor do servo girar, é importante
 * observar que  -1024<Valor<1024, visto que é uma restriçao do servo
 * Sendo -1024 o limite para esquerda e 1024 para a direita.
 */
void servo(int Valor){
	int Valor_Pulso;
	Valor_Pulso = SERVO_MID_PULSE_OUT+(2*Valor*(SERVO_MID_PULSE_OUT - SERVO_MIN_PULSE_OUT)/(SERVO_MAX_PULSE_IN - SERVO_MIN_PULSE_IN));
	// Garantir que o valor recebido está dentro do limite aceitável:
	if (Valor_Pulso > SERVO_MAX_PULSE_OUT)
		Valor_Pulso = SERVO_MAX_PULSE_OUT;
	else if (Valor_Pulso < SERVO_MIN_PULSE_OUT)
		Valor_Pulso = SERVO_MIN_PULSE_OUT;

	// Muda o tempo alto do pwm enviado para o servo:
	FTM1_C1V = FTM_CnV_VAL(Valor_Pulso * FTM1_SERVO_MOD_VALUE / 20000);
}

/* Função para configurar potência dos motorees
 * Recebe valores entre 0(motor desligado) e 1600(potência máxima)
 */
void motorPot(int pot){
	// Evitar confusões tirando números negativos:
	if (pot < 0)
		pot = 0;

	// Mudar o valor em que o pulso fica baixo no gerador de PWM:
	FTM0_C0V = FTM_CnV_VAL(pot);
	FTM0_C1V = FTM_CnV_VAL(pot);
}

/* Função que lê os sensores e guarda o tempo que cada um demorou para descarregar
 * no endereço enviado como parâmetro.
 * O endereço enviado como parâmetro deve ser de um vetor de 16 posições do tipo
 * unsigned int
 *
 * Esta função é crítica, ou seja, deve rodar o mais rápido possível
 * Autor: Arthur Moraes do Lago(arthurmoraeslago@gmail.com)
 */
void lerSensores(unsigned int* clock_count){
	// Podemos combinar os 2 registradores de leitura GPIO, pois os bits relevantes
	// para nós são apenas 8 em cada registrador. No GPIOA, apenas aleitura dos ultimos
	// 8 bits nos interessa, enquanto que no GPIOB, apenas a leitura dos 8 primeiros.
	// 1 variíavel de 32 bits então é o suficiente para armazenar esses valores com sobra,
	// e já que estamos em um processador de 32 bits, não há vantagem em remover essa sobra.

	// Variável para guardar a última leitura dos 2 registradores de entrada,
	// combinados em uma única variável de 32bits:
	int last_gpio_read;
	// Variáveis para guardar a leitura atual dos 2 registradores de entrada,
	// combinados em uma única variável de 32bits:
	int gpio_read;

	//// Primeiro devemos carregar os capacitores das barras de sensores:
	// Ligar gate do mosfet:
	GPIOB_PSOR = (1 << 14);

	// Esperar até que os capacitores estejam carregados:
	delay(CAPACITOR_CHARGE_TIME);
	// Zerar o vetor de contagem de tempo:
	// Fazemos isso aqui para dar um tempinho a mais para os capacitores carregarem.
	int i = 0;
	for (i; i < 16; i++){
		clock_count[i] = 0;
	}

	// Iniciar um cronômetro para medir o tempo até os capacitores descarregarem:
	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;

	// Desligar gate dos mosfets:
	GPIOB_PCOR = (1 << 14);

	// Inicializar valores iniciais para as leituras antigas:
	last_gpio_read = (GPIOA_PDIR & GPIOA_PINOS_BARRA_MASK) | (GPIOB_PDIR & GPIOB_PINOS_BARRA_MASK);

	// Repetir enquanto qualquer pino das barras de sensores ainda estiver alto:
	do{
		// Obter uma leitura dos pinos das barras de sensores combinados em um
		// unico registrador:
		gpio_read = (GPIOA_PDIR & GPIOA_PINOS_BARRA_MASK) | (GPIOB_PDIR & GPIOB_PINOS_BARRA_MASK);
		// Fazer alguma coisa somente se essa leitura for diferente da ultima:
		if (gpio_read != last_gpio_read){
			// Se a leitura foi diferente, é porque algum capacitor descarregou
			// Guardar a hora que essa descarga ocorreu:
			unsigned int high_time = PIT0_MOD_VALUE - PIT_CVAL0;
			// Isolar bits que mudaram usando um XOR:
			int gpio_change = gpio_read ^ last_gpio_read;
			// Percorrer os 16 bits da barra de sensores procurando a mudança:
			int i= 0;
			for (i; i < 16; i++){
				// Verificar se esse pino ainda não foi lido, se ja foi, ele
				// definitivamente vai falhar nos próximos ifs
				if (clock_count[i] == 0){
					// Veja a explicação dessa parte do código no README.md
					if (gpio_change & (1 << read_gpio_offset[i])){
						// Guardar a hora que esse capacitor descarregou:
						clock_count[i] = high_time;
					}
				}
			}
		}
		// A leitura atual passa a ser a leitura antiga:
		last_gpio_read = gpio_read;

	}while(gpio_read);

	// Desligar o cronômetro:
	PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK;

}

void calibrarSensores(){
	unsigned int clock_count[16];
	int i = CALIBRATION_TIMES, j;

	for(j=0;j<16;j++){
		calibration_vector[j].min = 0;
		calibration_vector[j].med = 0;
		calibration_vector[j].max = 999999;
	}
	// Inicia leituras do branco
	piscaLed(3, 500);
	while(i--){
		lerSensores(clock_count);
		for(j=0;j<16;j++){
			if(clock_count[j] > calibration_vector[j].min)
				calibration_vector[j].min = clock_count[j];
		}
	}

	delay(6400000);

	// Inicia leituras do preto
	piscaLed(6, 500);
	i=CALIBRATION_TIMES;
	while(i--){
		lerSensores(clock_count);
		for(j=0;j<16;j++){
			if(clock_count[j] < calibration_vector[j].max)
				calibration_vector[j].max = clock_count[j];
		}
	}

	for(j=0;j<16;j++){
		calibration_vector[j].med = (calibration_vector[j].min + calibration_vector[j].max)/2;
	}

	piscaLed(10, 300);
}


/* Função que recebe um vetor com os tempos de descarga dos 16 capacitores, e
 * substitui os valores por versões calibradas de acordo com o vetor de calibração
 *
 * Autor: Arthur Moraes do Lago(arthurmoraeslago@gmail.com)
 */
void ajustaSensores(unsigned int *clock_count){
	int sensor_media;
	// Para cada sensor:
	int i = 0;
	for (i; i < 16; i++){
		// Verificar se é maior que o máximo, se for só corta para o valor máximo:
		sensor_media = calibration_vector[i].med;
		if (clock_count[i] < sensor_media){
			clock_count[i] = SENSOR_MAX_VALUE;
		// Se for menor que o mínimo, só corta para o valor mínimo:
		}else{
			clock_count[i] = SENSOR_MIN_VALUE;
		}
	}

}

#define LAST_SIZE 20
struct {
	char left; //0 ou 1
	char right; //0 ou 1
	char total; //0 a 16
} ultimos[LAST_SIZE];

int last = 0, //O índice do dado mais antigo
	sum_left = 0,
	sum_right = 0,
	sum_total = 0,
	max_sum_total = 0,
	max_sum_left = 0,
	max_sum_right = 0;

struct {
    long int valor_retorno, diferenca;
    int qtde_brancos;
} brancos[3];

int min_regiao = 0;
int qtde_regioes = 0;

//int last_P[RUN_TIME];
//int last_P_index = 0;

/*Faz a media dos valores de acordo com os pesos, retornando
o valor que devemos mudar servomotor.
Tambem atualiza os ultimos dados e as somatórias correspondentes*/
int mediaSensores(unsigned int *clock_count){
	long int 		i,		valor_retorno_central = 0;
	static long int prev_valor_retorno_central = 0;
	int 			lendo_branco = 0;
	int 			left = 0,
					right = 0,
					total = 0;

    for(i = 0; i < 3 ; i++){
    	brancos[i].valor_retorno = 0;
    	brancos[i].diferenca = 60000;
    	brancos[i].qtde_brancos = 0;
    }

    qtde_regioes = 0;
	const int pesos[16] = {-32767, -30546, -27769, -24992, -20549, -15550, -9997, -3332, 3332, 9997, 15550, 20549, 24992, 27769, 30546, 32767};

	// Multipla cada leitura pelos seus pesos:
	for(i = 0; i < 16 ; i++){
		total += clock_count[i];
		// Se comecou a ler branco
		if((clock_count[i] == 1) && !lendo_branco){
			lendo_branco = 1;
			qtde_regioes++;
		}
		// Se for preto
		if(clock_count[i] == 0){
			lendo_branco = 0;
		}

		if(lendo_branco){
		    brancos[qtde_regioes-1].valor_retorno += clock_count[i]*pesos[i];
			brancos[qtde_regioes-1].qtde_brancos += clock_count[i];
		}
	}
	// Se nao achou nenhum branco
	if(qtde_regioes == 0){
		return prev_valor_retorno_central;
	}

	for(i=0;i<qtde_regioes;i++){
		// Conclui a media ponderada
		brancos[i].valor_retorno /= brancos[i].qtde_brancos;
		// Calcula a distancia da regiao de brancos ate a antiga regiao central
		brancos[i].diferenca = brancos[i].valor_retorno - prev_valor_retorno_central;
		if(brancos[i].diferenca < 0) brancos[i].diferenca = -brancos[i].diferenca;
	}

	// Ve qual a menor distancia
	long int min_diferenca = brancos[0].diferenca;
	min_regiao = 0;
	for(i=0;i<qtde_regioes;i++){
		if (brancos[i].diferenca < min_diferenca){
			min_diferenca = brancos[i].diferenca;
			min_regiao = i;
		}
	}

	// Retorna apenas a media da regiao central

	valor_retorno_central = brancos[min_regiao].valor_retorno;

	if(min_regiao < qtde_regioes-1) right = 1;
	if(min_regiao > 0) left = 1;

	for(i=0;i<qtde_regioes;i++){
		// Se uma regiao tiver mais de 3 brancos, provavel que seja cruzamento ou curva
		if(brancos[i].qtde_brancos > 3 && qtde_regioes > 1){
			valor_retorno_central = 0;
			right = 0;
			left = 0;
		}
	}

	// Atualiza <ultimos> dados
	sum_right += right - ultimos[last].right;
	sum_left += left - ultimos[last].left;
	sum_total += total - ultimos[last].total;
	ultimos[last].right = right;
	ultimos[last].left = left;
	ultimos[last].total = total;

	if(sum_total > max_sum_total) max_sum_total = sum_total;

	if(sum_left > max_sum_left) max_sum_left = sum_left;
	if(sum_right > max_sum_right) max_sum_right = sum_right;

	last = (last+1)%LAST_SIZE;

	prev_valor_retorno_central = valor_retorno_central;
	//last_P[last_P_index] = valor_retorno_central;
	//last_P_index = (last_P_index+1)%RUN_TIME;
	return valor_retorno_central;
}

enum {
	LINHA_UNICA,
	INCERTO_DIR,
	MARC_DIR,
	INCERTO_ESQ,
	MARC_ESQ,
	CRUZAMENTO,
	FORA_LINHA
} estadoPista;

void updateState(){
	static int verifyMarkTime = 1;
	if(qtde_regioes == 0) estadoPista = FORA_LINHA;
	else if(sum_total > 105){
		estadoPista = CRUZAMENTO;
	} else if (sum_total > 60){
		if(sum_left > 5 && sum_right > 5) estadoPista = CRUZAMENTO;
		else{
			if(sum_right >= 3 && sum_left < 2) estadoPista = INCERTO_DIR;
			if(sum_left >= 5 && sum_right < 2) estadoPista = INCERTO_ESQ;
			verifyMarkTime = 1;
		}
	} else {
		if(estadoPista != INCERTO_ESQ && estadoPista != INCERTO_DIR)
			estadoPista = LINHA_UNICA;
	}

	if(estadoPista == INCERTO_ESQ || estadoPista == INCERTO_DIR){
		if(verifyMarkTime-- == 0){
			verifyMarkTime = 1;
			if(estadoPista == INCERTO_ESQ)
					estadoPista = MARC_ESQ;
			else 	estadoPista = MARC_DIR;
		}
	}
}
/*void updateState(){
	static int verifyMarkTime = 5;
	if(qtde_regioes == 0) estadoPista = FORA_LINHA;
	else if(sum_total > 105){
		estadoPista = CRUZAMENTO;
	} else if (sum_total > 60){
		if(sum_left > 5 && sum_left > 5) estadoPista = CRUZAMENTO;
		else{
			if(sum_right >= 3 && sum_left < 2) estadoPista = INCERTO_DIR;
			if(sum_left >= 5 && sum_right < 2) estadoPista = INCERTO_ESQ;
			verifyMarkTime = 5;
		}
	} else {
		if(estadoPista != INCERTO_ESQ && estadoPista != INCERTO_DIR)
			estadoPista = LINHA_UNICA;
	}

	if(estadoPista == INCERTO_ESQ || estadoPista == INCERTO_DIR){
		if(verifyMarkTime-- == 0){
			verifyMarkTime = 5;
			if(estadoPista == INCERTO_ESQ)
					estadoPista = MARC_ESQ;
			else 	estadoPista = MARC_DIR;
		}
	}
}*/

/* Ordena as medidas tiradas de cada sensor, para depois pegar a
 *
 */
void insertionSort(unsigned int samples[MEDIAN_SAMPLE_COUNT][16]){
	int i, key, j,k;
	// Para cada sensor:
	for(k = 0; k < 16; k++){
		// Ordenar suas 5 medidas:
		for (i = 1; i < MEDIAN_SAMPLE_COUNT; i++){
			key = samples[i][k];

			j = i-1;

			while (j >= 0 && samples[j][k] > key)
			{
				samples[j+1][k] = samples[j][k];
				j = j-1;
			}

			samples[j+1][k] = key;
		}
	}
}

/* Retorna a mediana das leituras recebidas:
 */
void mediana(unsigned int *resposta, unsigned int samples[MEDIAN_SAMPLE_COUNT][16]){

	insertionSort(samples);
	int i = 0;
	for(i ; i < 16; i++){
		resposta[i] = samples[MEDIAN_SAMPLE_COUNT / 2][i];
	}
}


/* Retorna o tempo atual em pulsos de clock:
 */
unsigned long int getTime(){
	return (PIT1v_OVFcount + 1) * PIT1_MOD_VALUE - PIT_CVAL1;
}

/* Utilizamos o movimento do servo dentro do while, para ver a angulacao maxima para a direita,
	para a esquerda e posicao central.
*/
void calibra_servo(){
	while(1){
		servo(SERVO_MIN_PULSE_IN/2);
		delay(5000000);
		servo(0);
		delay(5000000);
		servo(SERVO_MAX_PULSE_IN/2);
		delay(5000000);
		servo(0);
		delay(5000000);
	}
}

/******************************************************************************/
/*********************************** Main *************************************/
/******************************************************************************/
//int last_servoPos[RUN_TIME];
//int last_servoPos_index = 0;

int main(){
	clockSetup(CORE_48MHZ_BUS_24MHZ);
	init_GPIO();
	init_PIT();
	init_FMT();
	init_NVIC();
	apagarLed();
	motorPot(0);//0% da potencia dos motores
	servo(0);


	delay(6400000);//Tempo suficiente para o servomotor chegar ao centroe encaixar a cabeca se precisar
	if(calibrar){
		calibrarSensores();
		piscaLed(10,500);
		return 0;
	}

	int i;
	for(i=0;i<LAST_SIZE;i++){
		ultimos[i].left = 0;
		ultimos[i].right = 0;
		ultimos[i].total = 0;
	}
	// Constantes para o PID:
	//Divisao de constantes em numerador e denominador visto o nao uso do float
	const int kP_Num = 95, kP_Den = 10, kI_Num = 0, kI_Den = 1000, kD_Num = 5, kD_Den = 100; //Ctes boas: 0.6, 0, -0.55 ou 0.53, 0, 0.01
	//45, 0, 0 completou mal, mas completou. kd de 0.1 melhorou um um pouco
	//0.3, 0 ,0 foi melhor 1min
	//MANDA SO A RESPOSTA
	//4.5, 0, o Velocidade 1300 ótimo
	//5.5, 0, 0 velocidade 1400 melhor ainda 27s
	//7, 0, 0, velocidade 1500 25s
	//8.5, 0, 0 velocidade 1600 oscilou mais e nao completou
	//8, 0, 0 velocidade 1600 oscilou mais ou menos mas completou
	//7.5, 0, 0 velocidade 1600 minima 1200: 23s
	//run time de mais ou menos 2500
	// Guarda a posição que estamos enviano ao servo:
	int servoPos = 0;
	// Variáveis para o PID:
	int resposta, P, I = 0, D = 0;
	// Guarda ultima média de sensores:
	int ultima_media = 0;
	// Guarda a hora da última execução:
	long int lastTime = getTime();
	// Tempo a ser rodado (em numero de loops) definido como RUN_TIME
	long int run_time = 0;


	//servo(-200000);
	motorPot(1600);  // 0 a 1600
	//return;
	while(1){
		if(run_time++ == RUN_TIME){
			motorPot(0);
			servo(0);
			break;
		}
		// Armazena algumas leituras dos sensores para realizar a mediana:
		unsigned int readSamples[MEDIAN_SAMPLE_COUNT][16];
		// Armazena a leitura já filtrada pela mediana dos sensores:
		unsigned int clock_count[16];
		// Obtém leitura dos sensores:
		int i = 0;
		for (i ; i < MEDIAN_SAMPLE_COUNT; i++){
			lerSensores(readSamples[i]);

		}
		mediana(clock_count, readSamples);
		// Usa valores de calibração para ajustar as leituras:
		ajustaSensores(clock_count);


		// Calcular tempo atual, variação de tempo e atualizar ultimo tempo para próxima iteração:
		long int currentTime = getTime();
		int dt = currentTime - lastTime;
		lastTime = currentTime;


		// PID:
		P = mediaSensores(clock_count);
		I += P * dt;
		D = (P - ultima_media); //dividimos por dt depois para maior precisao
		ultima_media=P;


		// Controle de velocidade proporcional (quadraticamente) ao erro do angulo
		if(qtde_regioes == 0) motorPot(0);
		else motorPot(MOTOR_MAX_SPEED - Square(P)/(Square(20000)/(MOTOR_MAX_SPEED-MOTOR_MIN_SPEED))); // era D

		// TODO: limites para integrativo, ou amortizar a integral no tempo

		resposta =  (kP_Num * P)/kP_Den -
					(kI_Num * I)/kI_Den -
					(kD_Num * D)/kD_Den; // TINHA UM DT AQUI

		//Ignora variações brutas
		//if(D < -15000 || D > 15000) resposta = 0;
		// Somar correção de erro obtida com PID a posição do servo:
		//servoPos = servoPos + resposta;
		servoPos = resposta;
		// Evitar acumular respostas fora da faixa de operação
		if (servoPos < SERVO_MIN_PULSE_IN){
			servoPos = SERVO_MIN_PULSE_IN;
		}else if(servoPos > SERVO_MAX_PULSE_IN){
			servoPos = SERVO_MAX_PULSE_IN;
		}
		// Mandar o servo se mexer:
		servo(servoPos);
		//last_servoPos[last_servoPos_index] = servoPos;
		//last_servoPos_index= (last_servoPos_index+1)%RUN_TIME;

		updateState();
		if(estadoPista == INCERTO_DIR || estadoPista == INCERTO_ESQ || estadoPista == MARC_DIR) acenderLed();
		else apagarLed();
		if(estadoPista == MARC_DIR && (servoPos>-30000)&& (servoPos<30000) && run_time > 0){
			//delay(100);
			//motorPot(0);
			//break;
		}

	}
	return 0;
}


/******************************************************************************/
/******************************** Interrupções ********************************/
/******************************************************************************/

// Interrupção quando o PIT0 chega a 0:
void PIT_CH1_IRQHandler(){
	PIT1v_OVFcount++;
	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;
}
