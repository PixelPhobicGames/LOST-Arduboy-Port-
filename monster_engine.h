
//This isnt really a game engine its more of a wrapper for arduboy2
// This is also more for porting Not an Actual Engine :(
#include "player.h"
#include "enemy.h"
#include <ArduboyPlaytune.h>
#include <Arduboy2.h>

static Arduboy2 arduboy;
static ArduboyPlaytune tunes(arduboy.audio.enabled);
typedef struct Animation
{
    int count;
} Animation;
static Animation animation;

typedef struct Level
{
    int id;
} Level;
static Level level;
typedef struct Transition
{
    int happened;
} Transition;
static Transition transition;

int Monster_Engine_Draw(int texture_x,int texture_y,const unsigned char PROGMEM texture[]) {
    Sprites::drawOverwrite(texture_x, texture_y, texture, 0);
    return 0;
}

// KEY CODES
// UP: 1
// DOWN: 3
// RIGHT:2
// LEFT: 4
// B: 5
// A: 6
bool controller_input(int input) {
    Arduboy2 arduboy;
    switch (input)
    {
    case 1:
        return arduboy.pressed(UP_BUTTON);
        break;
    case 2:
        return arduboy.pressed(RIGHT_BUTTON);
        break;
    case 3:
        return arduboy.pressed(DOWN_BUTTON);
        break;
    case 4:
        return arduboy.pressed(LEFT_BUTTON);
        break;
    case 5:
        return arduboy.pressed(A_BUTTON);
        break;
    case 6:
        return arduboy.pressed(B_BUTTON);
        break;
    }
}


int Monster_Engine_Play_Sound(ArduboyPlaytune tunes,int freq,int time) {
    tunes.initChannel(PIN_SPEAKER_1);
    tunes.initChannel(PIN_SPEAKER_2);
    tunes.tone(freq, time);
    return 0;
}

static void update_animations() {
    animation.count += 10;
    if (animation.count >= 100) {
        animation.count = 0;
    }
}
static void Monster_INIT() {
    player.x = 20;
    player.y = 50;
    player.projectilex = 0;
    player.projectiley = 0;
    player.direction = 1;
    player.exp = 0;
    animation.count = 0;
    enemy.x = 0;
    enemy.y = 0;
    enemy.isjump = false;
    enemy.jumpcount = 0;
    player.health = 125;
    player.isshoot = false;
    level.id = 0;
    player.ismoving = false;
    enemy.isenemy = false;
    transition.happened = false;
}

static void update_inputs() {
    if (controller_input(6) && player.isshoot == false && level.id != 0 ) {
        //tunes.tone(300, 100);
        Monster_Engine_Play_Sound(tunes,300,100);
        //tunes.tone(200, 100);
        Monster_Engine_Play_Sound(tunes,200,100);
        switch (player.direction)
        {
        case 1:
            player.projectilex = player.x;
            player.projectiley = player.y - 15;
            break;
        case 2:
            player.projectilex = player.x + 15;
            player.projectiley = player.y;
            break;
        case 3:
            player.projectilex = player.x;
            player.projectiley = player.y+ 15;
            break;
        case 4:
            player.projectilex = player.x - 15;
            player.projectiley =player.y;
            break;

        }
        player.isshoot = true;
    }
    if (controller_input(1) && player.isshoot == false && level.id != 0 && controller_input(2) == false && controller_input(4) == false && controller_input(3) == false) {
        player.direction = 1;
        player.y-= 2;
        if (animation.count >= 50 ) {
            Monster_Engine_Draw(player.x,player.y,player_1_2);
        }
        else {
            Monster_Engine_Draw(player.x,player.y,player_1);
        }
        player.ismoving = true;
    }

    if (controller_input(3) && player.isshoot == false && level.id != 0 && controller_input(2) == false && controller_input(1) == false && controller_input(4) == false) {
        player.direction = 3;
        player.y+= 2;
        if (animation.count >= 50 ) {
            Monster_Engine_Draw(player.x,player.y,player_3_2);
        }
        else {
            Monster_Engine_Draw(player.x,player.y,player_3);
        }
        player.ismoving = true;
    }

    if (controller_input(2) && player.isshoot == false && level.id != 0 && controller_input(4) == false && controller_input(1) == false && controller_input(3) == false) {
        player.direction = 2;
        player.x += 2;
        if (animation.count >= 50 ) {
            Monster_Engine_Draw(player.x,player.y,player_2_2);
        }
        else {
            Monster_Engine_Draw(player.x,player.y,player_2);
        }
        player.ismoving = true;
    }

    if (controller_input(4) && player.isshoot == false && level.id != 0 && controller_input(2) == false && controller_input(1) == false && controller_input(3) == false) {
        player.direction = 4;
        player.x -= 2;
        if (animation.count >= 50) {
            Monster_Engine_Draw(player.x,player.y,player_4_2);
        }
        else {
            Monster_Engine_Draw(player.x,player.y,player_4);
        }
        player.ismoving = true;
    }

    if (!controller_input(4)&& !controller_input(2) && !controller_input(1) && !controller_input(3)) {
        if (level.id != 0) {
            player.ismoving = false;
        }
    }

    if (player.ismoving == false && level.id != 0) {
        switch (player.direction) {
        case 1:
            Monster_Engine_Draw(player.x,player.y,player_1);
            break;
        case 2:
            Monster_Engine_Draw(player.x,player.y,player_2);
            break;
        case 3:
            Monster_Engine_Draw(player.x,player.y,player_3);
            break;
        case 4:
            Monster_Engine_Draw(player.x,player.y,player_4);
            break;
        }
    }

    if (player.isshoot == true) {
        switch (player.direction)
        {
        case 1:
            Monster_Engine_Draw(player.projectilex,player.projectiley,arrow_1);
            player.projectiley -= 8;
            if (player.projectiley <= -60) {
                player.isshoot = false;
            }
            break;
        case 2:
            Monster_Engine_Draw(player.projectilex,player.projectiley,arrow_2);
            player.projectilex += 8;
            if (player.projectilex >= 160) {
                player.isshoot = false;
            }
            break;
        case 3:
            Monster_Engine_Draw(player.projectilex,player.projectiley,arrow_3);
            player.projectiley += 8;
            if (player.projectiley >= 60) {
                player.isshoot = false;
            }
            break;
        case 4:
            Monster_Engine_Draw(player.projectilex,player.projectiley,arrow_4);
            player.projectilex -= 8;
            if (player.projectilex <= -10) {
                player.isshoot = false;
            }
            break;
        }
    }

    if (controller_input(5)) {
        if (player.x - enemy.x <= 50 && player.x - enemy.x >= 0 &&player.y- enemy.y >= -50 &&player.y- enemy.y <= 0) {
            enemy.isenemy = false;
            Monster_Engine_Play_Sound(tunes,50,100);
            if (enemy.isenemy == true) {
                player.exp += 1;
            }
        }
    }
    if (player.projectilex - enemy.x <= 50 && player.projectilex  - enemy.x >= 0 && player.projectiley - enemy.y >= -50 && player.projectiley - enemy.y <= 0) {
        enemy.isenemy = false;
        Monster_Engine_Play_Sound(tunes,100,100);
        player.exp += 1;
    }
    if (enemy.isenemy == true) {
        Monster_Engine_Draw(enemy.x,enemy.y,slime);
        if (enemy.isjump == false) {
            enemy.isjump = true;
        }
        if (player.y >= enemy.y) {
            enemy.y ++;
        }
        if (player.y <= enemy.y) {
            enemy.y --;
        }
        if (player.x >= enemy.x) {
            enemy.x ++;
        }
        if (player.x <= enemy.x) {
            enemy.x --;
        }

        if (player.x - enemy.x <= 50 && player.x - enemy.x >= 0 &&player.y- enemy.y >= -50 &&player.y- enemy.y <= 0) {
            player.health -= 1;
        }
        if (player.health == 0) {
            level.id =  0;
        }
        if (enemy.isjump == true) {
            if (enemy.jumpcount >= -10) {
                enemy.y -= (enemy.jumpcount * abs(enemy.jumpcount)) * .1;
                enemy.jumpcount -= 1;
            }
            else {
                enemy.jumpcount = 10;
                enemy.isjump = false;
            }
        }

    }
}

int update_hearts(float health,const unsigned char PROGMEM heart[]) {
    if (health >= 33 ) {
        //DrawTexture(hearts,380,5,RED);
        Sprites::drawOverwrite(92, 5, heart, 0);
    }
    if (health >= 66) {
        //DrawTexture(hearts,416,5,RED);
        Sprites::drawOverwrite(101, 5, heart, 0);
    }
    if (health >= 99) {
        //DrawTexture(hearts,452,5,RED);
        Sprites::drawOverwrite(110, 5, heart, 0);
    }
}

static void Create_CollisionBlock( int Smallx , int Bigx , int Smally,int Bigy ){
    if (player.x>= Smallx && player.x<= Bigx && player.y <= Bigy && player.y >= Smally){
      if (player.direction == 1){player.y +=2;}
      if (player.direction == 4){player.x+=2;}
      if (player.direction == 2){player.x-= 2;}
      if (player.direction == 3){player.y -= 2;}
	}
}

static void Right_sceneTranstion(int scene){
  if (player.x >= 125) {
      player.x = 10;
      transition.happened  = true;
      level.id = scene;
  }
  return NULL;
}
static void Left_sceneTranstion(int scene){
  if (player.x <= -15) {
    player.x = 110;
    transition.happened  = true;
    level.id = scene;
  }
  return NULL;
}
static void Up_sceneTranstion(int scene){
  if (player.y <= -10) {
      player.y= 50;
      transition.happened  = true;
      level.id = scene;
  }
  return NULL;
}
static void Down_sceneTranstion(int scene){
  if (player.y >= 60) {
      player.y = 10;
      transition.happened  = true;
      level.id = scene;
  }
  return NULL;
}

static void GameLoop(){
    switch (level.id) {
        case 0:
            Monster_Engine_Draw(0,0,title);
            if (controller_input(5)) {
                level.id = 1;
            }
            if (player.health <= 0) {
                Monster_INIT(); // <-- This resets everthing
            }
            break;

        case 1:
            Monster_Engine_Draw(50,25,tree);
            Monster_Engine_Draw(75,25,grass);
            Monster_Engine_Draw(25,15,grass);
            Create_CollisionBlock(50,75,20,50);
            Left_sceneTranstion(2);
            Right_sceneTranstion(13);
            Up_sceneTranstion(17);
            Create_CollisionBlock(0,128,60,70);
            break;
        case 2:
            Monster_Engine_Draw(25,15,grass);
            Monster_Engine_Draw(25,5,grass);
            Create_CollisionBlock(0,128,60,70);
            Right_sceneTranstion(1);
            Monster_Engine_Draw(15,15,pole);
            Monster_Engine_Draw(105,15,pole);
            Up_sceneTranstion(3);
            Create_CollisionBlock(-15,0,0,128);
            break;
        case 3:
            Monster_Engine_Draw(75,20,grass);
            Monster_Engine_Draw(25,20,grass);
            Down_sceneTranstion(2);
            Up_sceneTranstion(4);
            Right_sceneTranstion(17);
            Create_CollisionBlock(-15,0,0,128);
            break;

        case 4:
            Monster_Engine_Draw(5,20,grass);
            Create_CollisionBlock(-15,0,0,128);
            Down_sceneTranstion(3);
            Up_sceneTranstion(5);
            Right_sceneTranstion(22);

            break;
        case 5:
            Monster_Engine_Draw(100,25,tree);
            Create_CollisionBlock(100,20,25,55);
            Create_CollisionBlock(100,130,20,50);
            Create_CollisionBlock(-15,0,0,128);
            Down_sceneTranstion(4);
            Up_sceneTranstion(6);
            Right_sceneTranstion(27);
            break;
        case 6:
            Monster_Engine_Draw(50,20,grass);
            Monster_Engine_Draw(20,25,tree);
            Create_CollisionBlock(20,40,25,55);
            Create_CollisionBlock(-15,0,0,128);
            Down_sceneTranstion(5);
            Up_sceneTranstion(7);
            Right_sceneTranstion(32);
            break;
        case 7:
            Monster_Engine_Draw(0,10,river);
            Monster_Engine_Draw(30,30,grass);
            Create_CollisionBlock(0,128,0,20);
            Create_CollisionBlock(0,128,-10,0);
            Create_CollisionBlock(-15,0,0,128);
            Down_sceneTranstion(6);
            Right_sceneTranstion(8);
            break;
        case 8:
            Monster_Engine_Draw(0,10,river);
            Monster_Engine_Draw(60,40,tree);
            Create_CollisionBlock(0,128,0,20);
            Create_CollisionBlock(0,128,-10,0);
            Left_sceneTranstion(7);
            Right_sceneTranstion(9);
            Down_sceneTranstion(32);
            break;
        case 9:
            Monster_Engine_Draw(0,10,river);
            Monster_Engine_Draw(32,32,flower_1);
            Monster_Engine_Draw(40,32,flower_2);
            Monster_Engine_Draw(48,32,flower_1);
            Create_CollisionBlock(0,128,0,20);
            Create_CollisionBlock(0,128,-10,0);
            Left_sceneTranstion(8);
            Right_sceneTranstion(10);
            Down_sceneTranstion(33);
            break;
        case 10:
            Monster_Engine_Draw(0,10,river);
            Create_CollisionBlock(0,128,0,20);
            Create_CollisionBlock(0,128,-10,0);
            Left_sceneTranstion(9);
            Right_sceneTranstion(11);
            Down_sceneTranstion(34);
            break;
        case 11:
            Monster_Engine_Draw(0,10,river);
            Create_CollisionBlock(0,128,0,20);
            Create_CollisionBlock(0,128,-10,0);
            Left_sceneTranstion(10);
            Right_sceneTranstion(12);
            Down_sceneTranstion(35);
            break;
        case 12:
            Monster_Engine_Draw(0,10,river);
            Create_CollisionBlock(0,128,0,20);
            Create_CollisionBlock(120,128,0,64);
            Create_CollisionBlock(0,128,-10,0);
            Left_sceneTranstion(11);
            Down_sceneTranstion(36);
            break;

        case 13:
            Monster_Engine_Draw(32,32,flower_1);
            Monster_Engine_Draw(40,32,flower_2);
            Monster_Engine_Draw(48,32,flower_1);
            Left_sceneTranstion(1);
            Right_sceneTranstion(14);
            Up_sceneTranstion(18);
            Create_CollisionBlock(0,128,60,70);
            break;
        case 14:
            Monster_Engine_Draw(32,32,flower_1);
            Monster_Engine_Draw(40,32,flower_2);
            Monster_Engine_Draw(70,42,tree);
            Create_CollisionBlock(70,90,40,60);
            Left_sceneTranstion(13);
            Right_sceneTranstion(15);
            Up_sceneTranstion(19);
            Create_CollisionBlock(0,128,60,70);
            break;
        case 15:
            Monster_Engine_Draw(50,20,grass);
            Monster_Engine_Draw(60,22,tree);
            Create_CollisionBlock(60,80,40,20);
            Create_CollisionBlock(0,128,60,70);
            Left_sceneTranstion(14);
            Right_sceneTranstion(16);
            Up_sceneTranstion(20);
            break;
        case 16:
            Monster_Engine_Draw(20,10,tree);
            Create_CollisionBlock(50,70,20,50);
            Create_CollisionBlock(120,128,0,64);
            Create_CollisionBlock(0,128,60,70);
            Left_sceneTranstion(15);
            Up_sceneTranstion(21);
            break;
        case 17:
            Monster_Engine_Draw(40,10,tree);
            Create_CollisionBlock(40,60,10,30);
            Left_sceneTranstion(3);
            Right_sceneTranstion(18);
            Down_sceneTranstion(1);
            Up_sceneTranstion(22);
            break;
        case 18:
            Monster_Engine_Draw(40,20,grass);
            Left_sceneTranstion(17);
            Right_sceneTranstion(19);
            Down_sceneTranstion(13);
            Up_sceneTranstion(23);
            break;
        case 19:
            Monster_Engine_Draw(40,20,tree);
            Monster_Engine_Draw(70,20,tree);
            Create_CollisionBlock(40,100,20,50);
            Left_sceneTranstion(18);
            Right_sceneTranstion(20);
            Down_sceneTranstion(14);
            Up_sceneTranstion(24);
            break;
        case 20:
            Monster_Engine_Draw(40,20,flower_1);
            Monster_Engine_Draw(70,20,tree);
            Left_sceneTranstion(19);
            Right_sceneTranstion(21);
            Down_sceneTranstion(15);
            Up_sceneTranstion(25);
            break;
        case 21:
            Monster_Engine_Draw(60,30,tree);
            Create_CollisionBlock(60,90,50,30);
            Create_CollisionBlock(120,128,0,64);
            Left_sceneTranstion(20);
            Down_sceneTranstion(16);
            Up_sceneTranstion(26);
            break;
        case 22:
            Monster_Engine_Draw(70,20,tree);
            Create_CollisionBlock(70,100,20,50);
            Up_sceneTranstion(27);
            Left_sceneTranstion(4);
            Right_sceneTranstion(23);
            Down_sceneTranstion(17);
            break;
        case 23:
            Monster_Engine_Draw(60,10,tree);
            Monster_Engine_Draw(50,20,grass);
            Create_CollisionBlock(60,80,10,30);
            Left_sceneTranstion(23);
            Right_sceneTranstion(24);
            Down_sceneTranstion(18);
            Up_sceneTranstion(28);
            break;
        case 24:
            Monster_Engine_Draw(70,70,grass);
            Monster_Engine_Draw(50,20,flower_1);
            Monster_Engine_Draw(58,20,flower_1);
            Left_sceneTranstion(23);
            Right_sceneTranstion(25);
            Down_sceneTranstion(19);
            Up_sceneTranstion(29);
            break;
        case 25:
            Monster_Engine_Draw(70,20,tree);
            Monster_Engine_Draw(50,20,flower_1);
            Monster_Engine_Draw(58,20,flower_1);
            Create_CollisionBlock(70,100,20,50);
            Up_sceneTranstion(30);
            Down_sceneTranstion(20);
            Left_sceneTranstion(24);
            Right_sceneTranstion(26);
            break;
        case 26:
            Monster_Engine_Draw(80,10,tree);
            Monster_Engine_Draw(70,50,grass);
            Monster_Engine_Draw(50,20,flower_2);
            Monster_Engine_Draw(58,20,flower_1);
            Create_CollisionBlock(80,110,10,40);
            Create_CollisionBlock(120,128,0,64);
            Up_sceneTranstion(31);
            Down_sceneTranstion(21);
            Left_sceneTranstion(25);
            break;
        case 27:
            Monster_Engine_Draw(30,30,grass);
            Monster_Engine_Draw(50,30,tree);
            Create_CollisionBlock(50,70,30,50);
            Left_sceneTranstion(5);
            Right_sceneTranstion(28);
            Down_sceneTranstion(22);
            Up_sceneTranstion(32);
            break;
        case 28:
            Monster_Engine_Draw(50,20,flower_2);
            Monster_Engine_Draw(58,30,grass);
            Left_sceneTranstion(27);
            Right_sceneTranstion(29);
            Down_sceneTranstion(23);
            Up_sceneTranstion(33);
            break;
        case 29:
            Monster_Engine_Draw(30,30,tree);
            Create_CollisionBlock(30,50,30,50);
            Left_sceneTranstion(28);
            Right_sceneTranstion(30);
            Down_sceneTranstion(24);
            Up_sceneTranstion(34);
            break;
        case 30:
            Monster_Engine_Draw(30,50,tree);
            Create_CollisionBlock(60,80,20,40);
            Left_sceneTranstion(29);
            Right_sceneTranstion(31);
            Down_sceneTranstion(25);
            Up_sceneTranstion(35);
            break;
        case 31:
            Monster_Engine_Draw(120,50,grass);
            Monster_Engine_Draw(40,20,rock);
            Create_CollisionBlock(120,128,0,64);
            Left_sceneTranstion(30);
            Down_sceneTranstion(26);
            Up_sceneTranstion(36);
            break;
        case 32:
            Monster_Engine_Draw(60,40,tree);
            Monster_Engine_Draw(50,30,rock);
            Create_CollisionBlock(60,80,40,60);
            Left_sceneTranstion(6);
            Right_sceneTranstion(33);
            Down_sceneTranstion(27);
            Up_sceneTranstion(8);
            break;
        case 33:
            Monster_Engine_Draw(50,30,grass);
            Monster_Engine_Draw(40,10,rock);
            Left_sceneTranstion(32);
            Right_sceneTranstion(34);
            Down_sceneTranstion(28);
            Up_sceneTranstion(9);
            break;

        case 34:
            Monster_Engine_Draw(50,30,grass);
            Monster_Engine_Draw(40,10,tree);
            Create_CollisionBlock(40,60,10,30);
            Left_sceneTranstion(33);
            Right_sceneTranstion(35);
            Down_sceneTranstion(29);
            Up_sceneTranstion(10);
            break;
        case 35:
            Monster_Engine_Draw(10,30,grass);
            Monster_Engine_Draw(10,10,rock);
            Left_sceneTranstion(34);
            Right_sceneTranstion(36);
            Down_sceneTranstion(30);
            Up_sceneTranstion(11);
            break;
        case 36:
            Monster_Engine_Draw(50,30,grass);
            Monster_Engine_Draw(40,10,flower_1);
            Create_CollisionBlock(120,128,0,64);
            Left_sceneTranstion(35);
            Down_sceneTranstion(31);
            Up_sceneTranstion(12);
            break;
    }
}