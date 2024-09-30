#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#define LINHAS 10
#define COLUNAS 10
#define TAMANHO_NOME 50
#define TAMANHO_SENHA 20

// CRIANDO A ESTRUTURA DA CONTA BANCÁRIA
typedef struct
{
    int conta_numero;
    char titular[TAMANHO_NOME];
    char senha[TAMANHO_SENHA];
    float saldo;
} ContaBancaria;

// CRIANDO A ESTRUTURA DA TRANSAÇÃO
typedef struct
{
    int conta_origem;
    int conta_destino;
    float valor;
    char data[11];
} Transacao;

ContaBancaria banco[LINHAS][COLUNAS] = {{{0}}}; // INICIALIZA TODOS OS VALORES DA MATRIZ COM 0
int ultimo_numero = 0;                          // REGISTRA O ÚLTIMO O NÚMERO DA ULTIMA CONTA CRIADA

// LE AS CONTAS DO TXT E GUARDA NA MATRIX
void ler_todas_as_contas(const char *nome_arquivo)
{
    FILE *arq = fopen(nome_arquivo, "r");
    if (arq == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[500]; // BUFFER PARA ARMAZENAR CADA LINHA DO ARQUIVO
    int lin = 0, col = 0;
    while (lin < LINHAS && fgets(linha, sizeof(linha), arq) != NULL) // LÊ CADA LINHA DO ARQUIVO
    {
        if (sscanf(linha, "NÚMERO DA CONTA: %d", &banco[lin][col].conta_numero) == 1) // VERIFICA SE A LINHA É O NÚMERO DA CONTA
        {
            fgets(linha, sizeof(linha), arq);
            sscanf(linha, "TITULAR DA CONTA: %[^\n]", banco[lin][col].titular); // ARMAZENA O TITULAR DA CONTA
            fgets(linha, sizeof(linha), arq);
            sscanf(linha, "SENHA DA CONTA: %[^\n]", banco[lin][col].senha); // ARMAZENA A SENHA DA CONTA
            fgets(linha, sizeof(linha), arq);
            sscanf(linha, "SALDO DA CONTA: %f", &banco[lin][col].saldo); // ARMAZENA O SALDO DA CONTA

            if (banco[lin][col].conta_numero > ultimo_numero) // VERIFICA SE O NÚMERO DA CONTA É MAIOR QUE O ÚLTIMO NÚMERO
                ultimo_numero = banco[lin][col].conta_numero; // ATUALIZA O ÚLTIMO NÚMERO DA CONTA

            if (++col >= COLUNAS) // VERIFICA SE CHEGOU AO FIM DA COLUNA
            {
                col = 0; // REINICIA A COLUNA
                lin++;   // PASSA PARA A PRÓXIMA LINHA
            }
        }
    }
    fclose(arq);
}

void atualizar_contas_no_arquivo()
{
    FILE *arq = fopen("contas.txt", "w");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo para atualização\n");
        return;
    }

    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (banco[i][j].conta_numero != 0) // VERIFICA SE A CONTA EXISTE
            {
                fprintf(arq, "NÚMERO DA CONTA: %i\n", banco[i][j].conta_numero); // ATUALIZA O NÚMERO DA CONTA
                fprintf(arq, "TITULAR DA CONTA: %s\n", banco[i][j].titular);     // ATUALIZA O TITULAR DA CONTA
                fprintf(arq, "SENHA DA CONTA: %s\n", banco[i][j].senha);         // ATUALIZA A SENHA DA CONTA
                fprintf(arq, "SALDO DA CONTA: %.2f\n\n", banco[i][j].saldo);     // ATUALIZA O SALDO DA CONTA
            }
        }
    }
    fclose(arq);
}

// REGISTRA A CONTA NO TXT
void registrar_conta(ContaBancaria conta)
{
    FILE *arq = fopen("contas.txt", "a"); // ABRE O ARQUIVO PARA ADICIONAR NO FINAL
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arq, "NÚMERO DA CONTA: %i\n", conta.conta_numero);
    fprintf(arq, "TITULAR DA CONTA: %s\n", conta.titular);
    fprintf(arq, "SENHA DA CONTA: %s\n", conta.senha);
    fprintf(arq, "SALDO DA CONTA: %.2f\n\n", conta.saldo);
    fclose(arq);
}

// ACESSA A CONTA
ContaBancaria acessar_conta(int numero)
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (banco[i][j].conta_numero == numero) // VERIFICA SE O NÚMERO DA CONTA EXISTE
            {
                return banco[i][j]; // RETORNA A CONTA
            }
        }
    }

    return (ContaBancaria){0}; // RETORNA UMA CONTA VAZIA
}

// CRIA A CONTA E ARMAZENA NA MATRIZ
ContaBancaria criar_conta(char nome[TAMANHO_NOME], char senha[TAMANHO_SENHA])
{
    for (int lin = 0; lin < LINHAS; lin++)
    {
        for (int col = 0; col < COLUNAS; col++)
        {
            if (banco[lin][col].conta_numero == 0) // VERIFICA SE A CONTA ESTÁ VAZIA
            {
                ContaBancaria nova_conta;                  // CRIA UMA NOVA CONTA
                strcpy(nova_conta.titular, nome);          // COPIA O NOME PARA A CONTA
                strcpy(nova_conta.senha, senha);           // COPIA A SENHA PARA A CONTA
                nova_conta.saldo = 0.0;                    // INICIALIZA O SALDO COM 0
                nova_conta.conta_numero = ++ultimo_numero; // ATRIBUI O NÚMERO DA CONTA
                banco[lin][col] = nova_conta;              // ARMAZENA A CONTA NA MATRIZ
                registrar_conta(nova_conta);               // REGISTRA A CONTA NO ARQUIVO
                return nova_conta;                         // RETORNA A CONTA
            }
        }
    }
    return (ContaBancaria){0};
}

// VERIFICA A SENHA DA CONTA
int verificar_senha(ContaBancaria conta, char senha[TAMANHO_SENHA])
{
    return strcmp(conta.senha, senha) == 0; // COMPARA A SENHA DA CONTA COM A SENHA DIGITADA
}

// ATUALIZA O SALDO NA MATRIZ
void atualizar_saldo_na_matriz(int conta_numero, float novo_saldo)
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (banco[i][j].conta_numero == conta_numero)
            {
                banco[i][j].saldo = novo_saldo; // ATUALIZA O SALDO NA MATRIZ
                return;
            }
        }
    }
}

// DEPOSITA UM VALOR NA CONTA
void depositar(ContaBancaria *conta, float valor)
{
    conta->saldo += valor;                                        // ATUALIZA O SALDO DA CONTA
    atualizar_saldo_na_matriz(conta->conta_numero, conta->saldo); // ATUALIZA O SALDO NA MATRIZ
    atualizar_contas_no_arquivo();                                // ATUALIZA O SALDO NO ARQUIVO
}

// SACA UM VALOR DA CONTA
void sacar(ContaBancaria *conta, float valor)
{
    if (valor > conta->saldo)
        printf("Saldo insuficiente!\n");
    else
    {
        conta->saldo -= valor;                                        // ATUALIZA O SALDO DA CONTA
        atualizar_saldo_na_matriz(conta->conta_numero, conta->saldo); // ATUALIZA O SALDO NA MATRIZ
        atualizar_contas_no_arquivo();                                // ATUALIZA O SALDO NO ARQUIVO
    }
}

// FUNÇÃO PARA EXIBIR SALDO
void exibir_saldo(ContaBancaria *conta)
{
    printf("\nSaldo atual da conta: R$%.2f\n", conta->saldo);
}

void registrar_transferencia(Transacao transacao)
{
    FILE *arq = fopen("transferencias.txt", "a");
    if (arq == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arq, "CONTA DE ORIGEM: %i\n", transacao.conta_origem);
    fprintf(arq, "CONTA DE DESTINO: %i\n", transacao.conta_destino);
    fprintf(arq, "VALOR DA TRANSFERÊNCIA: %.2f\n", transacao.valor);
    fprintf(arq, "DATA DA TRANSFERÊNCIA: %s\n\n", transacao.data);

    fclose(arq);
}

// REALIZA A TRANSAÇÃO
Transacao transferir(ContaBancaria *conta_origem, ContaBancaria *conta_destino, float valor)
{
    Transacao transacao = {0};       // INICIALIZA A TRANSAÇÃO
    if (conta_origem->saldo < valor) // VERIFICA SE O SALDO É SUFICIENTE
    {
        printf("Saldo insuficiente!\n");
        return transacao;
    }

    sacar(conta_origem, valor);      // SACAR O VALOR DA CONTA DE ORIGEM
    depositar(conta_destino, valor); // DEPOSITAR O VALOR NA CONTA DE DESTINO

    transacao.conta_origem = conta_origem->conta_numero;   // ATRIBUI O NÚMERO DA CONTA DE ORIGEM
    transacao.conta_destino = conta_destino->conta_numero; // ATRIBUI O NÚMERO DA CONTA DE DESTINO
    transacao.valor = valor;                               // ATRIBUI O VALOR DA TRANSAÇÃO
    strcpy(transacao.data, __DATE__);                      // ATRIBUI A DATA DA TRANSAÇÃO

    registrar_transferencia(transacao); // REGISTRA A TRANSAÇÃO
    return transacao;
}

int main(void)
{
    ler_todas_as_contas("contas.txt");
    int opcao;
    char nome[TAMANHO_NOME], senha[TAMANHO_SENHA];
    int numero;
    float valor;
    ContaBancaria conta;

    printf("Olá, seja bem-vindo ao seu sistema bancário!\n");
    printf("Já é cliente?\n1. Sim\n2. Não\n");
    scanf(" %i", &opcao);

    if (opcao == 1)
    {
        printf("\nInforme o número da conta: ");
        scanf(" %i", &numero);
        printf("Digite sua senha: ");
        scanf(" %s", senha);
        conta = acessar_conta(numero);
        if (conta.conta_numero != 0 && verificar_senha(conta, senha))
            printf("\nAcesso permitido!\n");
        else
        {
            printf("\nSenha incorreta ou conta não encontrada!\n");
            return 1;
        }
    }
    else if (opcao == 2)
    {
        printf("Informe o nome do titular: ");
        scanf(" %s", nome);
        printf("Digite uma senha para sua conta: ");
        scanf(" %s", senha);
        conta = criar_conta(nome, senha);

        if (conta.conta_numero == 0)
        {
            printf("\nSem espaço para armazenar contas!\n");
            return 1;
        }
	else 
	{
	    printf("\nSua conta de número: %i foi criada com sucesso!\n", conta.conta_numero);
	}
    }
    else
    {
        printf("Opção inválida!");
        return 1;
    }

    while (opcao != 5)
    {
        printf("\nMENU:\n1. Depositar\n2. Sacar\n3. Transferir\n4. Consultar Saldo\n5. Sair\n");
        scanf(" %i", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Informe o valor a ser depositado: ");
            scanf(" %f", &valor);
            depositar(&conta, valor);
            printf("O valor de R$%.2f foi depositado\n", valor);
            break;
        case 2:
            printf("Informe o valor a ser sacado: ");
            scanf(" %f", &valor);
            sacar(&conta, valor);
            printf("O valor de R$%.2f foi sacado\n", valor);
            break;
        case 3:
            printf("Informe o número da conta destino: ");
            scanf(" %i", &numero);
            ContaBancaria conta_destino = acessar_conta(numero); // ACESSA A CONTA DE DESTINO
            if (conta_destino.conta_numero != 0)                 // VERIFICA SE A CONTA DE DESTINO EXISTE
            {
                printf("Informe o valor a ser transferido: ");
                scanf(" %f", &valor);
                Transacao transacao = transferir(&conta, &conta_destino, valor); // REALIZA A TRANSFERÊNCIA
                if (transacao.conta_origem != 0)                                 // VERIFICA SE A TRANSAÇÃO FOI REALIZADA
                {
                    printf("Transferência de R$%.2f para a conta de número: %i realizada com sucesso!\n", valor, conta_destino.conta_numero);
                }
            }
            else
            {
                printf("A conta de número: %i não foi encontrada!\n", numero);
            }
            break;
        case 4:
            exibir_saldo(&conta);
            break;
        case 5:
            printf("Encerrando...");
            return 0;
        default:
            printf("Opção inválida");
            break;
        }
    }
}
