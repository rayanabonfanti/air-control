#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 100

int encontrouNumeroVooRepetido(FILE *arquivoleituravoo, char numero_voo_repetido[TAM_MAX]);
void funcao_menu();
int verificaHoraVoo(char hora[TAM_MAX]);
int funcao_verifica_data_voo(char data[TAM_MAX]);
int contadorReserva(FILE *arquivoleiturareserva, char numero_voo_ocupacao[TAM_MAX]);
int quantidadeMaxPassageiros(FILE *arquivoleituravoo, char numero_voo_ocupacao[TAM_MAX]);
int verificaCPFNumeroVooNaReserva(FILE *arquivoleiturareserva, char numero_voo[TAM_MAX], char cpf[TAM_MAX]);

typedef struct
{
    char numero_do_voo[TAM_MAX];
    int qntd_max_passageiros; 
    char data_partida[TAM_MAX];
    char hora_partida[TAM_MAX];
    int status_voo; 
}voo;

typedef struct
{
    char numero_da_reserva[TAM_MAX];
    char numero_do_voo_reserva[TAM_MAX];
    char cpf_passageiro[TAM_MAX];
    char nome_completo_passageiro[TAM_MAX];
    char sexo; 
    char data_nascimento[TAM_MAX];
    int status_reserva; 
}reserva;

int main()
{
    int opcao; 
    char numero_voo_ocupacao[TAM_MAX]; 
    FILE *arquivoescritavoo; 
    FILE *arquivoleituravoo; 
    FILE *arquivoescritareserva; 
    FILE *arquivoleiturareserva;
    char *nomearquivovoo = "voo.dat"; 
    char *nomearquivoreserva = "reserva.dat"; 
    char *modoleitura = "rb"; 
    char *modogravacao = "ab"; 
    voo voo1; 
    voo voo2; 
    reserva reserva1; 
    reserva reserva2; 

    do 
    {
        funcao_menu(); 
        scanf("%d", &opcao); 
        switch(opcao) 
        {
            case 1: 
                system("cls"); 
                fflush(stdin); 
                printf("--------------------------------------------------\nDigite o numero do voo: ");
                gets(voo1.numero_do_voo); 
                fflush(stdin);
                printf("Digite a quantidade maxima de passageiros: ");
                scanf("%d", &voo1.qntd_max_passageiros);
                fflush(stdin);
                printf("Digite a data da partida: ");
                gets(voo1.data_partida);
                printf("Digite a hora da partida: ");
                gets(voo1.hora_partida);
                voo1.status_voo = 1; 

                if(verificaHoraVoo(voo1.hora_partida) == 1) 
                {
                        if(encontrouNumeroVooRepetido(arquivoleituravoo, voo1.numero_do_voo) == 1) 
                        {
                            printf("Numero do voo repetido (ou seja ja existe no arquivo), nao foi possivel cadastrar voo!\n");
                        }
                        else
                        {
                            arquivoescritavoo = fopen(nomearquivovoo,modogravacao);
                            if(arquivoescritavoo != NULL)
                            {
                                if(fwrite(&voo1,1,sizeof(voo),arquivoescritavoo))
                                {
                                    printf("Cadastro realizado com sucesso!\n");
                                }
                                else
                                {
                                    printf("N�o conseguiu gravar no arquivo os dados!\n");
                                }
                            }
                            else
                            {
                                printf("--------------------------------------------------\nErro ao criar o arquivo!\n");
                            }
                        }
                }
                else
                {
                    printf("Hora ou Data ou Numero do Voo incorreta, tente cadastrar novamente mais tarde\n");
                }
                fclose(arquivoescritavoo);
            break;
            case 2:
                system("cls");
                fflush(stdin);
                printf("digite o numero da reserva: ");
                gets(reserva1.numero_da_reserva);
                fflush(stdin);
                printf("digite o numero do voo da reserva: ");
                gets(reserva1.numero_do_voo_reserva);
                fflush(stdin);
                printf("digite o seu cpf: ");
                gets(reserva1.cpf_passageiro);
                fflush(stdin);
                printf("digite o seu nome completo: ");
                gets(reserva1.nome_completo_passageiro);
                fflush(stdin);
                printf("digite a letra correspondente ao seu sexo: (M - masculino ; F - Feminino ; O - Outros.) ");
                scanf("%c",&reserva1.sexo);
                fflush(stdin);
                printf("digite sua data de nascimento: ");
                gets(reserva1.data_nascimento);
                reserva1.status_reserva = 1;

                if(toupper(reserva1.sexo)=='M' || toupper(reserva1.sexo)=='F' || toupper(reserva1.sexo)=='O')
                {
                            if(encontrouNumeroVooRepetido(arquivoleituravoo, reserva1.numero_do_voo_reserva) == 1)
                            {
                                if(verificaCPFNumeroVooNaReserva(arquivoleiturareserva,reserva1.numero_do_voo_reserva,reserva1.cpf_passageiro) == 0)
                                {
                                    arquivoescritareserva = fopen(nomearquivoreserva,modogravacao);
                                    if(arquivoescritareserva != NULL)
                                    {
                                        if(fwrite(&reserva1,1,sizeof(reserva),arquivoescritareserva))
                                        {
                                            printf("Cadastro realizado com sucesso!\n");
                                        }
                                        else
                                        {
                                            printf("N�o conseguiu gravar no arquivo os dados!\n");
                                        }
                                    }
                                    else
                                    {
                                        printf("--------------------------------------------------\nErro ao criar o arquivo!\n");
                                    }
                                }
                                else
                                {
                                    printf("Ja existe uma pessoa com reserva ativa no mesmo cpf e no mesmo numero do voo, entao nao pode cadastrar reserva!\n");
                                    break;
                                }
                            }
                            else
                            {
                                printf("Numero do voo nao foi encontrado, nao foi possivel cadastrar reserva!\n");
                            }
                }
                else
                {
                    printf("digitou sexo errado, nao tem como cadastrar a reserva\n");
                }

                fclose(arquivoescritareserva);
            break;
            case 3:
                system("cls");
                fflush(stdin);
                printf("Digite o numero do voo que quer consultar: ");
                gets(numero_voo_ocupacao);
                int qtdPasseiros,contReserva;
                qtdPasseiros = quantidadeMaxPassageiros(arquivoleituravoo,numero_voo_ocupacao);
                contReserva = contadorReserva(arquivoleiturareserva,numero_voo_ocupacao);
                float percOcupacao = (float) (contReserva*100)/qtdPasseiros;
                printf("Percentual de Ocupacao: %.2f %\n",percOcupacao);
                printf("Lista de Passageiros:\n");
                listaPassageiros(arquivoleiturareserva,numero_voo_ocupacao);
            break;
            case 4:
                system("cls");
                printf("entrou no case 4\n");
            break;
            case 5:
                system("cls");
                printf("entrou no case 5\n");
            break;
            case 6:
                system("cls");
                printf("entrou no case 6\n");
            break;
            case 7:
                system("cls");
                printf("entrou no case 7\n");
            break;
            case 8:
                system("cls");
                printf("entrou no case 8\n");
            break;
            case 9:
                printf("Voce saiu do programa!\n");
            break;
            default:
                system("cls");
                printf("Opcao Invalida!\n");
        }
    }while (opcao != 9); 

    return 0;
}

void funcao_menu()
{
    printf("------------------Menu de Opcoes------------------\n");
    printf("1) Cadastrar voo\n");
    printf("2) Cadastrar reserva\n");
    printf("3) Consultar voo\n");
    printf("4) Consultar reserva\n");
    printf("5) Consultar passageiro\n");
    printf("6) Cancelar voo\n");
    printf("7) Cancelar reserva\n");
    printf("8) Excluir voo\n");
    printf("9) Sair do programa\n");
}

int verificaHoraVoo(char hora[TAM_MAX]) 
{
    
    if(strlen(hora) != 5 || hora[2] != ':' || isdigit(hora[0]) == 0 || isdigit(hora[1]) == 0 || isdigit(hora[3]) == 0 || isdigit(hora[4]) == 0)
    {
        return 0; 
    }
    else
    {
        return 1; 
    }
}

int encontrouNumeroVooRepetido(FILE *arquivoleituravoo, char numero_voo_repetido[TAM_MAX])
{
                voo voo2;
                arquivoleituravoo = fopen("voo.dat","rb"); 

                while(fread(&voo2,1,sizeof(voo),arquivoleituravoo)) 
                {
                    if(strstr(numero_voo_repetido,voo2.numero_do_voo)!=NULL) 
                    {
                        return 1; 
                        fclose(arquivoleituravoo);
                    }
                }
                return 0; 
                fclose(arquivoleituravoo);
}

int quantidadeMaxPassageiros(FILE *arquivoleituravoo, char numero_voo_ocupacao[TAM_MAX])
{
                voo voo2;
                arquivoleituravoo = fopen("voo.dat","rb"); 

                while(fread(&voo2,1,sizeof(voo),arquivoleituravoo)) 
                {
                    if(strstr(numero_voo_ocupacao,voo2.numero_do_voo)!=NULL) 
                    { 
                        return voo2.qntd_max_passageiros; 
                        fclose(arquivoleituravoo);
                    }
                }
                return 0; 
                fclose(arquivoleituravoo);
}

int contadorReserva(FILE *arquivoleiturareserva, char numero_voo_ocupacao[TAM_MAX])
{
                reserva reserva2;
                int contador = 0;
                arquivoleiturareserva = fopen("reserva.dat","rb"); 

                while(fread(&reserva2,1,sizeof(reserva),arquivoleiturareserva)) 
                {
                    if(strstr(numero_voo_ocupacao,reserva2.numero_do_voo_reserva)!=NULL) 
                    { 
                        if(reserva2.status_reserva == 1) 
                        {
                            contador++;
                        }
                    }
                }
                return contador; 
                fclose(arquivoleiturareserva);
}

void listaPassageiros(FILE *arquivoleiturareserva, char numero_voo_ocupacao[TAM_MAX])
{
                reserva reserva2;
                arquivoleiturareserva = fopen("reserva.dat","rb"); 

                while(fread(&reserva2,1,sizeof(reserva),arquivoleiturareserva)) 
                {
                    if(strstr(numero_voo_ocupacao,reserva2.numero_do_voo_reserva)!=NULL) 
                    { 
                        if(reserva2.status_reserva == 1) 
                        {
                            printf("%s;\n",reserva2.nome_completo_passageiro); 
                        }
                    }
                }
                fclose(arquivoleiturareserva);
}

int verificaCPFNumeroVooNaReserva(FILE *arquivoleiturareserva, char numero_voo[TAM_MAX], char cpf[TAM_MAX])
{
                reserva reserva2;
                arquivoleiturareserva = fopen("reserva.dat","rb"); 

                while(fread(&reserva2,1,sizeof(reserva),arquivoleiturareserva)) 
                {
                    if(strstr(numero_voo,reserva2.numero_do_voo_reserva)!=NULL) 
                    { 
                        if(strstr(cpf,reserva2.cpf_passageiro)!=NULL) 
                        {
                            if(reserva2.status_reserva == 1) 
                            {
                                return 1; 
                                fclose(arquivoleiturareserva);
                            }
                        }
                    }
                }
                return 0; 
                fclose(arquivoleiturareserva);
}

int funcao_verifica_data_voo(char data[TAM_MAX])
{
	char dia[2];
	char mes[2];
	char ano[4];

	if(isdigit(data) == 0 || strlen(data) != 10 || data[2] != '/' || data[5] != '/')
	{
		return 0;
	}
	else
	{
		if (mes == "01" || mes == "03" || mes == "05" || mes == "07" || mes == "09" || mes == "11" || mes == "12" )
        {
            if (dia >= "01" && dia <= "31")
            {
                if (ano >= "2030")
                {
                    return 1; 
                }
                else
                {
                    return 0; 
                }
            }
            else
            {
                return 0; 
            }
		}
        else
        {
            if (mes == "04" || mes == "06" || mes == "08" || mes == "10")
            {
                if (dia >= "01" && dia <= "30")
                {
                    if (ano >= "2030")
                    {
                         return 1;  
                    }
                    else
                    {
                        return 0; 
                    }
                }
                else
                {
                    return 0; 
                }
            }
            else
            {
                if (mes == "02")
                {
                    if (ano >= "2030")
                    {
                        if (dia >= "01" && dia <= "29")
                        {
                            return 1; 
                        }
                        else
                        {
                            return 0; 
                        }
                    }
                    else
                    {
                        if (dia >= "1" && dia <= "28")
                        {
                            if (ano >= "2030")
                            {
                                return 1; 
                            }
                            else
                            {
                                return 0; 
                            }
                        }
                        else
                        {
                            return 0; 
                        }
                    }
                }
            }
        }
    }
}


