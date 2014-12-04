#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <math.h>
#define NUMERO_LINHAS 23
#define NUMERO_COLUNAS 61
#define NUMERO_FANTASMAS 10
#define CICLOS_NOVO_FANTASMA 60
#define CICLOS_NOVO_MOVIMENTO 1

// Structs
typedef struct
{
	int linha;
	int coluna;
} Coordenada;

typedef struct
{
	char nome[40];
	int score;
} Highscore;

typedef struct
{
	int score;
	int ghosts;
} GameState;

/// Estrutura fantasma
/// A variável estado representa a direção que o fantasma está andando
/// 1: Subindo 2: Direita
/// 3: Descendo 4: Esquerda
typedef struct
{
    int linha;
    int coluna;
    int estado;
    int ultima_direcao;
} Fantasma;



//Manipulação de fases
void abre_fase (char m[NUMERO_LINHAS][NUMERO_COLUNAS], char nome_arquivo[40]);
void salvar_cenario(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS]);
void salvar_highscores(GameState estado_jogo, Highscore vetor_pontuacoes[6]);
void carregar_highscores(Highscore vetor_pontuacoes[6]);

//Manipulação do estado de jogo
void salvar_gamestate (GameState estado_jogo);
void novo_GameState (GameState *estado_jogo);

//Impressão matriz e outros
void imprime_matriz(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]);
void nova_cor(char caractere_impresso);
void gotoxy(int x, int y);
void menu();
void imprime_display(int score, int frutas);
void imprime_highscore(Highscore vetor_pontuacoes[6]);

//Mr. DO
Coordenada posicao_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]);
void move_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada coordenada_inicial, char direcao, char *caractere_passou, int *matou);
char novo_char(char char_antigo);

//Jogo em geral
void verifica_colisoes(GameState *estado, int *numero_frutas, char caractere_passou);
int verifica_estado(GameState estado_jogo, int fantasmas_criados, int quantidade_frutas);
void colidiu_fruta(GameState *estado, int *numero_frutas);


/// Fantasmas:
int conta_fantasmas(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS]);
int is_cross(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Fantasma fantasma);
int is_next_wall(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Fantasma fantasma);
void nova_direcao_fantasma(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Fantasma *fantasma, int ignorar_ultima);
void move_fantasma(Fantasma *fantasma, char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada ninho, int *matou);
void criar_novo_fantasma(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada ninho, Fantasma fantasmas[10], GameState *estado_jogo, int *indice_fantasma, int *quantidade_fantasmas_criados);
void novo_passo_fantasma(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada ninho, Fantasma fantasmas[10], GameState estado_jogo, int *matou);

Coordenada posicao_ninho(char mat[NUMERO_LINHAS][NUMERO_COLUNAS]);

int recoloca_fantasmas(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Fantasma fantasmas[10]);



int main(){
	char matriz[NUMERO_LINHAS][NUMERO_COLUNAS];

	// Fases
    char fase1[] = "fase1.txt";
    char fase_salva[] = "continuar.txt";

    // Estado do jogo
    GameState estado_jogo;
    estado_jogo.score = 0;
    estado_jogo.ghosts = 0;

    int quantidade_frutas = 32;

    int quantidade_fantasmas_criados = 0;

    int matou_mr_do = 0;

    //Contagem de ciclos
    int ciclos_novo_fantasma = 100;
    int ciclos_move_fantasma = 0;

    //Fantasmas
    Fantasma fantasmas[NUMERO_FANTASMAS];
    int indice_fantasma = 0;
    Coordenada coordenada_ninho;

    Highscore vetor_pontuacoes[6];
    carregar_highscores(vetor_pontuacoes);

    char caractere_mr_do_passou = 'v';

    //Entradas do usuário
    char entrada_usuario;
    int menu_opcao;
    int acaba_jogo = 0;
    int esc_pressionado = 0;

    int resultado_jogo = 0;

    //Manipulação do console
	HANDLE n_console;
	n_console = GetStdHandle(STD_OUTPUT_HANDLE);

    // Exibe menu e valida opcao do usuário
    do
    {
        menu();
        scanf("%d", &menu_opcao);
        if(menu_opcao > 4 || menu_opcao < 1)
        {
            printf("\nOpcao invalida, digite novamente");
            Sleep(1000);
            system("cls");
        }
    }while(menu_opcao > 4 || menu_opcao < 1);

    switch(menu_opcao)
    {
        case 1:
                abre_fase(matriz, fase1);
                estado_jogo.ghosts = conta_fantasmas(matriz);
                coordenada_ninho = posicao_ninho(matriz);
                indice_fantasma = recoloca_fantasmas(matriz, fantasmas);
                quantidade_fantasmas_criados = indice_fantasma+1;
                break;
        case 2:
                abre_fase(matriz, fase_salva);
                novo_GameState(&estado_jogo);
                estado_jogo.ghosts = conta_fantasmas(matriz);
                coordenada_ninho = posicao_ninho(matriz);
                indice_fantasma = recoloca_fantasmas(matriz, fantasmas);
                quantidade_fantasmas_criados = indice_fantasma+1;
                break;
        case 3:
                acaba_jogo = 1;
                imprime_highscore(vetor_pontuacoes);
                break;
        case 4:
                acaba_jogo = 1;
                break;
    }


    if(!acaba_jogo)
    {
        imprime_matriz(matriz);
        imprime_display(estado_jogo.score, quantidade_frutas);
    }

        while(!acaba_jogo)
        {
            //Verifica se já se passaram 0.1 segundos
            if(ciclos_move_fantasma >= CICLOS_NOVO_MOVIMENTO)
            {
                novo_passo_fantasma(matriz, coordenada_ninho,fantasmas, estado_jogo, &matou_mr_do);
                ciclos_move_fantasma = 0;
            }
            //Verifica se já passaram 3 segundos e se o numero maximo de fantasmas já foi excedido
            if((quantidade_fantasmas_criados < 10) && (ciclos_novo_fantasma >= CICLOS_NOVO_FANTASMA))
            {
                criar_novo_fantasma(matriz, coordenada_ninho, fantasmas, &estado_jogo, &indice_fantasma, &quantidade_fantasmas_criados);
                ciclos_novo_fantasma = 0;
            }
            if(kbhit() && !matou_mr_do)
            {
                entrada_usuario = getch();
                if(entrada_usuario == 27)
                {
                    salvar_cenario(matriz);
                    salvar_gamestate(estado_jogo);
                    acaba_jogo = 1;
                    esc_pressionado = 1;

                } else {
                    move_mr_do(matriz, posicao_mr_do(matriz), entrada_usuario, &caractere_mr_do_passou, &matou_mr_do);
                    verifica_colisoes(&estado_jogo, &quantidade_frutas, caractere_mr_do_passou);
                }
            }

            LockWindowUpdate(n_console);
            resultado_jogo = verifica_estado(estado_jogo, quantidade_fantasmas_criados, quantidade_frutas);
            imprime_matriz(matriz);
            imprime_display(estado_jogo.score, quantidade_frutas);
            LockWindowUpdate(NULL);

            switch(resultado_jogo)
            {
            case 1:
                system("cls");
                printf("Voce derrotou todos os fantasmas, parabens!\n");
                system("pause");
                acaba_jogo = 1;
                break;
            case 2:
                system("cls");
                printf("Voce pegou todas as frutas, parabens!\n");
                system("pause");
                acaba_jogo = 1;
                break;
            }

            if(matou_mr_do)
                acaba_jogo = 1;

            if(acaba_jogo && !esc_pressionado)
            {
                system("cls");
                salvar_highscores(estado_jogo, vetor_pontuacoes);
            }

            ciclos_novo_fantasma++;
            ciclos_move_fantasma++;

            Sleep(25);
        }
	return 0;
}

/// Funções:
/// Abre o arquivo de texto na matriz m
void abre_fase (char m[NUMERO_LINHAS][NUMERO_COLUNAS], char nome_arquivo[40])
{
    int i;
    int j;
    FILE *fp;

    fp = fopen(nome_arquivo, "r");

    if (fp == NULL)
    {
        printf("Erro na abertura da fase.");
    } else {
        for(i=0; i < NUMERO_LINHAS; i++)
        {
            for(j = 0; j < NUMERO_COLUNAS; j++)
            {
                m[i][j] = getc(fp);
            }
        }
        fclose(fp);
    }
}

/// Imprime a matriz mat, substituindo os caracters pelos respectivos caracteres que o jogador deve visualizar
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
void move_mr_do(char mat[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada coordenada_inicial, char direcao, char *caractere_passou, int *matou) {
	/*
	Direções:
	w - cima
	s - baixo
	d - direita
	a - esquerda
	*/
	int linha_inicial = coordenada_inicial.linha;
	int coluna_inicial = coordenada_inicial.coluna;
	char char_substituido = novo_char(*caractere_passou);
	switch(direcao){
		case 'w':
		    if((linha_inicial-1) >= 0){
                mat[linha_inicial][coluna_inicial] = char_substituido;
				*caractere_passou = mat[linha_inicial-1][coluna_inicial];
                mat[linha_inicial-1][coluna_inicial] = 'd';
		    }
			break;
		case 's':
		    if((linha_inicial+1) <= 22){
                mat[linha_inicial][coluna_inicial] = char_substituido;
				*caractere_passou = mat[linha_inicial+1][coluna_inicial];
                mat[linha_inicial+1][coluna_inicial] = 'd';
		    }
			break;
		case 'd':
		    if((coluna_inicial+1) <= 59){
                mat[linha_inicial][coluna_inicial] = char_substituido;
				*caractere_passou = mat[linha_inicial][coluna_inicial+1];
                mat[linha_inicial][coluna_inicial+1] = 'd';
		    }

			break;
		case 'a':
		    if((coluna_inicial-1) >= 0){
                mat[linha_inicial][coluna_inicial] = char_substituido;
				*caractere_passou = mat[linha_inicial][coluna_inicial-1];
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

///Retora uma struct do tipo coordenada que contém as linha e coluna do ninho (caracter 'n')
Coordenada posicao_ninho(char mat[NUMERO_LINHAS][NUMERO_COLUNAS])
{
	int i = 0;
	int j = 0;
	for(i = 0; i < NUMERO_LINHAS; i++){
		for(j = 0; j < NUMERO_COLUNAS; j++){
			if(mat[i][j] == 'n')
            {
                Coordenada coordenada = {i, j};
                return coordenada;
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

void colidiu_fruta(GameState *estado, int *numero_frutas)
{
    estado->score += 50;
    *numero_frutas = *numero_frutas - 1;
}

void verifica_colisoes(GameState *estado, int *numero_frutas, char caractere_passou)
{
    if(caractere_passou == 'f')
    {
        colidiu_fruta(estado, numero_frutas);
    }

}


/// Salva o cenario quando fecha o jogo
void salvar_cenario(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS])
{
    FILE *arq;
    int i;
    int j;

    arq = fopen("continuar.txt", "w");

    if (arq == NULL)
    {
        printf("Erro ao salvar arquivo");
    } else {

        for(i = 0; i < NUMERO_LINHAS; i++)
        {
            for(j = 0; j < NUMERO_COLUNAS; j++)
            {
                putc(matriz[i][j], arq);
            }
        }
        fclose(arq);
    }


}

/// Salva o score no fim de jogo
void salvar_highscores(GameState estado_jogo, Highscore vetor_pontuacoes[6])
{
    int i;
    int j;
    Highscore score_temporario;
    int score_e_maior = 0;
    FILE *arq;

    for(i = 0; i < 5; i++)
    {
        if(estado_jogo.score > vetor_pontuacoes[i].score)
        {
            score_e_maior = 1;
        }

    }

    if(score_e_maior)
    {
        vetor_pontuacoes[5].score = estado_jogo.score;
        printf("Nome do jogador: ");
        fflush(stdin);
        gets(vetor_pontuacoes[5].nome);
    }
        for(i = 0; i < 6; i++)
        {
            for(j = 0; j < 5; j++)
            {
                if(vetor_pontuacoes[j+1].score > vetor_pontuacoes[j].score)
                {
                    score_temporario = vetor_pontuacoes[j+1];
                    vetor_pontuacoes[j+1] = vetor_pontuacoes[j];
                    vetor_pontuacoes[j] = score_temporario;
                }
            }
        }

        arq = fopen("highscores.bin", "wb");
        if(arq == NULL)
        {
            printf("Erro na abertura de highscores.bin");
        } else {
            for(i = 0; i < 6; i++)
            {
                fwrite(&vetor_pontuacoes[i], sizeof(Highscore), 1, arq);
            }
        }
        fclose(arq);
}


/// Salva o estado do jogo
void salvar_gamestate (GameState estado_jogo)
{
    FILE *arq;

    arq = fopen("gamestate.bin", "wb");
    if (arq == NULL)
    {
        printf("Erro na escrita de gamestate.bin");
    } else {
        fwrite(&estado_jogo, sizeof(GameState), 1, arq);
    }
    fclose(arq);
}

/// Imprime o menu
void menu()
{
    printf("1 - Novo Jogo\n2 - Continuar\n3 - High Scores\n4 - Sair\nOpcao desejada: ");
}

/// novo gamestate (jogo salvo)
void novo_GameState (GameState *estado_jogo)
{
    FILE *arq;
    arq = fopen("gamestate.bin", "rb");
    if (arq == NULL)
    {
        printf("Erro na abertura de gamestate.bin");
    } else {
        fread(estado_jogo, sizeof(GameState), 1, arq);
    }
}

/// Verifica se ainda existem fantasmas ou frutas. Retorna 1 caso todos os fantasmas foram destruídos, retorna 2 caso todas as frutas tenham sido pegas
int verifica_estado(GameState estado_jogo, int fantasmas_criados, int quantidade_frutas)
{
    if((estado_jogo.ghosts == 0) && (fantasmas_criados ==  10))
    {
        return 1;
    }
    if(quantidade_frutas == 0)
    {
        return 2;
    }
    return 0;
}

/// Imprime a pontuação e outras informações
void imprime_display(int score, int frutas)
{
    gotoxy(62, 10);
    printf("PONTUACAO: %4.4d", score);
    gotoxy(62, 13);
    printf("%2.2d fruta(s)", frutas);
}

/// Fantasmas

/// Retorna o número de fantasmas presentes
int conta_fantasmas(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS])
{
    int i;
    int j;
    int numero_fantasmas = 0;
    for(i = 0; i <NUMERO_LINHAS; i++)
    {
        for(j = 0; j < NUMERO_COLUNAS; j++)
        {
            if(matriz[i][j] == 'i')
            {
                numero_fantasmas++;
            }
        }
    }
    return numero_fantasmas;
}

int is_cross(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Fantasma fantasma)
{
    int cruzamento = 0;

    if(matriz[fantasma.linha+1][fantasma.coluna] == 'v')
    {
        cruzamento++;
    }

    if(matriz[fantasma.linha-1][fantasma.coluna] == 'v')
    {
        cruzamento++;
    }

    if(matriz[fantasma.linha][fantasma.coluna+1] == 'v')
    {
        cruzamento++;
    }

    if(matriz[fantasma.linha][fantasma.coluna-1] == 'v')
    {
        cruzamento++;
    }


    if(cruzamento > 2)
    {
        return 1;
    } else {
        return 0;
    }
}

void nova_direcao_fantasma(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Fantasma *fantasma, int ignorar_ultima)
{
    srand(time(NULL));
    int nova_direcao;
    do
    {
        nova_direcao = rand() % 4;
    } while(nova_direcao == ignorar_ultima);

    fantasma->ultima_direcao = nova_direcao;
}

int is_next_wall(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Fantasma fantasma)
{
    int is_wall = 0;

    /// A variável estado representa a direção que o fantasma está andando
    /// 0: Subindo 1: Direita
    /// 2: Descendo 3: Esquerda

    switch(fantasma.ultima_direcao)
    {
    case 0:
        if((matriz[fantasma.linha-1][fantasma.coluna] == 'p') || ((fantasma.linha - 1) < 0))
            is_wall = 1;
        break;
    case 1:
        if((matriz[fantasma.linha][fantasma.coluna+1] == 'p') || ((fantasma.coluna + 1) > 59))
            is_wall = 1;
        break;
    case 2:
        if((matriz[fantasma.linha+1][fantasma.coluna] == 'p') || ((fantasma.linha + 1) > 22))
            is_wall = 1;
        break;
    case 3:
        if((matriz[fantasma.linha][fantasma.coluna-1] == 'p') || ((fantasma.coluna - 1) < 0))
            is_wall = 1;
        break;
    }
    return is_wall;
}

// Move o fantasma de acordo com  ultima_direcao atual
void move_fantasma(Fantasma *fantasma, char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada ninho, int *matou)
{
    /// A variável ultima_direcao representa a direção que o fantasma está andando
    /// 0: Subindo 1: Direita
    /// 2: Descendo 3: Esquerda
    int ninho_linha = ninho.linha;
    int ninho_coluna = ninho.coluna;

    if((fantasma->linha == ninho_linha) && (fantasma->coluna == ninho_coluna))
    {
        matriz[fantasma->linha][fantasma->coluna] = 'n';
    }
    else
    {
        matriz[fantasma->linha][fantasma->coluna] = 'v';
    }

    switch(fantasma->ultima_direcao)
    {
    case 0:
        if(matriz[fantasma->linha-1][fantasma->coluna] == 'd')
            *matou = 1;
        matriz[fantasma->linha-1][fantasma->coluna] = 'i';
        fantasma->linha = fantasma->linha-1;
        break;
    case 1:
        if(matriz[fantasma->linha][fantasma->coluna+1] == 'd')
             *matou = 1;
        matriz[fantasma->linha][fantasma->coluna+1] = 'i';
        fantasma->coluna = fantasma->coluna+1;
        break;
    case 2:
        if(matriz[fantasma->linha+1][fantasma->coluna] == 'd')
             *matou = 1;
        matriz[fantasma->linha+1][fantasma->coluna] = 'i';
        fantasma->linha = fantasma->linha+1;
        break;
    case 3:
        if(matriz[fantasma->linha][fantasma->coluna-1] == 'd')
             *matou = 1;
        matriz[fantasma->linha][fantasma->coluna-1] = 'i';
        fantasma->coluna = fantasma->coluna-1;
        break;
    }
}

void criar_novo_fantasma(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada ninho, Fantasma fantasmas[10], GameState *estado_jogo, int *indice_fantasma, int *quantidade_fantasmas_criados)
{
    Fantasma novo_fantasma = {ninho.linha, ninho.coluna, 1, 2};
    fantasmas[*indice_fantasma] = novo_fantasma;
    *indice_fantasma = *indice_fantasma + 1;
    estado_jogo->ghosts = estado_jogo->ghosts + 1;
    *quantidade_fantasmas_criados = *quantidade_fantasmas_criados + 1;
    matriz[ninho.linha][ninho.coluna] = 'i';
}

void novo_passo_fantasma(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Coordenada ninho, Fantasma fantasmas[10], GameState estado_jogo, int *matou)
{
    int i;
    int direcao_seguinte;
    for(i = 0; i < estado_jogo.ghosts; i++)
    {
        if(fantasmas[i].estado)
        {
            while(is_next_wall(matriz, fantasmas[i]))
            {
                if(is_cross(matriz, fantasmas[i]) == 1)
                {
                    nova_direcao_fantasma(matriz, &fantasmas[i], fantasmas[i].ultima_direcao);
                }
                else
                {
                    fantasmas[i].ultima_direcao = rand() %4;
                }
            }
            move_fantasma(&fantasmas[i], matriz, ninho, matou);
        }

    }
}

void imprime_highscore(Highscore vetor_pontuacoes[6])
{
    int i;
    for(i = 0; i < 5; i++)
    {
        printf("Nome: ");
        printf("\t%s", vetor_pontuacoes[i].nome);
        printf("\tPontuacao: %d", vetor_pontuacoes[i].score);
        printf("\n");
    }
    system("pause");
}

void carregar_highscores(Highscore vetor_pontuacoes[6])
{
    FILE *arq;
    arq = fopen("highscores.bin", "rb");
    Highscore score_temp;
    score_temp.nome[0] = '-';
    score_temp.nome[1] = '\0';
    score_temp.score = 0;
    int i;
        if(arq == NULL)
        {
            printf("Arquivo de highscores nao encontrando\n");
            for(i = 0; i < 6; i++)
            {
                vetor_pontuacoes[i] = score_temp;
            }
        } else {
            for(i = 0; i < 5; i++)
            {
                fread(&vetor_pontuacoes[i], sizeof(Highscore), 1, arq);
            }
            vetor_pontuacoes[5] = score_temp;
        }
        fclose(arq);
}

int recoloca_fantasmas(char matriz[NUMERO_LINHAS][NUMERO_COLUNAS], Fantasma fantasmas[10])
{
    int i;
    int j;
    int indice_fantasma = 0;
    for(i = 0; i < NUMERO_LINHAS; i++)
    {
        for(j = 0; j < NUMERO_COLUNAS; j++)
        {
            if(matriz[i][j] == 'i')
            {
                fantasmas[indice_fantasma].linha = i;
                fantasmas[indice_fantasma].coluna = j;
                fantasmas[indice_fantasma].estado = 1;
                fantasmas[indice_fantasma].ultima_direcao = 1;
                indice_fantasma++;
            }
        }
    }
    return indice_fantasma;

}
