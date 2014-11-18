#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#define NUMERO_LINHAS 23
#define NUMERO_COLUNAS 60

// Structs
typedef struct
{
	int linha;
	int coluna;
} Coordenada;

typedef struct
{
	char name[40];
	int score;
} Highscore;

typedef struct
{
	int score;
	int ghosts;
} GameState;

void imprime_matriz(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]);
Coordenada posicao_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]);
void move_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada coordenada_inicial, char direcao);
void gotoxy(int x, int y);

char novo_char(char char_antigo);

void nova_cor(char caractere_impresso);

//TO DO
void sair_jogo();
void abre_fase();

void salvar_highscores();
void salvar_cenarios();

void verifica_colisoes(GameState *estado);

void colidiu_fantasma();
void colidiu_fruta(GameState *estado);


// Variáveis globais
char caractere_mr_do_passou = 'v';

int main(){
	char matriz[NUMERO_LINHAS][NUMERO_COLUNAS] =
	{
		"ppppppppppppppppppppvvvivvvvvvvvvvvvvvvvvvvvvvvvvvvvvppppppp",
		"pppppppppppppppppppppppvppppppppppppppppppppppppppppvvvvpppp",
		"pffppppppppppppppppppppvpppppppppppppppppppppppppppppppvvvpp",
		"pffppppppppppppppppppppvpppppppppppppppppppppppppppppppppvvp",
		"pffppppppppppppppppppppvppppppppppppppppppppppppppppppppppvp",
		"pffppppppppppppppppppppvppppppppppppppppppppppppppppppppppvp",
		"pppppppppppppppppppppppvpppppppffpppppppppppppppppppppppppvp",
		"pppppppppppppppppppppppvpppppppffppppppppppppppppppppppppvvp",
		"pppppppppppppppppppppppippppppppffppppppppppppppppppppppppvp",
		"pppppppppppppppppppppppvppppppppffppppppppppppppppppppppppvp",
		"pppppppppppppppppppppppvppppppppppppppppppppppppppppppppppvp",
		"pppppppppffppppppppppppvpppppppppppppppppffpppppppppppppppvp",
		"ppppppppppffpppppppppppvpppppppppppppppppffpppppppppppppppvp",
		"pppppppppppffppppppppppnpppppppppppppppppffpppppppppppppppvp",
		"pppppppppppppffppppppppvpppppppppppppppppffpppppppppppppppvp",
		"pppppppppppppppppppppppvppppppppppppppppppppppppppppppppppvp",
		"pppppppppppppppppppppppvppppppppppppppppppppppppppppppppppvp",
		"pppppppppppppppppppppppvppppppppppppppppppppppppppppppppppvp",
		"vvvvvvvvpppppppppppppppvpppppppppppppppppppppppppppppppppvvp",
		"vvppppvvppppppppppppppptpppppppppppppppppppppppppppppppvvvpp",
		"vvppppvvpppppppppppppppvppppppppppppppppppppppppppppvvivpppp",
		"vvppppvvpppppppppppppppvppppppppppppppppppppppppppvvvppppppp",
		"vvvvvvvvvvvvvvvvvvvvvvvdvvvvvvvvvvivvvvvvvvvvvvvvvvppppppppp"
    };
    char entrada_usuario;
    Highscore vetor_pontuacoes[6];
    GameState estado_jogo;
    estado_jogo.score = 0;

	HANDLE n_console;
	n_console = GetStdHandle(STD_OUTPUT_HANDLE);


    imprime_matriz(matriz);
    while(1){
        if(kbhit()){
            entrada_usuario = getch();
            if(entrada_usuario == 27)
            {
                sair_jogo();
            }
            else
            {
                move_mr_do(matriz, posicao_mr_do(matriz), entrada_usuario);
                LockWindowUpdate(n_console);
                verifica_colisoes(&estado_jogo);
                imprime_matriz(matriz);
                printf("%d", estado_jogo.score);
                LockWindowUpdate(NULL);
                Sleep(20);
            }
        }
    }


	return 0;
}

/// Funções:

/// Imprime a mtriz mat, substituindo os caracters pelos respectivos caracteres que o jogador deve visualizar
void imprime_matriz(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]){
	/*
	Caracter 	| Significado 			| Código ASCII
		p	 			parede					176
		i				fantasma				153
		f				fruta					229
		d				Mr Do!					227
		t				tiro					250
		n				ninho dos fantasmas		206
		v				caminho (vazio)			32
	*/

	int linha;
	int coluna;
    gotoxy(0, 0);
	for(linha = 0; linha < NUMERO_LINHAS; linha++){
		for(coluna = 0; coluna < NUMERO_COLUNAS; coluna++){
			nova_cor(mat[linha][coluna]);
			switch(mat[linha][coluna]){
				case 'p':
					printf("%c", 176);
					break;
				case 'i':
					printf("%c", 153);
					break;
				case 'f':
					printf("%c", 229);
					break;
				case 'd':
					printf("%c", 227);
					break;
				case 't':
					printf("%c", 250);
					break;
				case 'n':
					printf("%c", 206);
					break;
				case 'v':
					printf("%c", 32);
					break;
			}
		}
		printf("\n");
	}
}

/// Recebe uma struct com a Coordenada inicial do Mr. Do e uma posicao ('d', 'e', 'c', 'b'). Move o Mr. Do para a posicao indicada
void move_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada coordenada_inicial, char direcao){
	/*
	Direções:
	w - cima
	s - baixo
	d - direita
	a - esquerda
	*/
	int linha_inicial = coordenada_inicial.linha;
	int coluna_inicial = coordenada_inicial.coluna;
	char char_substituido = novo_char(caractere_mr_do_passou);
	switch(direcao){
		case 'w':
		    if((linha_inicial-1) >= 0){
                mat[linha_inicial][coluna_inicial] = char_substituido;
				caractere_mr_do_passou = mat[linha_inicial-1][coluna_inicial];
                mat[linha_inicial-1][coluna_inicial] = 'd';
		    }
			break;
		case 's':
		    if((linha_inicial+1) <= 22){
                mat[linha_inicial][coluna_inicial] = char_substituido;
				caractere_mr_do_passou = mat[linha_inicial+1][coluna_inicial];
                mat[linha_inicial+1][coluna_inicial] = 'd';
		    }
			break;
		case 'd':
		    if((coluna_inicial+1) <= 59){
                mat[linha_inicial][coluna_inicial] = char_substituido;
				caractere_mr_do_passou = mat[linha_inicial][coluna_inicial+1];
                mat[linha_inicial][coluna_inicial+1] = 'd';
		    }

			break;
		case 'a':
		    if((coluna_inicial-1) >= 0){
                mat[linha_inicial][coluna_inicial] = char_substituido;
				caractere_mr_do_passou = mat[linha_inicial][coluna_inicial-1];
                mat[linha_inicial][coluna_inicial-1] = 'd';
		    }
			break;
	}


}

/// Retorna uma struct do tipo coordenada (int linha, int coluna), que contém os índices da posição que ocupada pelo caracter'd'
Coordenada posicao_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]){
	int i = 0;
	int j = 0;
	for(i = 0; i < NUMERO_LINHAS; i++){
		for(j = 0; j < NUMERO_COLUNAS; j++){
			if(mat[i][j] == 'd'){
			Coordenada coordenada_mr_do = {i, j};
			return coordenada_mr_do;
			}
		}
	}
}


void gotoxy(int x, int y){
    COORD coord = {0, 0};
    coord.X = x; coord.Y = y; // X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/// Returna um novo character baseado no character antigo, além de chamar os eventos corretos para a colisão com fantasma ou com frutas
char novo_char(char char_antigo){
	if(char_antigo == 'n')
        return 'n';
    else
        return 'v';
}

/// Muda a cor do terminal de acordo com o caractere que será impresso
void nova_cor(char caractere_impresso){
	/*
		Cores:
		1 - Azul escuro			 9 - Azul claro
		2 - Verde				10 - Verde neon
		3 - Azul				11 - Azul
		4 -	Vermelho escuro		12 - Vermelho
		5 -	Roxo				13 - Rosa
		6 -	Dourado				14 - Amarelo
		7 -	Branco				15 - Branco ?
		8 - Cinza
	*/

	int cor = 15;
	// Necessário para alterar a cor do console
	HANDLE h_console;
	h_console = GetStdHandle(STD_OUTPUT_HANDLE);

	switch(caractere_impresso){
		case 'p':
			cor = 2;
			break;
		case 'f':
			cor = 12;
			break;
		case 'd':
			cor = 14;
			break;
		case 'i':
			cor = 15;
			break;
		case 'n':
			cor = 8;
			break;
	}
	SetConsoleTextAttribute(h_console, cor);
}

void colidiu_fantasma(){

}

void colidiu_fruta(GameState *estado)
{
    estado->score += 50;
}

void verifica_colisoes(GameState *estado)
{
    if(caractere_mr_do_passou == 'f')
    {
        colidiu_fruta(estado);
    }

}
void sair_jogo()
{
    salvar_cenario();
    salvar_estado();

}

void salvar_cenario()
{

}

void salvar_estado()
{


}
