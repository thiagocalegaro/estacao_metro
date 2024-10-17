
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct linha
{
	char cor[15];
};
//struct para listar vizinhos de cada estação
struct vizinhos
{
	struct estacao *estacao;
	struct vizinhos *prox;
	int n;
	struct linha linhas;
};
struct estacao
{
	char nome[30];
	struct estacao *prox;
	struct estacao *ant;
	struct vizinhos *vizinho;
	int no;
	int situacao;
};

typedef struct vizinhos newviz;
typedef struct estacao newest;

newest *busca_estacao(newest *m, char nome[30])
{
	newest *aux_m = m;

	while (aux_m != NULL)
	{
		if (strcmp(aux_m->nome, nome) == 0)
		{
			//encontra a estação com o nome correspondente
			return aux_m;
		}
		aux_m = aux_m->ant;
	}

	//retorna NULL se a estação não for encontrada
	return NULL;
}

//função para criar as estações
newest *cria_estacao(newest *m, int n, char nome[30], int sit)
{
	newest *novo = (newest *)malloc(sizeof(newest)); //aloca memória
	newest *aux = m;
	novo->no = n;
	strcpy(novo->nome, nome);
	strupr(novo->nome);
	novo->situacao = sit;
	novo->vizinho = NULL;
	int i;
	if (m == NULL)
	{
		novo->ant = NULL;
		novo->prox = NULL;
		return novo;
	}
	novo->ant = aux;
	novo->prox = NULL;
	aux->prox = novo;
	novo->prox = m;
	return novo;
}



//void relatorio(newlinha)
newviz *criaAdj(newest *m, char nome[30], char cor[15])
{
	newest *estacao = busca_estacao(m, nome);
	if (estacao == NULL)
	{
		return NULL;
	}

	newviz *v = (newviz *)malloc(sizeof(newviz)); //alocacao de memoria para o vizinho
	v->estacao = estacao;						 
	strcpy(v->linhas.cor, cor);
	v->n = estacao->no; //recebe o no da estacao vizinha
	v->prox = NULL;
	return v;
}

void vizinho(newest *m, char nome[30], newviz *v)
{
	newest *estacao = busca_estacao(m, nome);
	if (estacao == NULL || v == NULL)
	{
		printf("\nEstacao ou vizinho nao encontrado");
	}
	else
	{
		if (estacao->vizinho == NULL)
		{						  //se a lista de adjac�ncias da estac�o for vazia
			estacao->vizinho = v; //a adjac�ncia passada como par�metro � a primeira da lista
		}
		else
		{									//se a lista de adjac�ncias da estac�o n�o for vazia
			newviz *aux = estacao->vizinho; //aux recebe a primeira adjac�ncia da lista
			while (aux->prox != NULL)
			{					 //enquanto aux n�o for o �ltimo n� da lista
				aux = aux->prox; //aux recebe o pr�ximo n� da lista
			}
			aux->prox = v; //o �ltimo n� da lista recebe a adjac�ncia passada como par�metro
		}
	}
}
void relatorio(newest *m){
	
		int i;
		if(m->situacao != -1){
			printf("Estacao ");
			puts(m->nome);
			
			if(m->situacao == 0){
				printf("Estacao em Manutencao\n");
			}
			else{
				printf("Estacao Operacional\n");
			}
			printf("Vizinhos: \n");
			newviz *aux = m->vizinho;
			//imprime as infos dos vizinhos
			while(aux != NULL){
    			printf("%s", aux->estacao->nome);
    			printf("(linha %s)\n", aux->estacao->vizinho->linhas);
    			aux = aux->prox;
			}
		}
		printf("\n\n");
		if(m->ant != NULL){
			relatorio(m->ant);
		}
		
}
void alterar(newest *m)
{
	char nome[30];
	char nome1[30];
	char nome2[30];
	char cor[15];
	int op = 0;
	newest *aux;
	printf("Digite o nome da estacao que deseja alterar\n");
	gets(nome);
	strupr(nome);
	aux = busca_estacao(m, nome);
	if (aux == NULL)
	{
		printf("Estacao nao encontrada");
	}
	else
	{
		printf("\n1. Alterar nome");
		printf("\n2. Realocar estacao");
		printf("\n3. Alterar estado da estacao");
		scanf("%d", &op);
		fflush(stdin);
		if(op == 1){
			printf("Digite o novo nome\n");
			gets(aux->nome);
		}
		else if(op == 2){
			printf("\n1. Tornar estacao a ponta de uma linha");
			printf("\n2. Colocar a estacao entre outras 2\n");
			scanf("%d", &op);
			if(op == 1){
				printf("Digite o nome da estacao sucessora desta\n");
				gets(nome1);
				printf("Digite o nome linha");
				gets(cor);
				strlwr(cor);
				// cria a adjacência
				vizinho(m, nome, criaAdj(m, nome1, cor));
				vizinho(m, nome1, criaAdj(m, nome, cor));
			}
			else if(op == 2){
				printf("Digite o nome da primeira estacao\n");
				gets(nome1);
				strupr(nome1);
				printf("Digite o nome da segunda estacao\n");
				gets(nome2);
				strupr(nome2);
				// cria a adjacência
				vizinho(m, nome, criaAdj(m, nome1, cor));
				vizinho(m, nome1, criaAdj(m, nome, cor));
				vizinho(m, nome, criaAdj(m, nome2, cor));
				vizinho(m, nome2, criaAdj(m, nome, cor));
			}
		}
		else if(op == 3){
			if(aux->situacao == 0){
				printf("\n1. Reabrir estacao");
			}
			else if(aux->situacao == -1){
				printf("Estacao desativada permanentemente");
			}
			else{
				printf("\n1. Fechar temporariamente");
				printf("\n2. Fechar permanentemente\n");
				scanf("%d", &op);
				if(op == 1 && aux->situacao == 0){
					aux->situacao = 1;
				}
				else if(op == 1 && aux->situacao == 1){
					aux->situacao = 0;
				}
				else if(op == 2){
					aux->situacao = -1;
				}
				else{
				printf("Opcao Invalida\n");
				}
			}
		}
		else{
			printf("Opcao Invalida\n");
		}
		
	}
}

void melhores_opc(newest *m)
{
    newest *aux = m;

    while (aux != NULL)
    {
        newviz *aux2 = aux->vizinho;
        int cont = 0;

        //Conta o número de vizinhos da estação
        while (aux2 != NULL)
        {
            aux2 = aux2->prox;
            cont++;
        }

        //Se a estação tem três ou mais vizinhos, imprime
        if (cont >= 3)
        {
            printf("Estacao ");
            puts(aux->nome);
            printf("\n");
        }

        aux = aux->ant;
    }
}
//função auxiliar recursiva para encontrar uma rota entre duas estações
void encontra_rota_recursiva(newest *m, char *inicio, char *fim, newest *visitado[], int indice, int *achou)
{
    newest *estacao = busca_estacao(m, inicio);
    visitado[indice] = estacao;

    if (strcmp(inicio, fim) == 0)
    {
        *achou = 1;
        printf("Rota encontrada: ");
        for (int i = 0; i <= indice; i++)
        {
            if (visitado[i]->situacao == 1) //imprime se a estação estiver funcionando
            {
                printf("%s ", visitado[i]->nome);
            }
        }
        printf("\n");
    }
    else
    {
        newviz *v = estacao->vizinho;
        while (v != NULL && !*achou)
        {
            if (busca_estacao(m, v->estacao->nome) != NULL)
            {
                //chama recursivamente para explorar as estações vizinhas
                encontra_rota_recursiva(m, v->estacao->nome, fim, visitado, indice + 1, achou);
            }
            v = v->prox;
        }
    }

    if (*achou == 0)
    {
        visitado[indice] = NULL;
    }
}

//Ffnção para encontrar rotas entre duas estações
void encontra_rotas(newest *m, char *inicio, char *fim)
{
    newest *visitado[50] = {NULL};
    int achou = 0;
    encontra_rota_recursiva(m, inicio, fim, visitado, 0, &achou);

    if (!achou)
    {
        printf("Nenhuma rota encontrada entre %s e %s\n", inicio, fim);
    }
}

//função auxiliar recursiva para encontrar uma rota de uma estação para uma linha 
void encontra_rota_linha_recursiva(newest *m, char inicio[30], char linha_desejada[15], newest *visitado[], int indice, int *achou)
{
    newest *estacao = busca_estacao(m, inicio);
    visitado[indice] = estacao;
	newviz *aux = estacao->vizinho;
    for (int i = 0; i < 6; i++)
    {
        if (strcmp(aux->linhas.cor, linha_desejada) == 0)
        {
            *achou = 1;
            printf("Rota encontrada: ");
            for (int j = 0; j <= indice; j++)
            {
                printf("%s ", visitado[j]->nome);
            }
            printf("\n");
            return;
        }
    }

    if (!*achou)
    {
        newviz *v = estacao->vizinho;
        while (v != NULL && !*achou)
        {
            if (busca_estacao(m, v->estacao->nome) != NULL)
            {
                //chama recursivamente para explorar as estações vizinhas
                encontra_rota_linha_recursiva(m, v->estacao->nome, linha_desejada, visitado, indice + 1, achou);
            }
            v = v->prox;
        }
    }

    if (*achou == 0)
    {
        visitado[indice] = NULL;
    }
}

//função para encontrar uma rota de uma estação para uma linha 
void encontra_rota_linha(newest *m, char *inicio, char *linha_desejada)
{
    newest *visitado[30] = {NULL};
    int achou = 0;
    encontra_rota_linha_recursiva(m, inicio, linha_desejada, visitado, 0, &achou);

    if (!achou)
    {
        printf("Nenhuma rota encontrada de %s para a linha %s\n", inicio, linha_desejada);
    }
}



int main()
{

	int i = 1;
	int op = 0;
	char nome[30];
	char cor[15];
	newest *metro = NULL;
	int sit = 1; //sit 1 indica que a estação está ativa/funcionando, 0 em mautenção e -1 fechada permanentemente

	//cria as linhas e estações padrões
	metro = cria_estacao(metro, i++, "Reboleira", sit);
	metro = cria_estacao(metro, i++, "Colegio Militar/Luz", sit);
	metro = cria_estacao(metro, i++, "Jardim Zoologico", sit);
	metro = cria_estacao(metro, i++, "Praca da Espanha", sit);
	metro = cria_estacao(metro, i++, "S.Sebastiao", sit);
	metro = cria_estacao(metro, i++, "Marques de Pombal", sit);
	metro = cria_estacao(metro, i++, "Restauradores", sit);
	metro = cria_estacao(metro, i++, "Baixa-Chiado", sit);
	metro = cria_estacao(metro, i++, "Terreiro do Paco", sit);
	metro = cria_estacao(metro, i++, "Santa Apolonia", sit);

	metro = cria_estacao(metro, i++, "Saldanha", sit);
	metro = cria_estacao(metro, i++, "Alameda", sit);
	metro = cria_estacao(metro, i++, "Olaias", sit);
	metro = cria_estacao(metro, i++, "Olivais", sit);
	metro = cria_estacao(metro, i++, "Oriente", sit);
	metro = cria_estacao(metro, i++, "Encarnacao", sit);
	metro = cria_estacao(metro, i++, "Aeroporto", sit);

	metro = cria_estacao(metro, i++, "Odivelas", sit);
	metro = cria_estacao(metro, i++, "Senhor Roubado", sit);
	metro = cria_estacao(metro, i++, "Ameixoeira", sit);
	metro = cria_estacao(metro, i++, "Campo Grande", sit);
	metro = cria_estacao(metro, i++, "Telheiras", sit);

	metro = cria_estacao(metro, i++, "Estrela", sit);
	metro = cria_estacao(metro, i++, "Campo Pequeno", sit);
	metro = cria_estacao(metro, i++, "Entre Campos", sit);
	metro = cria_estacao(metro, i++, "Cidade Universitaria", sit);
	metro = cria_estacao(metro, i++, "Roma", sit);
	metro = cria_estacao(metro, i++, "Intendente", sit);
	metro = cria_estacao(metro, i++, "Rossio", sit);
	metro = cria_estacao(metro, i++, "Cais do Sodre", sit);



	//estações vizinhas
	vizinho(metro, "REBOLEIRA", criaAdj(metro, "JARDIM ZOOLOGICO", "azul"));
	vizinho(metro, "COLEGIO MILITAR/LUZ", criaAdj(metro, "JARDIM ZOOLOGICO", "azul"));
	vizinho(metro, "JARDIM ZOOLOGICO", criaAdj(metro, "PRACA DA ESPANHA", "azul"));
	vizinho(metro, "PRACA DA ESPANHA", criaAdj(metro, "S.SEBASTIAO", "azul"));
	vizinho(metro, "S.SEBASTIAO", criaAdj(metro, "MARQUES DE POMBAL", "azul"));
	vizinho(metro, "MARQUES DE POMBAL", criaAdj(metro, "RESTAURADORES", "azul"));
	vizinho(metro, "RESTAURADORES", criaAdj(metro, "BAIXA-CHIADO", "azul"));
	vizinho(metro, "BAIXA-CHIADO", criaAdj(metro, "TERREIRO DO PACO", "azul"));
	vizinho(metro, "TERREIRO DO PACO", criaAdj(metro, "SANTA APOLONIA", "azul"));
	vizinho(metro, "S.SEBASTIAO", criaAdj(metro, "SALDANHA", "vermelha"));
	vizinho(metro, "SALDANHA", criaAdj(metro, "ALAMEDA", "vermelha"));
	vizinho(metro, "ALAMEDA", criaAdj(metro, "OLAIAS", "vermelha"));
	vizinho(metro, "OLAIAS", criaAdj(metro, "OLIVAIS", "vermelha"));
	vizinho(metro, "OLIVAIS", criaAdj(metro, "ORIENTE", "vermelha"));
	vizinho(metro, "ORIENTE", criaAdj(metro, "ENCARNACAO", "vermelha"));
	vizinho(metro, "ENCARNACAO", criaAdj(metro, "AEROPORTO", "vermelha"));
	vizinho(metro, "ODIVELAS", criaAdj(metro, "SENHOR ROUBADO", "amarela"));
	vizinho(metro, "SENHOR ROUBADO", criaAdj(metro, "AMEIXOEIRA", "amarela"));
	vizinho(metro, "AMEIXOEIRA", criaAdj(metro, "CAMPO GRANDE", "amarela"));
	vizinho(metro, "CAMPO GRANDE", criaAdj(metro, "TELHEIRAS", "amarela"));
	vizinho(metro, "ESTRELA", criaAdj(metro, "MARQUES DE POMBAL", "verde"));
	vizinho(metro, "MARQUES DE POMBAL", criaAdj(metro, "SALDANHA", "verde"));
	vizinho(metro, "SALDANHA", criaAdj(metro, "CAMPO PEQUENO", "verde"));
	vizinho(metro, "CAMPO PEQUENO", criaAdj(metro, "ENTRE CAMPOS", "verde"));
	vizinho(metro, "ENTRE CAMPOS", criaAdj(metro, "CIDADE UNIVERSITARIA", "verde"));
	vizinho(metro, "CIDADE UNIVERSITARIA", criaAdj(metro, "CAMPO GRANDE", "verde"));
	vizinho(metro, "CAMPO GRANDE", criaAdj(metro, "ROMA", "verde"));
	vizinho(metro, "ROMA", criaAdj(metro, "ALAMEDA", "verde"));
	vizinho(metro, "ALAMEDA", criaAdj(metro, "INTENDENTE", "verde"));
	vizinho(metro, "INTENDENTE", criaAdj(metro, "ROSSIO", "verde"));
	vizinho(metro, "ROSSIO", criaAdj(metro, "BAIXA-CHIADO", "verde"));
	vizinho(metro, "BAIXA-CHIADO", criaAdj(metro, "CAIS DO SODRE", "verde"));
	
	vizinho(metro, "COLEGIO MILITAR/LUZ", criaAdj(metro, "REBOLEIRA", "azul"));
	vizinho(metro, "JARDIM ZOOLOGICO", criaAdj(metro, "COLEGIO MILITAR/LUZ", "azul"));
	vizinho(metro, "PRACA DA ESPANHA", criaAdj(metro, "JARDIM ZOOLOGICO", "azul"));
	vizinho(metro, "S.SEBASTIAO", criaAdj(metro, "PRACA DA ESPANHA", "azul"));
	vizinho(metro, "MARQUES DE POMBAL", criaAdj(metro, "S.SEBASTIAO", "azul"));
	vizinho(metro, "RESTAURADORES", criaAdj(metro, "MARQUES DE POMBAL", "azul"));
	vizinho(metro, "BAIXA-CHIADO", criaAdj(metro, "RESTAURADORES", "azul"));
	vizinho(metro, "TERREIRO DO PACO", criaAdj(metro, "BAIXA-CHIADO", "azul"));
	vizinho(metro, "SANTA APOLONIA", criaAdj(metro, "TERREIRO DO PACO", "azul"));
	vizinho(metro, "SALDANHA", criaAdj(metro, "S.SEBASTIAO", "vermelha"));
	vizinho(metro, "ALAMEDA", criaAdj(metro, "SALDANHA", "vermelha"));
	vizinho(metro, "OLAIAS", criaAdj(metro, "ALAMEDA", "vermelha"));
	vizinho(metro, "OLIVAIS", criaAdj(metro, "OLAIAS", "vermelha"));
	vizinho(metro, "ORIENTE", criaAdj(metro, "OLIVAIS", "vermelha"));
	vizinho(metro, "ENCARNACAO", criaAdj(metro, "ORIENTE", "vermelha"));
	vizinho(metro, "AEROPORTO", criaAdj(metro, "ENCARNACAO", "vermelha"));
	vizinho(metro, "SENHOR ROUBADO", criaAdj(metro, "ODIVELAS", "amarela"));
	vizinho(metro, "AMEIXOEIRA", criaAdj(metro, "SENHOR ROUBADO", "amarela"));
	vizinho(metro, "CAMPO GRANDE", criaAdj(metro, "AMEIXOEIRA", "amarela"));
	vizinho(metro, "TELHEIRAS", criaAdj(metro, "CAMPO GRANDE", "amarela"));
	vizinho(metro, "MARQUES DE POMBAL", criaAdj(metro, "ESTRELA", "verde"));
	vizinho(metro, "SALDANHA", criaAdj(metro, "MARQUES DE POMBAL", "verde"));
	vizinho(metro, "CAMPO PEQUENO", criaAdj(metro, "SALDANHA", "verde"));
	vizinho(metro, "ENTRE CAMPOS", criaAdj(metro, "CAMPO PEQUENO", "verde"));
	vizinho(metro, "CIDADE UNIVERSITARIA", criaAdj(metro, "ENTRE CAMPOS", "verde"));
	vizinho(metro, "CAMPO GRANDE", criaAdj(metro, "CIDADE UNIVERSITARIA", "verde"));
	vizinho(metro, "ROMA", criaAdj(metro, "CAMPO GRANDE", "verde"));
	vizinho(metro, "ALAMEDA", criaAdj(metro, "ROMA", "verde"));
	vizinho(metro, "INTENDENTE", criaAdj(metro, "ALAMEDA", "verde"));
	vizinho(metro, "ROSSIO", criaAdj(metro, "INTENDENTE", "verde"));
	vizinho(metro, "BAIXA-CHIADO", criaAdj(metro, "ROSSIO", "verde"));
	vizinho(metro, "CAIS DO SODRE", criaAdj(metro, "BAIXA-CHIADO", "verde"));
	
	char inicio[30];
    char fim[30];
    char linha[15];
	while (op != 8)
	{
		//menu
		printf("\n1. Gerar Relatorio");
		printf("\n2. Alterar Dados");
		printf("\n3. Criar Nova Estacao");
		printf("\n4. Tracar Rotas");
		printf("\n5. Verificar Trajeto de Estacao para Linha");
		printf("\n6. Melhores Estacoes para Turistas");
		printf("\n7. Verificar Grafo de Euler");
		printf("\n8. Encerrar Programa\n");
		scanf("%d", &op);
		fflush(stdin);
		system("cls");
		switch (op){
		case 1:
			relatorio(metro);
			system("pause");
			system("cls");
			break;
		case 2:
			alterar(metro);
			break;
		case 3:
			printf("Digite o nome da nova estacao\n");
			gets(nome);
			metro = cria_estacao(metro, i++, nome, sit);
			printf("\nPara definir o local da estacao na linha va para *2. Alterar Dados*");
			break;
		case 4:
    		printf("Digite a estação de origem: ");
   			gets(inicio);
    		printf("Digite a estação de destino: ");
    		gets(fim);
			encontra_rotas(metro, inicio, fim);
            break;
        case 5:
        	printf("Digite a estação de início: ");
    		gets(inicio);
    		printf("Digite a linha que deseja chegar: ");
   			gets(linha);
    		encontra_rota_linha(metro, inicio, linha);
			break;
		case 6:
			melhores_opc(metro);
			break;
		case 7:
			system("cls");
			printf("Ainda em desenvolvimento\n");
			system("pause");
			break;
		case 8:
			printf("Programa encerrado");
			break;
		default:
			printf("Valor invalido");
			break;
		}
	}

	free(metro);
	return 0;
}