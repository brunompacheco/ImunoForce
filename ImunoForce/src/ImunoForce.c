#include "Multiplayer.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <allegro5/allegro_font.h>  // Biblioteca para utilização de fontes
#include <allegro5/allegro_ttf.h>   // Biblioteca para utilização de fontes
#include "AllegroDef.h"
#include "ImunoEngine.h"


//Object object_head = {0,header,NULL,NULL,NULL,NULL};
main(int argc, char *argv[]) {
	int frame = 1, bTrig = 10,bTrig2 = 10 , bulletFreq = 10, i;
	int currentPlayer = 1;
	int dead = 1;
	int monkey = 1; // JUST FOR TEST
	int shoot_enable = 0;
	int shoot_enable2 = 0;
	int bullet_type = 1;
	int DNA_points = 0;
	int DNA_spent = 0;
	int UPGRADE[3];
	//int upgrades = sizeof(UPGRADE) / (sizeof(UPGRADE[0]));
	for(i = 0; i < sizeof(UPGRADE) / (sizeof(UPGRADE[0])); i++) {
		UPGRADE[i] = 1;
	}

	ALLEGRO_DISPLAY* display;
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* event_queue;

	bool isSet = false;
	bool quit = false;
	int host = false;
	int gameState = 0;
	int multiState = 0;
	int select = 0;

	char DOWN = 1, UP = 1, LEFT = 1, RIGHT = 1;
	char* inPkt;
	Data data[BUFLEN / sizeof(Data)];

	Object* p = object_search(0);

	initialization();

	ALLEGRO_FONT *arial_24 = al_load_font("arial.ttf", 24, 0);
	srand((unsigned) time(NULL)); // Uncertainty principle
	//creation(display, timer, event_queue);
	display = al_create_display(DISPLAY_W, DISPLAY_H);
	if (!display) {

		al_show_native_message_box(NULL, NULL, NULL, "al_create_display() failed", NULL, 0);
		exit(EXIT_FAILURE);
	}
	timer = al_create_timer(1 / 60.0);
	if (!timer) {
		al_show_native_message_box(NULL, NULL, NULL, "al_create_timer() failed",
		NULL, 0);
		exit(EXIT_FAILURE);
	}
	event_queue = al_create_event_queue();
	if (!event_queue) {
		al_show_native_message_box(NULL, NULL, NULL, "al_create_event_queue() failed", NULL, 0);
		exit(EXIT_FAILURE);
	}

	server_initialise();

	sprites[player][B_L] = al_load_bitmap("Sprites/Ship1B_L.png");
	sprites[player][B_C] = al_load_bitmap("Sprites/Ship1B_C.png");
	sprites[player][B_R] = al_load_bitmap("Sprites/Ship1B_R.png");
	sprites[player][S_L] = al_load_bitmap("Sprites/Ship1S_L.png");
	sprites[player][S_C] = al_load_bitmap("Sprites/Ship1S_C.png");
	sprites[player][S_R] = al_load_bitmap("Sprites/Ship1S_R.png");
	sprites[player][F_L] = al_load_bitmap("Sprites/Ship1F_L.png");
	sprites[player][F_C] = al_load_bitmap("Sprites/Ship1F_C.png");
	sprites[player][F_R] = al_load_bitmap("Sprites/Ship1F_R.png");

	sprites[player2][B_L] = al_load_bitmap("Sprites/Ship2B_L.png");
	sprites[player2][B_C] = al_load_bitmap("Sprites/Ship2B_C.png");
	sprites[player2][B_R] = al_load_bitmap("Sprites/Ship2B_R.png");
	sprites[player2][S_L] = al_load_bitmap("Sprites/Ship2S_L.png");
	sprites[player2][S_C] = al_load_bitmap("Sprites/Ship2S_C.png");
	sprites[player2][S_R] = al_load_bitmap("Sprites/Ship2S_R.png");
	sprites[player2][F_L] = al_load_bitmap("Sprites/Ship2F_L.png");
	sprites[player2][F_C] = al_load_bitmap("Sprites/Ship2F_C.png");
	sprites[player2][F_R] = al_load_bitmap("Sprites/Ship2F_R.png");


	/*sprites[player2][B_L] = al_load_bitmap("Sprites/ShipB_L.png");
	sprites[player2][B_C] = al_load_bitmap("Sprites/ShipB_C.png");
	sprites[player2][B_R] = al_load_bitmap("Sprites/ShipB_R.png");
	sprites[player2][S_L] = al_load_bitmap("Sprites/ShipS_L.png");
	sprites[player2][S_C] = al_load_bitmap("Sprites/ShipS_C.png");
	sprites[player2][S_R] = al_load_bitmap("Sprites/ShipS_R.png");
	sprites[player2][F_L] = al_load_bitmap("Sprites/ShipF_L.png");
	sprites[player2][F_C] = al_load_bitmap("Sprites/ShipF_C.png");
	sprites[player2][F_R] = al_load_bitmap("Sprites/ShipF_R.png");*/

//	sprites[enemy][0] = al_load_bitmap("Sprites/Seeker0.png");
//	sprites[enemy][1] = al_load_bitmap("Sprites/Seeker1.png");
//	sprites[enemy][2] = al_load_bitmap("Sprites/Seeker2.png");
//	sprites[enemy][3] = al_load_bitmap("Sprites/Seeker3.png");
//	sprites[enemy][4] = al_load_bitmap("Sprites/Seeker4.png");
//	sprites[enemy][5] = al_load_bitmap("Sprites/Seeker5.png");
//	sprites[enemy][6] = al_load_bitmap("Sprites/Seeker6.png");
//	sprites[enemy][7] = al_load_bitmap("Sprites/Seeker7.png");

	sprites[enemy][0] = al_load_bitmap("Sprites/Virus2.png");


	sprites[enemy_b][0] = al_load_bitmap("Sprites/Virus3.png");

	sprites[enemy_c][0] = al_load_bitmap("Sprites/Virus1-1.png");
	sprites[enemy_c][1] = al_load_bitmap("Sprites/Virus1-2.png");
	sprites[enemy_c][2] = al_load_bitmap("Sprites/Virus1-3.png");
	sprites[enemy_c][3] = al_load_bitmap("Sprites/Virus1-4.png");

	sprites[bullet][0] = al_load_bitmap("Sprites/bullet3.png");

	sprites[background][0] = al_load_bitmap("Sprites/BackgroundB.png");
	ALLEGRO_BITMAP *cursor = al_load_bitmap("Sprites/Select.png");

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 100, 0, "         WE ARE LOADING BITCH!!!");
	al_flip_display();

	for(i = 0; i < 9; i++) {
		masks[player][i] = mask_new(sprites[player][i]);
		masks[player2][i] = masks[player][i];
		printf("Creating player mask n%d\n", i + 1);
	}

	for(i = 0; i < 9; i++) {
		masks[player2][i] = mask_new(sprites[player2][i]);
		printf("Creating player2 mask n%d\n", i + 1);
	}
//	for(i = 0; i < 8; i++) {
//		masks[enemy][i] = mask_new(sprites[enemy][i]);
//		printf("Creating enemy mask n%d\n", i + 1);
//	}

	masks[enemy][0] = mask_new(sprites[enemy][0]);
	
	masks[enemy_b][0] = mask_new(sprites[enemy_b][0]);
	
	for (i = 0; i < 4; i++) {
		masks[enemy_c][i] = mask_new(sprites[enemy_c][i]);
		printf("Creating enemy_c mask n%d\n", i + 1);
	}
	for(i = 0; i < 1; i++) {
		masks[bullet][i] = mask_new(sprites[bullet][i]);
		printf("Creating bullet mask n%d\n", i + 1);
	}

	background_add(0, 0);
	background_add(0, -DISPLAY_H);

	Object normal;
	normal.type = bullet;
	normal.vector_size = 1;
	normal.frame_delay = 10;
	normal.img_i = 0;
	normal.height = al_get_bitmap_height(sprites[normal.type][normal.img_i]);
	normal.width = al_get_bitmap_width(sprites[normal.type][normal.img_i]);
	normal.vx = 0;
	normal.vy = -12;
	normal.life = -1;

	Object left;
	left.type = bullet;
	left.vector_size = 1;
	left.frame_delay = 10;
	left.img_i = 0;
	left.height = al_get_bitmap_height(sprites[left.type][left.img_i]);
	left.width = al_get_bitmap_width(sprites[left.type][left.img_i]);
	left.vx = 3;
	left.vy = -10;
	left.life = -1;

	Object right;
	right.type = bullet;
	right.vector_size = 1;
	right.frame_delay = 10;
	right.img_i = 0;
	right.height = al_get_bitmap_height(sprites[right.type][right.img_i]);
	right.width = al_get_bitmap_width(sprites[right.type][right.img_i]);
	right.vx = -3;
	right.vy = -10;
	right.life = -1;

	Object d_normal;
	d_normal.type = bullet;
	d_normal.vector_size = 1;
	d_normal.frame_delay = 10;
	d_normal.img_i = 0;
	d_normal.height = al_get_bitmap_height(sprites[d_normal.type][d_normal.img_i]);
	d_normal.width = al_get_bitmap_width(sprites[d_normal.type][d_normal.img_i]);
	d_normal.vx = 0;
	d_normal.vy = 12;
	d_normal.life = -1;

	Object d_left;
	d_left.type = bullet;
	d_left.vector_size = 1;
	d_left.frame_delay = 10;
	d_left.img_i = 0;
	d_left.height = al_get_bitmap_height(sprites[d_left.type][d_left.img_i]);
	d_left.width = al_get_bitmap_width(sprites[d_left.type][d_left.img_i]);
	d_left.vx = 3;
	d_left.vy = 10;
	d_left.life = -1;

	Object d_right;
	d_right.type = bullet;
	d_right.vector_size = 1;
	d_right.frame_delay = 10;
	d_right.img_i = 0;
	d_right.height = al_get_bitmap_height(sprites[d_right.type][d_right.img_i]);
	d_right.width = al_get_bitmap_width(sprites[d_right.type][d_right.img_i]);
	d_right.vx = -3;
	d_right.vy = 10;
	d_right.life = -1;

	Object enemies[5];
	enemies[0].type = enemy_c;
	enemies[0].vector_size = 4;
	enemies[0].frame_delay = 7;
	enemies[0].img_i = 0;
	enemies[0].height = al_get_bitmap_height(sprites[enemies[0].type][enemies[0].img_i]);
	enemies[0].width = al_get_bitmap_width(sprites[enemies[0].type][enemies[0].img_i]);
	enemies[0].vx = 0.6;
	enemies[0].vy = 3;
	enemies[0].life = 3;
	strcpy_s(enemies[0].String, sizeof(enemies[0].String), "NonSeeker");	// defines if its a seeker or not (1 yes 0 no)

	enemies[1].type = enemy;
	enemies[1].vector_size = 1;
	enemies[1].frame_delay = 5;
	enemies[1].img_i = 0;
	enemies[1].height = al_get_bitmap_height(sprites[enemies[1].type][enemies[1].img_i]);
	enemies[1].width = al_get_bitmap_width(sprites[enemies[1].type][enemies[1].img_i]);
	enemies[1].vx = 0;
	enemies[1].vy = 1;
	enemies[1].life = 3;
	strcpy_s(enemies[1].String, sizeof(enemies[1].String), "Seeker");	// defines if its a seeker or not (1 yes 0 no)

	enemies[2].type = enemy_b;
	enemies[2].vector_size = 1;
	enemies[2].frame_delay = 5;
	enemies[2].img_i = 0;
	enemies[2].height = al_get_bitmap_height(sprites[enemies[2].type][enemies[2].img_i]);
	enemies[2].width = al_get_bitmap_width(sprites[enemies[2].type][enemies[2].img_i]);
	enemies[2].vx = 0;
	enemies[2].vy = 4;
	enemies[2].life = 5;
	strcpy_s(enemies[2].String, sizeof(enemies[2].String), "Wall");

	enemies[3].type = enemy_c;
	enemies[3].vector_size = 4;
	enemies[3].frame_delay = 7;
	enemies[3].img_i = 0;
	enemies[3].height = al_get_bitmap_height(sprites[enemies[3].type][enemies[3].img_i]);
	enemies[3].width = al_get_bitmap_width(sprites[enemies[3].type][enemies[3].img_i]);
	enemies[3].vx = -0.6;
	enemies[3].vy = 3;
	enemies[3].life = 3;
	strcpy_s(enemies[3].String, sizeof(enemies[3].String), "NonSeeker");	// defines if its a seeker or not (1 yes 0 no)

	enemies[4].type = enemy;
	enemies[4].vector_size = 1;
	enemies[4].frame_delay = 5;
	enemies[4].img_i = 0;
	enemies[4].height = al_get_bitmap_height(sprites[enemies[4].type][enemies[4].img_i]);
	enemies[4].width = al_get_bitmap_width(sprites[enemies[4].type][enemies[4].img_i]);
	enemies[4].vx = 0;
	enemies[4].vy = 10;
	enemies[4].life = 3;
	strcpy_s(enemies[4].String, sizeof(enemies[4].String), "Shooter");	// defines if its a seeker or not (1 yes 0 no)

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//initializes players
	p = player_add("Ronaldo", 5, 12);
	//player_add(p,"Sprites/sperm_0L.png");
	p = object_search(1);

	al_start_timer(timer);
	while (!quit) {
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			quit = true;
		}

		if (ev.type == ALLEGRO_EVENT_TIMER && gameState==1) {
			//	FRAME COUNT
			frame = (frame >= 60) ? 1 : frame + 1;

			//	MOVEMENT
			p = object_search(1);
				p->vy += 0.8 * keys[KEY_DOWN] - 0.8 * keys[KEY_UP];
				p->vx += 0.8 * keys[KEY_RIGHT] - 0.8 * keys[KEY_LEFT];

			p = object_search(2);
				p->vy += 0.8 * keys2[KEY_DOWN] - 0.8 * keys2[KEY_UP];
				p->vx += 0.8 * keys2[KEY_RIGHT] - 0.8 * keys2[KEY_LEFT];
			// ADD enemy
			int offset = rand() % 400 + 200;
			int enemy_rand = rand() % 2;
			if (enemy_count() < log2(get_score() + 2) * 2) {
				int level = get_score() / 5;
				level = (level > 5) ? 5 : level;
				switch(rand() % (level + 1)) {
					case 5:
						p = object_search(1);
						for(i = 0; i < 4; i++) {
							enemy_add(enemies[4], p->x, -50 - 30 * i);
						}
					break;
					case 4:
						for(i = 0; i < 3; i++) {
							enemy_add(enemies[1], (rand() % 2) * 900 - 50, offset + 200 * i - 200);
						}
					break;
					case 3:
						for(i = 0; i < 3; i++) {
							enemy_add(enemies[1], offset + 200 * i - 200, -50);
						}
					break;
					case 2:
						if (!object_find("Wall")) {
							for(i = 0; i < 50; i++) {
								enemy_add(enemies[2], offset - 180, -50 - 50 * i);
								enemy_add(enemies[2], offset + 180, -50 - 50 * i);
							}
						}

					break;
					case 1:
						for(i = 0; i < 10; i++) {
							enemy_add(enemies[3 * enemy_rand], 50 * i, (enemy_rand == 1) ? (-300 + 25 * i) : (-50 - 25 * i));
						}
					break;
					case 0:
						for(i = 0; i < 3; i++) {
							enemy_add(enemies[3 * enemy_rand], offset + 50 * i, (enemy_rand == 1) ? (-125 + 25 * i) : (-50 - 25 * i));
						}
					break;
				}
			}

			if (keys[KEY_1] * dead) {
				enemy_add(enemies[1], rand() % 540, -50);
				//enemy_add(enemies[3], rand() % 540, -50);
				dead = 0;
			}
			if (keys[KEY_2] * monkey) {
				enemy_add(enemies[4], rand() % 540, -50);
				monkey = 0;
			}

			//	FIRE
			if (keys[KEY_SPACE] && bTrig == 10) {
				shoot_enable = 1;
				bTrig = frame % bulletFreq;
			} else if (keys[KEY_SPACE] && frame % bulletFreq == bTrig) {
				shoot_enable = 1;
			} else if (!keys[KEY_SPACE]) {
				bTrig = 10;
			}

			if (keys2[KEY_SPACE] && bTrig2 == 10) {
				shoot_enable2 = 1;
				bTrig2 = frame % bulletFreq;
			} else if (keys2[KEY_SPACE] && frame % bulletFreq == bTrig2) {
				shoot_enable2 = 1;
			} else if (!keys2[KEY_SPACE]) {
				bTrig2 = 10;
			}


			switch(bullet_type * shoot_enable) {
				case 3:
					bullet_add(d_normal, object_search(currentPlayer));
					bullet_add(d_left, object_search(currentPlayer));
					bullet_add(d_right, object_search(currentPlayer));
				case 2:
					bullet_add(left, object_search(currentPlayer));
					bullet_add(right, object_search(currentPlayer));
				case 1:
					bullet_add(normal, object_search(currentPlayer));
					shoot_enable = 0;
				break;
				default:
				break;
			}
			p = object_search(2);
			switch(bullet_type * shoot_enable2) {
				case 3:
					bullet_add(d_normal, object_search(2));
					bullet_add(d_left, object_search(2));
					bullet_add(d_right, object_search(2));
				case 2:
					bullet_add(left, object_search(2));
					bullet_add(right, object_search(2));
				case 1:
					bullet_add(normal, object_search(2));
					shoot_enable2 = 0;
				break;
				default:
				break;
			}

			//anim(p, 5, p_sprites, p_masks, 12);
			object_anim();
			object_colision();
			object_move();
		}

		//	KEYBOARD VERIFICATION
		//keyboard_verification(ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					keys[KEY_UP] = true;
				break;
				case ALLEGRO_KEY_DOWN:
					keys[KEY_DOWN] = true;
				break;
				case ALLEGRO_KEY_LEFT:
					keys[KEY_LEFT] = true;
				break;
				case ALLEGRO_KEY_RIGHT:
					keys[KEY_RIGHT] = true;
				break;
				case ALLEGRO_KEY_SPACE:
					keys[KEY_SPACE] = true;
				break;
				case ALLEGRO_KEY_1:
					keys[KEY_1] = true;
				break;
				case ALLEGRO_KEY_2:
					keys[KEY_2] = true;
				break;
				case ALLEGRO_KEY_ENTER:
					keys[KEY_ENTER] = true;
				break;
			}
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_UP:
					keys[KEY_UP] = false;
					UP = 1;
				break;
				case ALLEGRO_KEY_DOWN:
					keys[KEY_DOWN] = false;
					DOWN = 1;
				break;
				case ALLEGRO_KEY_LEFT:
					keys[KEY_LEFT] = false;
					LEFT = 1;
				break;
				case ALLEGRO_KEY_RIGHT:
					keys[KEY_RIGHT] = false;
					RIGHT = 1;
				break;
				case ALLEGRO_KEY_SPACE:
					keys[KEY_SPACE] = false;
				break;
				case ALLEGRO_KEY_1:
					keys[KEY_1] = false;
					dead = 1;
				break;
				case ALLEGRO_KEY_2:
					keys[KEY_2] = false;
					monkey = 1;
				break;
				case ALLEGRO_KEY_ENTER:
					keys[KEY_ENTER] = false;
				break;
				case ALLEGRO_KEY_ESCAPE:
					gameState = 0;
				break;
			}
		}

		if (al_is_event_queue_empty(event_queue)) {
			int cost[sizeof(UPGRADE) / (sizeof(UPGRADE[0]))];
			DNA_points = get_score() - DNA_spent;
			int width, height;
			switch(gameState) {
				case 0:			// MENU
					select = (select > 3) ? 3 : select;
					select = (select < 0) ? 0 : select;
					multiState = 0;
					if(host == true){
						al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 100, 0, "         RETURN TO GAME");
					}else{
						al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 100, 0, "         SINGLE-PLAYER");
					}
					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 125, 0, "         MULTI-PLAYER  ");
					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 150, 0, "         UPGRADE  ");
					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 175, 0, "         QUIT  ");
					width = al_get_bitmap_width(cursor);
					height = al_get_bitmap_height(cursor);
					al_draw_scaled_bitmap(cursor,0,0,width,height,120, 100 + 25*select ,width*0.8,height*0.8,0);
					//al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 100 + select * 25, 0, "       >");

					if (keys[KEY_UP] * UP) {
						select += -1;
						UP = 0;
					}
					if (keys[KEY_DOWN] * DOWN) {
						select += 1;
						DOWN = 0;
					}

					if (keys[KEY_ENTER]) {
						gameState = select + 1;
						keys[KEY_ENTER] = false;
					}
				break;
				case 1:
					background_draw();
					object_draw();
					if(!player_alive()){
						gameState = 5;
					}
					p = object_search(1);
					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 150, 0, "         LIFE: %d ", p->life);
					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 350, 0, "         SCORE: %d ", get_score());
					al_draw_textf(arial_24, al_map_rgb(242, 210, 99), 100, 400, 0, "		  DNA: %d ", DNA_points);
					if(host == true){
						// DATA WRITE
						memset(data, '\0', BUFLEN);
						p = object_search(0);
						for(i = 0; i < BUFLEN / sizeof(Data); i++) {
								if ( (p->type != background) && (p->type != header) ) {
									data[i].img_i = p->img_i;
									data[i].type = p->type;
									data[i].x = p->x;
									data[i].y = p->y;
									data[i].dir = p->dir;
									p = p->next;
								} else if (p->type == background) {
									p = p->next;
								}
						}


						r_receive(keys2);

					//	printf("%s\n", inPkt);

						d_send(data);
					}
				break;
				case 2:			// MULTIPLAYER
					switch(multiState) {
						case 0:			// Menu
							select = (select > 1) ? 1 : select;
							select = (select < 0) ? 0 : select;
							al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 100, 0, "         HOST");
							al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 125, 0, "         JOIN  ");
							width = al_get_bitmap_width(cursor);
							height = al_get_bitmap_height(cursor);
							al_draw_scaled_bitmap(cursor,0,0,width,height,120, 100 + 25*select ,width*0.8,height*0.8,0);
							//al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 100 + select * 25, 0, "       >");

							if (keys[KEY_UP] * UP) {
								select += -1;
								UP = 0;
							}
							if (keys[KEY_DOWN] * DOWN) {
								select += 1;
								DOWN = 0;
							}

							if (keys[KEY_ENTER]) {
								multiState = select + 1;
								keys[KEY_ENTER] = false;
							}
						break;
						case 1:			// Host
							if (!isSet) {
								set_server();
								isSet = true;
								p = player_add("Jefferson", 5, 12);
							}
							host = true;
							gameState = 1;
						break;
						case 2:			// Join
							background_draw();

							if (!isSet) {
								set_client();
								isSet = true;
								p = object_search(1);
								p = object_del(p);
							}

							r_send(keys);

							d_receive(data);

							//al_draw_bitmap(sprites[data[0].type][data[0].img_i], data[0].x, data[0].y, 0);

							//data_draw(data, sprites);
							for(i = 0; i < BUFLEN / sizeof(Data); i++) {
								if (&data[i] != NULL) {
									if (data[i].type != header && data[i].type != background) {
										al_draw_bitmap(sprites[data[i].type][data[i].img_i], data[i].x, data[i].y, data[i].dir);
									}
								}
							}

						break;
					}
				break;
				case 3:
					select = (select > (sizeof(UPGRADE) / (sizeof(UPGRADE[0]))) - 1) ? (sizeof(UPGRADE) / (sizeof(UPGRADE[0]))) - 1 : select;
					select = (select < 0) ? 0 : select;
					cost[0] = UPGRADE[0] * 10;
					cost[1] = UPGRADE[1] * 5;
					cost[2] = UPGRADE[2] * 20;

					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 100, 0, "         TRIGGER: %d", 11 - bulletFreq);
					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 125, 0, "         FORCE: %d", -normal.life);
					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 150, 0, "         BULLET: %d", bullet_type);
					width = al_get_bitmap_width(cursor);
					height = al_get_bitmap_height(cursor);
					al_draw_scaled_bitmap(cursor,0,0,width,height,120, 100 + 25*select ,width*0.8,height*0.8,0);
					//al_draw_bitmap(cursor,100,100 + 25*select,0);
					//al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 100 + select * 25, 0, "       >");
					for(i = 0; i < sizeof(UPGRADE) / (sizeof(UPGRADE[0])); i++) {
						al_draw_textf(arial_24, al_map_rgb(242, 210, 99), 300, 100 + i * 25, 0, " %d ", cost[i]);
					}
					if (keys[KEY_UP] * UP) {
						select += -1;
						UP = 0;
					}
					if (keys[KEY_DOWN] * DOWN) {
						select += 1;
						DOWN = 0;
					}
					if (keys[KEY_RIGHT] * RIGHT * (DNA_points >= cost[select])) {
						DNA_spent += cost[select];
						UPGRADE[select] += 1;
						RIGHT = 0;
					}
					if (keys[KEY_LEFT] * LEFT) {
						UPGRADE[select] += -1;
						LEFT = 0;
					}

					UPGRADE[0] = (UPGRADE[0] > 10) ? 10 : UPGRADE[0];
					bulletFreq = 10 - (UPGRADE[0] - 1);
					normal.life = -UPGRADE[1];
					bullet_type = UPGRADE[2];

				break;
				case 4:
					quit = true;
				break;
				case 5:
					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 100, 0, "         GAME OVER");
					al_draw_textf(arial_24, al_map_rgb(255, 255, 255), 100, 125, 0, "         PRESS ENTER TO CONTINUE");
					if (keys[KEY_ENTER]) {
						game_reset();
						gameState = 0;
						multiState = 0;
						host = 0;
						for(i = 0; i < sizeof(UPGRADE) / (sizeof(UPGRADE[0])); i++) {
							UPGRADE[i] = 1;
						}
						UPGRADE[0] = (UPGRADE[0] > 10) ? 10 : UPGRADE[0];
						bulletFreq = 10 - (UPGRADE[0] - 1);
						normal.life = -UPGRADE[1];
						bullet_type = UPGRADE[2];
						DNA_spent = 0;
						p = player_add("Ronaldo", 5, 12);
						keys[KEY_ENTER] = false;
					}
					break;
			}
			//object_track();

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

	exit(EXIT_SUCCESS);
}
