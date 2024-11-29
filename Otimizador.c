#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>


// Fun��o para alterar o tipo de inicializa��o de um servi�o
void AlterarServico(SC_HANDLE hSCManager, const char* nomeServico, DWORD startType) {
    SC_HANDLE hServico = OpenService(hSCManager, nomeServico, SERVICE_CHANGE_CONFIG);
    if (hServico == NULL) {
        printf("Erro ao abrir o servi�o %s: %lu\n", nomeServico, GetLastError());
        return;
    }
    // Alterar o tipo de inicializa��o do servi�o
    if (!ChangeServiceConfig(hServico,
                             SERVICE_NO_CHANGE, // Tipo de servi�o (n�o alterar)
                             startType,         // Novo tipo de inicializa��o
                             SERVICE_NO_CHANGE, // Erro de controle (n�o alterar)
                             NULL,              // Caminho bin�rio (n�o alterar)
                             NULL,              // Grupo de carga (n�o alterar)
                             NULL,              // Tag ID (n�o alterar)
                             NULL,              // Depend�ncias (n�o alterar)
                             NULL,              // Conta de servi�o (n�o alterar)
                             NULL,              // Senha (n�o alterar)
                             NULL)) {           // Nome de exibi��o (n�o alterar)
        printf("Erro ao alterar o servi�o %s: %lu\n", nomeServico, GetLastError());
    } else {
        printf("Servi�o %s alterado com sucesso!\n", nomeServico);
    }
    // Fechar o handle do servi�o
    CloseServiceHandle(hServico);
}

// fun��o powershell para desabilitar os aplicativos em segundo plano

void DesativarAplicativosSegundoPlano() {
    LPCWSTR comando = L"powershell -Command \""
        L"$apps = @( "
        L"'Cortana', "
        L"'Microsoft.WindowsCamera', "
        L"'Microsoft.Getstarted', "
        L"'microsoft.windowscommunicationsapps', "  // Email e Calend�rio
        L"'Microsoft.ZuneVideo', "                 // Filmes e TV
        L"'Microsoft.XboxGameOverlay', "           // Game Bar
        L"'Microsoft.WindowsSoundRecorder', "      // Gravador de Voz
        L"'Microsoft.WindowsFeedbackHub', "        // Hub de Coment�rios
        L"'Microsoft.Maps', "
        L"'Microsoft.WindowsStore', "              // Microsoft Store
        L"'Microsoft.GetHelp', "                   // Obter Ajuda
        L"'Microsoft.OneNote', "
        L"'Microsoft.Paint3D', "
        L"'Microsoft.WindowsAlarms', "             // Rel�gio do Windows
        L"'Microsoft.WindowsMediaPlayer', "        // Reprodutor Multim�dia
        L"'Microsoft.SkypeApp', "
        L"'Microsoft.MicrosoftSolitaireCollection', " // Solitaire & Casual Games
        L"'Microsoft.MSPaint', "                   // Paint
        L"'Microsoft.3DViewer', "                  // Visualizador 3D
        L"'Microsoft.XboxApp') "                   // Xbox Console Companion
        L";"
        L"foreach ($app in $apps) {"
        L"    Get-AppxPackage -Name $app | Stop-Process -Force -ErrorAction SilentlyContinue; "
        L"    Set-ProcessMitigation -System -Disable $app"
        L"}\"";

    // Executa o comando do PowerShell via ShellExecute
    ShellExecuteW(NULL, L"open", L"powershell", comando, NULL, SW_HIDE);
}

//Fun��o para otimizar e desfragmentar todas as unidades
void OtimizarDisco() {
    // Comando para otimizar e desfragmentar todas as unidades
    LPCWSTR comando = L"defrag.exe C: /O"; // /O otimiza e desfragmenta

    // Executa o comando via ShellExecute
    ShellExecuteW(NULL, L"open", L"cmd.exe", comando, NULL, SW_HIDE);
}

// Menu inicial
void menu() {
    system("cls");
    printf("============================================\n");
    printf("         OTIMIZADOR - Versao 2.1\n");
    printf("--------------------------------------------\n");
    printf("       Desenvolvido por Luiz Gumz\n");
    printf("============================================\n\n");
}

int main() {    
    
    // Vari�veis
    char escolha_cmd, escolha;

    // Comandos system
    do {menu();
        printf("\nDeseja iniciar a serie de comandos no terminal?");
        printf("\nVoce sera consultado antes de cada comando!");
        printf("\nOpcoes de input: [s] sim ou [n] nao.\n");
        scanf(" %c", &escolha_cmd);
       
        if(escolha_cmd != 's' && escolha_cmd != 'n') {
                 printf("\nInput invalido! Tente novamente.\n");
        }
    } while (escolha_cmd != 's' && escolha_cmd != 'n');
    
    if(escolha_cmd == 'n') {
        menu();
        printf("\nOperacao cancelada!\n");
        sleep(1000);
    } else if(escolha_cmd == 's') {
        int retorno_cmd;

        // Upgrade das aplica��es
        do {menu();
            printf("\nDeseja iniciar o upgrade de todas as aplicacoes? (command: winget upgrade)\n");
            printf("Opcoes de input: [s] sim ou [n] nao\n");
            scanf(" %c", &escolha);
            
            if (escolha != 's' && escolha != 'n') {
                printf("\nInput invalido! Tente novamente.\n");
            } 
        } while (escolha != 's' && escolha != 'n');
        
        if(escolha == 'n'){
            printf("\nOperacao cancelada!");
            sleep(1000);
        } else if(escolha == 's'){
               printf("\nIniciando upgrade...\n");
               retorno_cmd = system("winget upgrade --all");
               // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
                printf("\nUpgrade concluido com sucesso!\n");
            } else {
                printf("\nErro durante o upgrade! Codigo de erro: %d\n", retorno_cmd);
            }
        getchar();
        }
        
        // Repara��o e verifica��o de erros no(s) disco(s) nas parti��es do sistema
        while(escolha != 's' && escolha != 'n') {
            menu();
            printf("\nDeseja iniciar a reparacao e verificao de erros no disco?  (command: sfc /scannow)\n");
            printf("Opcoes de input: [s] sim ou [n] nao\n");
            scanf(" %c", &escolha);

            if (escolha != 's' && escolha != 'n') {
                menu();
                printf("\nInput invalido! Tente novamente.\n");
            } 
        }
        
        if(escolha == 'n'){
            menu();
            printf("\nOperacao cancelada!");
            sleep(1000);
        } else if(escolha == 's'){
            //execu��o sfc /scannow
            printf("\nIniciando reparacao e verificao...\n");
            retorno_cmd = system("sfc /scannow");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nVerificacao concluida com sucesso!\n");
            } else {
                printf("\nErro durante a verificacao! Codigo de erro: %d\n", retorno_cmd);
            }
            // execu��o chkdsk /f C:
            printf("\nIniciando verificacao do disco...\n");
            retorno_cmd = system("chkdsk /f C:");
            // Verifica��o da execu��o do comando anterior
            if (retorno_cmd == 0) {
                printf("\nVerificacao do disco concluida com sucesso!\n");
            } else {
                printf("\nErro durante a verificacao do disco! Codigo de erro: %d\n", retorno_cmd);
            }
        getchar();
        system("cls");
        }
        
        // Comandos relacionados ao comportamento dos temporizadores e gerenciadores de interrup��es
        do {menu();
            printf("\nDeseja iniciar os ajustes relacionados ao comportamento dos temporizadores e ao gerenciamento de interrupcoes do sistema?\n");
            printf("(commands: bcdedit /set useplatformtick yes || bcdedit /set disabledynamictick yes || bcdedit /deletevalue useplatformclock\n");
            printf("Opcoes de input: [s] sim ou [n] nao\n");
            scanf(" %c", &escolha);
           
            if (escolha != 's' && escolha != 'n') {
                printf("\nInput invalido! Tente novamente.\n");
            }
        } while (escolha != 's' && escolha != 'n');
        
        if(escolha == 'n'){
                     printf("\nOperacao cancelada!");
                     sleep(1000);
        } else if(escolha == 's'){
            //execu��o bcdedit /set useplatformtick yes
            printf("\nIniciando a sincronizacao de temporizadores...\n");
            retorno_cmd = system("bcdedit /set useplatformtick yes");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nInicializacao concluida com sucesso!\n");
            } else {
                printf("\nErro durante a verificacao! Codigo de erro: %d\n", retorno_cmd);
            }
            // execu��o bcdedit /set useplatformtick yes
            printf("\nDesabilitando o Dynamic Tick...\n");
            retorno_cmd = system("bcdedit /set disabledynamictick yes");
            // Verifica��o da execu��o do comando anterior
            if (retorno_cmd == 0) {
                printf("\nDesabilitacao concluida com sucesso!\n");
            } else {
                printf("\nErro durante a verificacao do disco! Codigo de erro: %d\n", retorno_cmd);
            }
            // execu��o bcdedit /deletevalue useplatformclock
            printf("\nForcando o uso do HPET para maior eficiencia...\n");
            retorno_cmd = system("bcdedit /deletevalue useplatformclock");
            // Verifica��o da execu��o do comando anterior
            if (retorno_cmd == 0) {
                printf("\nHPET habilitado com sucesso!\n");
            } else {
                printf("\nErro durante a verificacao do disco! Codigo de erro: %d\n", retorno_cmd);
            }
        getchar();
        system("cls");
        }
        
        // Limpeza de disco
        do {menu();
            printf("\nDeseja iniciar o utilitario de limpeza de disco do sistema?  (command: cleanmgr.exe)\n");
            printf("Opcoes de input: [s] sim ou [n] nao\n");
            scanf(" %c", &escolha);
           
            if (escolha != 's' && escolha != 'n') {
                printf("\nInput invalido! Tente novamente.\n");
            } 
        } while (escolha != 's' && escolha != 'n');
        
        if(escolha == 'n'){
                     printf("\nOperacao cancelada!");
                     sleep(1000);
        } else if(escolha == 's'){
            //limpeza da pasta temp
            system("del /s /q %temp%\\*");
            system("del /s /q C:\\Windows\temp\\*");  
            //execu��o cleanmgr.exe
            printf("\n\nIniciando a limpeza de disco...\n");
            retorno_cmd = system("cleanmgr.exe");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nInicializacao concluida com sucesso!\n");
            } else {
                printf("\nErro durante a verificacao! Codigo de erro: %d\n", retorno_cmd);
            }
            //Ativa��o do TRIM em SSDs
            printf("\nHabilitando o TRIM para otimizacao...\n");
            system("fsutil behavior set disabledeletenotify 0");
        getchar();
        system("cls");
        }
        
        // Analise e set o uso de mem�ria do sistema
        do {menu();
            printf("\nDeseja que o sistema maximize de uso da memoria disponivel pelo sistema?  (command: Fsutil behavior set memoryusage 2)\n");
            printf("Opcoes de input: [s] sim ou [n] nao\n");
            scanf(" %c", &escolha);
           
            if (escolha != 's' && escolha != 'n') {
                printf("\nInput invalido! Tente novamente.\n");
            } 
        } while (escolha != 's' && escolha != 'n');
        
        if(escolha == 'n'){
                     printf("\nOperacao cancelada!");
                     sleep(1000);
        } else if(escolha == 's'){ 
            //execu��o Fsutil behavior set memory usage 2
            printf("\nIniciando set de memoria...\n");
            retorno_cmd = system("Fsutil behavior set memoryusage 2");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nSet concluido com sucesso!\n");
            } else {
                printf("\nErro durante a execucao! Codigo de erro: %d\n", retorno_cmd);
            }
        getchar();
        system("cls");
        }
        
        // Analise de sa�de do sistema
        do {menu();
            printf("\nDeseja verificar a saude do sistema?  (command: DISM /Online /Cleanup-Images)\n");
            printf("Opcoes de input: [s] sim ou [n] nao\n");
            scanf(" %c", &escolha);
           
            if (escolha != 's' && escolha != 'n') {
                printf("\nInput invalido! Tente novamente.\n");
            } 
        } while (escolha != 's' && escolha != 'n');
        
        if(escolha == 'n'){
                     printf("\nOperacao cancelada!");
                     sleep(1000);
        } else if(escolha == 's'){ 
            //execu��o do Check Health
            printf("\nIniciando verifica��o de sa�de...\n");
            retorno_cmd = system("DISM /Online /Cleanup-Image /CheckHealth");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nVerificacao concluida com sucesso!\n");
            } else {
                printf("\nErro durante a execucao! Codigo de erro: %d\n", retorno_cmd);
            }
            //execu��o do Scan Health
            printf("\nEscaneando sa�de do sistema...\n");
            retorno_cmd = system("DISM /Online /Cleanup-Image /ScanHealth");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nScan concluido com sucesso!\n");
            } else {
                printf("\nErro durante a execucao! Codigo de erro: %d\n", retorno_cmd);
            }
            //execu��o do Restore Health
            printf("\nRestaurando sa�de do sistema...\n");
            retorno_cmd = system("DISM /Online /Cleanup-Image /RestoreHealth");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nSaude restaurada com sucesso!\n");
            } else {
                printf("\nErro durante a execucao! Codigo de erro: %d\n", retorno_cmd);
            }
        getchar();
        system("cls");
        }
        
        //Op��es de energia
        do {menu();
            printf("\nDeseja habilitar as melhores opcoes de energia?  (command: powercfg -duplicatescheme e9a42b...)\n");
            printf("Opcoes de input: [s] sim ou [n] nao\n");
            scanf(" %c", &escolha);
           
            if (escolha != 's' && escolha != 'n') {
                printf("\nInput invalido! Tente novamente.\n");
            } 
        } while (escolha != 's' && escolha != 'n');
        
        if(escolha == 'n'){
                     printf("\nOperacao cancelada!");
                     sleep(1000);
        } else if(escolha == 's'){
            //execu��o hibernate off
            printf("\nDesabilitando modo hibernacao...\n");
            retorno_cmd = system("powercfg.exe -h off");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nSet concluido com sucesso!\n");
            } else {
                printf("\nErro durante a execucao! Codigo de erro: %d\n", retorno_cmd);
            }
            //execu��o melhor cfg de energia
            printf("\nDesabilitando modo hibernacao...\n");
            retorno_cmd = system("powercfg -duplicatescheme e9a42b02-d5df-448d-aa00-03f14749eb61");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nConfigura��o de energia implantada com sucesso!\n");
            } else {
                printf("\nErro durante a execucao! Codigo de erro: %d\n", retorno_cmd);
            }
        getchar();
        system("cls");
        }
        
        //Limpeza de cache DNS
        do {menu();
            printf("\nDeseja limpar o cache do servidor de dominios?  (command: ipconfig /flushdns...)\n");
            printf("Opcoes de input: [s] sim ou [n] nao\n");
            scanf(" %c", &escolha);
           
            if (escolha != 's' && escolha != 'n') {
                printf("\nInput invalido! Tente novamente.\n");
            } 
        } while (escolha != 's' && escolha != 'n');
        
        if(escolha == 'n'){
                     printf("\nOperacao cancelada!");
                     sleep(1000);
        } else if(escolha == 's'){
            //execu��o hibernate off
            printf("\nLimpando cache...\n");
            retorno_cmd = system("ipconfig /flushdns");
            // Verifica��o da execu��o do comando anterior
            if(retorno_cmd == 0){
               printf("\nCache limpo com sucesso!\n");
            } else {
                printf("\nErro durante a execucao! Codigo de erro: %d\n", retorno_cmd);
            }
        getchar();
        system("cls");
        }  
    }
    
    // Desativa��o de servi�os desneces�rios do windows
    do {menu();
        printf("\nDeseja desabilitar servicos desnecessarios do windows? ");
        printf("Opcoes de input: [s] sim ou [n] nao\n");
        scanf(" %c", &escolha);

        if (escolha != 's' && escolha != 'n') {
            printf("\nInput invalido! Tente novamente.\n");
        }
    } while (escolha != 's' && escolha != 'n');

    if (escolha == 'n') {
        printf("\nOperacao cancelada!\n");
        sleep(1000);  
    } else if (escolha == 's') {
        struct {
        const char* nome;
        DWORD tipoInicializacao;
    } servicos[] = {
            {"XblAuthManager", SERVICE_DISABLED},
            {"XboxGipSvc", SERVICE_DISABLED},
            {"XblGameSave", SERVICE_DISABLED},
            {"MapsBroker", SERVICE_DISABLED},
            {"WpcMonSvc", SERVICE_DISABLED},
            {"Fax", SERVICE_DISABLED},
            {"DiagTrack", SERVICE_DISABLED},
            {"WbioSrvc", SERVICE_DISABLED},
            {"RemoteRegistry", SERVICE_DISABLED},
            {"WAPPushService", SERVICE_DISABLED},
            {"lfsvc", SERVICE_DISABLED},
            {"WSearch", SERVICE_DEMAND_START},    // Manual
            {"wuauserv", SERVICE_DEMAND_START},   // Manual
            {"TabletInputService", SERVICE_DISABLED},
            {"ALG", SERVICE_DEMAND_START},        // Manual
            {"AJRouter", SERVICE_DISABLED},
            {"CertPropSvc", SERVICE_DISABLED},
            {"RetailDemo", SERVICE_DISABLED},
            {"SCardSvr", SERVICE_DISABLED},
            {"WerSvc", SERVICE_DISABLED},
            {"DPS", SERVICE_DISABLED},
            {"wisvc", SERVICE_DISABLED},
            {"icssvc", SERVICE_DISABLED},
            {"PcaSvc", SERVICE_DISABLED},
            {"BDESVC", SERVICE_DISABLED},
            {"SysMain", SERVICE_DISABLED}
        };
        
        // Abrir o gerenciador de controle de servi�os
        SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (hSCManager == NULL) {
            printf("Erro ao abrir o Gerenciador de Controle de Servi�os: %lu\n", GetLastError());
            return 1; // Sa�da com erro
        }
        // Percorrer a lista de servi�os e alterar a configura��o de cada um
        for (int i = 0; i < sizeof(servicos) / sizeof(servicos[0]); i++) {
            AlterarServico(hSCManager, servicos[i].nome, servicos[i].tipoInicializacao);
        }
        printf("Opera��o realizada com sucesso!\n");

        // Fechar o gerenciador de controle de servi�os
        CloseServiceHandle(hSCManager);
        sleep(1000);
        system("cls");
    }
    
    // Mudan�a de valor do MenuShowDelay no Regedit
    do {menu();
        printf("\nDeseja diminuir o valor da chave \"MenuShowDelay?\" ");
        printf("\nOpcoes de input: [s] sim ou [n] nao\n");
        scanf(" %c", &escolha);

        if (escolha != 's' && escolha != 'n') {
            printf("\nInput invalido! Tente novamente.\n");
        }
    } while (escolha != 's' && escolha != 'n');

    if (escolha == 'n') {
        printf("\nOperacao cancelada!\n");
        sleep(1000);  
    } else if (escolha == 's') {
        HKEY hKey;
        LPCWSTR subKey = L"Control Panel\\Desktop";

        // Abre a chave no HKEY_CURRENT_USER
    if (RegOpenKeyExW(HKEY_CURRENT_USER, subKey, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        LPCWSTR valueName = L"MenuShowDelay";
        // Define o valor da chave
        if (RegSetValueExW(hKey, valueName, 0, REG_SZ, (BYTE*)L"8", sizeof(L"8")) == ERROR_SUCCESS) {
            printf("Valor alterado com sucesso para 8.\n");
        } else {
            printf("Falha ao alterar o valor.\n");
        }
        // Fecha a chave do registro
        RegCloseKey(hKey);
    } else {
        printf("Falha ao abrir a chave do registro.\n");
        sleep(1000);
        system("cls");
    }
    // Desativa��o de aplicativos em segundo plano
    do {menu();
        printf("\nDeseja desativas os aplicativos de execucao em segundo plano?");
        printf("\nOpcoes de input: [s] sim ou [n] nao\n");
        scanf(" %c", &escolha);

        if (escolha != 's' && escolha != 'n') {
            printf("\nInput invalido! Tente novamente.\n");
        }
    } while (escolha != 's' && escolha != 'n');

    if (escolha == 'n') {
       printf("\nOperacao cancelada!\n");
       sleep(1000);  
    } else if (escolha == 's') {
           printf("Desabilitando aplicativos em segundo plano...\n");
           DesativarAplicativosSegundoPlano();
           printf("Aplicativos desabilitados com sucesso.\n");
           sleep(1000);
           system("cls");
    }
    
    // Otimiza��o de disco
    do {menu();
        printf("\nDeseja otimizar e fragmentar o disco?");
        printf("\nOpcoes de input: [s] sim ou [n] nao\n");
        scanf(" %c", &escolha);

        if (escolha != 's' && escolha != 'n') {
            printf("\nInput invalido! Tente novamente.\n");
        }
    } while (escolha != 's' && escolha != 'n');

    if (escolha == 'n') {
       printf("\nOperacao cancelada!\n");
       sleep(1000);  
    } else if (escolha == 's') {
           printf("Otimizando e desfragmentando a unidade...\n");
           OtimizarDisco();
           printf("Otimiza��o iniciada com sucesso.\n");
           sleep(1000);
           system("cls");
    }           

    //Sa�da
    menu();
    printf("\nExecu��o dos comandos realizada com sucesso.");
    printf("\nAperte qualquer tecla para sair!");
    getchar();
    
    return 0;
}
}
