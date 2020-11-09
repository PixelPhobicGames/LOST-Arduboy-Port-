#include "LOST.h"


int number;

void setup() {
    arduboy.begin();
    arduboy.setFrameRate(45);
    arduboy.audio.on();
    Monster_INIT();
}
void loop() {
    if (!(arduboy.nextFrame()))
        return;
    arduboy.clear();
    number = rand() % 10 + 10;
    GameLoop();
    update_inputs();

    if (transition.happened ) {
        if (number == 15) {
            enemy.isenemy = true;
        }
        transition.happened = false;
    }
    update_animations();

    if (level.id != 0) {
        update_hearts(player.health,heart);
    }

    arduboy.display();
}
