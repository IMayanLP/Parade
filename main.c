#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TAD/jogo.h"
#include "TAD/lista.h"
#include "TAD/pilha.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

// Tamanho da tela
#define SCREEN_WIDTH 1900
#define SCREEN_HEIGTH 1050
// Largura da carta
#define CARD_WIDTH 100
#define CARD_HEIGTH 130
// Interface
#define inicioM 675
// Teclado
#define KEY_SEEN     1
#define KEY_RELEASED 2

// embaralhar cartas
void embaralhar(Lista *ldse, Pilha *pila) {
	time_t t;
	srand((unsigned) time(&t));

	int num;
	struct carta novacarta;
	while(quantidade(ldse) != 0) {
		do{
			num = rand() % 100;
		} while (num < 1 || num > quantidade(ldse));
		acessarIndice(ldse, &novacarta, num);
		inserirPilha(pila, novacarta);
		removerIndice(ldse, num);
	}
}

// gerar baralho ordenado
int gerarBaralho(Lista *a){
	struct carta novacarta;
	int i, j;
	for(i = 0; i <= 10; i++){
		for(j = 1; j <= 6; j++){
			novacarta.valor = i;
			switch(j){
			case 1: novacarta.naipe = 'A'; break;
			case 2: novacarta.naipe = 'B'; break;
			case 3: novacarta.naipe = 'C'; break;
			case 4: novacarta.naipe = 'D'; break;
			case 5: novacarta.naipe = 'E'; break;
			case 6: novacarta.naipe = 'F'; break;
			}
			inserirInicio(a, novacarta);
		}
	}
}

// iniciar elementos allegro
void must_init(bool test, const char *description){
    if(test) return;

    printf("Nao foi possivel carregar: %s\n", description);
    exit(1);
}

int main(){
    // Iniciando allegro, teclado e mouse
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");

    // Iniciando timer (limitador de FPS)
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    // Iniciando fila de eventos
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    // TELA CHEIA
    ALLEGRO_MONITOR_INFO info; // informacoes do monitor
    int res_x_comp, res_y_comp; // resolucao X e Y do computador
    al_get_monitor_info(0, &info); // recolhendo informacoes do monitor para o struct info
    res_x_comp = info.x2 - info.x1; // x1-> largura inicial da tela (0) || x2 -> largura final da tela
    res_y_comp = info.y2 - info.y1; // mesma logica de cima

    // Flags do display
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR); // menos essa, essa eh dos bitmaps

    ALLEGRO_DISPLAY* disp = al_create_display(res_x_comp, res_y_comp); // criando display do tamanho do monitor;
    must_init(disp, "display"); // iniciando display

    float red_x = res_x_comp / (float) SCREEN_WIDTH; // largura redimensionada
    float red_y = res_y_comp / (float) SCREEN_HEIGTH; // altura redimensionada

    // Transformacao para manter a proprocao com o monitor
    ALLEGRO_TRANSFORM transformar;
    al_identity_transform(&transformar);
    al_scale_transform(&transformar, red_x, red_y);
    al_use_transform(&transformar);

    // Iniciando fonte
    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    // Iniciando formas primitivas (linha, retangulo e os carai)
    must_init(al_init_primitives_addon(), "primitives");

    // registrando a fonte dos eventos na fila de eventos
    al_register_event_source(queue, al_get_keyboard_event_source()); // teclado
    al_register_event_source(queue, al_get_display_event_source(disp)); // display
    al_register_event_source(queue, al_get_timer_event_source(timer)); // timer
    al_register_event_source(queue, al_get_mouse_event_source()); // mouse

    al_hide_mouse_cursor(disp); // esconde mouse
    al_grab_mouse(disp); // impede o mouse de sair para fora da janela

    // Imagem
    must_init(al_init_image_addon(), "imagens");

    // Carregando sprites
    ALLEGRO_BITMAP* spritesheet = al_load_bitmap("src/spritesheet.png");
    must_init(spritesheet, "sprites");

    ALLEGRO_BITMAP* background = al_load_bitmap("src/fundo.png");
    must_init(background, "background");

    ALLEGRO_BITMAP* seta = al_load_bitmap("src/seta.png");
    must_init(seta, "seta");

    ALLEGRO_BITMAP* cartaSprite;
    must_init(cartaSprite, "cartaSprite");

    ALLEGRO_BITMAP* botaoSprite = al_create_sub_bitmap(spritesheet, 200, 780, 200, 40);
    must_init(botaoSprite, "botaoSprite");

    ALLEGRO_BITMAP* botaoProx = al_create_sub_bitmap(spritesheet, 400, 780, 200, 100);
    must_init(botaoProx, "botaoProx");

    int i, j;

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    // floats para receberem a posicao do mouse
    float x, y;
    x = 100;
    y = 100;

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    // Iniciando atividades do timer
    al_start_timer(timer);

    /** LOGICA DO PARADE **/
    // Criando gerador para o baralho
    Lista *gerador;
    	gerador = criar();
		gerarBaralho(gerador);

	// Formando a Pilha
		Pilha *baralho;
		baralho = criarPilha();
		embaralhar(gerador, baralho);

	// Lista da mesa
		Lista *mesa;
		mesa = criar();

	// Lista da mão
		Lista *mao;
		mao = criar();

	// Listas das collections
		Lista *collectionP[6];
		Lista *collectionM[6];
		for(i = 0; i < 6; i++){
			collectionP[i] = criar();
			collectionM[i] = criar();
		}

	// Gerando Mesa do Jogo
		struct carta cartaTemp;
		struct carta cartaTempC;
		for(i=0; i<5; i++){
			removerPilha(baralho, &cartaTemp);
			inserirInicio(mesa, cartaTemp);
		}

		for(i=0; i<5; i++){
			removerPilha(baralho, &cartaTemp);
			inserirInicio(mao, cartaTemp);
		}

		// variaveis condicionais do jogo
		int collectionHidden = 1;
		int xis, ypslo, altura;
		// 0 = Player || 1 = PC
		int vez = 0, jogando = 1, collectionsNtEmpty;

    // INICIO DO LOOP
    while(1){
        al_wait_for_event(queue, &event);

        collectionsNtEmpty = 0;
        for(i = 0; i < 6; i++){
            if(quantidade(collectionP[i]) > 0){
                collectionsNtEmpty++;
            }
        }
        if(collectionsNtEmpty == 6){
            jogando = 0;
        }

        // Eventos do jogo
        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(key[ALLEGRO_KEY_ESCAPE]) done = true;
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] &= KEY_SEEN;
                redraw = true;
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                x = event.mouse.x;
                y = event.mouse.y;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                // interacoes do mouse com a logica do jogo
                if(jogando == 1){
                    if(vez == 0){
                        if(collectionHidden == 1){
                            for(i = 0; i < quantidade(mao); i++){
                                if((x > inicioM+(i*110) && x < (inicioM+(i*110)+CARD_WIDTH)) && (y > 500 && y<630) && (quantidade(mao) > 0)){
                                    if(quantidadePilha(baralho) > 0) {
                                        acessarIndice(mao, &cartaTemp, i+1);
                                        for(j = cartaTemp.valor + 1; j <= quantidade(mesa); j++){
                                            acessarIndice(mesa, &cartaTempC, j);
                                            if(cartaTempC.valor <= cartaTemp.valor || cartaTempC.naipe == cartaTemp.naipe){
                                                removerIndice(mesa, j);
                                                switch(cartaTempC.naipe){
                                                    case 'A': inserirInicio(collectionP[0], cartaTempC); break;
                                                    case 'B': inserirInicio(collectionP[1], cartaTempC); break;
                                                    case 'C': inserirInicio(collectionP[2], cartaTempC); break;
                                                    case 'D': inserirInicio(collectionP[3], cartaTempC); break;
                                                    case 'E': inserirInicio(collectionP[4], cartaTempC); break;
                                                    case 'F': inserirInicio(collectionP[5], cartaTempC); break;
                                                }
                                                j--;
                                            }
                                        }
                                        removerIndice(mao, i+1);
                                        inserirInicio(mesa, cartaTemp);
                                        removerPilha(baralho, &cartaTemp);
                                        inserirInicio(mao, cartaTemp);
                                        vez = 1;
                                    } else if(quantidadePilha(baralho) == 0 && quantidade(mao) > 4){
                                        acessarIndice(mao, &cartaTemp, i+1);
                                        for(j = cartaTemp.valor + 1; j <= quantidade(mesa); j++){
                                            acessarIndice(mesa, &cartaTempC, j);
                                            if(cartaTempC.valor <= cartaTemp.valor || cartaTempC.naipe == cartaTemp.naipe){
                                                removerIndice(mesa, j);
                                                switch(cartaTempC.naipe){
                                                    case 'A': inserirInicio(collectionP[0], cartaTempC); break;
                                                    case 'B': inserirInicio(collectionP[1], cartaTempC); break;
                                                    case 'C': inserirInicio(collectionP[2], cartaTempC); break;
                                                    case 'D': inserirInicio(collectionP[3], cartaTempC); break;
                                                    case 'E': inserirInicio(collectionP[4], cartaTempC); break;
                                                    case 'F': inserirInicio(collectionP[5], cartaTempC); break;
                                                }
                                                j--;
                                            }
                                        }
                                        removerIndice(mao, i+1);
                                        inserirInicio(mesa, cartaTemp);
                                        vez = 1;
                                    } else if(quantidade(mao) <= 4 || quantidade(mao) >= 3){
                                        acessarIndice(mao, &cartaTemp, i+1);
                                        switch(cartaTemp.naipe){
                                            case 'A': inserirInicio(collectionP[0], cartaTemp); break;
                                            case 'B': inserirInicio(collectionP[1], cartaTemp); break;
                                            case 'C': inserirInicio(collectionP[2], cartaTemp); break;
                                            case 'D': inserirInicio(collectionP[3], cartaTemp); break;
                                            case 'E': inserirInicio(collectionP[4], cartaTemp); break;
                                            case 'F': inserirInicio(collectionP[5], cartaTemp); break;
                                        }
                                        removerIndice(mao, i+1);
                                    }
                                }
                            }
                        }
                    } else {
                        if((x > 850 && x < 1050) && (y > 660 && y < 760)){
                            if(quantidadePilha(baralho) > 0){
                                removerPilha(baralho, &cartaTemp);
                                for(j = cartaTemp.valor + 1; j <= quantidade(mesa); j++){
                                    acessarIndice(mesa, &cartaTempC, j);
                                    if(cartaTempC.valor <= cartaTemp.valor || cartaTempC.naipe == cartaTemp.naipe){
                                        removerIndice(mesa, j);
                                        switch(cartaTempC.naipe){
                                            case 'A': inserirInicio(collectionM[0], cartaTempC); break;
                                            case 'B': inserirInicio(collectionM[1], cartaTempC); break;
                                            case 'C': inserirInicio(collectionM[2], cartaTempC); break;
                                            case 'D': inserirInicio(collectionM[3], cartaTempC); break;
                                            case 'E': inserirInicio(collectionM[4], cartaTempC); break;
                                            case 'F': inserirInicio(collectionM[5], cartaTempC); break;
                                        }
                                        j--;
                                    }
                                }
                                inserirInicio(mesa, cartaTemp);
                                vez = 0;
                            }
                        }
                    }
                    if(quantidade(mao) == 2){
                        jogando = 0;
                    }
                    if((x > 850 && x < 1050) && (y > 800 && y < 840) && collectionHidden == 1){
                        collectionHidden = 0;
                    } else if((x > 850 && x < 1050) && (y > 1000 && y < 1040) && collectionHidden == 0){
                        collectionHidden = 1;
                    }
                }
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        // se o ESC for pressionado, quebra o loop
        if(done) break;

        // se algum evento estiver ativo na fila e o redraw ligado -> redesenha a tela atualizada
        if(redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_bitmap(background, 0, 0, 0);

            if(jogando == 1){
                if(collectionHidden == 1) {
                    for(i = 0; i < quantidade(mao); i++){
                        if((x > inicioM+(i*110) && x < (inicioM+(i*110)+CARD_WIDTH)) && (y > 500 && y<630) && (quantidade(mao) > 0)){
                            al_draw_rectangle(inicioM+(i*110),500,inicioM+(i*110)+CARD_WIDTH,630,al_map_rgb(200,200,200), 5);
                        }
                    }
                    cartaSprite = al_create_sub_bitmap(spritesheet, 100, 780, CARD_WIDTH, CARD_HEIGTH);
                    al_draw_bitmap(cartaSprite, 120, 130, 0);
                    xis = 230;
                    ypslo = 130;
                    for(i = 1; i <= quantidade(mesa); i++){
                        acessarIndice(mesa, &cartaTemp, i);
                        if(i == 15){
                            xis = 120;
                            ypslo = 270;
                        }
                        switch(cartaTemp.naipe){
                            case 'A': altura = 0; break;
                            case 'B': altura = 130; break;
                            case 'C': altura = 260; break;
                            case 'D': altura = 390; break;
                            case 'E': altura = 520; break;
                            case 'F': altura = 650; break;
                        }
                        cartaSprite = al_create_sub_bitmap(spritesheet, cartaTemp.valor*CARD_WIDTH, altura, CARD_WIDTH, CARD_HEIGTH);
                        al_draw_bitmap(cartaSprite, xis, ypslo, 0);
                        xis += 110;
                    }
                    xis = 675;
                    ypslo = 500;
                    for(i = 0; i < quantidade(mao); i++){
                        acessarIndice(mao, &cartaTemp, i+1);
                        switch(cartaTemp.naipe){
                            case 'A': altura = 0; break;
                            case 'B': altura = 130; break;
                            case 'C': altura = 260; break;
                            case 'D': altura = 390; break;
                            case 'E': altura = 520; break;
                            case 'F': altura = 650; break;
                        }
                        cartaSprite = al_create_sub_bitmap(spritesheet, cartaTemp.valor*CARD_WIDTH, altura, CARD_WIDTH, CARD_HEIGTH);
                        al_draw_bitmap(cartaSprite, xis, ypslo, 0);
                        xis += 110;
                    }
                    //al_draw_filled_rectangle(x, y, x + 5, y + 5, al_map_rgb(255, 0, 0));
                    cartaSprite = al_create_sub_bitmap(spritesheet, 0, 780, CARD_WIDTH, CARD_HEIGTH);
                    al_draw_bitmap(cartaSprite, 120, 500, 0);
                    al_draw_textf(font, al_map_rgb(255, 255, 255), 120, 480, 0, "%d restantes", quantidadePilha(baralho));
                    al_draw_bitmap(botaoSprite, 850, 800, 0);
                    if(vez == 1){
                        al_draw_bitmap(botaoProx, 850, 660, 0);
                    }
                } else {
                    xis = 120;
                    ypslo = 110;
                    for(i = 0; i < 6; i++){
                        for(j = 0; j < quantidade(collectionP[i]); j++){
                            acessarIndice(collectionP[i], &cartaTemp, j+1);
                            switch(cartaTemp.naipe){
                                case 'A': altura = 0; break;
                                case 'B': altura = 130; break;
                                case 'C': altura = 260; break;
                                case 'D': altura = 390; break;
                                case 'E': altura = 520; break;
                                case 'F': altura = 650; break;
                            }
                            cartaSprite = al_create_sub_bitmap(spritesheet, cartaTemp.valor*100, altura, CARD_WIDTH, CARD_HEIGTH);
                            al_draw_bitmap(cartaSprite, xis, ypslo, 0);
                            xis += 110;
                        }
                        xis = 120;
                        ypslo += 140;
                    }
                    al_draw_bitmap(botaoSprite, 850, 1000, 0);
                }
            } else {
                al_draw_textf(font, al_map_rgb(255, 255, 255), 500, 500, 0, "Acabou o jogo");
            }

            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", x, y);
            al_draw_bitmap(seta, x, y, 0);

            al_flip_display();

            redraw = false;
        }
    }

    // Destroi os bitmaps, fontes, tela, timer e tudo mais ao final do loop
    al_destroy_bitmap(cartaSprite);
    al_destroy_bitmap(botaoSprite);
    al_destroy_bitmap(seta);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

