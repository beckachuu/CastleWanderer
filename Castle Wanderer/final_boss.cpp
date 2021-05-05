#include "final_boss.h"
#include <SDL_image.h>

FinalBoss::FinalBoss(int* bossNameCount)
{
    frameTime = 0;

    moveTime = 0;
    nextMoveTime = 0;

    speakTime = 0;
    nextSpeakTime = 0;

    bossTexture = loadFromImage("image/bossSheet.png");
    setSpriteClips();

    bubbleSpeechTexture = loadFromImage("image/speechBubble.png");
    bubbleSpeechRect = { NULL };

    healthTexture = loadFromImage("image/health.png");
    healthRect.h = 10;

    healthBarTexture = loadFromImage("image/boss health bar.png");
    healthBarRect.w = 210;
    healthBarRect.h = 40;

    bossWalkVelocity = 0;

    bossWidth = 0;
    bossHeight = 0;

    toRight = false;
    toLeft = true;
    walking = false;

    attackStrength = 0;
    attackTime = 0;

    bossNameTexture = loadFromText(bossName, &bossNameRect, white);

    speechTexture = NULL;
    speechRect = { NULL };

    frame = closeAttack1;

    bossPosX = 1000;
    bossPosY = baseGround;

    bossVelX = 0;
    bossVelY = 0;

    health = rand() % (maxHealth - minHealth) + minHealth;
    damageReceived = 0;
    die = false;

    attacking = false;
}

FinalBoss::~FinalBoss()
{
    free();
}


void FinalBoss::setSpriteClips() {
    bossSpriteClips[closeAttack1].x = 6;
    bossSpriteClips[closeAttack1].y = 76;
    bossSpriteClips[closeAttack1].w = 204;
    bossSpriteClips[closeAttack1].h = 276;

    bossSpriteClips[closeAttack2].x = 401;
    bossSpriteClips[closeAttack2].y = 34;
    bossSpriteClips[closeAttack2].w = 199;
    bossSpriteClips[closeAttack2].h = 318;

    bossSpriteClips[closeAttack3].x = 749;
    bossSpriteClips[closeAttack3].y = 1;
    bossSpriteClips[closeAttack3].w = 242;
    bossSpriteClips[closeAttack3].h = 351;

    bossSpriteClips[closeAttack4].x = 347;
    bossSpriteClips[closeAttack4].y = 47;
    bossSpriteClips[closeAttack4].w = 86;
    bossSpriteClips[closeAttack4].h = 195;

    bossSpriteClips[closeAttack5].x = 456;
    bossSpriteClips[closeAttack5].y = 47;
    bossSpriteClips[closeAttack5].w = 102;
    bossSpriteClips[closeAttack5].h = 195;

    bossSpriteClips[distantAttack1].x = 331;
    bossSpriteClips[distantAttack1].y = 400;
    bossSpriteClips[distantAttack1].w = 119;
    bossSpriteClips[distantAttack1].h = 195;

    bossSpriteClips[distantAttack2].x = 462;
    bossSpriteClips[distantAttack2].y = 400;
    bossSpriteClips[distantAttack2].w = 119;
    bossSpriteClips[distantAttack2].h = 195;

    bossSpriteClips[distantAttack3].x = 462;
    bossSpriteClips[distantAttack3].y = 400;
    bossSpriteClips[distantAttack3].w = 119;
    bossSpriteClips[distantAttack3].h = 195;

    bossSpriteClips[distantAttack4].x = 462;
    bossSpriteClips[distantAttack4].y = 400;
    bossSpriteClips[distantAttack4].w = 119;
    bossSpriteClips[distantAttack4].h = 195;

    bossSpriteClips[die1].x = 462;
    bossSpriteClips[die1].y = 263;
    bossSpriteClips[die1].w = 119;
    bossSpriteClips[die1].h = 195;

    bossSpriteClips[die2].x = 462;
    bossSpriteClips[die2].y = 263;
    bossSpriteClips[die2].w = 119;
    bossSpriteClips[die2].h = 195;

    bossSpriteClips[die3].x = 462;
    bossSpriteClips[die3].y = 263;
    bossSpriteClips[die3].w = 119;
    bossSpriteClips[die3].h = 195;

    bossSpriteClips[die4].x = 462;
    bossSpriteClips[die4].y = 263;
    bossSpriteClips[die4].w = 119;
    bossSpriteClips[die4].h = 195;

    bossSpriteClips[die5].x = 462;
    bossSpriteClips[die5].y = 263;
    bossSpriteClips[die5].w = 119;
    bossSpriteClips[die5].h = 195;
}

void FinalBoss::renderBoss(SDL_Renderer* renderer, SDL_Rect* clip)
{
    if (clip != NULL)
    {
        bossWidth = clip->w;
        bossHeight = clip->h;
    }

    SDL_Rect renderBoss = { bossPosX, bossPosY, bossWidth, bossHeight };

    if (toLeft == true) {
        SDL_RenderCopyEx(renderer, bossTexture, clip, &renderBoss, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, bossTexture, clip, &renderBoss);
    }
}

void FinalBoss::renderSpeechBubble(SDL_Renderer* renderer) {

    speechRect.x = bossPosX - 20;
    speechRect.y = bossPosY - speechRect.h - 70;

    bubbleSpeechRect.x = speechRect.x - 20;
    bubbleSpeechRect.y = speechRect.y - 17;
    bubbleSpeechRect.w = speechRect.w + 50;
    bubbleSpeechRect.h = (speechRect.h + 40) * 6 / 5;

    SDL_RenderCopy(renderer, bubbleSpeechTexture, NULL, &bubbleSpeechRect);
    SDL_RenderCopy(renderer, speechTexture, NULL, &speechRect);
}

void FinalBoss::renderHealthBar(SDL_Renderer* renderer) {
    healthRect.x = bossPosX - 7;
    healthRect.y = bossPosY - 12;
    healthRect.w = 188 - damageReceived * 188 / health;
    SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);

    healthBarRect.x = healthRect.x - 17;
    healthBarRect.y = healthRect.y - 21;
    SDL_RenderCopy(renderer, healthBarTexture, NULL, &healthBarRect);

    bossNameRect.x = healthBarRect.x + healthBarRect.w / 2 - bossNameRect.w / 2;
    bossNameRect.y = healthBarRect.y - 7;
    SDL_RenderCopy(renderer, bossNameTexture, NULL, &bossNameRect);
}

void FinalBoss::renderCurrentAction(SDL_Renderer* renderer, unsigned int currentTime) {
    if (currentTime > frameTime + nextFrameTime) {
        

        frameTime = currentTime;
    }


    SDL_Rect* currentClip = &bossSpriteClips[frame];

    renderBoss(renderer, currentClip);

    if (speechTexture != NULL) {
        renderSpeechBubble(renderer);
    }

    renderHealthBar(renderer);
}


void FinalBoss::move(int targetPosX, int targetPosY, int targetWidth, unsigned int currentTime)
{
    if (damageReceived > 0) {
        chaseTarget(targetPosX, targetPosY, targetWidth);
        attacking = true;
    }


    bossPosX += (bossVelX + plusVelocity);

    bossPosY += bossVelY;

    checkBossLimits();
}

void FinalBoss::checkBossLimits() {

    //If walked too far up
    if (walking && bossPosY < walkLimit) {
        bossPosY = walkLimit;
    }

    //If got too far down
    if (bossPosY > baseGround) {
        bossPosY = baseGround - 1;
    }
}



void FinalBoss::receiveAttack(int damage, int currentTime) {
    damageReceived += damage;
    if (damageReceived >= health) {
        die = true;
    }

    bossPosX -= bossVelX;
}

void FinalBoss::chaseTarget(int targetPosX, int targetPosY, int targetWidth) {
    if (bossPosX + bossWidth < targetPosX + approxDistant) {
        bossVelX = bossWalkVelocity;
        toLeft = false;
        toRight = true;
    }
    else if (bossPosX > targetPosX + targetWidth - approxDistant) {
        bossVelX = -bossWalkVelocity;
        toLeft = true;
        toRight = false;
    }
    else {
        bossVelX = 0;
    }

    if (bossPosY < targetPosY - approxDistant) {
        bossVelY = bossWalkVelocity;
    }
    else if (bossPosY > targetPosY + approxDistant) {
        bossVelY = -bossWalkVelocity;
    }
    else {
        bossVelY = 0;
    }
}

bool FinalBoss::isAttacking() {
    return attacking;
}

int FinalBoss::getAttackDamage(unsigned int currentTime) {
    if (currentTime > attackTime + nextAttackTime) {
        attackStrength = rand() % (maxAttackStrength - minAttackStrength) + minAttackStrength;
        attackTime = currentTime;
    }
    else {
        attackStrength = 0;
    }
    return attackStrength;
}

bool FinalBoss::isDead() {
    return die;
}



int FinalBoss::getBossPosX() {
    return bossPosX;
}
int FinalBoss::getBossPosY() {
    return bossPosY;
}

void FinalBoss::setPlusVelocity(int bgVelocity) {
    plusVelocity = bgVelocity;
}

int FinalBoss::getBossVelX() {
    return bossVelX;
}
int FinalBoss::getBossVelY() {
    return bossVelY;
}


int FinalBoss::getBossWidth() {
    return bossSpriteClips[frame].w;
}
int FinalBoss::getBossHeight() {
    return bossSpriteClips[frame].h;
}



void FinalBoss::free()
{
    freeTexture(bossNameTexture);
    freeTexture(bossTexture);
    freeTexture(speechTexture);
    freeTexture(bubbleSpeechTexture);
    freeTexture(healthTexture);
    freeTexture(healthBarTexture);
}