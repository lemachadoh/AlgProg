/*
Trabalho final de algoritmos e programaçao
Criação de um jogo com o auxilio da biblioteca raylib
DUPLA:
nome: Alan                     cartao: 00
nome: Leandra                  cartao: 00302222
*/
#include <raylib.h>
#include <stdio.h>

// Estrutura para representar um monstro
typedef struct {
    Vector2 posicao;
    bool active;
    Vector2 orientacao;
} Monstros;

// Estrutura para representar o jogador
typedef struct {
    Vector2 posicao;
    int saude;
    int score;
    int vidas;
    bool espada;
    char orientacao; 
    int nivel;
} Jogador;

typedef struct{
    Vector2 posicao;
}Pedras;

typedef struct 
{
    Vector2 tela[16][24]; //16 linhas por 24 colunas,
}Nivel;

Jogador jogador = {0};
Monstros monsters[10] = {0}; //maximo 10 monstros
Vector2 princessPosition = {700, 300};
bool princessRescued = false;
Pedras pedras[100] = {0}; //maximo de 100 obstaculos

bool IsCollision(Vector2 posicao, Nivel nivel) {
    // Verifica se a posição está livre de colisão com monstros e limites do cenário
    for (int i = 0; i < 5; i++) {
        if (monsters[i].active && CheckCollisionCircles(posicao, 10, monsters[i].posicao, 20)) {
            return true;
        }
    }

    for(int i = 0; i < 100; i++){
        if(CheckCollisionCircles(posicao, 10, pedras[i].posicao, 20)){
            return true;
        }
    }

    if (posicao.x < 0 || posicao.y < 60 || posicao.x > GetScreenWidth() || posicao.y > GetScreenHeight()) {
        return true;
    }
    int coluna = posicao.x / 50; //50 pixels 
    int linha = (posicao.y - 60) / 50;
    return nivel.tela[coluna][linha] == 'P'; //verifica se no pixel * é 'P'
}

void CarregaNivel(Nivel *nivel, const char *arquivo_nivel){
    int i,j;
    FILE *arquivo = fopen(arquivo_nivel, "r");
    
    if(arquivo == NULL){
        printf("Erro ao iniciar o arquivo %s\n", arquivo_nivel);
        return;
    }
    for(i = 0; i < 16; i++){
        for(j = 0; j < 24; j++){
            fscanf(arquivo, " %c", &nivel->tela[i][j]);
        }
    }
    fclose(arquivo);
}

int main() {
    const int largura = 1200;
    const int altura = 860;

    InitWindow(largura, altura, "Zelda - INF");

    bool gameOver = false;
    int NivelAtual = 1;
    char ArquivoNivel[100];

    sprintf(ArquivoNivel, "nivel%d.txt", NivelAtual);
    Nivel nivel;
    CarregaNivel(&nivel, ArquivoNivel);


    // Inicialização do jogador
    jogador.posicao     = (Vector2){100, 100};
    jogador.saude       = 100;
    jogador.score       = 0;
    jogador.vidas       = 3;
    jogador.espada      = false;
    jogador.orientacao  = 'N';
    jogador.nivel       = 1;

    // Inicialização dos monstros
    for (int i = 0; i < 5; i++) {
        monsters[i].posicao = (Vector2){GetRandomValue(200, 600), GetRandomValue(150, 400)};
        monsters[i].active = true;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!gameOver){
            // Atualização
            if (!princessRescued) {
                // Movimentação do jogador

                if (IsKeyDown((KEY_RIGHT && KEY_D)) && !IsCollision((Vector2){jogador.posicao.x + 2, jogador.posicao.y},nivel)) jogador.posicao.x += 2;
                if (IsKeyDown((KEY_LEFT && KEY_A))  && !IsCollision((Vector2){jogador.posicao.x - 2, jogador.posicao.y},nivel)) jogador.posicao.x -= 2;
                if (IsKeyDown((KEY_DOWN && KEY_S))  && !IsCollision((Vector2){jogador.posicao.x, jogador.posicao.y + 2},nivel)) jogador.posicao.y += 2;
                if (IsKeyDown((KEY_UP && KEY_W))    && !IsCollision((Vector2){jogador.posicao.x, jogador.posicao.y - 2},nivel)) jogador.posicao.y -= 2;

                // Verificar resgate da princesa
                if (CheckCollisionCircles(jogador.posicao, 10, princessPosition, 20)) {
                    princessRescued = true;
                }

                //Espada
                if(IsKeyPressed(KEY_SPACE)){
                    jogador.espada = true;
                }
                
                for (int i = 0; i < 5; i++) {
                    if (monsters[i].active) {
                        monsters[i].orientacao.x = GetRandomValue(-1, 1);
                        monsters[i].orientacao.y = GetRandomValue(-1, 1);
                        monsters[i].orientacao.x += monsters[i].orientacao.x;
                        monsters[i].orientacao.y += monsters[i].orientacao.y;

                        // Verificar limites do cenário para os monstros
                        if (monsters[i].orientacao.x < 0) monsters[i].orientacao.x = 0;
                        if (monsters[i].orientacao.y < 60) monsters[i].orientacao.y = 60;
                        if (monsters[i].orientacao.x > largura) monsters[i].orientacao.x = largura;
                        if (monsters[i].orientacao.y > altura) monsters[i].orientacao.y = altura;
                    }
                }
                // Verificar colisão do jogador com os monstros
                for (int i = 0; i < 10; i++) {
                    if(!gameOver && monsters[i].active &&CheckCollisionCircles(jogador.posicao, 10, monsters[i].posicao, 20)){
                        jogador.posicao = (Vector2){100,100};
                        for(int j = 0; j < 10; j++){
                            monsters[j].posicao = (Vector2){GetRandomValue(200,600),GetRandomValue(150,400)};
                            monsters[j].active = true;
                        }
                        jogador.vidas--;
                        if(jogador.vidas<=0){
                            gameOver = true;
                        }
                    }
                }
            }

            // Ativar espada
            if (IsKeyPressed(KEY_J) && !jogador.espada) {
                jogador.espada = true;
                Vector2 swordTarget;
                switch (jogador.orientacao) {
                    case 'N': swordTarget = (Vector2){jogador.posicao.x, jogador.posicao.y - 30}; break;
                    case 'S': swordTarget = (Vector2){jogador.posicao.x, jogador.posicao.y + 30}; break;
                    case 'L': swordTarget = (Vector2){jogador.posicao.x + 30, jogador.posicao.y}; break;
                    case 'O': swordTarget = (Vector2){jogador.posicao.x - 30, jogador.posicao.y}; break;
                }
                for (int i = 0; i < 10; i++) {
                    if (monsters[i].active && CheckCollisionCircles(swordTarget, 10, monsters[i].jogador.posicao, 20)) {
                        monsters[i].active = false;
                        jogador.score += 100;
                    }
                }
            }
        }else{

            //tela de gameover
        }

            // Renderização
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Exibir informações na tela
            DrawRectangle(0,0,largura,60,BLACK);
            DrawText(FormatText("Saude: %d", jogador.saude), 10, 10, 20, BLACK);
            DrawText(FormatText("Score: %d", jogador.score), 10, 40, 20, BLACK);
            DrawText(FormatText("Vidas: %d", jogador.vidas), 10, 70, 20, BLACK);
            DrawText(FormatText("Nivel %d", jogador.nivel),400,10,20,RED);

            // Desenhar cenário
            DrawRectangle(0, 60, largura, altura - 60, LIGHTGRAY);

            // Desenhar jogador
            DrawCircleV(jogador.posicao, 10, ORANGE);

            // Desenhar monstros
            for (int i = 0; i < 5; i++) {
                if (monsters[i].active) {
                    DrawCircleV(monsters[i].posicao, 20, BLUE);
                }
            }

            // Desenhar princesa
            if (!princessRescued) {
                DrawCircleV(princessPosition, 20, GREEN);
            }

        
            if (princessRescued) {
                DrawText("Princess Rescued!", 330, altura / 2 - 10, 30, DARKBLUE);
            }

            EndDrawing();
        }

        CloseWindow();

        return 0;
}
