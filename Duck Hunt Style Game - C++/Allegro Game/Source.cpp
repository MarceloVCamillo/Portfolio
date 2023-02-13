// Made by Marcelo Camillo & Lucas Honorio

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <iostream>
#include <random>
using namespace std;

//possiveis variaveis
#define alvos 1    //numero de alvos - 1
#define SCREEN_W 800
#define SCREEN_H 600
#define n_alvos 200
//casualidade
#define black al_map_rgb(0,0,0)
#define blue al_map_rgb(0,0,255)
#define red al_map_rgb(255,0,0)
#define green al_map_rgb(0,255,0)
#define raio  32
#define DificuldadeInicial 1
#define SecretoInicial false
#define SecretoProb 9
#define Einicial 20  //quantidade de vidas inicial


#define crosshairsize 16

#define FPS 60


ALLEGRO_SAMPLE* tiro = NULL;   //typedef
ALLEGRO_SAMPLE_INSTANCE* instance1 = NULL;
int comSom = 1;
ALLEGRO_KEYBOARD_STATE  KeyState;   //typedef
ALLEGRO_EVENT events;            //typedef
ALLEGRO_MOUSE_STATE state;        //typedef
ALLEGRO_BITMAP* alvo;
ALLEGRO_BITMAP* alvosecreto;
ALLEGRO_FONT* font;
ALLEGRO_FONT* font2;



struct alvo
{
    float y0;
    float velocidade;
    float theta;
    float vx;
    float g;
    float vy;
    float dt;
    float x;
    float y;
    int lado;
} c, d;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    random_device rd;
    mt19937 mt(rd());
    default_random_engine generator;
    uniform_int_distribution<int> dist(0, 99);

int random() {
    int randoms = dist(generator);
    return randoms;
}


void reinicia_alvoc(float* x, float* y, float* t, float* y0, int lado){         /////ALVO DA ESQUERDA
    
    int r = random();
    if (lado == 0)
    {
        *x = -raio;
        *y = 0;
        *t = 0;
        *y0 = (200) + (((r) % 12) * ((SCREEN_H) / 20));
    }
    else
        printf("erro no reinicia alvo da esquerda\n");
    c.theta = (random() % 100) * 3.14 / 450;

}
void reinicia_alvod(float* x, float* y, float* t, float* y0, int lado){         /////ALVO DA DIREITA
    int r = random();
    if (lado == 1)
    {
        *x = SCREEN_W + raio;
        *y = 0;
        *t = 0;
        *y0 = (200) + (((r) % 12) * ((SCREEN_H) / 20));
    }
    else
        printf("erro no reinicia_alvo da direita\n");
    d.theta = (random() % 100) * 3.14 / 500;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool randsecreto1()
{
    bool secreto = false;
    if (random() % SecretoProb == 0) {
        secreto = true;
    }
    else {
        secreto = false;
    }
    return secreto;
}

bool randsecreto2()
{
    bool secreto = false;
    if (random() % SecretoProb == 1) {
        secreto = true;
    }
    else {
        secreto = false;
    }
    return secreto;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mov_alvoc(float* a, float* b, float y0, float* t, int lado, float dificuldade)         /////ALVO DA ESQUERDA
{
    *t += 0.0166;
    if (lado == 0)
    {
        c.velocidade = 250 * (dificuldade);
        c.g = 20 * (*t) * (dificuldade);
        c.vy = c.velocidade * sin(c.theta);
        c.vx = c.velocidade * cos(c.theta);
        *a = *t * c.vx - raio;
        *b = y0 - (c.vy) * *t + (c.g * pow(*t, 2) / 2);
    }
}
void mov_alvod(float* a, float* b, float y0, float* t, int lado, float dificuldade)         /////ALVO DA DIREITA
{
    int x0;
    x0 = SCREEN_W;
    *t += 0.0166;
    if (lado == 1)
    {
        d.velocidade = 250 * (dificuldade);
        d.g = 20 * (*t) * (dificuldade);
        d.vy = d.velocidade * sin(d.theta);
        d.vx = d.velocidade * cos(d.theta);
        *a = x0 - (*t * d.vx - raio);
        *b = y0 - (d.vy) * *t + (d.g * pow(*t, 2) / 2);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void imprime_tela(float cx, float cy, float dx, float dy, int p, int e, float dificuldade, bool secreto1, bool secreto2)
{
    al_draw_textf(font, black, 9 * SCREEN_W / 10, 9 * SCREEN_H / 10, ALLEGRO_ALIGN_CENTER, "PONTOS %d", p);
    al_draw_textf(font, black, SCREEN_W / 10, 9 * SCREEN_H / 10, ALLEGRO_ALIGN_CENTER, "ERROS %d", e / 2);
    al_flip_display();
    al_clear_to_color(al_map_rgb(255 - (dificuldade - DificuldadeInicial) * 255/3 , 255 -  (dificuldade-DificuldadeInicial) * 255/3 , 255 - (dificuldade - DificuldadeInicial) /3 * 255));
    if (secreto2) {
        al_draw_bitmap(alvosecreto, cx - raio, cy - raio, ALLEGRO_ALIGN_CENTER);
    }
    else if (!secreto2) {
        al_draw_bitmap(alvo, cx - raio, cy - raio, ALLEGRO_ALIGN_CENTER);
    }
    if (secreto1) {
        al_draw_bitmap(alvosecreto, dx - raio, dy - raio, ALLEGRO_ALIGN_CENTER);
    }
    else if (!secreto1) {
        al_draw_bitmap(alvo, dx - raio, dy - raio, ALLEGRO_ALIGN_CENTER);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void imprime_tela_menu() {
    al_draw_filled_rectangle(SCREEN_W / 2 - SCREEN_W / 5, SCREEN_H / 3 - SCREEN_H / 20, SCREEN_W / 2 + SCREEN_W / 5, SCREEN_H / 3 + SCREEN_H / 12, red);
    al_draw_filled_rectangle(SCREEN_W / 2 - SCREEN_W / 5, 2 * SCREEN_H / 3 - SCREEN_H / 20, SCREEN_W / 2 + SCREEN_W / 5, 2 * SCREEN_H / 3 + SCREEN_H / 12, red);
    al_draw_textf(font, green, SCREEN_W / 2, SCREEN_H / 3, ALLEGRO_ALIGN_CENTER, "PRESS R FOR RESTART");
    al_draw_textf(font, green, SCREEN_W / 2, 2 * SCREEN_H / 3, ALLEGRO_ALIGN_CENTER, "PRESS ESC TO QUIT");
    al_draw_textf(font, green,  SCREEN_W / 2, 17 * SCREEN_H / 20, ALLEGRO_ALIGN_CENTER, "AUTORES");
    al_draw_textf(font, green,  SCREEN_W / 2, 18 * SCREEN_H / 20, ALLEGRO_ALIGN_CENTER, "MARCELO VINICIUS CAMILLO");
    al_draw_textf(font, green,  SCREEN_W / 2, 19 * SCREEN_H / 20, ALLEGRO_ALIGN_CENTER, "LUCAS HONORIO GALATTI");
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    bool fim = false;      //guarda se o jogo foi finalizado ou nao
    bool rodando = true;   //guarda estado de game-over ou nao

    //alvos secretos
    bool secreto1 = SecretoInicial;
    bool secreto2 = SecretoInicial;

    int p = 0, e = Einicial;
    c.lado = 0;
    d.lado = 1;
    float dificuldade = DificuldadeInicial;    //Dificuldade inicial

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //condicoes iniciais


    al_init();

    //Inicia a fonte
    al_init_font_addon();
    al_init_ttf_addon();
    font = al_load_font("fonte_menu.ttf", 20, NULL);
    font2 = al_load_font("fonte_menu.ttf", 10, NULL);

    //Inicia o allegro em modo janela
    al_set_new_display_flags(ALLEGRO_WINDOWED);

    if (!al_init())
    {
        printf("O display nao iniciou\n");
    }
    ALLEGRO_DISPLAY* display;
    display = al_create_display(SCREEN_W, SCREEN_H);

    if (!display)
    {
        printf("Erro na criacao do display\n");
    }

    al_draw_text(font, green, SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, "START");

    al_flip_display();
    al_rest(1.0);


    al_init_image_addon();   //inicia o addon de imagens
    if (!al_init_image_addon())
    {
        printf("O add-on de imagen nao iniciou\n");
    }

    al_init_primitives_addon();  //inicia as figuras primitivas (retangulo, circulo, triangulo)
    if (!al_init_primitives_addon())
    {
        printf("O add-on de imagens primitivas nao iniciou\n");
    }

    al_install_mouse();    //inicia o mouse
    if (!al_install_mouse())
    {
        printf("O mouse nao iniciou\n");
    }

    al_install_keyboard();  //INICIA O TECLADO
    if (!al_install_keyboard())
    {
        printf("O teclado nao iniciou\n");
    }

    al_install_audio();
    if (!al_install_audio())
    {
        printf("falha na inicializacao do audio\n");
    }

    if (comSom && !al_init_acodec_addon())
    {
        printf("failed to initialize audio codecs!\n");
        return -1;
    }

    if (comSom && !al_reserve_samples(1))
    {
        printf("failed to reserve samples!\n");
        return -1;
    }

    if (comSom)
    {
        al_reserve_samples(10);
        //carrega audio
        tiro = al_load_sample("tiro.wav");
        if (!tiro)
        {
            printf("Audio clip sample not loaded!\n");
            return -1;
        }
    }
    //Inicia som
    instance1 = al_create_sample_instance(tiro);
    al_attach_sample_instance_to_mixer(instance1, al_get_default_mixer());

    //INICIA IMAGENS
    alvo = al_load_bitmap("alvo.png");
    alvosecreto = al_load_bitmap("alvosecret.png");
    ALLEGRO_BITMAP* mira = al_load_bitmap("mira.png");


    //registra o log de eventos
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    //inicia o timer
    al_start_timer(timer);   

    // cria o cursor do mouse
    ALLEGRO_MOUSE_CURSOR* cursor = al_create_mouse_cursor(mira, crosshairsize, crosshairsize);
    al_set_mouse_cursor(display, cursor);



    //inicia alvo
    reinicia_alvoc(&c.x, &c.y, &c.dt, &c.y0, c.lado);  // nesse caso inicia o alvo
    reinicia_alvod(&d.x, &d.y, &d.dt, &d.y0, d.lado);
    //al_grab_mouse(display);    //segura o mouse na tela

    while (!fim)      //enquanto nao foi finalizado  
    {
        al_wait_for_event(event_queue, &events); //void
        al_get_mouse_state(&state);
        // TELA DE INICIAR JOGO

        if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            //fecha a tela
            fim = true;
        }
        if (events.type == ALLEGRO_EVENT_KEY_DOWN)            //controle da janela do jogo
        {
            //Bind de teclado
            switch (events.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                fim = true;       //esc fecha jogo
                break;
            case ALLEGRO_KEY_R:    //R reseta o jogo
                rodando = true;
                reinicia_alvod(&d.x, &d.y, &d.dt, &d.y0, d.lado);
                reinicia_alvoc(&c.x, &c.y, &c.dt, &c.y0, c.lado);
                dificuldade = DificuldadeInicial;
                e = Einicial;
                p = 0;
            }
        }
        else if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            // se click na hitbox alvo esquerda
            if ((events.mouse.x - c.x) * (events.mouse.x - c.x) + (events.mouse.y - c.y) * (events.mouse.y - c.y) < (raio * raio))
            {
                //reinicia alvo, add ponto e aumenta dificuldade
                reinicia_alvoc(&c.x, &c.y, &c.dt, &c.y0, c.lado);
                p++;
                dificuldade += 0.01;
                if (secreto2)
                {
                    // se for o alvo secreto aumenta a vida
                    e = e + 3;
                }
                else {
                    // perde 1 e de errar o alvo da esquerda, mas ganha 1 e por acertar o da direita -- net 0
                    e++;
                }
                secreto2 = randsecreto2();
            }
            else
            {
                // se errar o tiro diminui a vida   -- 1 vida = 2 e's pq errou os 2 alvos (repete 1 e para cada alvo)
                e--;
            }
                
            // se click na hitbox alvo direita
            if ((events.mouse.x - d.x) * (events.mouse.x - d.x) + (events.mouse.y - d.y) * (events.mouse.y - d.y) < (raio * raio))
            {
                //reinicia alvo, add ponto e aumenta dificuldade
                reinicia_alvod(&d.x, &d.y, &d.dt, &d.y0, d.lado);
                p++;
                dificuldade += 0.01;
                if (secreto1)
                {
                    // se for o alvo secreto aumenta a vida
                    e = e + 3;
                }
                else 
                {
                    // perde 1 e de errar o alvo da esquerda, mas ganha 1 e por acertar o da direita -- net 0
                    e++;
                }
                secreto1 = randsecreto1();
            }
            else 
            {
                // se errar o tiro diminui a vida   -- 1 vida = 2 e's pq errou os 2 alvos (repete 1 e para cada alvo)
                e--;
            }
            //se game over
            if (rodando == false) 
            {
                // Interacoes menu de game over
                if ((events.mouse.x > SCREEN_W / 2 - SCREEN_W / 5) && (events.mouse.y > SCREEN_H / 3 - SCREEN_H / 20) && (events.mouse.x < SCREEN_W / 2 + SCREEN_W / 5) && (events.mouse.y < SCREEN_H / 3 + SCREEN_H / 12))
                {
                    //reiniciar o jogo
                    rodando = true;
                    reinicia_alvod(&d.x, &d.y, &d.dt, &d.y0, d.lado);
                    reinicia_alvoc(&c.x, &c.y, &c.dt, &c.y0, c.lado);
                    dificuldade = DificuldadeInicial;
                    e = Einicial;
                    p = 0;
                }
                if ((events.mouse.x > SCREEN_W / 2 - SCREEN_W / 5) && (events.mouse.y > 2 * SCREEN_H / 3 - SCREEN_H / 20) && (events.mouse.x < SCREEN_W / 2 + SCREEN_W / 5) && (events.mouse.y < 2 * SCREEN_H / 3 + SCREEN_H / 12))
                {
                    //fechar o jogo
                    fim = true;       //esc fecha jogo
                    break;
                }
            }
            // Som de tiro
            if (comSom)
            {
                al_stop_sample_instance(instance1);
                al_play_sample_instance(instance1);
            }
        }

        if (events.type == ALLEGRO_EVENT_TIMER)
        {
            //numero de erros ate game over
            if (e <= 0)  
            {
                //imprime menu de gameover
                imprime_tela_menu();
            }
            if (rodando == true) {
                //se alvo esquerdo sair da tela
                if ((c.x > SCREEN_W + 4 * raio) || (c.y > SCREEN_H + 4 * raio) || (c.x < 0 - 4 * raio) || (c.y < 0 - 4 * raio))          //borda da janela
                {
                    //reinicia e perde vida
                    reinicia_alvoc(&c.x, &c.y, &c.dt, &c.y0, c.lado);
                    e -= 2;
                }
                //se alvo direito sair da tela
                if ((d.x > SCREEN_W + 4 * raio) || (d.y > SCREEN_H + 4 * raio) || (d.x < 0 - 4 * raio) || (d.y < 0 - 4 * raio))          //borda da janela
                {
                    //reinicia e perde vida
                    reinicia_alvod(&d.x, &d.y, &d.dt, &d.y0, d.lado);
                    e -= 2;
                }
                //movimento dos alvos a cada tick
                mov_alvoc(&c.x, &c.y, c.y0, &c.dt, c.lado, dificuldade);
                mov_alvod(&d.x, &d.y, d.y0, &d.dt, d.lado, dificuldade);

                //imprime a tela a cada tick
                imprime_tela(c.x, c.y, d.x, d.y, p, e, dificuldade, secreto1, secreto2);
                //numero de erros ate game over
                if (e <= 0)  
                {
                    // seta game over
                    rodando = false;
                }
            }
            else if (rodando == false) 
            {
                //reseta alvos e dificuldade apos gameover
                reinicia_alvod(&d.x, &d.y, &d.dt, &d.y0, d.lado);
                reinicia_alvoc(&c.x, &c.y, &c.dt, &c.y0, c.lado);
                dificuldade = DificuldadeInicial;
            }
        }
    }

    al_destroy_sample_instance(instance1);
    al_ungrab_mouse();
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_mouse_cursor(cursor);
    al_destroy_sample(tiro);

    return 0;
}
