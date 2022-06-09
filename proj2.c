/******************************************************************************/
/********************************* Projeto 2 **********************************/
/******************************** Pedro Curto *********************************/
/******************************** ist 1103091 *********************************/
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proj2.h"

int main() {
    char c;
    while ((c = getchar()) != 'q') {
        switch (c) {
            case 'a': a_command();
                break;
            case 'l': l_command();
                break;
            case 'v': v_command();
                break;
            case 'p': p_command();
                break;
            case 'c': c_command();
                break;
            case 't': t_command();
                break;
            case 'r': r_command();
                break;
            case 'e': e_command();
                break;
        }
    }
    /* Liberta a memória associada ao programa */
    destroi_todos_voos();
    return 0;
}


/*-------------------------------- Comando a ---------------------------------*/

/* O comando "a" adiciona um novo aeroporto ao sistema e imprime-o para
 * o stdout. */
void a_command() {
    Aeroporto a;
    /* Obtém o id, país e cidade do aeroporto */
    scanf("%s", a.id);
    scanf("%s", a.pais);
    obtem_cidade_aeroporto(a.cidade);
    /* Se o aeroporto for validado, inicializa-o e copia-o para o array global,
     * aumenta a quantidade de aeroportos e imprime-o para o stdout */
    if (valida_aeroporto(a)) {
        strcpy(aeroportos[qtd_aeroportos].id, a.id);
        strcpy(aeroportos[qtd_aeroportos].pais, a.pais);
        strcpy(aeroportos[qtd_aeroportos].cidade, a.cidade);
        aeroportos[qtd_aeroportos].n_voos = 0;
        qtd_aeroportos++;
        printf("airport %s\n", a.id);
    }
}


/*-------------------------------- Comando l ---------------------------------*/

/* O comando "l" lista aeroportos por ordem alfabética do seu código: se não
 * forem fornecidos argumentos, lista todos os aeroportos; se for fornecida
 * uma lista de aeroportos, imprime-os à medida que são fornecidos. */
void l_command() {
    char c = getchar(), id_aer[ID_AEROPORTO];
    /* Se o comando for apenas 'l', lista todos os aeroportos, um por linha,
     * por ordem alfabética de ID. */
    if (c == '\n') {
        ordena_aeroportos();
        imprime_aeroportos();
        return;
    }
    /* Lê cada código de voo individualmente até ao final da linha, verificando
     * se existe; em caso afirmativo, adiciona-o para o array de aeroportos;
     * senão, gera o erro pretendido e não executa o código que o adicionaria.
     */
    else {
        while (c != '\n') {
            scanf("%s", id_aer);
            if (!encontra_imprime_aeroporto(id_aer))
                printf("%s: no such airport ID\n", id_aer);
            c = getchar();
        }
    }
}


/*-------------------------------- Comando v ---------------------------------*/

/* Se não forem fornecidos argumentos ao comando "v", este lista todos os
 * voos pela ordem de criação; caso contrário, adiciona um voo com os
 * parâmetros fornecidos. */
void v_command() {
    Voo *v = (Voo*) malloc(sizeof(Voo));
    if (!v)
        termina_programa();
    if (getchar() == '\n') {
        imprime_voos();
        return;
    }
    else {
        scanf("%s", v->codigo);
        scanf("%s", v->id_partida);
        scanf("%s", v->id_chegada);
        scanf("%d-%d-%d", &v->data_partida.dia, &v->data_partida.mes,
               &v->data_partida.ano);
        scanf("%d:%d", &v->hora_partida.horas, &v->hora_partida.minutos);
        scanf("%d:%d", &v->duracao.horas, &v->duracao.minutos);
        scanf("%d", &v->capacidade);
        v->lotacao = 0;
        consome_linha();
        if (validar_voo(v)) {
            aumenta_voos(v->id_partida);
            push_voo(v);
        }
    }
}


/*-------------------------------- Comando p ---------------------------------*/

/* O comando "p" lista todos os voos que têm partida no aeroporto cujo ID
 * é fornecido, da data e hora mais antiga para a mais recente. */
void p_command() {
    int i = 0;
    char c = 'p', id_aux[ID_AEROPORTO];
    Voo* mesma_partida[MAX_VOOS];
    v_node *aux = head_voos;
    /* Lê e verifica o id fornecido do aeroporto e seleciona os voos com o
     * mesmo aeroporto de partida */
    scanf("%s", id_aux);
    consome_linha();
    if (!verifica_aeroporto_existe(id_aux)) {
        printf("%s: no such airport ID\n", id_aux);
        return;
    }
    /* Selecionar os voos com o mesmo aeroporto de partida */
    for (; aux != NULL; aux = aux->next) {
        if (!strcmp(aux->voo->id_partida, id_aux))
            mesma_partida[i++] = aux->voo;
    }
    /* Ordena os voos e imprime-os para o stdout */
    ordena_voos(mesma_partida, i, c);
    lista_voos(mesma_partida, i, c);
}


/*-------------------------------- Comando c ---------------------------------*/

/* O comando "c" lista todos os voos que chegam a um aeroporto cujo ID
 * é fornecido; lista-os da data e hora mais antiga para a mais recente. */
void c_command() {
    int i = 0;
    char c = 'c', id_aux[ID_AEROPORTO];
    Voo* mesma_chegada[MAX_VOOS];
    v_node *aux = head_voos;
    /* Lê e verifica o id de aeroporto fornecido */
    scanf("%s", id_aux);
    consome_linha();
    if (!verifica_aeroporto_existe(id_aux)) {
        printf("%s: no such airport ID\n", id_aux);
        return;
    }
    /* Seleciona os voos com o mesmo aeroporto de chegada e obtém a data
     * e hora de chegada de cada um */
    for (; aux != NULL; aux = aux->next) {
        if (!strcmp(aux->voo->id_chegada, id_aux)) {
            mesma_chegada[i++] = aux->voo;
            obtem_hora_data_chegada(aux->voo);
        }
    }
    /* Ordena o array de voos com o mesmo aeroporto de chegada com base na
     * nova data e hora, e imprime para o stdout */
    ordena_voos(mesma_chegada, i, c);
    lista_voos(mesma_chegada, i, c);
}


/*-------------------------------- Comando t ---------------------------------*/

/* O comando "t" avança a data do sistema para a data fornecida. */
void t_command() {
    Data data_aux;
    /* Leitura e verificação da data fornecida */
    scanf("%d-%d-%d", &data_aux.dia, &data_aux.mes, &data_aux.ano);
    consome_linha();
    if (compara_datas(data_aux, data_sistema) == -1) {
        printf("invalid date\n");
        return;
    }
    /* Se a data fornecida corresponder ao esperado, altera-a e imprime-a */
    data_sistema.dia = data_aux.dia;
    data_sistema.mes = data_aux.mes;
    data_sistema.ano = data_aux.ano;
    printf("%.2d-%.2d-%.2d\n", data_sistema.dia, data_sistema.mes,
           data_sistema.ano);
    return;
}


/*-------------------------------- Comando r ---------------------------------*/

/* Se o comando "r" apenas receber dois argumentos, código de voo e data,
 * lista todas as reservas do voo que tem esse mesmo código e parte nessa
 * data; caso contrário, adiciona uma reserva com um código e número de
 * passageiros fornecidos a um voo. */
void r_command() {
    char codigo[CODIGO_VOO];
    Data d;
    Reserva *reserva = (Reserva*) malloc(sizeof(Reserva));
    Voo *v;
    /* Verifica se a memória foi excedida */
    if (!reserva)
        termina_programa();
    scanf("%s", codigo);
    scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
    v = obtem_voo_dia(codigo, d);
    /* Lista as reservas de um voo */
    if (getchar() == '\n') {
        if (verifica_codigo_data(codigo, d, v)) {
            lista_reservas(v);
        }
    }
    /* Adiciona uma reserva */
    else {
        obtem_codigo_reserva(reserva);
        scanf("%d", &reserva->num_pass);
        consome_linha();
        if (validar_reserva(reserva, v, codigo)) {
            v->lotacao += reserva->num_pass;
            push_reserva(&v->head_reservas, reserva);
        }
        else
            free(reserva);
        }
}


/*-------------------------------- Comando e ---------------------------------*/

/* O comando "e" elimina todos os voos com o código fornecido, se o código
 * fornecido for de voo, ou a reserva com o código fornecido, se este for de
 * reserva. */
void e_command() {
    char codigo_inp[MAX_INPUT];
    int change = FALSE;
    scanf("%s", codigo_inp);
    /* Elimina uma reserva */
    if (strlen(codigo_inp)+1 > CODIGO_VOO) {
        destroi_reserva(codigo_inp);
    }
    /* Procura na linked list global de voos todos os voos com o código
     * fornecidoe elimina-os junto com as suas reservas */
    else {
        change = destroi_voos(codigo_inp);
        if (change == FALSE)
            printf("not found\n");
    }
}


/*------------------------- Funções auxiliares gerais ------------------------*/

/* Consome uma linha até ao seu final. */
void consome_linha() {
    while (getchar() != '\n');
}


/* Verifica se o caracter fornecido é uma letra. Devolve 1 se for, e 0 em
 * caso contrário. */
int is_upper(char c) {
    return (c >= 'A' && c <= 'Z');
}


/* Verifica se o caracter fornecido é uma letra. Devolve 1 se for, e 0 em
 * caso contrário. */
int eh_letra(char c) {
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }
    if (c >= 'a' && c <= 'z') {
        return 1;
    }
    return 0;
}


/* Verifica se o caracter fornecido é um dígito. Devolve 1 se for, e 0 em
 * caso contrário. */
int is_digit(char c) {
    return (c >= '0' && c <= '9');
}


/* Esta função é invocada quando a memória foi excedida, e termina o programa,
 * libertando a memória e imprimindo a mensagem de erro adequada, junto do
 * código de erro adequado. */
void termina_programa() {
    printf("No memory.\n");
    destroi_todos_voos();
    exit(137);
}


/*--------------------- Funções auxiliares de aeroportos ---------------------*/

/* Esta função obtém a cidade associada a um aeroporto, lendo a linha até ao
 * seu final. */
void obtem_cidade_aeroporto(char cidade[]) {
    char c = getchar();
    int i = 0;
    while (c == ' ' || c == '\t')
        c = getchar();
    while (c != '\n') {
        cidade[i++] = c;
        c = getchar();
    }
    cidade[i] = '\0';
}


/* Verifica se o ID de um aeroporto é uma string apenas com maiúsculas.
 * Se sim, devolve 1; em caso contrário, devolve 0. */
int verifica_id_aeroporto(char id[]) {
    int i;
    for (i = 0; i < ID_AEROPORTO - 1; i++) {
        if (!is_upper(id[i])) {
            return 0;
        }
    }
    return 1;
}


/* Esta função verifica todos os erros associados a um aeroporto; se ocorrer
 * algum, imprime a respetiva mensagem de erro e retorna 0; senão, o voo é
 * válido e devolve 1. */
int valida_aeroporto(Aeroporto a) {
    if (!verifica_id_aeroporto(a.id)) {
        printf("invalid airport ID\n");
        return 0;
    }
    else if (qtd_aeroportos >= MAX_AEROPORTOS) {
        printf("too many airports\n");
        return 0;
    }
    else if (verifica_aeroporto_existe(a.id)) {
        printf("duplicate airport\n");
        return 0;
    }
    return 1;
}


/* Esta função verifica se um aeroporto existe no array global de
 * aeroportos. Retorna 1 se existir e 0 em caso contrário. */
int verifica_aeroporto_existe(char id_verif[]) {
    int j;
    for (j = 0; j < qtd_aeroportos; j++) {
        if (!strcmp(aeroportos[j].id, id_verif)) {
            return 1;
        }
    }
    return 0;
}


/* Função que ordena o array global de aeroportos por ordem alfabética de IDs,
 * com base no algoritmo bubble sort. */
void ordena_aeroportos() {
    int i, j;
    for (i = 0; i < qtd_aeroportos; i++) {
        for (j = 0; j < qtd_aeroportos-1-i; j++) {
            if (strcmp(aeroportos[j].id, aeroportos[j + 1].id) > 0) {
                TROCA_AER(aeroportos[j], aeroportos[j + 1]);
            }
        }
    }
}


/* Esta função pecorre o array global de aeroportos, previamente ordenado, e
 * imprime um aeroporto por linha. */
void imprime_aeroportos() {
    int i;
    for (i = 0; i < qtd_aeroportos; i++) {
        printf("%s %s %s %d\n", aeroportos[i].id, aeroportos[i].cidade,
               aeroportos[i].pais, aeroportos[i].n_voos);
    }
}

/* Esta função percorre o array global de aeroportos e, se encontrar o
 * aeroporto com o mesmo ID que o fornecido, imprime-o para o stdout e devolve
 * 1; caso contrário, devolve 0. */
int encontra_imprime_aeroporto(char id_aer[]) {
    int i;
    for (i = 0; i < qtd_aeroportos; i++) {
        if (!strcmp(aeroportos[i].id, id_aer)) {
            printf("%s %s %s %d\n", aeroportos[i].id,
                   aeroportos[i].cidade, aeroportos[i].pais,
                   aeroportos[i].n_voos);
            return 1;
        }
    }
    return 0;
}

/*------------------------ Funções auxiliares de voos ------------------------*/

/* Esta função obtém o número presente num código de voo, e devolve 1 se o
 * número estiver compreendido entre 1 e 9999, como suposto, e 0 em caso
 * contrário. */
int verifica_codigo_voo(char string[], int size) {
    int i;
    for (i = 2; i < size-1; i++) {
        if ((!eh_letra(string[i])) && (!is_digit(string[i])))
            return 0;
    }
    return 1;
}


/* Esta função procura na lista global de voos por um voo com o código igual
 * ao fornecido, e que exista no mesmo dia; se tiver sucesso, retorna um
 * ponteiro para o voo, caso contrário, o ponteiro que retorna é NULL. */
Voo* obtem_voo_dia(char codigo[], Data d) {
    v_node *aux = head_voos;
    for (; aux != NULL; aux = aux->next) {
        if (!strcmp(aux->voo->codigo, codigo)) {
            if (aux->voo->data_partida.dia == d.dia &&
            aux->voo->data_partida.mes == d.mes && 
            aux->voo->data_partida.ano == d.ano) {
                return aux->voo;
            }
        }
    }
    return NULL;
}


/* Verifica se existe um voo na data fornecida. Devolve 1 em caso afirmativo,
 * e 0 em caso contrário. */
int verifica_codigo_data(char codigo[], Data d, Voo *v) {
    if (v == NULL) {
        printf("%s: flight does not exist\n", codigo);
        return 0;
    }
    if (compara_datas(d, data_sistema) == -1) {
        printf("invalid date\n");
        return 0;
    }
    return 1;
}


/* Verifica todos os erros associados a um voo, imprimindo a respetiva
 * mensagem de erro se algum ocorrer e devolvendo 0 nesse caso. Caso contrário,
 * o voo é válido e devolve 1. */
int validar_voo(Voo *v) {
    int len = strlen(v->codigo);
    if ((!eh_letra(v->codigo[0])) || (!eh_letra(v->codigo[1])) ||
        !verifica_codigo_voo(v->codigo, len))
        printf("invalid flight code\n");
    else if (obtem_voo_dia(v->codigo, v->data_partida))
        printf("flight already exists\n");
    else if (!verifica_aeroporto_existe(v->id_chegada))
        printf("%s: no such airport ID\n", v->id_chegada);
    else if (!verifica_aeroporto_existe(v->id_partida))
        printf("%s: no such airport ID\n", v->id_partida);
    else if (qtd_voos >= MAX_VOOS)
        printf("too many flights\n");
    else if (compara_datas(v->data_partida, data_sistema) == -1)
        printf("invalid date\n");
    else if (v->duracao.horas * 60 + v->duracao.minutos > DURACAO_MAXIMA)
        printf("invalid duration\n");
    else if (v->capacidade < MIN_PASSAGEIROS)
        printf("invalid capacity\n");
    else {
        return 1;
    }
    return 0;
}


/* Esta função imprime todos os voos, um por linha, pela ordem
 * de criação. */
void imprime_voos() {
    v_node *aux = head_voos;
    for (; aux != NULL; aux = aux-> next)
        printf("%s %s %s %.2d-%.2d-%.2d %.2d:%.2d\n", aux->voo->codigo,
               aux->voo->id_partida, aux->voo->id_chegada,
               aux->voo->data_partida.dia, aux->voo->data_partida.mes,
               aux->voo->data_partida.ano, aux->voo->hora_partida.horas,
               aux->voo->hora_partida.minutos);
}


/* Aumenta em 1 o parâmetro n_voos do aeroporto de partida e o número
 * de voos global. */
void aumenta_voos(char id[]) {
    int i;
    for (i = 0; i < qtd_aeroportos; i++) {
        if (!strcmp(aeroportos[i].id, id)) {
            aeroportos[i].n_voos++;
        }
    }
    qtd_voos++;
}


/* Esta função ordena uma lista fornecida de voos por data e hora ascendente,
 * ou seja, da data e hora mais antiga para a mais recente, com base no
 * algoritmo bubble sort; se for fornecido o caracter 'p', ordenado com
 * base na hora e data de partida; senão, se for fornecido o caracter 'c',
 * ordenado com base na hora e data de chegada. */
void ordena_voos(Voo* lst[], int size, char c) {
    int i, j;
    /* Ordena um array de voos com o mesmo aeroporto de partida */
    if (c == 'p') {
        for (i = 0; i < size - 1; i++) {
            for (j = 0; j < size - 1 - i; j++) {
                if ((compara_datas(lst[j]->data_partida,
                                   lst[j + 1]->data_partida) == 2))  {
                    TROCA_VOO(lst[j], lst[j + 1]);
                }
                else if ((compara_datas(lst[j]->data_partida,
                                        lst[j + 1]->data_partida) == 0)) {
                    if (compara_horas(lst[j]->hora_partida,
                                      lst[j + 1]->hora_partida) == 1) {
                        TROCA_VOO(lst[j], lst[j + 1]);
                    }
                }
            }
        }
    }
        /* Ordena um array de voos com o mesmo aeroporto de chegada */
    else if (c == 'c') {
        for (i = 0; i < size - 1; i++) {
            for (j = 0; j < size - 1 - i; j++) {
                if ((compara_datas(lst[j]->data_chegada,
                                   lst[j + 1]->data_chegada) == 2)) {
                    TROCA_VOO(lst[j], lst[j + 1]);
                } else if ((compara_datas(lst[j]->data_chegada,
                                          lst[j + 1]->data_chegada) == 0)) {
                    if (compara_horas(lst[j]->hora_chegada,
                                      lst[j + 1]->hora_chegada) == 1) {
                        TROCA_VOO(lst[j], lst[j + 1]);
                    }
                }
            }
        }
    }
}


/* Esta função lista uma quantidade "size" de voos fornecidos através do
 * array de aeroportos lst[]; o caracter c é utilizado para saber se foi
 * o comando c ou p que requisitou esta função, uma vez que os outputs são
 * diferentes. */
void lista_voos(Voo* voos[], int size, char c) {
    int i;
    if (c == 'p') {
        for (i = 0; i < size; i++) {
            printf("%s %s %.2d-%.2d-%.2d %.2d:%.2d\n", voos[i]->codigo,
                   voos[i]->id_chegada, voos[i]->data_partida.dia,
                   voos[i]->data_partida.mes, voos[i]->data_partida.ano,
                   voos[i]->hora_partida.horas, voos[i]->hora_partida.minutos);
        }
    }
    else if (c == 'c') {
        for (i = 0; i < size; i++) {
            if (compara_datas(voos[i]->data_chegada, data_sistema) == 2 ||
                compara_datas(voos[i]->data_chegada, data_sistema) == 0) {
                printf("%s %s %.2d-%.2d-%.2d %.2d:%.2d\n", voos[i]->codigo,
                       voos[i]->id_partida, voos[i]->data_chegada.dia,
                       voos[i]->data_chegada.mes, voos[i]->data_chegada.ano,
                       voos[i]->hora_chegada.horas,
                       voos[i]->hora_chegada.minutos);
            }
        }
    }
}


/*------------------- Funções auxiliares de tempo e datas --------------------*/

/* Esta função converte uma data para um número inteiro, para facilitar a
 * comparação de datas. Note-se que, com base na conversão feita por função,
 * se a diferença entre uma data fornecida e a data do sistema for menor
 * que 0, esta é do passado, e se for maior que 10000, esta é de mais de um
 * ano no futuro. */
int data_para_inteiro(Data d) {
    int res;
    res = d.ano * 10000 + d.mes * 100 + d.dia;
    return res;
}


/* Esta função compara duas datas d1 e d2; se d1 for uma data inválida em
 * relação a d2 (mais antiga ou mais de um ano no futuro), retorna -1;
 * se d1 for uma data mais antiga que d2, retorna 1;  se d2 for uma data mais
 * antiga que d1, retorna 2; se as datas forem iguais, retorna 0. */
int compara_datas(Data d1, Data d2) {
    int num1, num2;
    num1 = data_para_inteiro(d1);
    num2 = data_para_inteiro(d2);
    if (num1 < num2 || num1 - num2 > 10000)
        return -1;
    else if (num2 < num1)
        return 2;
    else
        return 0;
}


/* Esta função converte uma hora para um número inteiro, para comparar
 * facilmente duas horas. */
int hora_para_inteiro(Hora h1) {
    int res;
    res = h1.horas * 100 + h1.minutos;
    return res;
}


/* Esta função compara duas horas (h1 e h2) fornecidas como argumento; se
 * h1 for mais antiga que h2, devolve 0; se h2 for mais antiga que h1,
 * devolve 1; no caso de serem semelhantes, devolve 2. */
int compara_horas(Hora h1, Hora h2) {
    int num1, num2;
    num1 = hora_para_inteiro(h1);
    num2 = hora_para_inteiro(h2);
    if (num1 < num2)
        return 0;
    else if (num2 < num1)
        return 1;
    return 2;
}

/* Fornecido um ponteiro para um voo, a função obtém a sua data e hora de
 * chegada com base na hora de partida, duração e data de partida */
void obtem_hora_data_chegada(Voo* v) {
    /* Obtém a hora de chegada através da de partida e da duração */
    v->hora_chegada.horas = v->hora_partida.horas + v->duracao.horas;
    v->hora_chegada.minutos = v->hora_partida.minutos + v->duracao.minutos;
    v->data_chegada = v->data_partida;
    if (v->hora_chegada.minutos >= 60) {
        v->hora_chegada.horas++;
        v->hora_chegada.minutos -= 60;
    }
    /* Obtém a data de chegada */
    if (v->hora_chegada.horas >= 24) {
        v->hora_chegada.horas -= 24;
        v->data_chegada = obtem_data_chegada(v->data_chegada);
    }
}


/* A função recebe uma data d1, que vai incrementar em um dia, e com base nessa
 * alteração verifica e avança se necessário o mês e ano. */
Data obtem_data_chegada(Data d) {
    int dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    d.dia++;
    if (d.mes == 12 && d.dia > 31) {
        d.dia = 1;
        d.mes = 1;
        d.ano++;
    }
    else if (d.dia > dias_mes[d.mes-1]) {
        d.dia = 1;
        d.mes++;
    }
    return d;
}


/*---------------------- Funções auxiliares de reservas ----------------------*/

/* A partir do stdin, obtém um código de reserva e copia-o para a reserva
 * fornecida como argumento. */
void obtem_codigo_reserva(Reserva *reserva) {
    char inp[MAX_INPUT], *cod_res;
    scanf("%s", inp);
    cod_res = (char*) malloc(sizeof(char)*(strlen(inp)+1));
    if (!cod_res)
        termina_programa();
    strcpy(cod_res, inp);
    reserva->codigo = cod_res;
}


/* Verifica um código de reserva fornecido. Devolve 1 se for válido, e 0 em
 * caso contrário. */
int verifica_codigo_reserva(char *codigo, int size) {
    int i;
    if (size < MIN_COD_RESERVA)
        return 0;
    for (i = 0; i < size; i++) {
        if (!is_digit(codigo[i]) && !is_upper(codigo[i])) {
            return 0;
        }
    }
    return 1;
}


/* Esta função verifica se uma reserva com o código fornecido como argumento
 * já se encontra presente na lista de reservas cuja cabeça é também
 * fornecida como argumento. */
int codigo_reserva_ja_existe(char *cod_res) {
    v_node *aux = head_voos;
    r_node *r_aux;
    for (; aux != NULL; aux = aux->next) {
        for (r_aux = aux->voo->head_reservas; r_aux != NULL;
             r_aux = r_aux->next) {
            if (!strcmp(r_aux->reserva->codigo, cod_res)) {
                return 1;
            }
        }
    }
    return 0;
}


/* Esta função verifica toda os erros associados a uma reserva, gerando a
 * respetiva mensagem de erro se necessário. Se ocorrer algum erro, devolve 0;
 * senão, a reserva é válida e devolve 1. */
int validar_reserva(Reserva *reserva, Voo *v, char *codigo) {
    int size = strlen(reserva->codigo);
    if (!verifica_codigo_reserva(reserva->codigo, size)) {
        printf("invalid reservation code\n");
        return 0;
    }
    else if (v == NULL) {
        printf("%s: flight does not exist\n", codigo);
        return 0;
    }
    else if (codigo_reserva_ja_existe(reserva->codigo)) {
        printf("%s: flight reservation already used\n", reserva->codigo);
        return 0;
    }
    else if (reserva->num_pass + v->lotacao > v->capacidade) {
        printf("too many reservations\n");
        return 0;
    }
    else if (compara_datas(v->data_partida, data_sistema) == -1) {
        printf("invalid date\n");
        return 0;
    }
    else if (reserva->num_pass < PASSAGEIROS_RESERVA) {
        printf("invalid passenger number\n");
        return 0;
    }
    return 1;
}


/* Esta função dá print num código de reserva e o respetivo número de
 * passageiros, um por linha. A lista está previamente ordenada. */
void lista_reservas(Voo *v) {
    r_node *aux = v->head_reservas;
    for (;aux != NULL; aux = aux->next)
        printf("%s %d\n", aux->reserva->codigo, aux->reserva->num_pass);
}


/*---------------------------- Funções para nodes ----------------------------*/

/* Adiciona um voo fornecido como argumento à linked list global de voos. */
void push_voo(Voo *v) {
    v_node *new = (v_node*) malloc(sizeof(v_node));
    /* Verifica se a memória é excedida */
    if (!new)
        termina_programa();
    /* Inicializar o voo */
    new->voo = v;
    new->next = NULL;
    new->voo->head_reservas = NULL;
    /* Se a head dos voos for NULL, insere-se o voo no início da lista */
    if (head_voos == NULL) {
        head_voos = new;
        last_voos = head_voos;
        last_voos->next = NULL;
        return;
    }
    /* Se a lista tiver mais que um elemento, insere o novo voo após o último */
    else {
        last_voos->next = new;
        last_voos = new;
        last_voos->next = NULL;
        return;
    }
}


/* Destrói os voos da linked list global que têm o código igual ao fornecido
 * por argumento. */
int destroi_voos(char cod[]) {
    v_node *cur = head_voos, *prev = NULL, *temp;
    int change = FALSE;
    while (cur != NULL) {
        if (!strcmp(cur->voo->codigo, cod) &&
        compara_datas(cur->voo->data_chegada, data_sistema) == 2) {
            /* Se o voo a eliminar estiver na primeira posição, para apagar,
             * altera a cabeça da lista de acordo */
            if (prev == NULL) {
                temp = cur;
                head_voos = cur->next;
                cur = cur->next;
                destroi_reservas(temp->voo->head_reservas);
                free(temp);
            }
            /* Apaga um voo que não esteja na primeira posição */
            else {
                temp = cur;
                prev->next = cur->next;
                prev = cur;
                cur = cur->next;
                destroi_reservas(temp->voo->head_reservas);
                free(temp);
            }
            change = TRUE;
        }
        /* Se o strcmp não for bem sucedido, avança para o próximo voo */
        else {
            prev = cur;
            cur = cur->next;
        }
    }
    return change;
}


/* Destrói a linked list global de voos, libertando toda a memória associada. */
void destroi_todos_voos() {
    v_node *cur = head_voos, *temp;
    while (cur != NULL) {
        temp = cur;
        cur = cur->next;
        destroi_reservas(temp->voo->head_reservas);
        free(temp);
    }
}


/* Adiciona uma reserva, fornecida como argumento, à linked list de reservas
 * também fornecida como argumento. */
void push_reserva(r_node **head, Reserva *r) {
    r_node *prev, *cur = *head, *new = (r_node*) malloc(sizeof(r_node));
    if (!new)
        termina_programa();
    new->reserva = r;
    /* Se a lista de reservas estiver vazia, insere-se no início */
    if (cur == NULL) {
        *head = new;
        (*head)->next = NULL;
        return;
    }
    /* Verifica o primeiro node; se for para inserir antes deste, tem de se
     * alterar a cabeça */
    if (strcmp(cur->reserva->codigo, r->codigo) > 0) {
        new->next = *head;
        *head = new;
        return;
    }
    /* Verificação de todos os nodes exceto o primeiro */
    for (;cur != NULL; cur = cur->next) {
        if (strcmp(cur->reserva->codigo, r->codigo) > 0) {
            prev->next = new;
            new->next = cur;
            return;
        }
        prev = cur;
    }
    /* Chegando aqui, o elemento a inserir é maior que todos na lista, portanto
     * insere-se no fim; cur está a NULL e prev é o último elemento da lista */
    prev->next = new;
    new->next = NULL;
    return;
}


/* Encontra e destrói uma reserva cujo código é fornecida como argumento. */
void destroi_reserva(char *codigo_res) {
    r_node *prev = NULL, *cur, *temp;
    v_node *aux = head_voos;
    for (; aux != NULL; aux = aux->next) {
        /* Se a cabeça das reservas de um voo apontar para NULL, avança
         * para o próximo voo*/
        if (aux->voo->head_reservas != NULL) {
            /* Verifica se a reserva a retirar se encontra na cabeça da lista,
             * para remover e ligar a lista propriamente */
            if (!strcmp(aux->voo->head_reservas->reserva->codigo, codigo_res)) {
                temp = aux->voo->head_reservas;
                aux->voo->head_reservas = aux->voo->head_reservas->next;
                aux->voo->lotacao -= temp->reserva->num_pass;
                free(temp);
                return;
            }
            /* Verifica todas as reservas presentes na lista
             * exceto a primeira */
            prev = aux->voo->head_reservas;
            cur = prev->next;
            while (cur != NULL) {
                if (!strcmp(cur->reserva->codigo, codigo_res)) {
                    prev->next = cur->next;
                    aux->voo->lotacao -= cur->reserva->num_pass;
                    free(cur);
                    return;
                }
                prev = cur;
                cur = cur->next;
            }
        }
    }
    /* Se a função chegar aqui, significa que a reserva a apagar não
     * foi encontrada */
    printf("not found\n");
}


/* Destrói uma linked list de reservas, cuja cabeça é fornecida como
 * argumento. */
void destroi_reservas(r_node *head) {
    r_node *aux;
    while(head != NULL) {
        aux = head->next;
        free(head);
        head = aux;
    }
}


/*--------------------------- Fim do ficheiro --------------------------------*/
