#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Contato {
    char nome[250];
    char endereco[250];
    char bairro[250];
    char cidade[250];
    char estado[250];
    char cep[250];
    char telefone[250];
    char celular[250];
};

void exibirMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Inserir contato\n");
    printf("2. Deletar contato\n");
    printf("3. Alterar cadastro\n");
    printf("4. Consultar contato\n");
    printf("5. Sair\n");
    printf("Escolha uma opcao: ");
}

void inserirContato(FILE *arquivo) {
    struct Contato novoContato;
    printf("\n--- Inserir Contato ---\n");
    printf("Nome: ");
    scanf(" %[^\n]s", novoContato.nome);
    printf("Endereco: ");
    scanf(" %[^\n]s", novoContato.endereco);
    printf("Bairro: ");
    scanf(" %[^\n]s", novoContato.bairro);
    printf("Cidade: ");
    scanf(" %[^\n]s", novoContato.cidade);
    printf("Estado: ");
    scanf(" %[^\n]s", novoContato.estado);
    printf("CEP: ");
    scanf(" %[^\n]s", novoContato.cep);
    printf("Telefone: ");
    scanf(" %[^\n]s", novoContato.telefone);
    printf("Celular: ");
    scanf(" %[^\n]s", novoContato.celular);

    // Grava o contato no arquivo
    fprintf(arquivo, "%s;%s;%s;%s;%s;%s;%s;%s\n",
            novoContato.nome, novoContato.endereco, novoContato.bairro,
            novoContato.cidade, novoContato.estado, novoContato.cep,
            novoContato.telefone, novoContato.celular);

    fclose(arquivo);
    arquivo = fopen("agenda.txt", "a+");

    printf("Contato inserido com sucesso!\n");
}


void deletarContato(FILE *arquivo) {
    char nomeParaDeletar[250];
    printf("\n--- Deletar Contato ---\n");
    printf("Digite o nome do contato a ser deletado: ");
    scanf(" %[^\n]", nomeParaDeletar);

    FILE *tempFile = fopen("temp_agenda.txt", "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Erro ao criar arquivo temporário.\n");
        return;
    }

    fseek(arquivo, 0, SEEK_SET);

    char linha[256];
    int contatoDeletado = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char nome[50];
        sscanf(linha, "%49[^;]", nome);  // Lê o nome até o próximo ponto e vírgula

        if (strcmp(nome, nomeParaDeletar) != 0) {
            // Se não for o contato a ser deletado, escreve no arquivo temporário
            fprintf(tempFile, "%s", linha);
        } else {
            contatoDeletado = 1;
        }
    }

    fclose(tempFile);
    fclose(arquivo);

    remove("agenda.txt");
    rename("temp_agenda.txt", "agenda.txt");

    arquivo = fopen("agenda.txt", "a+");

    if (contatoDeletado) {
        printf("Contato deletado com sucesso!\n");
    } else {
        printf("Contato não encontrado.\n");
    }
}

int consultarContato(FILE *arquivo) {
    int opcaoConsulta;
    char termo[250];

    printf("\n--- Consultar Contato ---\n");
    printf("1. Por nome\n");
    printf("2. Por cidade\n");
    printf("3. Por estado\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcaoConsulta);

    printf("Digite o termo a ser consultado: ");
    scanf(" %[^\n]s", termo);

    fseek(arquivo, 0, SEEK_SET);

    char linha[256];
    int encontrado = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Utiliza strtok para dividir a linha em campos delimitados por ";"
        char *nome = strtok(linha, ";");      // O primeiro campo é o nome
        char *endereco = strtok(NULL, ";");    // O segundo campo é o endereço
        char *bairro = strtok(NULL, ";");      // O terceiro campo é o bairro
        char *cidade = strtok(NULL, ";");      // O quarto campo é a cidade
        char *estado = strtok(NULL, ";");      // O quinto campo é o estado
        char *cep = strtok(NULL, ";");         // O sexto campo é o CEP
        char *telefone = strtok(NULL, ";");    // O sétimo campo é o telefone
        char *celular = strtok(NULL, ";");     // O oitavo campo é o celular

        // Utilizando a função strcmp para comparar as strings exatamente
        if ((opcaoConsulta == 1 && strcmp(nome, termo) == 0) ||
            (opcaoConsulta == 2 && strcmp(cidade, termo) == 0) ||
            (opcaoConsulta == 3 && strcmp(estado, termo) == 0)) {

            printf("\n--- Informacoes do Contato ---\n");
            printf("Nome: %s\n", nome);
            printf("Endereco: %s\n", endereco);
            printf("Bairro: %s\n", bairro);
            printf("Cidade: %s\n", cidade);
            printf("Estado: %s\n", estado);
            printf("CEP: %s\n", cep);
            printf("Telefone: %s\n", telefone);
            printf("Celular: %s\n", celular);
            encontrado = 1;
        }
    }


    if (!encontrado) {
        printf("Contato com o termo '%s' não encontrado.\n", termo);
    }

    return encontrado;
}

void alterarCadastro(FILE *arquivo) {
    char termo[250];
    int foi_encontrado;
    printf("\n--- Alterar Cadastro ---\n");
    
    foi_encontrado = consultarContato(arquivo);

    // Consultar o contato a ser alterado
    printf("Digite o nome do contato a ser alterado: ");
    scanf(" %[^\n]", termo);

    // Verificar se o contato foi encontrado
    if (!foi_encontrado) {
        printf("Contato não encontrado.\n");
        return;
    }

    FILE *tempFile = fopen("temp_agenda.txt", "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Erro ao criar arquivo temporário.\n");
        return;
    }
    fseek(arquivo, 0, SEEK_SET);

    char linha[250];
    int contatoAlterado = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char nome[250];
        sscanf(linha, "%250[^;]", nome);  // Lê o nome até o próximo ponto e vírgula

        if (strcmp(nome, termo) != 0) {
            // Se não for o contato a ser alterado, escreve no arquivo temporário
            fprintf(tempFile, "%s", linha);
        } else {
            contatoAlterado = 1;
        }
    }

    fclose(tempFile);
    fclose(arquivo);

    if (!contatoAlterado) {
        printf("Contato não encontrado.\n");
        remove("temp_agenda.txt");  // Remove o arquivo temporário, pois não será mais necessário
        return;
    }

    // Reabre o arquivo original no modo de escrita
    arquivo = fopen("agenda.txt", "w");

    // Reescreve o conteúdo do arquivo original (sem o contato antigo)
    tempFile = fopen("temp_agenda.txt", "r");
    while (fgets(linha, sizeof(linha), tempFile) != NULL) {
        fprintf(arquivo, "%s", linha);
    }

    fclose(tempFile);
    fclose(arquivo);

    // Agora, inserir o novo contato
    remove("agenda.txt");
    rename("temp_agenda.txt", "agenda.txt");

    arquivo = fopen("agenda.txt", "a+");
    inserirContato(arquivo);

    printf("Cadastro alterado com sucesso!\n");
}

int main() {
    FILE *arquivo;
    arquivo = fopen("agenda.txt", "a+");

    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }

    int opcao;
    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirContato(arquivo);
                break;
            case 2:
                deletarContato(arquivo);
                break;
            case 3:
                alterarCadastro(arquivo);
                break;
            case 4:
                consultarContato(arquivo);
                break;
            case 5:
                printf("Saindo do programa. Obrigado!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);

    fclose(arquivo);

    return 0;
}
