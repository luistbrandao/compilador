//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
SINTAXE DO PARSER

<programa> :: = int main"("")" <bloco>
<bloco> :: = “{ “{ <decl_var> }*{<comando>}*“ }”
<comando> :: = <comando_básico> | <iteração> | if "("<expr_relacional>")" <comando> {else <comando>} ?
<comando_básico> :: = <atribuição> | <bloco>
<iteração> :: = while "("<expr_relacional>")" <comando> | do <comando> while "("<expr_relacional>")"";"
<atribuição> :: = <id> "=" <expr_arit> ";"
<expr_relacional> :: = <expr_arit> <op_relacional> <expr_arit>
<expr_arit> :: = <expr_arit> "+" <termo> | <expr_arit> "-" <termo> | <termo>
<termo> :: = <termo> "*" <fator> | <termo> “ / ” <fator> | <fator>
<fator> :: = “(“ <expr_arit> “)” | <id> | <real> | <inteiro> | <char>

*/


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
void Parser(FILE *arq);
void programa(FILE *arq);
void bloco(FILE *arq);
void declaracao_variavel(FILE *arq);
void Comando(FILE *arq);
void ComandoBasico(FILE *arq);
void expr_relacional(FILE *arq);
void atribuicao(FILE *arq);
void expr_aritm(FILE *arq);
void termo(FILE *arq);
void fator(FILE *arq);
void iteracao(FILE *arq);
void expr_aritm_2(FILE *arq);
void caso_if_else(FILE *arq);


//VARIAVEIS GLOBAIS
int linha = 1;
int coluna = 1;
Struct_Token lexm;
char caractere;

int main(int argc, char *argv[]) { // lembrar de colocar por arg na linha de comando
	FILE *arq;
	//char nomeArquivo[15] = { "teste1.txt" };
	arq = fopen(argv[1], "r");
	caractere = getc(arq);

	if (arq == NULL) {
		printf("Erro ao abrir arquivo!\n");
	}
	else {
		Parser(arq);
	}
	
	/*while (!feof(arq)) {
	printf("\n%s", Scanner(arq).lexema);
	}*/
	
	fclose(arq);
	return 0;
}

Struct_Token Scanner(FILE *arq) {
	int i = 0;
	int condicaoWhile = 0;
	int loop_asterisco = 0;


	// Limpa o token atual
	lexm.lexema[0] = '\0';
	lexm.tipo = -1;

	while (condicaoWhile == 0) {

		if (isalpha(caractere) != 0 || caractere == '_') {  // se caractere for letra, eh !=0 ou pode começar com _ (underline)
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
				//fseek(arq, -1, SEEK_CUR); // quando tem 2 tokens colados um no outro. para processar o 2º
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
					//fseek(arq, -1, SEEK_CUR);
					
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
			else {  // não é num float, então é int
				//fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posição na leitura do caractere. fseek volta 1 pos

				lexm.tipo = Tipo_int;
				lexm.lexema[i] = '\0';
				return lexm;
			}
		}

		else if (caractere == '.') { // se começar com um ponto, obrigatoriamente o resto tem q ser numero float. se nao, erro.
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
				//fseek(arq, -1, SEEK_CUR);
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

		else if (caractere == '=') { // verifica se é igualdade ou atribuição
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
			//fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posição na leitura do caractere. fseek volta 1 pos
		//	caractere = getc(arq);
			lexm.lexema[i] = '\0';
			return lexm;
		}

		else if (caractere == '>') { // verifica se é > ou >=
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			caractere = getc(arq);

			if (caractere == '=') {  // coloquei getc
				lexm.lexema[i] = caractere;
				i++;
				coluna++;
				caractere = getc(arq);
				lexm.tipo = PalavraReservada_maior_igual;
			}
			else {
				lexm.tipo = PalavraReservada_maior;
				//fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posição na leitura do caractere. fseek volta 1 pos
			}
			//caractere = getc(arq);
			lexm.lexema[i] = '\0';
			return lexm;
		}

		else if (caractere == '<') { // verifica se é < ou <=
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			caractere = getc(arq);
			lexm.tipo = PalavraReservada_menor;
			if (caractere == '=') {
				lexm.lexema[i] = caractere;
				i++;
				coluna++;
				caractere = getc(arq);
				lexm.tipo = PalavraReservada_menor_igual;
			}
			else {
				//fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posição na leitura do caractere. fseek volta 1 pos
				lexm.lexema[i] = '\0';
			}
			return lexm;
		}

		else if (caractere == '(') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_abre_parent;
			caractere = getc(arq);
			return lexm;
		}

		else if (caractere == ')') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_fecha_parent;
			caractere = getc(arq);
			return lexm;
		}

		else if (caractere == '{') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_abre_chaves;
			caractere = getc(arq);
			return lexm;
		}

		else if (caractere == '}') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_fecha_chaves;
			caractere = getc(arq);
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
				caractere = getc(arq);
				return lexm;
			}
			else {  // erro de excl
				printf("Erro da exclamacao na linha %d e coluna %d.\n", linha, coluna);
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
					coluna = 1;
					linha++;
					caractere = getc(arq);
				}
			}

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
						if (caractere == '/') { // ver se o prox caractere é um / pra finalizar o comentario
							coluna++;
							//i++;
							lexm.lexema[i] = '\0';  // rever aqui
							loop_asterisco = 1;
							condicaoWhile = 0;
							caractere = getc(arq);
							//if (!feof(arq)) {
							//	fseek(arq, -1, SEEK_CUR); // quando tem 2 tokens colados um no outro. para processar o 2º
							//}
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

			else { // se não for nenhum dos casos acima, é divisão
				lexm.lexema[i] = '/';
				i++;
				lexm.tipo = PalavraReservada_div;
				lexm.lexema[i] = '\0';
				//fseek(arq, -1, SEEK_CUR); // ele estava pulando uma posição na leitura do caractere. fseek volta 1 pos
				return lexm;
			}
		}

		else if (caractere == '+') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_soma;
			caractere = getc(arq);
			return lexm;
		}

		else if (caractere == '-') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_sub;
			caractere = getc(arq);
			return lexm;
		}

		else if (caractere == '*') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_multi;
			caractere = getc(arq);
			return lexm;
		}

		else if (caractere == ',') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_virgula;
			caractere = getc(arq);
			return lexm;
		}

		else if (caractere == ';') {
			lexm.lexema[i] = caractere;
			i++;
			coluna++;
			lexm.lexema[i] = '\0';
			lexm.tipo = PalavraReservada_ponto_virg;
			caractere = getc(arq);
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
			condicaoWhile = 0;  // volta para o inicio da função para verificar os proximos caracteres
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
					caractere = getc(arq);
					return lexm;
				}
				else {
					lexm.lexema[i] = '\0';
					printf("Erro de caractere mal formado na linha %d e coluna %d.\n", linha, coluna);
					fclose(arq);
					exit(0);
				}
			}
			else {
				lexm.lexema[i] = '\0';
				printf("Erro de caractere mal formado na linha %d e coluna %d.\n", linha, coluna);
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

void Parser(FILE *arq) {
	programa(arq);
}

void programa(FILE *arq) {

	lexm = Scanner(arq);
	if (lexm.tipo != PalavraReservada_int) {
		printf("Erro na linha %d e coluna %d. Falta o 'int'\n", linha, coluna);
		fclose(arq);
		exit(0);
	}

	lexm = Scanner(arq);
	if (lexm.tipo != PalavraReservada_main) {
		printf("Erro na linha %d e coluna %d. Falta o 'main'\n", linha, coluna);
		fclose(arq);
		exit(0);
	}

	lexm = Scanner(arq);
	if (lexm.tipo != PalavraReservada_abre_parent) {
		printf("Erro na linha %d e coluna %d. Falta o '('\n", linha, coluna);
		fclose(arq);
		exit(0);
	}

	lexm = Scanner(arq);
	if (lexm.tipo != PalavraReservada_fecha_parent) {
		printf("Erro na linha %d e coluna %d. Falta o ')'\n", linha, coluna);
		fclose(arq);
		exit(0);
	}
	lexm = Scanner(arq);
	bloco(arq);
	if (lexm.tipo != -1) {
		printf("Erro na linha %d e coluna %d. Existencia de codigo apos o '}' final\n", linha, coluna);
		fclose(arq);
		exit(0);
	}
}

void bloco(FILE *arq) {

	if (lexm.tipo == PalavraReservada_abre_chaves) {
		lexm = Scanner(arq);
		while (lexm.tipo == PalavraReservada_int || lexm.tipo == PalavraReservada_float || lexm.tipo == PalavraReservada_char) {
			declaracao_variavel(arq);
		}
		while (lexm.tipo == PalavraReservada_abre_chaves || lexm.tipo == PalavraReservada_if || lexm.tipo == PalavraReservada_do || lexm.tipo == PalavraReservada_while || lexm.tipo == ID) {
			Comando(arq);
		}
		if (lexm.tipo != PalavraReservada_fecha_chaves) {
			printf("Erro na linha %d e coluna %d. Falta um '}'\n", linha, coluna);
			fclose(arq);
			exit(0);
		}

		else {
			lexm = Scanner(arq);
		}
	}
	else {
		printf("Erro na linha %d e coluna %d. Falta um '{'\n", linha, coluna);
		fclose(arq);
		exit(0);
	}
}

void declaracao_variavel(FILE *arq) {

	lexm = Scanner(arq);

	if (lexm.tipo != ID) {
		printf("Erro na linha %d e coluna %d. Falta nome da variavel\n", linha, coluna);
		fclose(arq);
		exit(0);
	}

	lexm = Scanner(arq);
	while (lexm.tipo == PalavraReservada_virgula) { // ver se tem mais de 1 variavel
		lexm = Scanner(arq);
		if (lexm.tipo != ID) {
			printf("Erro na linha %d e coluna %d. Falta variavel na declaracao\n", linha, coluna);
			fclose(arq);
			exit(0);
		}
		lexm = Scanner(arq);
	}

	if (lexm.tipo != PalavraReservada_ponto_virg) {
		printf("Erro na linha %d e coluna %d. Falta um ';'\n", linha, coluna);
		fclose(arq);
		exit(0);
	}
	lexm = Scanner(arq);
}

void Comando(FILE *arq) {
	if (lexm.tipo == ID || lexm.tipo == PalavraReservada_abre_chaves) {
		ComandoBasico(arq);
	}
	else if (lexm.tipo == PalavraReservada_while || lexm.tipo == PalavraReservada_do) {
		iteracao(arq);
	}
	else if (lexm.tipo == PalavraReservada_if) {
		caso_if_else(arq);
	}
}

void ComandoBasico(FILE *arq) {
	if (lexm.tipo == ID) {
		atribuicao(arq);
	}
	else {  // tem que ser um '{'
		bloco(arq);
	}
}

void expr_relacional(FILE *arq) {

	expr_aritm(arq);

	if (lexm.tipo == PalavraReservada_igualdade || lexm.tipo == PalavraReservada_maior || lexm.tipo == PalavraReservada_menor ||
		lexm.tipo == PalavraReservada_maior_igual || lexm.tipo == PalavraReservada_menor_igual || lexm.tipo == PalavraReservada_diferente) {

		lexm = Scanner(arq);
		expr_aritm(arq);
	}
	else {
		printf("Erro na linha %d e coluna %d. Falta algum operador matematico\n", linha, coluna);
		fclose(arq);
		exit(0);
	}
}

void atribuicao(FILE *arq) {

	lexm = Scanner(arq);

	if (lexm.tipo != PalavraReservada_atribuicao) {
		printf("Erro na linha %d e coluna %d. Falta sinal de atribuicao\n", linha, coluna);
		fclose(arq);
		exit(0);
	}
	lexm = Scanner(arq);
	expr_aritm(arq);

	if (lexm.tipo != PalavraReservada_ponto_virg) {
		printf("Erro na linha %d e coluna %d. Falta um ';'\n", linha, coluna);
		fclose(arq);
		exit(0);
	}
	else {
		lexm = Scanner(arq);
	}
}

void expr_aritm(FILE *arq) {
	termo(arq);
	expr_aritm_2(arq);
}

void termo(FILE *arq) {
	fator(arq);
	while (lexm.tipo == PalavraReservada_div || lexm.tipo == PalavraReservada_multi) {
		lexm = Scanner(arq);
		fator(arq);
	}
}

void fator(FILE *arq) {
					
	if (lexm.tipo == PalavraReservada_abre_parent) {
		lexm = Scanner(arq);
		expr_aritm(arq);
		if (lexm.tipo != PalavraReservada_fecha_parent) {
			printf("Erro na linha %d e coluna %d. Falta um ')'", linha, coluna);
			fclose(arq);
			exit(0);
		}
		else {
			lexm = Scanner(arq);
		}
	}
	else if (lexm.tipo == ID || lexm.tipo == Tipo_char || lexm.tipo == Tipo_float || lexm.tipo == Tipo_int) {
		lexm = Scanner(arq);
	}
	else {
		printf("Erro na linha %d e coluna %d. Expressao nao tem fator conhecido\n", linha, coluna);  // rever aqui
		fclose(arq);
		exit(0);
	}
}

void iteracao(FILE *arq) {
	if (lexm.tipo == PalavraReservada_while) {  // CASO DO WHILE
		lexm = Scanner(arq);
		if (lexm.tipo != PalavraReservada_abre_parent) {
			printf("Erro na linha %d e coluna %d. Falta um '('\n", linha, coluna);
			fclose(arq);
			exit(0);
		}
		lexm = Scanner(arq);
		expr_relacional(arq);

		if (lexm.tipo != PalavraReservada_fecha_parent) {
			printf("Erro na linha %d e coluna %d. Falta um '}'\n", linha, coluna);
			fclose(arq);
			exit(0);
		}

		lexm = Scanner(arq);
		Comando(arq);
	}

	else if (lexm.tipo == PalavraReservada_do) {
		lexm = Scanner(arq);
		Comando(arq);
		if (lexm.tipo != PalavraReservada_while) {
			printf("Erro na linha %d e coluna %d. Falta um 'while'\n", linha, coluna);
			fclose(arq);
			exit(0);
		}

		lexm = Scanner(arq);
		if (lexm.tipo != PalavraReservada_abre_parent) {
			printf("Erro na linha %d e coluna %d. Falta um '('\n", linha, coluna);
			fclose(arq);
			exit(0);
		}

		lexm = Scanner(arq);
		expr_relacional(arq);

		if (lexm.tipo != PalavraReservada_fecha_parent) {
			printf("Erro na linha %d e coluna %d. Falta um ')'\n", linha, coluna);
			fclose(arq);
			exit(0);
		}

		lexm = Scanner(arq);
		if (lexm.tipo != PalavraReservada_ponto_virg) {
			printf("Erro na linha %d e coluna %d. Falta um ';'\n", linha, coluna);
			fclose(arq);
			exit(0);
		}
		lexm = Scanner(arq);
	}
}

void caso_if_else(FILE *arq) {
	lexm = Scanner(arq);
	if (lexm.tipo == PalavraReservada_abre_parent) {
		lexm = Scanner(arq);
		expr_relacional(arq);
		if (lexm.tipo == PalavraReservada_fecha_parent) {
			lexm = Scanner(arq);
			Comando(arq);
			if (lexm.tipo == PalavraReservada_else) {
				lexm = Scanner(arq);
				Comando(arq);
			}
		}
		else {
			printf("Erro na linha %d e coluna %d. Ultimo token lido: %s . Comando 'if' nao tem ')'\n", linha, coluna, lexm.lexema);
			fclose(arq);
			exit(0);
		}
	}
	else {
		printf("Erro na linha %d e coluna %d. Ultimo token lido: %s . Comando 'if' nao tem '('\n", linha, coluna, lexm.lexema);
		fclose(arq);
		exit(0);
	}
}

void expr_aritm_2(FILE *arq) {
	if (lexm.tipo == PalavraReservada_soma || lexm.tipo == PalavraReservada_sub) {
		lexm = Scanner(arq);
		termo(arq);
		expr_aritm_2(arq);
	}
}