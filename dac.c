#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include "esone.h"
#include "cJSON.h"

struct camac_ext ext;

/* Aplica a fórmula matemática com arredondamento correto */
unsigned short calcular_dac_10bits(double valor_usuario) {
    double resultado;
    if (valor_usuario <= 0.0) return 0;
    if (valor_usuario >= VMAX) return 1023;
    
    resultado = (valor_usuario / VMAX) * FATOR;
    return (unsigned short)(resultado + 0.5);
}

int processar_json_dac(const char *json_string, unsigned short *valores_dac) {
    cJSON *root = NULL;
    cJSON *canais_array = NULL;
    cJSON *item = NULL;
    int i;
    int tamanho_array;

    /* 1. Faz o parse da string JSON com validação */
    root = cJSON_Parse(json_string);
    if (!root) {
        printf("Erro de sintaxe no JSON próximo a: %s\n", cJSON_GetErrorPtr());
        return 0; /* Falha */
    }

    /* 2. Busca pela chave "canais" dentro do objeto */
    canais_array = cJSON_GetObjectItem(root, "canais");
    if (!canais_array || canais_array->type != cJSON_Array) {
        printf("Erro: Chave 'canais' nao encontrada ou nao eh um array.\n");
        cJSON_Delete(root);
        return 0;
    }

    /* 3. Valida a quantidade de elementos (deve ser exatamente 8) */
    tamanho_array = cJSON_GetArraySize(canais_array);
    if (tamanho_array != 8) {
        printf("Erro: O array deve conter exatamente 8 elementos. Encontrados: %d\n", tamanho_array);
        cJSON_Delete(root);
        return 0;
    }

    /* 4. Varre o array obtendo os valores e aplicando a fórmula */
    for (i = 0; i < 8; i++) {
        item = cJSON_GetArrayItem(canais_array, i);
        if (item && item->type == cJSON_Number) {
            /* item->valuedouble armazena o valor numerico no cJSON */
            valores_dac[i] = calcular_dac_10bits(item->valuedouble);
        } else {
            printf("Erro: Elemento no indice %d nao eh um numero valido.\n", i);
            cJSON_Delete(root);
            return 0;
        }
    }

    /* 5. Libera OBRIGATORIAMENTE a memória alocada pelo cJSON */
    cJSON_Delete(root);
    return 1; /* Sucesso */
}

int main(){
    WSADATA wsa;
    SOCKET servidor, cliente;
    struct sockaddr_in servidorAddr, clienteAddr;
    int addrLen;
    char buffer[1024] = {0};
    char json[512] = {0};
    int bytesLidos;
    char respostaHttp[1024];
    const char *corpoResposta;
    int valor = 0;
    char versao[] = "1.0.0-SNAPSHOT";
    // char *string_json;
    char *mensagemRetorno = "";
    char *statusRetorno = "";
    char *comandoRetorno = "desconhecido";
    short porta = 8081;
    unsigned short dac_output[8];
    addrLen = sizeof(clienteAddr);

    init(); // inicializa CAMAC
    

    printf("\nPrograma reles versao: %s\n", versao);
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("\n[ERROR] Falha ao inicializar Winsock.\n");
        return 1;
    }

    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor == INVALID_SOCKET)
    {
        printf("\n[ERROR] Falha ao criar socket.\n");
        WSACleanup();
        return 1;
    }

    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = INADDR_ANY;
    servidorAddr.sin_port = htons(porta);

    if (bind(servidor, (struct sockaddr *)&servidorAddr, sizeof(servidorAddr)) == SOCKET_ERROR)
    {
        printf("\n[ERROR] Erro no bind da porta %d.\n", porta);
        closesocket(servidor);
        WSACleanup();
        return 1;
    }

    listen(servidor, 3);
    printf("\n[INFO] Servidor de Hardware (CAMAC) rodando na porta %d...\n", porta);

    while (1)
    {
        cliente = accept(servidor, (struct sockaddr *)&clienteAddr, &addrLen);
        if (cliente == INVALID_SOCKET)
        {
            printf("\n[ERROR] Falha ao aceitar conexao do cliente.\n");
            WSACleanup();
            return 1;
        }

        printf("[INFO] Conexao recebida de um cliente.\n");

        memset(buffer, 0, sizeof(buffer));
        bytesLidos = recv(cliente, buffer, sizeof(buffer) - 1, 0);
        printf("\n[INFO] Bytes lidos: %d\n", bytesLidos);
        if (bytesLidos > 0)
        {
            // Garante que o bloco lido inicialmente é tratado como string
            buffer[bytesLidos] = '\0'; 

            printf("\n[INFO] Verificar necessidade de mais dados.\n");
            
            // Se for um POST e ainda não tiver a chave '{', busca o resto no socket
            if (strstr(buffer, "POST") != NULL && strstr(buffer, "{") == NULL)
            {
                char *proximo_bloco = buffer + bytesLidos;
                int espaco_restante = sizeof(buffer) - bytesLidos - 1;

                int maisBytes = recv(cliente, proximo_bloco, espaco_restante, 0);
                if (maisBytes > 0)
                {
                    bytesLidos += maisBytes;
                    buffer[bytesLidos] = '\0'; // Atualiza o fim real da string acumulada
                }
            }

            printf("\n[INFO] Total de bytes acumulados: %d\n", bytesLidos);

            extractJson(buffer, json);
            
            if (processar_json_dac(json, dac_output)) //Se ok 
            {
                if(parametros.function >=0 && parametros.function < 7) {
                    comandoRetorno = "read";
                } else if(parametros.function >=16 && parametros.function < 25) {
                    comandoRetorno = "write";
                } else if(parametros.function == 25) {
                    comandoRetorno = "execute";
                } else {
                    comandoRetorno = "desconhecido";
                }

                if (retornoStruct.qx == 0) {
                    statusRetorno = "Erro Grave / No-X";
                    mensagemRetorno = "O módulo não respondeu (pode estar ausente ou desligado).";
                } else if(retornoStruct.qx == 1) {
                        statusRetorno = "Comando Aceito, Resposta Falsa";
                        mensagemRetorno = "O módulo existe e entendeu o comando, mas a operação lógica retornou falso.";
                    } else if(retornoStruct.qx == 3){
                            statusRetorno = "Sucesso";
                            mensagemRetorno = "Comando aceito (X=1) e operação realizada com sucesso (Q=1).";
                        } else {
                                statusRetorno = "Erro Desconhecido";
                                mensagemRetorno = "Erro desconhecido.";                    
                            }
                corpoResposta = resposta(statusRetorno, comandoRetorno, mensagemRetorno, retornoStruct.data, retornoStruct.qx);
            } else {
                corpoResposta = resposta("erro", "executar", "JSON invalido.", 0, retornoStruct.qx);
                }
            sprintf(respostaHttp,
                    "HTTP/1.1 %s\r\n"
                    "Content-Type: application/json; charset=utf-8\r\n"
                    "Connection: close\r\n"
                    "\r\n"
                    "%s",
                    retornoStruct.data < 0  || retornoStruct.data > 65535 ? "400 Bad Request" : "200 OK",
                    corpoResposta);
            send(cliente, respostaHttp, (int)strlen(respostaHttp), 0);
        }

        closesocket(cliente);
    }
    closesocket(servidor);
    WSACleanup();
    return 0;
    
}

