//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>



// LISTA DE PALAVRAS RESERVADAS


#define PalavraReservada_main 1
#define PalavraReservada_if 2
#define PalavraReservada_else 3
#define PalavraReservada_for 4
#define PalavraReservada_do 5
#define PalavraReservada_while 6
#define PalavraReservada_int 7
#define PalavraReservada_float 8
#define PalavraReservada_char 9
#define PalavraReservada_abre_parent 10
#define PalavraReservada_fecha_parent 11
#define PalavraReservada_abre_chaves 12
#define PalavraReservada_fecha_chaves 13
#define PalavraReservada_soma 14
#define PalavraReservada_sub 15
#define PalavraReservada_multi 16
#define PalavraReservada_div 17
#define PalavraReservada_atribuicao 18
#define PalavraReservada_igualdade 19
#define PalavraReservada_virgula 20
#define PalavraReservada_ponto_virg 21
#define PalavraReservada_maior 22
#define PalavraReservada_menor 23
#define PalavraReservada_maior_igual 24
#define PalavraReservada_menor_igual 25
#define PalavraReservada_diferente 26
#define Tipo_int 27
#define Tipo_float 28
#define Tipo_char 29
#define ID 30

typedef struct Token {
	char lexema[300];
	int tipo;
}Struct_Token;


Struct_Token Scanner(FILE *arq);
int RetornaTipoPalavra(char palavra[]);


//VARIAVEIS GLOBAIS
int linha = 1;
int coluna = 1;
Struct_Token lexm;

int main(int argc, char *argv[]) { // lembrar de colocar por arg na linha de comando
	Struct_Token tk;
	FILE *arq;
	//char nomeArquivo[15] = { "teste1.txt" };
	arq = fopen(argv[1], "r");

	while (!feof(arq)) {
		tk = Scanner(arq);
		/*if (tk.tipo != -1) {
			printf("\n%s - %d ", lexm.lexema, lexm.tipo);
		}*/
	}

	fclose(arq);
	return 0;
}

Struct_Token Scanner(FILE *arq) {
	int i = 0;
	char caractere = getc(arq);
	int condicaoWhile = 0;
	int loop_asterisco = 0;


	// Limpa o token atual
	lexm.lexema[0] = '\0';
	lexm.tipo = -1;

	while (condicaoWhile == 0) {

		if (isalpha(caractere) != 0 || caractere == '_') {  // se caractere for letra, eh !=0 ou pode come�ar com _ (underline)
			lexm.lexema[i] = caractere;
			caractere = getc(arq);
			coluna++;
			i++;
			while (isalnum(caractere) != 0 || isalpha(caractere) != 0 || caractere == '_') {  // se a cond acima passar, inicia com letra e agr vai pegando o resto do token
				lexm.lexema[i] = caractere;
				coluna++;
				i++;
				caractere = getc(arq);
			}
			lexm.lexema[i] = '\0';
			if (!feof(arq)) {
				fseek(arq, -1, SEEK_CUR); // quando tem 2 tokens colados um no outro. para processar o 2�
			}
			lexm.tipo = RetornaTipoPalavra(lexm.lexema);
			return lexm;
		}

		else if (isdigit(caractere) != 0) { // se caractere for digito, eh !=0
			lexm.lexema[i] = caractere;
			coluna++;
			i++;
			caractere = getc(arq);
			// se a condicao acima passar, eh numero, agr verifica se o resto do token ainda eh numero
			while (isdigit(caractere) != 0) {
				lexm.lexema[i] = caractere;
				coluna++;
				i++;
				caractere = getc(arq);
			}

			if (caractere == '.') { // se ele achar um ponto, obrigatoriamente o resto tem q ser numero (float). se nao, erro.
				lexm.lexema[i] = caractere;
				i++;
				coluna++;
				caractere = getc(arq);
				lexm.tipo = Tipo_float;
				if (isdigit(caractere) != 0) { // verifica se o primeiro caractere apos o ponto eh numero, se for, coluna++ e incrementa i
					lexm.lexema[i] = caractere;
					coluna++;
					i++;
					caractere = getc(arq);
					while (isdigit(caractere) != 0) { // loop pra entrar quantos numeros quiser apos o ponto do float
						lexm.lexema[i] = caractere;
						coluna++;
						i++;
						caractere = getc(arq);
					}
					lexm.lexema[i] = '\0';
					return lexm;
				}
				else {  // erro de float mal formado
					lexm.lexema[i] = '\0';
					lexm.tipo = -1;
					printf("Erro de float mal formado na linha %d e coluna %d e ultimo token lido: %s\n", linha, coluna, lexm.lexema);
					fclose(arq);
					exit(0);
				}
			}
			else {  // n�o � num float, ent�o � int
				fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posi��o na leitura do caractere. fseek volta 1 pos
				lexm.tipo = Tipo_int;
				lexm.lexema[i] = '\0';
				return lexm;
			}
		}

		else if (caractere == '.') { // se come�ar com um ponto, obrigatoriamente o resto tem q ser numero float. se nao, erro.
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			caractere = getc(arq);
			lexm.tipo = Tipo_float;
			if (isdigit(caractere) != 0) { // verifica se o primeiro caractere apos o ponto eh numero, se for, coluna++ e incrementa i
				lexm.lexema[i] = caractere;
				coluna++;
				i++;
				caractere = getc(arq);
				while (isdigit(caractere) != 0) { // loop pra entrar quantos numeros quiser apos o ponto do float
					lexm.lexema[i] = caractere;
					coluna++;
					i++;
					caractere = getc(arq);
				}
				lexm.lexema[i] = '\0';
				return lexm;

			}
			else {  // erro do float mal formado
				lexm.tipo = -1;
				lexm.lexema[i] = '\0';
				printf("Erro de float mal formado na linha %d e coluna %d e ultimo token lido: %s\n", linha, coluna, lexm.lexema);
				fclose(arq);
				exit(0);
			}
		}

		else if (caractere == '=') { // verifica se � igualdade ou atribui��o
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			caractere = getc(arq);
			lexm.tipo = PalavraReservada_atribuicao;
			if (caractere == '=') {
				lexm.lexema[i] = caractere;
				i++;
				coluna++;
				caractere = getc(arq);
				lexm.tipo = PalavraReservada_igualdade;
			}
			fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posi��o na leitura do caractere. fseek volta 1 pos
			lexm.lexema[i] = '\0';
			return lexm;
		}

		else if (caractere == '>') { // verifica se � > ou >=
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			caractere = getc(arq);
			
			if (caractere == '=') {
				lexm.lexema[i] = caractere;
				i++;
				coluna++;
				caractere = getc(arq);
				lexm.tipo = PalavraReservada_maior_igual;
			}

			else {
				fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posi��o na leitura do caractere. fseek volta 1 pos
			}

			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_maior;
			return lexm;
		}

		else if (caractere == '<') { // verifica se � < ou <=
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			caractere = getc(arq);
			//lexm.tipo = PalavraReservada_menor;
			if (caractere == '=') {
				lexm.lexema[i] = caractere;
				i++;
				coluna++;
				caractere = getc(arq);
				lexm.tipo = PalavraReservada_menor_igual;
			}
			else {
				fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posi��o na leitura do caractere. fseek volta 1 pos
			}
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_menor;
			return lexm;
		}

		else if (caractere == '(') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_abre_parent;
			return lexm;
		}

		else if (caractere == ')') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_fecha_parent;
			return lexm;
		}

		else if (caractere == '{') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_abre_chaves;
			return lexm;
		}

		else if (caractere == '}') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_fecha_chaves;
			return lexm;
		}

		else if (caractere == '!') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			caractere = getc(arq);
			lexm.lexema[i] = '\0';
			if (caractere == '=') {
				lexm.lexema[i] = caractere;
				i++;
				coluna++;
				lexm.lexema[i] = '\0';
				lexm.tipo = PalavraReservada_diferente;
				return lexm;
			}
			else {  // erro de excl
				printf("Erro da exclamacao na linha %d e coluna %d. Ultimo token lido: %s\n", linha, coluna, lexm.lexema);
				fclose(arq);
				exit(0);
			}
		}

		//comentario
		else if (caractere == '/') { // verifica a primeira ocasiao do /
			coluna++;
			caractere = getc(arq);
			if (caractere == '/') { // comentario de linha 
				coluna++;
				caractere = getc(arq);
				while (caractere != '\n' && (!feof(arq))) {  // quebra o comentario de linha com \n
					coluna++;
					caractere = getc(arq);
				}

				if (caractere == '\n') {
					//i = 0;  // zera o contador;
					coluna = 1;
					linha++;
					caractere = getc(arq);
				}
			} // fim do if segundo /

			else if (caractere == '*') {  // verifica comentario multilinha
				coluna++;
				caractere = getc(arq);
				while (loop_asterisco == 0) {
					while (caractere != '*') {  // verifica termino do comentario multilinha
						coluna++;
						caractere = getc(arq);
						if (caractere == '\n') {
							linha++;
							coluna = 1;
							caractere = getc(arq);
						}
						else if (caractere == '\t') {
							coluna = coluna + 4;
							caractere = getc(arq);
						}
						else if (feof(arq) != 0) {
							lexm.lexema[i] = '\0';
							printf("Erro de fim de arquivo no meio do comentario na linha %d e coluna %d.\n", linha, coluna);
							fclose(arq);
							exit(0);
						}
					}

					if (caractere == '*') { // testa o * do fim do comentario multilinha
						coluna++;
						caractere = getc(arq);
						if (caractere == '/') { // ver se o prox caractere � um / pra finalizar o comentario
							coluna++;
							i++;
							lexm.lexema[i] = '\0';  // rever aqui
							loop_asterisco = 1;
							condicaoWhile = 0;
							caractere = getc(arq);
							if (!feof(arq)) {
								fseek(arq, -1, SEEK_CUR); // quando tem 2 tokens colados um no outro. para processar o 2�
							}
						}
						else if (feof(arq) != 0) {  // testa se for fim de arquivo
							printf("Erro de fim de arquivo no meio do comentario na linha %d e coluna %d\n", linha, coluna);
							fclose(arq);
							exit(0);
						}
						else {
							coluna++;
							loop_asterisco = 0;
						}
					}
				}  // FIM LOOP ASTERISCO
			}

			else { // se n�o for nenhum dos casos acima, � divis�o
				lexm.lexema[i] = '/';
				i++;
				lexm.tipo = PalavraReservada_div;
				lexm.lexema[i] = '\0';
				if (!feof(arq)) {
					fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posi��o na leitura do caractere. fseek volta 1 pos
				}
				return lexm;
			}
		}

		else if (caractere == '+') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_soma;
			return lexm;
		}

		else if (caractere == '-') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_sub;
			return lexm;
		}

		else if (caractere == '*') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_multi;
			return lexm;
		}

		else if (caractere == ',') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_virgula;
			return lexm;
		}

		else if (caractere == ';') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_ponto_virg;
			return lexm;
		}

		else if (caractere == '\n') {
			linha++;
			coluna = 1;
			caractere = getc(arq);
		}

		else if (caractere == ' ') {
			coluna++;
			caractere = getc(arq);
			condicaoWhile = 0;  // volta para o inicio da fun��o para verificar os proximos caracteres
		}

		else if (caractere == '\t') { // tab: coluna+4 e linha permanece  e colocar flag
			coluna = coluna + 4;
			caractere = getc(arq);
			condicaoWhile = 0;
		}

		else if (caractere == 39) { // aspas simples
			coluna++;
			caractere = getc(arq);
			if (isalnum(caractere) != 0) {
				lexm.lexema[i] = caractere;
				coluna++;
				i++;
				lexm.lexema[i] = '\0';
				caractere = getc(arq);
				lexm.tipo = Tipo_char;
				if (caractere == 39) {
					coluna++;
					lexm.lexema[i] = '\0';
					return lexm;
				}
				else {
					lexm.lexema[i] = '\0';
					printf("Erro de caractere mal formado na linha %d e coluna %d. Ultimo token lido: %s\n", linha, coluna, lexm.lexema);
					fclose(arq);
					exit(0);
				}
			}
			else {
				lexm.lexema[i] = '\0';
				printf("Erro de caractere mal formado na linha %d e coluna %d. Ultimo token lido: %s\n", linha, coluna, lexm.lexema);
				fclose(arq);
				exit(0);
			}
		}

		else if (caractere == EOF) { //feof(arq)
			lexm.tipo = -1;
			//fclose(arq);
			//exit(0);
			return lexm;
		}

		else { // erro de caractere invalido
			printf("Erro de caractere invalido na linha %d e coluna %d\n", linha, coluna);
			fclose(arq);
			exit(0);
		}
	}
}

int RetornaTipoPalavra(char palavra[]) {
	if (strcmp(palavra, "main") == 0) {
		return PalavraReservada_main;
	}
	else if (strcmp(palavra, "if") == 0) {
		return PalavraReservada_if;
	}
	else if (strcmp(palavra, "else") == 0) {
		return PalavraReservada_else;
	}
	else if (strcmp(palavra, "do") == 0) {
		return PalavraReservada_do;
	}
	else if (strcmp(palavra, "while") == 0) {
		return PalavraReservada_while;
	}
	else if (strcmp(palavra, "for") == 0) {
		return PalavraReservada_for;
	}
	else if (strcmp(palavra, "int") == 0) {
		return PalavraReservada_int;
	}
	else if (strcmp(palavra, "char") == 0) {
		return PalavraReservada_char;
	}
	else if (strcmp(palavra, "float") == 0) {
		return PalavraReservada_float;
	}
	else {
		return ID;
	}
}