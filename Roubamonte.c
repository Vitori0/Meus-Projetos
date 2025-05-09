#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// ---------------------------------- STRUCTS E INICIALIZAÇÕES ---------------------------------- //

int r = 0; // variável retorno
typedef struct carta{
    int numero; // numero da carta
    int naipe; // naipe da carta, cada naipe é representado de 1 a 4;
    struct carta *ponteiro; // apontar pra PRÓXIMO caso for LISTA ou FILA // apontar pra ANTERIOR caso for PILHA
}carta;

typedef struct monte_do_jogador{ // PILHA DINÂMICA
    carta *topo;
}monte_do_jogador;
monte_do_jogador *criamonte() { // INICIALIZA MONTE DO JOGADOR
    monte_do_jogador *mdj; // monte do jogador
    mdj = (monte_do_jogador*)malloc(sizeof(monte_do_jogador));
    mdj->topo = NULL;
    return mdj;
}

typedef struct pilha_de_compra{ // PILHA DINÂMICA
    carta *topo;
}pilha_de_compra;
pilha_de_compra *criapilha() { // INICIALIZA PILHA DE COMPRA
    pilha_de_compra *pdc; // pilha de compra
    pdc = (pilha_de_compra*)malloc(sizeof(pilha_de_compra));
    pdc->topo = NULL;
    return pdc;
}

typedef struct area_de_descarte{ // LISTA ENCADEADA
    carta *inicio;
}area_de_descarte;
area_de_descarte *crialista() { // INICIALIZA ÁREA DE DESCARTE
    area_de_descarte *add; // área de descarte
    add = (area_de_descarte*)malloc(sizeof(area_de_descarte));
    add->inicio = NULL;
    return add;
}

typedef struct carta_da_vez{ // FILA DINÂMICA
    carta *inicio;
    carta *final; // não será utilizado
}carta_da_vez;
carta_da_vez *criafila() { // INICIALIZA CARTA DA VEZ
    carta_da_vez *cdv; // carta da vez
    cdv = (carta_da_vez*)malloc(sizeof(carta_da_vez));
    cdv->inicio = NULL;
    cdv->final = NULL;
    return cdv;
}

typedef struct jogador{
    char nome[15]; // nome do jogador
    monte_do_jogador *monte; // monte do jogador (PILHA DINÂMICA)
    int quantidade_de_cartas; // quantidade de cartas do jogador
}jogador;

// -------------------------------------------- FUNÇÕES -------------------------------------------- //

void randomizar_pilha_de_compra(pilha_de_compra *pdc, int quantidade_cartas) {
    int nai = 1; // espadas(♠)
    int num = 1; // numero
    int i;
    for (i = 0; i < quantidade_cartas * 4; i++) { // ordenando as cartas
        carta *carta_nova = (carta*)malloc(sizeof(carta));
        carta_nova->naipe = nai;
        carta_nova->numero = num;
        carta_nova->ponteiro = pdc->topo;
        pdc->topo = carta_nova;
        num++;
        if (num - 1 == quantidade_cartas) {
            num = 1;
        }
        if (i + 1 == quantidade_cartas) {
            nai = 2; // paus(♣)
        }
        else if (i + 1 == quantidade_cartas * 2) {
            nai = 3; // copas(♥)
        }
        else if (i + 1 == quantidade_cartas * 3) {
            nai = 4; // ouro(♦)
        }
    }
    carta *aux; // ponteiro auxiliar
    carta *aux2; // ponteiro auxiliar 2
    int j = 0;
    int guarda_numero;
    int guarda_naipe;
    int random; // número randômico
    aux = pdc->topo;
    srand(time(NULL));
    while (aux != NULL) { // randomizar as cartas
        random = rand()%(quantidade_cartas*4);
        aux2 = pdc->topo;
        while (j != random) {
            aux2 = aux2->ponteiro;
            j++;
        }
        guarda_numero = aux->numero;
        guarda_naipe = aux->naipe;
        aux->numero = aux2->numero;
        aux->naipe = aux2->naipe;
        aux2->numero = guarda_numero;
        aux2->naipe = guarda_naipe;
        aux = aux->ponteiro;
        j = 0;
    }
}

void retirar_cdv (pilha_de_compra *pdc, carta_da_vez *cdv) {
    carta *nova_cdv = (carta*)malloc(sizeof(carta)); 
    carta *remove_topo_pdc = (carta*)malloc(sizeof(carta));     //  (
    cdv->inicio = nova_cdv;                                     //  |
    nova_cdv->numero = pdc->topo->numero;                       //  |       passando carta topo da pilha de compra para a carta da vez
    nova_cdv->naipe = pdc->topo->naipe;                         // <        após isso, a carta topo da pilha de compra passa a ser a anterior
    remove_topo_pdc = pdc->topo;                                //  |       e a carta topo antiga, é liberada da memória
    pdc->topo = pdc->topo->ponteiro;                            //  |
    free(remove_topo_pdc);                                      //  (
}

void imprime_cdv(carta_da_vez *cdv) { // Mostrar carta da vez
    printf ("\nCARTA DA VEZ:\n");
    if (cdv->inicio->naipe == 1) {
        printf ("(ESPADAS) (%d)\n" ,cdv->inicio->numero);
    }
    else if (cdv->inicio->naipe == 2) {
        printf ("(PAUS) (%d)\n" ,cdv->inicio->numero);
    }
    else if (cdv->inicio->naipe == 3) {
        printf ("(COPAS) (%d)\n" ,cdv->inicio->numero);
    }
    else if (cdv->inicio->naipe == 4) {
        printf ("(OURO) (%d)\n" ,cdv->inicio->numero);
    }
}

void imprime_mdj(jogador j[], int quantidade_jogadores) { // mostrar monte dos jogadores
    quantidade_jogadores--;
    int i;
    printf ("\nCARTA DE CIMA DO MONTE DOS JOGADORES:\n");
    for (i = 0; i <= quantidade_jogadores; i++) {
        if (j[i].monte->topo == NULL) { // se o topo do monte do jogador for NULL, monte vazio
            printf ("[%d] %s ---> MONTE VAZIO\n" ,i ,j[i].nome);
        }
        else {
            if (j[i].monte->topo->naipe == 1) {
                printf ("[%d] %s ---> %d Cartas | Carta topo -> (ESPADAS) (%d)\n" ,i ,j[i].nome ,j[i].quantidade_de_cartas ,j[i].monte->topo->numero);
            }
            else if (j[i].monte->topo->naipe == 2) {
                printf ("[%d] %s ---> %d Cartas | Carta topo -> (PAUS) (%d)\n" ,i ,j[i].nome ,j[i].quantidade_de_cartas ,j[i].monte->topo->numero);
            }
            else if (j[i].monte->topo->naipe == 3) {
                printf ("[%d] %s ---> %d Cartas | Carta topo -> (COPAS) (%d)\n" ,i ,j[i].nome ,j[i].quantidade_de_cartas ,j[i].monte->topo->numero);
            }
            else if (j[i].monte->topo->naipe == 4) {
                printf ("[%d] %s ---> %d Cartas | Carta topo -> (OURO) (%d)\n" ,i ,j[i].nome ,j[i].quantidade_de_cartas ,j[i].monte->topo->numero);
            }
        }
    }
}//Código muito organizado, mostra certinho as cartas do topo de cada jogador, avisa quando o monte tá vazio e ainda é fácil de entender.

void imprime_add(area_de_descarte *add) { // mostrar cartas na área de descarte
    printf ("\nAREA DE DESCARTE:\n");
    if (add->inicio == NULL) {
        printf ("AREA DE DESCARTE VAZIA\n");
    }
    else {
        carta *aux;
        aux = add->inicio;
        while (aux != NULL) {
            if (aux->naipe == 1) {
                printf ("(ESPADAS) (%d)\n" ,aux->numero);
            }
            else if (aux->naipe == 2) {
                printf ("(PAUS) (%d)\n" ,aux->numero);
            }
            else if (aux->naipe == 3) {
                printf ("(COPAS) (%d)\n" ,aux->numero);
            }
            else if (aux->naipe == 4) {
                printf ("(OURO) (%d)\n" ,aux->numero);
            }
        aux = aux->ponteiro;
        }
    }
}

int empilhar_add(jogador j[], carta_da_vez *cdv, area_de_descarte *add, int vez) { // COLOCAR AS CARTAS DA AREA DE DESCARTE E CARTA DA VEZ NO MONTE DO JOGADOR
    carta *libera_add = (carta*)malloc(sizeof(carta)); // ponteiro para liberar carta da area de descarte
    carta *libera_cdv = (carta*)malloc(sizeof(carta)); // ponteiro para liberar carta da vez
    carta *mdj_nova_add = (carta*)malloc(sizeof(carta)); // carta vindo da add
    carta *mdj_nova_cdv = (carta*)malloc(sizeof(carta)); // carta vindo da cdv
    carta *aux; // ponteiro auxiliar
    carta *aux2 = add->inicio; // liberar add quando estiver no meio ou final
    aux = add->inicio;
    while (aux != NULL) {
        if (aux->numero != cdv->inicio->numero) {   // aux vai parar uma carta antes 
            if (aux == NULL) {
                return 0; // CARTA NÃO ENCONTRADA
            }
            aux = aux->ponteiro;
        }
        else {
            mdj_nova_add->naipe = aux->naipe;               //      CARTA DA AREA DE DESCARTE
            mdj_nova_add->numero = aux->numero;             //                  |
            mdj_nova_add->ponteiro = j[vez].monte->topo;    //                  V
            j[vez].monte->topo = mdj_nova_add;              //          MONTE DO JOGADOR
            
            mdj_nova_cdv->naipe = cdv->inicio->naipe;       //            CARTA DA VEZ
            mdj_nova_cdv->numero = cdv->inicio->numero;     //                  |
            mdj_nova_cdv->ponteiro = j[vez].monte->topo;    //                  V
            j[vez].monte->topo = mdj_nova_cdv;              //          MONTE DO JOGADOR
        
            if (aux == add->inicio){ // se a carta da add estiver no começo
                add->inicio = aux->ponteiro;                                          
                libera_add = aux;                                                        
                free(libera_add);                                          
            }                                                                                   
            else if (aux != add->inicio && aux != NULL) { // se a carta da add estiver no meio
                while (aux2->ponteiro != aux) {
                    aux2 = aux2->ponteiro;
                }
                aux2->ponteiro = aux->ponteiro;
                libera_add = aux;                                                      
                free(libera_add);                    
            }                                                                 
            else { // se a carta da add estiver no final                                        
                aux2 = add->inicio;
                while (aux2->ponteiro != aux) {
                    aux2 = aux2->ponteiro;
                }
                aux2->ponteiro = NULL;                                              
                libera_add = aux;                                                  
                free(libera_add);                                                     
            }

            libera_cdv = cdv->inicio;   //  (
            cdv->inicio = NULL;         //  <   liberar carta da vez
            free(libera_cdv);           //  (
            
            j[vez].quantidade_de_cartas += 2; // +2 cartas no monte do jogador
            return 1; // CARTA EMPILHADA
        }
    }
}

int roubar_mdj(jogador j[], carta_da_vez *cdv, int vez, int roubar) {
    carta *aux;
    if (j[roubar].monte->topo == NULL) {
        return 0; // Monte vazio
    }
    else {
        if (cdv->inicio->numero == j[roubar].monte->topo->numero) { // se a carta da vez for igual a do topo do jogador em que deseja roubar
            while (j[roubar].monte->topo != NULL) { // roubar o monte do jogador selecionado, carta por carta
                j[vez].quantidade_de_cartas++; // aumentando uma carta no monte do jogador que jogando
                j[roubar].quantidade_de_cartas--;   // diminuindo uma carta do monte do jogador que está sendo roubado
                    if (j[roubar].monte->topo->ponteiro == NULL) {              
                        j[roubar].monte->topo->ponteiro = j[vez].monte->topo;   //  (       se estiver na última carta
                        j[vez].monte->topo = j[roubar].monte->topo;             //  <       do monte do jogador que
                        j[roubar].monte->topo = NULL;                           //  (       está sendo roubado
                        j[roubar].quantidade_de_cartas--;
                        break;
                    }
                aux = j[vez].monte->topo;
                j[vez].monte->topo = j[roubar].monte->topo;
                j[roubar].monte->topo = j[roubar].monte->topo->ponteiro;
                j[vez].monte->topo->ponteiro = aux;
            }
        cdv->inicio->ponteiro = j[vez].monte->topo;
        j[vez].monte->topo = cdv->inicio;
        j[vez].quantidade_de_cartas++;
        cdv->inicio = NULL;
        return 1; // MONTE ROUBADO
        }
        else {
            return 0; // se a carta da vez for diferente a do topo do jogador em que deseja roubar
        }
    }
}

int empilhar_cdv (jogador j[], carta_da_vez *cdv, int vez) {
    if (j[vez].monte->topo == NULL) {
        return 0; // Monte vazio
    }
    else {
        if (cdv->inicio->numero == j[vez].monte->topo->numero) {
            j[vez].quantidade_de_cartas++;
            carta *nova_cdv = (carta*)malloc(sizeof(carta)); // colocar cdv no topo do monte do jogador
            nova_cdv->numero = cdv->inicio->numero;
            nova_cdv->naipe = cdv->inicio->naipe;
            nova_cdv->ponteiro = j[vez].monte->topo;
            j[vez].monte->topo = nova_cdv;
            carta *libera_cdv = (carta*)malloc(sizeof(carta)); // ponteiro para liberar carta da vez
            libera_cdv = cdv->inicio;
            cdv->inicio = NULL;
            free(libera_cdv);
            return 1; // CARTA DA VEZ EMPILHADA
        }
        else {
            return 0; // se a carta da vez for diferente a do topo de seu próprio monte
        }   
    }
}

void colocar_add(carta_da_vez *cdv, area_de_descarte *add) {
    if (add->inicio == NULL) { // se add estiver vazia
        add->inicio = cdv->inicio;
        add->inicio->ponteiro = NULL;
        cdv->inicio = NULL;
    }
    else { // se add não estiver vazia
        cdv->inicio->ponteiro = add->inicio;
        add->inicio = cdv->inicio;
        cdv->inicio = NULL;
    }
}

// --------------------------------------------- MAIN --------------------------------------------- //

int main () {
    
    pilha_de_compra *pdc = criapilha();
    area_de_descarte *add = crialista();
    carta_da_vez *cdv = criafila();
    int quantidade_jogadores, quantidade_cartas;
    
    printf ("Bem vindo ao jogo rouba-montes!!\n");
    printf ("Quantos jogadores irao participar?: ");
    scanf ("%d" ,&quantidade_jogadores); // quantidade de jogadores
    jogador j[quantidade_jogadores-1]; // vetor onde cada posição é de uma struct jogador
    printf ("Com quantas cartas por naipe pretende jogar? (MAXIMO 13): ");
    scanf ("%d" ,&quantidade_cartas);
    printf ("\n");
    for (int i = 0; i < quantidade_jogadores; i++) {
        printf("Nome do jogador (%d): " ,i+1);
        getchar();
        scanf("%s" ,j[i].nome);
        j[i].quantidade_de_cartas = 0;
        j[i].monte = criamonte();
    }
    randomizar_pilha_de_compra(pdc, quantidade_cartas); // desembaralhar cartas
    
    printf ("\nIniciando jogo rouba-montes"); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000); printf (".\n"); Sleep(1000);
    int vez = 0; // variável para controlar a vez do jogador
    int escolha;
    while (pdc->topo != NULL) { // Enquanto pilha de compra não estiver vazia, ou seja, enquanto o jogo não acabar
        printf ("Vez do(a) %s:" ,j[vez].nome);
        printf ("\nRetirando carta da vez"); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000);
        retirar_cdv(pdc, cdv); // Função retirar carta da vez
        printf ("\n\nCartas na Mesa: \n");
        Sleep(2000);
        imprime_cdv(cdv); // Função imprimir carta da vez
        Sleep(2000);
        imprime_mdj(j, quantidade_jogadores); // Função imprimir monte dos jogadores
        Sleep(2000);
        imprime_add(add); // Função imprimir área de descarte
        Sleep(2000);
        for (int z = 0; z >= 0; z++) { // loop infinito até acabar a vez do jogador
            printf ("\n[1] - Retirar carta da area de descarte\n[2] - Roubar monte do jogador\n[3] - Colocar carta no topo do proprio monte\n[4] - Colocar carta na area de descarte e encerrar a jogada\n");
            printf ("\nEscolha uma jogada: ");
            scanf ("%d" ,&escolha);
            if (escolha == 1 || escolha == 2 || escolha == 3 || escolha == 4) {
                if (escolha == 1) { // retirar carta da add e cdv
                    printf ("Retirando carta da vez e da area de descarte"); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000);
                    r = empilhar_add(j, cdv, add, vez);
                    if (r == 0) {
                        printf ("\nJogada invalida!!!\n"); Sleep(2000);
                    }
                    else {
                        printf ("\nCarta da vez e da area de descarte retirada\n"); Sleep(2000);
                        break;
                    }
                }
                else if (escolha == 2) {  // roubar monte do j
                    int roubar;
                    printf ("Escolha o numero do jogador que deseja roubar o monte: ");
                    scanf ("%d" ,&roubar); 
                    printf ("Roubando monte do jogador"); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000);
                    r = roubar_mdj(j, cdv, vez, roubar);
                    if (r == 0) {
                        printf ("\nJogada invalida!!!\n"); Sleep(2000);
                    }
                    else {
                        printf ("\nMonte do jogador roubado\n"); Sleep(2000);
                        break;
                    }
                }
                else if (escolha == 3) { // cdv no proprio monte
                    printf ("Empilhando carta da vez no proprio monte"); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000);
                    r = empilhar_cdv(j, cdv, vez);
                    if (r == 0) {
                        printf ("\nJogada invalida!!!\n"); Sleep(2000);
                    }
                    else {
                        printf ("\nCarta da vez empilhada no proprio monte\n"); Sleep(2000);
                        break;
                    }
                }
                else { // cdv na add e encerrar a vez
                    printf ("Colocando carta da vez na area de descarte"); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000);
                    colocar_add(cdv, add);
                     printf ("\nCarta da vez colocada na area de descarte\n"); Sleep(2000);
                    vez++; // passando a vez
                    if (vez == quantidade_jogadores) {  // quando chegar no último jogador, volta para o primeiro
                        vez = 0;
                    }
                    break;
                }
            }
            else {
                printf ("\nOPCAO DE ESCOLHA INVALIDA!!!\n");
            }
        }
    system("cls");
    }
    int quantidade_maior_carta;
    for (vez = 0; vez < quantidade_jogadores; vez++) {
        if (vez == 0) {
            quantidade_maior_carta = j[vez].quantidade_de_cartas;
        }
        else {
            if (j[vez].quantidade_de_cartas > quantidade_maior_carta) {
                quantidade_maior_carta = j[vez].quantidade_de_cartas;
            }
        }
    }
    printf ("E o(s) vencedor(es) e(sao):"); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000);
    for (vez = 0; vez < quantidade_jogadores; vez++) {
        if (j[vez].quantidade_de_cartas == quantidade_maior_carta) {
            printf ("\nPARABENS!!! %s\n" ,j[vez].nome); // printando os jogadores que venceram o jogo
        }
    }
    printf ("Encerrando o jogo roubamontes"); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000); printf ("."); Sleep(1000);
    return 0; // encerrando programa
}
