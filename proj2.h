/* Ficheiro header do projeto: contém as declarações de constantes, macros,
 * estruturas, variáveis globais e todos os protótipos das funções que irão
 * ser utilizadas no ficheiro .c
 * */

/*-------------------------------- CONSTANTES --------------------------------*/

/* Número máximo de aeroportos */
#define MAX_AEROPORTOS 40
/* Comprimento máximo da string que contém o id de um aeroporto */
#define ID_AEROPORTO 4
/* Comprimento máximo da string que contém o país de um aeroporto */
#define LEN_PAIS 31
/* Comprimento máximo da string que contém a cidade de um aeroporto */
#define LEN_CIDADE 51
/* Número máximo de voos */
#define MAX_VOOS 30000
/* Comprimento máximo da string que contém o código de um voo */
#define CODIGO_VOO 9
/* Quantidade mínima de passageiros */
#define MIN_PASSAGEIROS 10
/* Duração máxima de um voo em minutos */
#define DURACAO_MAXIMA 720
/* Constante auxiliar booleana */
#define FALSE 0
/* Constante auxiliar booleana */
#define TRUE 1
/* Tamanho máximo do input fornecido */
#define MAX_INPUT 65535
/* Tamanho mínimo de passageiros numa reserva */
#define PASSAGEIROS_RESERVA 1
/* Tamanho mínimo de um código de reserva */
#define MIN_COD_RESERVA 10

/*---------------------------------- MACROS ----------------------------------*/

/* Troca o valor do aeroporto a pelo aeroporto b. */
#define TROCA_AER(a, b) { Aeroporto tmp; tmp = a; a = b; b = tmp; }

/* Troca o pointer para voo a pelo pointer para voo b. */
#define TROCA_VOO(a, b) { Voo* tmp; tmp = a; a = b; b = tmp; }


/*-------------------------------- ESTRUTURAS --------------------------------*/

/* Estrutura hora */
typedef struct {
    int horas;
    int minutos;
} Hora;


/* Estrutura data */
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;


/* Estrutura aeroporto */
typedef struct {
    char id[ID_AEROPORTO];
    char pais[LEN_PAIS];
    char cidade[LEN_CIDADE];
    int n_voos;
} Aeroporto;


/* Estrutura reserva */
typedef struct {
    char *codigo;
    int num_pass;
} Reserva;


/* Node para reserva */
typedef struct st_node {
    struct st_node *next;
    Reserva *reserva;
} r_node;


/* Estrutura voo */
typedef struct {
    char codigo[CODIGO_VOO];
    char id_chegada[ID_AEROPORTO];
    char id_partida[ID_AEROPORTO];
    Data data_partida;
    Data data_chegada;
    Hora hora_partida;
    Hora hora_chegada;
    Hora duracao;
    int capacidade;
    int lotacao;
    r_node *head_reservas;
} Voo;


/* Node para voo */
typedef struct s_node {
    struct s_node *next;
    Voo *voo;
} v_node;


/*---------------------------- VARIÁVEIS GLOBAIS -----------------------------*/

/* Array que contém os aeroportos adicionados até ao momento. */
Aeroporto aeroportos[MAX_AEROPORTOS];

/* Cabeça da linked list de voos. */
v_node *head_voos = NULL;
/* Pointer para o último elemento da linked list de voos. */
v_node *last_voos = NULL;

/* A quantidade de aeroportos e voos existentes até ao momento */
int qtd_aeroportos = 0, qtd_voos = 0;

/* Data atual do sistema */
Data data_sistema = {1, 1, 2022};


/*-------------------------------- PROTÓTIPOS --------------------------------*/

/* Comandos */
void a_command();
void l_command();
void v_command();
void p_command();
void c_command();
void t_command();
void r_command();
void e_command();
/* Funções auxiliares gerais */
void consome_linha();
int is_upper(char c);
int is_digit(char c);
void termina_programa();
/* Funções auxiliares de aeroportos */
void obtem_cidade_aeroporto(char cidade[]);
int verifica_id_aeroporto(char id[]);
int valida_aeroporto(Aeroporto a);
int verifica_aeroporto_existe(char id_verif[]);
void ordena_aeroportos();
void imprime_aeroportos();
int encontra_imprime_aeroporto(char id_aer[]);
/* Funções auxiliares de voos */
int verifica_codigo_voo(char string[], int size);
Voo* obtem_voo_dia(char codigo[], Data d);
int verifica_codigo_data(char codigo[], Data d, Voo *v);
int validar_voo(Voo *v);
void imprime_voos();
void aumenta_voos(char id[]);
void ordena_voos(Voo* lst[], int size, char c);
void lista_voos(Voo* lst[], int size, char c);
/* Funções auxiliares de tempo e datas */
int data_para_inteiro(Data d);
int compara_datas(Data d1, Data d2);
int hora_para_inteiro(Hora h1);
int compara_horas(Hora h1, Hora h2);
void obtem_hora_data_chegada(Voo* v);
Data obtem_data_chegada(Data d);
/* Funções auxiliares para reservas */
void obtem_codigo_reserva(Reserva *reserva);
int verifica_codigo_reserva(char *codigo, int size);
int codigo_reserva_ja_existe(char *cod_res);
int validar_reserva(Reserva *reserva, Voo *v, char *codigo);
void lista_reservas(Voo *v);
/* Funções para nodes */
void push_voo(Voo *v);
int destroi_voos(char cod[]);
void destroi_todos_voos();
void push_reserva(r_node **head, Reserva *r);
void destroi_reserva(char *codigo_res);
void destroi_reservas(r_node *head);

/*--------------------------- Fim do ficheiro --------------------------------*/
