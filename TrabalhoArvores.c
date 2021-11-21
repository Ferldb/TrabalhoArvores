#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define N 100

//estrutura arvore binaria de nodos da expressao
typedef struct arvore{
	int num;
	char op;
	struct arvore *esq, *dir;
} NO;

typedef NO* PONT;

//pilha
char pilha[N];	//pilha para utilizar na conversao
int topo = -1;	//variavel de controle do topo da pilha

//pilha da arvore
PONT pilhaArv[N];	//pilha para utilizar na construcao da arvore
int topoArv = -1;	//variavel de controlo do topo da pilha arvore

//empilha um elemento
void empilha(char x){
	if(topo >= N-1){		//se pilha cheia
		printf("\nPilha cheia");
	} else {
		topo = topo + 1;	//atualiza controle do topo da pilha
		pilha[topo] = x;	//insere elemento no topo da pilha
	}
}

//desempilha um elemento
char desempilha(){
	char x;
	if(topo < 0){			//se pilha esta vazia
		printf("\nPilha vazia: expressao infixa invalida");
		getchar();
		exit(1);
	} else {
		x = pilha[topo];	//recebe elemento do topo
		topo = topo - 1;	//decrementa topo
		return(x);			//retorna elemento
	}
}

//empilha nodo arvore
void empilhaArv(PONT no){
	if(topoArv >= N-1){			//se pilha esta cheia
		printf("\nPilha cheia");
	} else {
		topoArv = topoArv + 1;	//atualiza controle do topo da pilha
		pilhaArv[topoArv] = no;	//insere elemento no topo da pilha
	}
}

//desempilha nodo arvore
PONT desempilhaArv(){
	PONT no;
	if(topoArv < 0){			//se pilha esta vazia
		printf("\nPilha vazia: expressao infixa invalida");
		getchar();
		exit(1);
	} else {
		no = pilhaArv[topoArv];	//recebe elemento do topo
		topoArv = topoArv - 1;	//decrementa topo
		return(no);				//retorna elemento
	}
}

//cria novo nodo
PONT criaNovoNo(char ch, int num){
	PONT novoNo = (PONT)malloc(sizeof(NO));	//aloca memoria
	novoNo->esq = NULL;						//nodo esq = null
	novoNo->dir = NULL;						//nodo dir = null
	novoNo->op = ch;						//atribui char
	novoNo->num = num;						//atribui valor
	return(novoNo);							//retorna nodo
}

//libera nodo
void libera_NO(PONT no){
    if(no == NULL) return;
    
    libera_NO(no->dir);			//chamada recursiva nodo esq
    libera_NO(no->esq);			//chamada recursiva nodo dir
    free(no);					//libera nodo
}

//lista de precedencia de operacoes, determina a prioridade de operadores
int precedencia(char x){
	if (x == '+' || x == '-')		//menor prioridade
		return 1;
	else if (x == '*' || x == '/')
		return 2;
	else if (x == '$' || x == '#')
		return 3;					//maior prioridade
	else return 0;
}

//operadores validos
int operador(char x){
	if(x == '*' || x == '/' || x == '+' || x == '-' || x == '$' || x == '#') return 1; 	//se for operador valido
	else return 0;																		//se nao for nenhum dos operadores validos
}

//verifica se expressao eh valida
int validaExp(char *exp){
	int i = 0, pD = 0, pE = 0, cont = 0, j;	//PD: parenteses direito - PE: parenteses esquerdo - CONT: contador de pares de parenteses
	int valido, op = 0,  e = 0;	//OP: contador de operadores - E: verifica expressões com 1 operando e 0 operador
	int tam = strlen(exp);
	
	//percorre expressão ata o fim
	while (exp[i] != '\0'){
		if(isdigit(exp[i])) {				//se eh um valor
			if(op > 0) op--;				//se sucede operador (contagem de operadores aninhados)
			valido = 1;
		}
		else if (exp[i] == '(') {			//se eh parenteses esquerdo
			valido = 1;
			pE++;							//contagem de parenteses esquerdos
			cont++;							//contagem para verificar pares de parenteses ( + )

		}
		else if (exp[i] == ')'){			//se eh parenteses direito
			valido = 1;
			pD++;							//contagem de parenteses direitos
			if(pE > 0) cont--;				//se sucede parenteses esquerdo, diminui contador
		} 
		else if (operador(exp[i]) == 1) {	//se eh um operador valido
			if(exp[i] == '#'){ 				//raiz quadrada
				e++;						//controla operadores, para expressões com apenas 1 operando e nenhum operador
				valido = 1;
				if (op > 0 && operador(exp[i-2]) == 1) op--;
				else valido == 0;
			}
			else if(op > 0 && exp[i] != '-')		//se sucede outro operador e nao eh sinal negativo
				return valido = 0;
			else if(exp[i] == '-' && isdigit(exp[i+1]))	//verifica se valor éeh negativo
				valido = 1;
			else{
				op++;						//contagem de operadores para verificar operadores aninhados
				valido = 1;
				e++;						//controla operadores, para expressoes com apenas 1 operando e nenhum operador
			}
		}
		else if (exp[i] == ' ') {
			if (isdigit(exp[i-1]) && isdigit(exp[i+1])) 	//se valor antecede e sucede espaço
				return valido = 0;
			else if(exp[i-1] == ')' && isdigit(exp[i+1]))	//sem operador entre valor e parenteses
				return valido = 0;
			else if(exp[i+1] == '(' && isdigit(exp[i-1]))	//sem operador entre valor e parenteses
				return valido == 0;
			valido = 1;	//se é espaço
		}
		else {												//se eh caractere invalido
			return valido = 0;
		}
		i++;
	}
	
	if(pE != pD || cont > 0 || op > 0 || e == 0) valido = 0;
	
	return valido;
}

//conversao expressao infixa para posfixa
char* posfixa(char *infixa){
	int i, j = 0;	//contadores para strings
	int tam = strlen(infixa);
	char x, e;		//e = caractere iterado  x = caractere desempilhado
	char *pos = (char*) malloc(tam * sizeof(char));	//recebera a expressao posfixa
	
	empilha('(');			//empilha parenteses esquerdo
	strcat(infixa,")");		//tratamento string entrada
	
	//percorre expressao infixa
	for(i = 0; infixa[i] != '\0'; i++){
		e = infixa[i];						//recebe caractere iterado
		if(e == '('){						//se encontra parenteses esquerdo, empilha
			empilha(e);
		} else if ( isdigit(e) ){			//se encontra operando, insere na expressao posfixa
			pos[j++] = e;
			if(j>0 && !isdigit(infixa[i+1]))
				pos[j++] = ' ';				//insere espaco apos operando completo
		} else if ( operador(e) == 1 ){		//se encontra operador valido
			if(e == '-' && (operador(infixa[i-1]) == 1 || i == 0 || (operador(infixa[i-2]) == 1 && infixa[i-1] == ' '))){ //se encontrou valor negativo
				pos[j++] = e;
			} else{
				x = desempilha();				//desempilha topo da pilha
				while( operador(x) == 1 && precedencia(x) >= precedencia(e) ){	//compara precedencia do topo da pilha e do caractere iterado
					pos[j++] = x;												//insere operador desempilhado na expressao posfixa
					pos[j++] = ' ';												//insere espaco apos operador
					x = desempilha();											//desempilha topo da pilha
				}
				empilha(x);														//empilha ultimo elemento desempilhado (pode ser parenteses)
				empilha(e);														//empilha caractere iterado
			}
		} else if ( e == ')') {				//se encontra parenteses direito
			x = desempilha();				//desempilha topo da pilha
			while (x != '('){				//desempilha ate encontrar parenteses esquerdo
				pos[j++] = x;				//insere elemento desempilhado na expressao posfixa
				pos[j++] = ' ';				//insere espaco apos operador
				x = desempilha();			//desempilha topo da pilha
			}
		} else if(e == ' '){				//se enccontra espaco
			if (pos[j-1] != ' ')			//insere espaco na expressao caso nao exista espaco na posicao anterior
				pos[j++] = e;
		} else {							//retorna expressao invalida
			printf("\nExpressao infixa invalida...");
			getchar();
			break;
		}
	}
	
	if (topo > 0){							//se topo eh maior do que 0 ao final do loop, retorna expressao invalida
		printf("\nExpressao infixa invalida...");
		getchar();
		exit(1);
	}
	
	pos[j] = '\0';							//insere caractere de fim da string
	
	topo = -1;
	
	return pos;
}

//imprimir arvore na ordem esq dir raiz
void imprimeArvore(PONT raiz){
	if(raiz != NULL){
		imprimeArvore(raiz->esq);
		imprimeArvore(raiz->dir);
		if(raiz->op != ' ') printf(" %c",raiz->op);
		else printf(" %d",raiz->num);
	}
}

//transforma expressao posfixa em arvore
PONT posfixa_arvore(char *pos){
	int num;			//num para operandos
	int i;				//variavel para iteracao
	PONT r = NULL, o, no;
	
	//itera ate o final da string
	for(i = 0; pos[i]; i++){
		if (pos[i] == ' ') continue; 					//se posicao iterada eh um espaco
		else if(isdigit(pos[i])){						//se posicao iterada eh um numero
			num = 0;									//inicializa variavel auxiliar
			while(isdigit(pos[i])){						//laco para extracao do valor inteiro
				num = num*10 + (int)(pos[i]-'0');		//soma cada digito multiplicando valor existente
				i++;									//avanca uma posicao na string
			}
			no = criaNovoNo(' ', num);
			empilhaArv(no);								//empilha valor
		}
		else if (pos[i] == '-' && isdigit(pos[i+1])){	//se encontra valor negativo
			num = 0;
			i++;
			while(isdigit(pos[i])){						//laco para extracao do valor inteiro
				num = num*10 + (int)(pos[i]-'0');		//soma cada digito multiplicando valor existente
				i++;									//avanca uma posicao na string
			}
			num = num * (-1);
			no = criaNovoNo(' ', num);
			empilhaArv(no);
		}
		else{
			o = criaNovoNo(pos[i],0);
			if (pos[i] == '#'){
				o->dir = desempilhaArv();
				o->esq = NULL;
			} else{
				o->dir = desempilhaArv(); //desempilha operando/operador 2
				o->esq = desempilhaArv(); //desempilha operando/operador 1
			}
			
			empilhaArv(o);
		}
	}
	
	r = desempilhaArv();		//desempilha raiz da pilha
	
	free(pos);
	
	topoArv = -1;
	
	return r;
}


// Algoritmo que calcula a arvore
int calcular(PONT node){

	// Verifica se o operador eh NULL, se sim retorna o numero do nodo
	if (node->op == ' ') {
        return node->num;
    }
	else if(node->op =='$'){ // Verifica se o operador = $, se sim realiza o calculo de maneira recursiva
             return pow(calcular(node->esq),calcular(node->dir));
    } else if (node->op=='#') { // Verifica se o operador = #, se sim realiza o calculo de maneira recursiva
    	 if ((int)sqrt(calcular(node->dir) > 0)){ // Validacao para raiz negativa
			return (int)sqrt(calcular(node->dir)); 
	} else {
		printf("Impossivel calcular raiz de numero negativo");
	}
}
    else if(node->op=='-'){ // Verifica se o operador = -, se sim realiza o calculo de maneira recursiva
    		if ((node->esq) && (node->dir) != NULL){
				return (calcular(node->esq)) - (calcular(node->dir));
        }
			if ( (node->esq) != NULL  && (node->dir) == NULL){
				return (calcular(node->esq) * (-1));
		}
			if ( (node->dir) != NULL && (node->esq) == NULL){
				return (calcular(node->dir) * (-1));
			}
		
    }else { 
        if(node->op=='+') // Verifica se o operador = +, se sim realiza o calculo de maneira recursiva
            return (calcular(node->esq)) + (calcular(node->dir));
        if(node->op=='*') // Verifica se o operador = *, se sim realiza o calculo de maneira recursiva
            return (calcular(node->esq)) * (calcular(node->dir));
        if(node->op=='/') // Verifica se o operador = /, se sim realiza o calculo de maneira recursiva
            return (calcular(node->esq)) / (calcular(node->dir));
	}
	
	return 0;
}


struct elemento{
   	PONT raiz;
    struct elemento *prox;
};
typedef struct elemento Elem;

struct fila{
    struct elemento *inicio;
    struct elemento *final;
    int qtd;
};
typedef struct fila Fila;

Fila* cria_Fila(){
    Fila* fi = (Fila*) malloc(sizeof(Fila));
    if(fi != NULL){
        fi->final = NULL;
        fi->inicio = NULL;
        fi->qtd = 0;
    }
    return fi;
}

void libera_Fila(Fila* fi){
    if(fi != NULL){
        Elem* no;
        while(fi->inicio != NULL){
            no = fi->inicio;
            fi->inicio = fi->inicio->prox;
            free(no);
        }
        free(fi);
    }
}

int consulta_Fila(Fila* fi, PONT raiz){
    if(fi == NULL)
        return 0;
    if(fi->inicio == NULL)//fila vazia
        return 0;
    raiz = fi->inicio->raiz;
    return 1;
}

int insere_Fila(Fila* fi, PONT raiz){
    if(fi == NULL)
        return 0;
    Elem *no = (Elem*) malloc(sizeof(Elem));
    if(no == NULL)
        return 0;
    no->raiz = raiz;
    no->prox = NULL;
    if(fi->final == NULL)//fila vazia
        fi->inicio = no;
	else
        fi->final->prox = no;
    fi->final = no;
    fi->qtd++;
    return 1;
}

int remove_Fila(Fila* fi){
    if(fi == NULL)
        return 0;
    if(fi->inicio == NULL)//fila vazia
        return 0;
    Elem *no = fi->inicio;
    fi->inicio = fi->inicio->prox;
    if(fi->inicio == NULL)//fila ficou vazia
        fi->final = NULL;
    free(no);
    fi->qtd--;
    return 1;
}

void imprime_Fila(Fila* fi){
    if(fi == NULL)
        return;
    Elem* no = fi->inicio;
    while(no != NULL){
        printf("\nExpressao: ");
        imprimeArvore(no->raiz);
        printf("\nResultado: %d",calcular(no->raiz));
        libera_NO(no->raiz);
        printf("\nAlocacao liberada");
        printf("\n-------------------------------\n");
        no = no->prox;
    }
}

void main(){

	PONT arv = NULL;
	char infixa[N];
	char *pos;
	int tam, valido;
	int result = 0;
	Fila* fi = cria_Fila();
	
	printf("+ Soma\t\t\t- Subtracao\n* Multiplicacao\t\t/ Divisao\n$ Exponenciacao\t\t# Raiz quadrada\n");
	
	do{
		printf("\nExpressao infixa ou S para sair: ");
		fgets(infixa, N, stdin);
		fflush(stdin);
		if(infixa[0] == 'S')
			break;
		tam = strlen(infixa);
		infixa[tam-1] = '\0';
		
		if(validaExp(infixa) == 1){
			pos = posfixa(infixa);
		
			arv = posfixa_arvore(pos);
		
			insere_Fila(fi,arv);
		} else printf("\nExpressao %s invalida",infixa);
		
	}while(1);
	printf("\n");
	imprime_Fila(fi);
	libera_Fila(fi);
	printf("\nFila liberada!\n");
	
	system("pause");
}
