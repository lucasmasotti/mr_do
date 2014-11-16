#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#define NUMERO_LINHAS 23
#define NUMERO_COLUNAS 60

void imprime_matriz(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]);
struct coordenada posicao_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]);
void move_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS], struct coordenada coordenada_inicial, char direcao);
void gotoxy(int x, int y);

struct coordenada{
	int linha;
	int coluna;
};

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

    imprime_matriz(matriz);
    while(1){
        if(kbhit()){
            entrada_usuario = getch();
            move_mr_do(matriz, posicao_mr_do(matriz), entrada_usuario);
            imprime_matriz(matriz);
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

/// Recebe uma struct com a coordenada inicial do Mr. Do e uma posicao ('d', 'e', 'c', 'b'). Move o Mr. Do para a posicao indicada
void move_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS], struct coordenada coordenada_inicial, char direcao){
	/*
	Direções:
	w - cima
	s - baixo
	d - direita
	a - esquerda
	*/
	int linha_inicial = coordenada_inicial.linha;
	int coluna_inicial = coordenada_inicial.coluna;
	switch(direcao){
		case 'w':
		    if((linha_inicial-1) >= 0){
                mat[linha_inicial][coluna_inicial] = 'v';
                mat[linha_inicial-1][coluna_inicial] = 'd';
		    }
			break;
		case 's':
		    if((linha_inicial+1) <= 22){
                mat[linha_inicial][coluna_inicial] = 'v';
                mat[linha_inicial+1][coluna_inicial] = 'd';
		    }
			break;
		case 'd':
		    if((coluna_inicial+1) <= 59){
                mat[linha_inicial][coluna_inicial] = 'v';
                mat[linha_inicial][coluna_inicial+1] = 'd';
		    }

			break;
		case 'a':
		    if((coluna_inicial-1) >= 0){
                mat[linha_inicial][coluna_inicial] = 'v';
                mat[linha_inicial][coluna_inicial-1] = 'd';
		    }
			break;
	}


}

/// Retorna uma struct do tipo coordenada (int linha, int coluna), que contém os índices da posição que ocupada pelo caracter'd'
struct coordenada posicao_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]){
	int i = 0;
	int j = 0;
	for(i = 0; i < NUMERO_LINHAS; i++){
		for(j = 0; j < NUMERO_COLUNAS; j++){
			if(mat[i][j] == 'd'){
			struct coordenada coordenada_mr_do = {i, j};
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
