#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

union udados{
	int ValorI;
	float ValorN;
	char ValorD[10];
	char ValorC;
	char ValorT[20];
};

struct tpdados{
	union udados valor;
	struct tpdados *prox;
};
typedef struct tpdados TpDados;

struct campos{
	char nome_campo[40];
	char tipo;
	char PK;
	struct campos *FK;
	struct tpdados *Pdados, *Patual;
	struct campos *prox;
};
typedef struct campos Campos;

struct tabela{
	char nome_tabela[40];
	struct campos *PCampos;
	struct tabela *ant, *prox;
};
typedef struct tabela Tabela;

struct bancodados{
	char nome_BD[40];
	struct tabela *PTabelas;
};
typedef struct bancodados BancoDados;
//---------------------------------------------------------

//----------------------------------------------------------------
struct FILAC{
	Campos *ptr;
	struct FILAC *prox;
};
typedef struct FILAC FILAC;

void init (FILAC **F){
	*F = NULL;
}

char isEmpty (FILAC *F){
	return F == NULL;
}

void enqueue (FILAC **F, Campos *L){
	FILAC *Nova;
	Nova = (FILAC*) malloc(sizeof(FILAC));
	Nova->prox = NULL;
	Nova->ptr = L;
	
	if (*F == NULL)	
		*F = Nova;
	else{
		FILAC *Aux = *F;
		while (Aux->prox != NULL)
			Aux = Aux->prox;
		Aux->prox = Nova;
	}
}

void dequeue (FILAC **F, Campos **L){
	*L = (*F)->ptr;
	if (*F != NULL)
		*F = (*F)->prox;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TESTETB(Tabela *L){
	while(L != NULL){
		printf("\n%s", L -> nome_tabela);
		L = L -> prox;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TESTECAMPO(Tabela *L){
	Campos *C = L -> PCampos;
	while(L != NULL){
		printf("\n----- %s -----", L -> nome_tabela);
		 C = L -> PCampos;
		while(C != NULL){
			printf("\n > %s | %c | %c", C -> nome_campo, C -> tipo, C -> PK);
			C = C -> prox;
		}
		L = L -> prox;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CREATEDB(BancoDados **L, char nome[]){
	*L = (BancoDados *)malloc(sizeof(BancoDados));
	strcpy((*L) -> nome_BD, nome);
	(*L) -> PTabelas = NULL;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CREATETB(BancoDados **PBancoDados, char nome[]){
	Tabela *nova = (Tabela *)malloc(sizeof(Tabela));
	strcpy(nova -> nome_tabela, nome);
	nova -> ant = nova -> prox = NULL;
	nova -> PCampos = NULL;
	
	if((*PBancoDados) -> PTabelas == NULL) //Nenhuma caixa criada
		(*PBancoDados) -> PTabelas = nova;
	else{
		Tabela *aux = (*PBancoDados) -> PTabelas;
		if(aux -> ant == aux -> prox){ //Tem apenas uma caixa na lista
			aux -> prox = nova;
			nova -> ant = aux;	
		}
		else{ // Tem mais de uma caixa na lista... Inserir *nova no final
			while(aux -> prox != NULL)
				aux = aux -> prox;
			aux -> prox = nova;
			nova -> ant = aux;			
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CREATECAMPO (Tabela *L, char nometab[], char nome_camp[], char tp[]){
	while(L != NULL && strcmp(L -> nome_tabela, nometab) != 0)
		L = L -> prox;
	if(L != NULL){
		Campos *nova = (Campos *)malloc(sizeof(Campos));
		
		strcpy(nova -> nome_campo, nome_camp);
		if(strcmp(tp, "INTEGER") == 0){
			nova -> tipo = 'I';
		} else if(strcmp(tp, "CHARACTER(20)") == 0){
			nova -> tipo = 'T';
		} else if(strcmp(tp, "DATE") == 0){
			nova -> tipo = 'D';
		} else if(strcmp(tp, "CHARACTER(1)") == 0){
			nova -> tipo = 'C';
		} else{
			nova -> tipo = 'N';
		}
		nova -> Patual = NULL;
		nova -> PK = 'N';
		nova -> FK = NULL;
		nova -> Pdados = NULL;
		nova -> prox= NULL;
		
		Campos *Pcamp = L -> PCampos;
		if(Pcamp == NULL)
			L -> PCampos = nova;
		else{
			while(Pcamp -> prox != NULL)
				Pcamp = Pcamp -> prox;
			Pcamp -> prox = nova;
		}
	}
	 else
		printf("Tabela nao encontrada.");
	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DEFINEPK(Tabela *L, char nometab[], char nome_camp[]){
	while(L != NULL && strcmp(nometab, L -> nome_tabela) != 0)
		L =L -> prox;
	if(L != NULL){
		Campos *C = L -> PCampos;
		while(C != NULL && strcmp(nome_camp, C -> nome_campo) != 0)
			C = C -> prox;
		if(C != NULL)
			C -> PK = 'S';
	}	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DEFINEFK (Tabela *L, char nometab[], char nometabfora[], char nomecampo[]){		
	Tabela *L2 = L;
	
	while(L != NULL && strcmp(L -> nome_tabela, nometab) != 0)
		L = L -> prox;
	
	if(L != NULL){
		Campos *C = L -> PCampos;
		while(L2 != NULL && strcmp(L2 -> nome_tabela, nometabfora) != 0)
			L2 = L2 -> prox;
		if(L2 != NULL){
			Campos *C2 = L2 -> PCampos;
			while(C != NULL && strcmp(C -> nome_campo, nomecampo) != 0)
				C = C -> prox;              
			if(C != NULL){                                
				while(C2 != NULL && strcmp(C2 -> nome_campo, nomecampo) != 0)
					C2 = C2 -> prox;                    
				if(C2 != NULL){
					C -> FK = C2; 

				}
			}	
		}
	}	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Script(BancoDados **PBancoDados){
	FILE *ptrArq = fopen("Script2.txt", "r");
	char palavra[40], nomecampo[40], tipo[40], nometab[40], nometabfora[40], aux[40];
	int i, j, flag, k;
	
	
	fscanf(ptrArq, "%s", palavra);
	while(!feof(ptrArq)){
		
		if(strcmp(palavra, "CREATE") == 0){
			fscanf(ptrArq, "%s", palavra);
			if(strcmp(palavra, "DATABASE") == 0){
				fscanf(ptrArq, "%[^;];\n", palavra);				
				CREATEDB(&(*PBancoDados), palavra); 
				//fscanf(ptrArq, "%s", palavra);					
			}
			else{
				fscanf(ptrArq, " %[^' '] (", nometab);
				//fscanf(ptrArq, "%s", palavra);
				CREATETB(&(*PBancoDados), nometab);
				strcpy(nomecampo, " ");

				while(strcmp(palavra, ");") != 0){					
					if(strcmp(nomecampo, "CONSTRAINT") == 0){
						fscanf(ptrArq, "%[^(](", palavra);
						fscanf(ptrArq, "%[^)])\n", palavra);
						strcpy(aux," ");
						i = 0;
						j = 0;
						flag =  0;
								
							
						while(palavra[i] != '\0'){

							aux[j] = palavra[i];
							if(palavra[i] == ','){
								aux[j] = '\0';
								DEFINEPK((*PBancoDados) -> PTabelas, nometab, aux);
								k = 0;
								strcpy(aux, " ");
								j = 0;
								i++;
								i++;
								while(palavra[i] != '\0'){
									aux[j] = palavra[i];
									i++;
									j++;	
								}
								aux[j] = '\0';
								DEFINEPK((*PBancoDados) -> PTabelas, nometab, aux);
								flag = 1;
							}
							else{						
								i++;
								j++;	
							}
						}
						if(flag == 0)
							DEFINEPK((*PBancoDados) -> PTabelas, nometab, palavra);
						fscanf(ptrArq, "%s", palavra);
					}
					else{
						fscanf(ptrArq, "%s", nomecampo);
						fscanf(ptrArq, "%s", tipo);
						fscanf(ptrArq, "%s", palavra);

						
						if(strcmp(palavra, "NOT") == 0){
							fscanf(ptrArq, "%s", palavra);
						}
						if(strcmp(nomecampo, "CONSTRAINT") != 0)
							CREATECAMPO((*PBancoDados) -> PTabelas, nometab, nomecampo, tipo);
					}
		
				}

			}		
		}
		else{
			fscanf(ptrArq, "%s", palavra); // TABLE
			fscanf(ptrArq, "%s", nometab); // aluguel
			fscanf(ptrArq, "%s", palavra); // ADD
			fscanf(ptrArq, "%s", palavra); // CONSTRAINT
			fscanf(ptrArq, "%s ", palavra); // CLIENTE_ALUGUEL			
			fscanf(ptrArq, "%s", palavra); // FOREIGN				
			fscanf(ptrArq, "%[^(](", palavra); // KEY
			fscanf(ptrArq, "%[^)])", nomecampo);
			fscanf(ptrArq, "%s", palavra);
			fscanf(ptrArq, "%s", nometabfora); // cliente
			fscanf(ptrArq, "%s", palavra);

			
			DEFINEFK((*PBancoDados) -> PTabelas, nometab, nometabfora, nomecampo);
		}

		
		fscanf(ptrArq, "%s", palavra);
	
	}
	
	fclose(ptrArq);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BuscaTab(Tabela **T, char nometab[]){
	while((*T) -> prox != NULL && strcmp(nometab, (*T) -> nome_tabela) != 0)
		*T = (*T) -> prox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BuscaCampo(Campos **C, char nomecampo[]){
		while((*C) -> prox != NULL && strcmp(nomecampo, (*C) -> nome_campo) != 0)
			*C = (*C) -> prox;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CREATEDADO(Campos *C, char aux[]){
	int valorI;
	float valorN;
	char valorC;
	
	TpDados *auxC;
	TpDados *D = (TpDados *)malloc(sizeof(TpDados));
	D -> prox = NULL;
	
	if(C -> tipo == 'I'){
		valorI = atoi(aux);
		D -> valor.ValorI = valorI;
	}
	else if(C -> tipo == 'N'){
		valorN = atof(aux);
		D -> valor.ValorN = valorN;
	}
	else if(C -> tipo == 'D'){
		strcpy(D -> valor.ValorD, aux);
	}
	else if(C -> tipo == 'C'){
		valorC = aux[0];
		D -> valor.ValorC = valorC;
	}
	else{ // C -> tipo == 'T'
		strcpy(D -> valor.ValorT, aux);
	}
	
	if(C -> Pdados == NULL)
		C -> Pdados = D;
	else{
		auxC = C -> Pdados;
		while(auxC -> prox != NULL)
			auxC = auxC -> prox;
		auxC -> prox = D;
	}
	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SELECT(Tabela *L){
	Campos *C, *aux;
	TpDados *D;
	
	FILAC *F;
	init(&F);

	printf("\n-> %s", L -> nome_tabela);
	printf("\n--------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	C = L -> PCampos;
	
	while(C != NULL){
		printf("%-20s", C -> nome_campo);
		enqueue(&F, C);
		C -> Patual = C -> Pdados;		
		C = C -> prox;
	}

	printf("\n");
	while(!isEmpty(F)){
		dequeue(&F, &C);
			
		if(C -> Patual != NULL){
			enqueue(&F, C);
			
			
			if(C -> tipo == 'I')
				printf("%-20d", C -> Patual -> valor.ValorI);
			else if(C -> tipo == 'N')
				printf("%-20f", C -> Patual -> valor.ValorN);
			else if(C -> tipo == 'C')
				printf("%-20c", C -> Patual -> valor.ValorC);
			else if( C -> tipo == 'T')
				printf("%-20s", C -> Patual -> valor.ValorT);
			else	
				printf("%-20s", C -> Patual -> valor.ValorD);
			C -> Patual = C -> Patual -> prox;
		}
		if(C -> prox == NULL)
			printf("\n");
			
	}	
	printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UPDATE(Campos *L, char campoTroca[], char valorTroca[], char campoCondicao[], char valorCondicao[], char sinalInicio, char sinalFim){ 
	
		
	Campos *CTroca = L;
	Campos *CCondicao = L; 	
	int valorCondicaoI;
	float valorCondicaoF;
	char valorCondicaoC;
	
	BuscaCampo(&CTroca, campoTroca);
	BuscaCampo(&CCondicao, campoCondicao);
	CTroca -> Patual = CTroca -> Pdados;
	CCondicao -> Patual = CCondicao -> Pdados;
	
	if(CCondicao -> tipo == 'I'){
		valorCondicaoI = atoi(valorCondicao);
		if(sinalFim == '='){
			while(CCondicao -> Patual != NULL){
				if(valorCondicaoI == CCondicao -> Patual -> valor.ValorI){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}	
		}
		else if(sinalFim == '>'){
			while(CCondicao -> Patual != NULL){
				if(CCondicao -> Patual -> valor.ValorI > valorCondicaoI){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}
		else if(sinalFim == '<'){
			while(CCondicao -> Patual != NULL){
				if(CCondicao -> Patual -> valor.ValorI < valorCondicaoI){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}	
	}
	else if(CCondicao -> tipo == 'N'){
		valorCondicaoF = atof(valorCondicao);
		if(sinalFim == '='){
			while(CCondicao -> Patual != NULL){
				if(valorCondicaoF == CCondicao -> Patual -> valor.ValorN){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}	
		}
		else if(sinalFim == '>'){
			while(CCondicao -> Patual != NULL){
				if(CCondicao -> Patual -> valor.ValorN > valorCondicaoF){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}
		else if(sinalFim == '<'){
			while(CCondicao -> Patual != NULL){
				if(CCondicao -> Patual -> valor.ValorN < valorCondicaoF){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}	
	}	
	else if(CCondicao -> tipo == 'C'){
		valorCondicaoC = valorCondicao[0];
		if(sinalFim == '='){
			while(CCondicao -> Patual != NULL){
				if(CCondicao -> Patual -> valor.ValorC == valorCondicaoC){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}
		else{ // tudo que for diferente
			while(CCondicao -> Patual != NULL){
				if(CCondicao -> Patual -> valor.ValorC != valorCondicaoC){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}
	}
	else if(CCondicao -> tipo == 'T'){
		if(sinalFim == '='){
			while(CCondicao -> Patual != NULL){
				if(stricmp(CCondicao -> Patual -> valor.ValorT, valorCondicao) == 0){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}
		else{ // tudo que for diferente
			while(CCondicao -> Patual != NULL){
				if(stricmp(CCondicao -> Patual -> valor.ValorT, valorCondicao) != 0){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}
	}	
	else{ // Tipo 'D'
		if(sinalFim == '='){
			while(CCondicao -> Patual != NULL){
				if(stricmp(CCondicao -> Patual -> valor.ValorD, valorCondicao) == 0){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}
		else{ // tudo que for diferente
			while(CCondicao -> Patual != NULL){
				if(stricmp(CCondicao -> Patual -> valor.ValorT, valorCondicao) != 0){
					if(CTroca -> tipo == 'I'){
						CTroca -> Patual -> valor.ValorI = atoi(valorTroca);
					}
					else if(CTroca -> tipo == 'N'){
						CTroca -> Patual -> valor.ValorN = atof(valorTroca);
					}
					else if(CTroca -> tipo == 'C'){
						CTroca -> Patual -> valor.ValorC = valorTroca[0];
					}
					else if(CTroca -> tipo == 'D'){
						strcpy(CTroca -> Patual -> valor.ValorD, valorTroca);
					}
					else{
						strcpy(CTroca -> Patual -> valor.ValorT, valorTroca);
					}
				}
				CCondicao -> Patual = CCondicao -> Patual -> prox;	
				CTroca -> Patual = CTroca -> Patual -> prox;
			}
		}
	}	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DELETEW(Tabela *T, char nomecampo[], char valorCondicao[], char sinal){
	Campos *C = T -> PCampos, *CCondicao = T -> PCampos;
	TpDados *aux, *ant;
	
	int valorCondicaoI;
	float valorCondicaoF;
	char valorCondicaoC;

	if(sinal == ';'){//deletar tudo
		while(C != NULL){
			C -> Patual = C -> Pdados;
			C -> Pdados = NULL;
			while(C -> Patual != NULL){
				aux = C -> Patual;
				C -> Patual = C -> Patual -> prox;
				free(aux);	
			}
			C = C -> prox;
		}
	}
	else{ // tem condição
		while(C != NULL){
			C -> Patual = C -> Pdados;
			
			C = C -> prox;
		}
		C = T -> PCampos;
	
		BuscaCampo(&CCondicao, nomecampo);	
				
		if(CCondicao -> tipo == 'I'){
			valorCondicaoI = atoi(valorCondicao);
			if(sinal == '='){
				while(CCondicao -> Pdados != NULL && CCondicao -> Pdados -> valor.ValorI == valorCondicaoI){ // primeira caixa;
					
					while(C != NULL){                                                                       
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(CCondicao -> Patual -> valor.ValorI == valorCondicaoI){
							while(C != NULL){
								ant = C -> Pdados;                                  
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}
			}
			else if(sinal == '>'){
				while(CCondicao -> Pdados != NULL && CCondicao -> Pdados -> valor.ValorI > valorCondicaoI){ // primeira caixa;
					
					while(C != NULL){                                                                       
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(CCondicao -> Patual -> valor.ValorI > valorCondicaoI){
							while(C != NULL){
								ant = C -> Pdados;                                  
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}				
			}
			else if(sinal == '<'){
				while(CCondicao -> Pdados != NULL && CCondicao -> Pdados -> valor.ValorI < valorCondicaoI){ // primeira caixa;
					
					while(C != NULL){                                                                       
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(CCondicao -> Patual -> valor.ValorI < valorCondicaoI){
							while(C != NULL){
								ant = C -> Pdados;                                  
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}				
			}
		}
		else if(CCondicao -> tipo == 'N'){
			valorCondicaoF = atof(valorCondicao);
			if(sinal == '='){
				while(CCondicao -> Pdados != NULL && CCondicao -> Pdados -> valor.ValorN == valorCondicaoF){ // primeira caixa;
					
					while(C != NULL){                                                                       
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(CCondicao -> Patual -> valor.ValorN == valorCondicaoF){
							while(C != NULL){
								ant = C -> Pdados;                                  
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}
			}
			else if(sinal == '>'){
				while(CCondicao -> Pdados != NULL && CCondicao -> Pdados -> valor.ValorN > valorCondicaoF){ // primeira caixa;
					
					while(C != NULL){                                                                       
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(CCondicao -> Patual -> valor.ValorN > valorCondicaoF){
							while(C != NULL){
								ant = C -> Pdados;                                  
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}				
			}
			else if(sinal == '<'){
				while(CCondicao -> Pdados != NULL && CCondicao -> Pdados -> valor.ValorN < valorCondicaoF){ // primeira caixa;
					
					while(C != NULL){                                                                       
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(CCondicao -> Patual -> valor.ValorN < valorCondicaoF){
							while(C != NULL){
								ant = C -> Pdados;                                  
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}				
			}
		}
		else if(CCondicao -> tipo == 'C'){
			valorCondicaoC = valorCondicao[0];
			if(sinal == '='){
				while(CCondicao -> Pdados != NULL && CCondicao -> Pdados -> valor.ValorC == valorCondicaoC){ // primeira caixa;
					
					while(C != NULL){
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(CCondicao -> Patual -> valor.ValorC == valorCondicaoC){
							while(C != NULL){
								ant = C -> Pdados;
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}				
			}
			else{
				while(CCondicao -> Pdados != NULL && CCondicao -> Pdados -> valor.ValorC != valorCondicaoC){ // primeira caixa;
					
					while(C != NULL){
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(CCondicao -> Patual -> valor.ValorC != valorCondicaoC){
							while(C != NULL){
								ant = C -> Pdados;
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}
			}
		
		}
		else if(CCondicao -> tipo == 'T'){
			if(sinal == '='){
				while(CCondicao -> Pdados != NULL && stricmp(CCondicao -> Pdados -> valor.ValorT, valorCondicao) == 0){ // primeira caixa;
					
					while(C != NULL){
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(stricmp(CCondicao -> Pdados -> valor.ValorT, valorCondicao) == 0){
							while(C != NULL){
								ant = C -> Pdados;
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}				
			}
			else{
				while(CCondicao -> Pdados != NULL && stricmp(CCondicao -> Pdados -> valor.ValorT, valorCondicao) != 0){ // primeira caixa;
					
					while(C != NULL){
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(stricmp(CCondicao -> Pdados -> valor.ValorT, valorCondicao) != 0){
							while(C != NULL){
								ant = C -> Pdados;
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}				
			}
		}
		else{ // tipo == 'D'
			if(sinal == '='){
				while(CCondicao -> Pdados != NULL && stricmp(CCondicao -> Pdados -> valor.ValorD, valorCondicao) == 0){ // primeira caixa;
					
					while(C != NULL){
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(stricmp(CCondicao -> Pdados -> valor.ValorD, valorCondicao) == 0){
							while(C != NULL){
								ant = C -> Pdados;
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}				
			}
			else{
				while(CCondicao -> Pdados != NULL && stricmp(CCondicao -> Pdados -> valor.ValorD, valorCondicao) != 0){ // primeira caixa;
					
					while(C != NULL){
						
						aux = C -> Pdados;
						C -> Pdados = C -> Pdados -> prox;
						C -> Patual = C -> Pdados;
						free(aux);
						C = C -> prox;
						
					}
					C = T -> PCampos;
				}
				if(CCondicao -> Pdados != NULL){
					while(C != NULL){
						if(C -> Patual == C -> Pdados)
							C -> Patual = C -> Patual -> prox;
						C = C -> prox;
					}
					
					C = T -> PCampos;
					while(CCondicao -> Patual != NULL){
						if(stricmp(CCondicao -> Pdados -> valor.ValorD, valorCondicao) != 0){
							while(C != NULL){
								ant = C -> Pdados;
								while(ant -> prox != C -> Patual)
									ant = ant -> prox;
								aux = C -> Patual;
								C -> Patual = C -> Patual -> prox;
								ant -> prox = C -> Patual;
								free(aux);
								C = C -> prox;	
							}
								C = T ->PCampos;	
						}
						else{
							CCondicao -> Patual = CCondicao -> Patual -> prox;
						}
					}					
				}				
			}	
		} 
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void LeLinha(char linha[], BancoDados **PBancoDados){	
	int i = 0, j = 0, flag = 0, k;
	char DML[20], DQL[20], nometab[40], nomecampo[40], aux[40], auxTroca[40], auxCondicao[40], sinalFim, sinalInicio;
	
	Campos *Campos = NULL;
	Tabela *Tabela = (*PBancoDados) -> PTabelas;
	
	

	while(linha[i] != ' ' && linha[i] != '\0'){
		DML[i] = linha[i];
		i++;
	}
	DML[i] = '\0';
	i++;
	if(strcmp(DML, "INSERT") == 0){
	
		while(linha[i] != ' ' && linha[i] != '\0'){
			DQL[j] = linha[i];
			i++;
			j++;
		}
		DQL[j] = '\0';
		i++;
		j = 0;
		if(strcmp(DQL, "INTO") == 0){
			while(linha[i] != ' '){
				nometab[j] = linha[i];
				i++;               			
				j++;
			}
			nometab[j] = '\0';
			i++;
			i++;
			j = i;
			while(linha[j] != '(')
				j++;
			j++;
			while(flag != 1){ 
				k = 0;
				strcpy(nomecampo, " ");
				strcpy(aux, " ");
				while(linha[i] != ',' && linha[i] != ')'){
					nomecampo[k] = linha[i];
					i++;
					k++;
				}
				i++;
				i++;
				nomecampo[k] = '\0';
				k = 0;
				while(linha[j] != ',' && linha[j] != ')'){
					if(linha[j] == 39){
						j++;
					}
					else{
						aux[k] = linha[j];
						j++;
						k++;
					}	
				}	
				aux[k] = '\0';
				if(linha[j] == ')')
					flag = 1;
				j++;
				j++;
				BuscaTab(&Tabela, nometab);	
				if(Tabela != NULL){
					Campos = Tabela -> PCampos;
					BuscaCampo(&Campos, nomecampo);
					CREATEDADO(Campos, aux);
				}
			}	
		}
	}	
	else if(strcmp(DML, "UPDATE") == 0){
		while(linha[i] != ' '){
			nometab[j] = linha[i];
			i++;
			j++;
		}
		nometab[j] = '\0';
		i = i+5;
		j = 0;
		while(linha[i] != ' '){
			nomecampo[j] = linha[i];
			i++;
			j++;
		}
		nomecampo[j] = '\0';
		sinalInicio = linha[i+1];
		i = i+3;
		j = 0;
		while(linha[i] != ' '){
			auxTroca[j] = linha[i];
			i++;
			j++;
		}
		auxTroca[j] = '\0';
		i = i+7;
		j = 0;
		while(linha[i] != ' '){
			aux[j] = linha[i];
			i++;
			j++;
		}
		aux[j] = '\0';
		sinalFim = linha[i+1];
		i = i+3;
		j = 0;
		while(linha[i] != ';'){
			auxCondicao[j] = linha[i];
			i++;
			j++;
		}
		auxCondicao[j] = '\0';	
		BuscaTab(&Tabela, nometab);
		UPDATE(Tabela -> PCampos, nomecampo, auxTroca, aux, auxCondicao, sinalInicio, sinalFim);
	}	
	else if(strcmp(DML, "DELETE") == 0){
		i = i+5;
		while(linha[i] != ' ' && linha[i] != ';'){
			nometab[j] = linha[i];
			i++;
			j++;
		}
		nometab[j] = '\0';
		if(linha[i] == ';'){// deletar tudo
			sinalFim = linha[i];
			strcpy(nomecampo, " ");
			strcpy(auxCondicao, " ");
		}
		else{ //tem WHERE
			j = 0;
			i = i+7;
			while(linha[i] != ' '){
				nomecampo[j] = linha[i];
				i++;
				j++;
			}
			nomecampo[j] = '\0';
			sinalFim = linha[i+1];
			j = 0;
			i = i+3;                                 
			while(linha[i] != ';'){
				auxCondicao[j] = linha[i];
				i++;
				j++;
			}
			auxCondicao[j] = '\0';
		}
		BuscaTab(&Tabela, nometab);
		DELETEW(Tabela, nomecampo, auxCondicao, sinalFim);
		
	}
	else if(strcmp(DML, "SELECT") == 0){
		i = 1+6;
		if(linha[i] == '*'){
			i = i+7;
			j = 0;
			while(linha[i] != ';'){
				nometab[j] = linha[i];
				i++;
				j++;
			}
			nometab[j] = '\0';
			BuscaTab(&Tabela, nometab);
			SELECT(Tabela);
		}
		else{

		}
		
	}
	
			
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(){
	BancoDados *PBancoDados = NULL;	
	char str[200];
	
	Script(&PBancoDados);
	

	do{
		printf("\n> ");
		fflush(stdin);
		gets(str);
		
		
		LeLinha(str, &PBancoDados);
	
		
	}while(stricmp(str,"EXIT") != 0);
	

}

	/*
	LeLinha("INSERT INTO veiculo (id_veiculo, marca, modelo, dono, placa, combustivel, ano, chassi) values (1, 'Volkswagen', 'Gol', 'Jose da Silva', 'ABC-1234', 'G', 2010, 'AXB1234T789');", &PBancoDados);
	LeLinha("INSERT INTO veiculo (id_veiculo, marca, modelo, dono, placa, combustivel, ano, chassi) values (2, 'Volkswagen', 'Fusca', 'Ana Paula', 'GFG-2233', 'G', 1979, 'AXZ4433R1343');", &PBancoDados);
	LeLinha("INSERT INTO veiculo (id_veiculo, marca, modelo, dono, placa, combustivel, ano, chassi) values (3, 'Chevrolet', 'Onix', 'Pedro Souza', 'FWX-3435', 'F', 2020, 'GYX88994X343');", &PBancoDados);
	LeLinha("INSERT INTO veiculo (id_veiculo, marca, modelo, dono, placa, combustivel, ano, chassi) values (4, 'Chevrolet', 'Tracker', 'Maria Julia', 'MNM-5532', 'F', 2022, 'GYX99884Z343');", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (1, 'Filtro Combustivel', 25.00, 12);", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (2, 'Vela Ignicao 4X', 96.00, 7);", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (3, 'Sensor Temperatura', 112.30, 1);", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (4, 'Pastilha Freio 2X', 95.70, 4);", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (5, 'Fluido p/ Freio D4', 20.00, 6);", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (6, 'Cabo de Vela 4X', 70.00, 2);", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (7, 'Junta do Carter', 27.00, 5);", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (8, 'Amortecedor Diant', 230.00, 2);", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (9, 'Amortecedor Tras', 198.00, 4);", &PBancoDados);
	LeLinha("INSERT INTO peca (id_peca, descricao, valor, estoque) values (10, 'Oleo Motor', 32.00, 19);", &PBancoDados);
	LeLinha("INSERT INTO servico (id_servico, descricao, valor) values (1, 'Revisao Motor', 250.00);", &PBancoDados);
	LeLinha("INSERT INTO servico (id_servico, descricao, valor) values (2, 'Revisao Freio', 100.00);", &PBancoDados);
	LeLinha("INSERT INTO servico (id_servico, descricao, valor) values (3, 'Revisao Suspensao', 340.00);", &PBancoDados);
	LeLinha("INSERT INTO servico (id_servico, descricao, valor) values (4, 'Troca de Vela', 30.00);", &PBancoDados);
	LeLinha("INSERT INTO servico (id_servico, descricao, valor) values (5, 'Troca de Oleo', 25.00);", &PBancoDados);
	LeLinha("INSERT INTO servico (id_servico, descricao, valor) values (6, 'Troca Filtro', 20.00);", &PBancoDados);
	LeLinha("INSERT INTO ordem_servico (id_os, id_veiculo, km_entrada, data_entrada, hora_entrada, data_saida, hora_saida, valor_total) values (1, 3, 12000, '2023-09-12', '10:50', '2023-09-14', '9:30', 474.70);", &PBancoDados);
	LeLinha("INSERT INTO os_peca (id_os, id_peca, quantidade, valor) values (1, 2, 1, 96.00);", &PBancoDados);
	LeLinha("INSERT INTO os_peca (id_os, id_peca, quantidade, valor) values (1, 10, 4, 32.00);", &PBancoDados);
	LeLinha("INSERT INTO os_peca (id_os, id_peca, quantidade, valor) values (1, 4, 1, 95.70);", &PBancoDados);
	LeLinha("INSERT INTO os_servico (id_os, id_servico, valor) values (1, 4, 30.00);", &PBancoDados);
	LeLinha("INSERT INTO os_servico (id_os, id_servico, valor) values (1, 5, 25.00);", &PBancoDados);
	LeLinha("INSERT INTO os_servico (id_os, id_servico, valor) values (1, 2, 100.00);", &PBancoDados);
	LeLinha("INSERT INTO ordem_servico (id_os, id_veiculo, km_entrada, data_entrada, hora_entrada, data_saida, hora_saida, valor_total) values (2, 1, 95000, '2023-10-01', '08:10', '2023-10-04', '17:00', 1196.00);", &PBancoDados);
	LeLinha("INSERT INTO os_peca (id_os, id_peca, quantidade, valor) values (2, 8, 2, 230.00);", &PBancoDados);
	LeLinha("INSERT INTO os_peca (id_os, id_peca, quantidade, valor) values (2, 9, 2, 198.00);", &PBancoDados);
	LeLinha("INSERT INTO os_servico (id_os, id_servico, valor) values (2, 3, 340.00);", &PBancoDados);
	LeLinha("", &PBancoDados);

	SELECT(PBancoDados -> PTabelas);
	printf("\n");
	SELECT(PBancoDados -> PTabelas -> prox);
	printf("\n");
	SELECT(PBancoDados -> PTabelas -> prox -> prox);
	printf("\n");
	SELECT(PBancoDados -> PTabelas -> prox -> prox -> prox);
	printf("\n");
	SELECT(PBancoDados -> PTabelas -> prox -> prox -> prox -> prox);
	printf("\n");
	SELECT(PBancoDados -> PTabelas -> prox -> prox -> prox -> prox -> prox);
	
	getch();

*/




























