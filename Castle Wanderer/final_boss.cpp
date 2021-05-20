#include "final_boss.h"
#include <SDL_image.h>

FinalBoss::FinalBoss()
{
    frameTime = 0;

    moveTime = 0;


    bossNameTexture = loadFromText(bossName, &bossNameRect, white);

    bossTexture = loadFromImage("image/Sprite sheets/bossSheet.png");
    setSpriteClips();

    healthTexture = loadFromImage("image/health.png");
    healthRect.h = 10;

    healthBarTexture = loadFromImage("image/boss health bar.png");
    healthBarRect.w = 370;
    healthBarRect.h = 40;

    bossWalkVelocity = 4;

    bossWidth = 0;
    bossHeight = 0;

    attackStrength = 0;
    attackTime = 0;

    frame = closeAttack1;

    bossLeftPoint = 0;
    bossPosY = 0;

    healtime = 0;
    teleportTime = 0;

    resetBoss();
}

void FinalBoss::resetBoss() {

    bossWalkVelocity = 4;

    toRight = false;
    toLeft = true;
    walking = false;

    attackStrength = 0;

    frame = closeAttack1;

    bossRightPoint = 1200;
    bossFeetPoint = baseGround;

    bossVelX = 0;
    bossVelY = 0;

    damageReceived = 0;
    die = false;
    attackClose = false;

}

FinalBoss::~FinalBoss()
{
    free();
}


void FinalBoss::setSpriteClips() {
    bossSpriteClips[closeAttack1].x = 6;
    bossSpriteClips[closeAttack1].y = 77;
    bossSpriteClips[closeAttack1].w = 205;
    bossSpriteClips[closeAttack1].h = 276;

    bossSpriteClips[closeAttack2].x = 402;
    bossSpriteClips[closeAttack2].y = 35;
    bossSpriteClips[closeAttack2].w = 198;
    bossSpriteClips[closeAttack2].h = 318;

    bossSpriteClips[closeAttack3].x = 749;
    bossSpriteClips[closeAttack3].y = 1;
    bossSpriteClips[closeAttack3].w = 242;
    bossSpriteClips[closeAttack3].h = 351;

    bossSpriteClips[closeAttack4].x = 1072;
    bossSpriteClips[closeAttack4].y = 41;
    bossSpriteClips[closeAttack4].w = 295;
    bossSpriteClips[closeAttack4].h = 311;

    bossSpriteClips[closeAttack5].x = 1462;
    bossSpriteClips[closeAttack5].y = 93;
    bossSpriteClips[closeAttack5].w = 278;
    bossSpriteClips[closeAttack5].h = 259;

    bossSpriteClips[healing1].x = 6;
    bossSpriteClips[healing1].y = 462;
    bossSpriteClips[healing1].w = 204;
    bossSpriteClips[healing1].h = 274;

    bossSpriteClips[healing2].x = 399;
    bossSpriteClips[healing2].y = 446;
    bossSpriteClips[healing2].w = 194;
    bossSpriteClips[healing2].h = 290;

    bossSpriteClips[healing3].x = 790;
    bossSpriteClips[healing3].y = 435;
    bossSpriteClips[healing3].w = 192;
    bossSpriteClips[healing3].h = 301;

    bossSpriteClips[healing4].x = 1121;
    bossSpriteClips[healing4].y = 462;
    bossSpriteClips[healing4].w = 241;
    bossSpriteClips[healing4].h = 274;

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

void FinalBoss::renderBoss(SDL_Renderer* renderer)
{

    SDL_Rect renderBoss = { bossLeftPoint, bossPosY, bossWidth, bossHeight };

    if (toRight == true) {
        SDL_RenderCopyEx(renderer, bossTexture, &bossSpriteClips[frame], &renderBoss, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, bossTexture, &bossSpriteClips[frame], &renderBoss);
    }
}

void FinalBoss::renderHealthBar(SDL_Renderer* renderer) {
    healthRect.x = bossRightPoint - 300;
    healthRect.y = bossFeetPoint - bossApproxHeight - 12;
    healthRect.w = healthRectLength - damageReceived * healthRectLength / health;
    SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);

    healthBarRect.x = healthRect.x - 17;
    healthBarRect.y = healthRect.y - 21;
    SDL_RenderCopy(renderer, healthBarTexture, NULL, &healthBarRect);

    bossNameRect.x = healthBarRect.x + 45;
    bossNameRect.y = healthBarRect.y - 7;
    SDL_RenderCopy(renderer, bossNameTexture, NULL, &bossNameRect);
}

void FinalBoss::setCurrentFrame(int currentTime) {
    if (currentTime > frameTime + nextFrameTime) {
        if (attackClose) {
            if (frame < closeAttack1 || frame >= closeAttack5) {
                frame = closeAttack1;
            }
            else frame++;
        }
        else {
            if (frame < healing1 || frame >= healing4) {
                frame = healing1;
            }
            else frame++;
        }

        bossWidth = bossSpriteClips[frame].w;
        bossHeight = bossSpriteClips[frame].h;

        frameTime = currentTime;
    }

    if (toLeft) {
        bossLeftPoint = bossRightPoint - bossWidth;
    }
    else {
        bossLeftPoint = bossRightPoint - bossApproxWidth;
    }
    bossPosY = bossFeetPoint - bossHeight;
}

void FinalBoss::renderCurrentAction(SDL_Renderer* renderer) {
    renderBoss(renderer);
    renderHealthBar(renderer);
}


bool FinalBoss::isAttackingClose() {
    return attackClose;
}

void FinalBoss::checkBossLimits() {

    //If out of screen
    if (bossRightPoint < maxLeftPos) {
        bossRightPoint += bossWalkVelocity;
        toRight = true;
        toLeft = false;
    }
    else if (bossRightPoint > maxRightPos) {
        bossRightPoint -= bossWalkVelocity;
        toRight = false;
        toLeft = true;
    }

    if (bossFeetPoint > baseGround) {
        bossFeetPoint = baseGround - 1;
    }
    if (bossFeetPoint < walkLimit) {
        bossFeetPoint = walkLimit;
    }
}

void FinalBoss::attackTarget(int targetLeft, int targetRight, int targetFeetPoint, int currentTime) {
    attackClose = true;

    //Teleport
    if (currentTime > teleportTime + nextTeleportTime && rand() % chanceOfTeleport == 0) {
        bossFeetPoint = targetFeetPoint;
        if (rand() % 2 == 0) {
            bossRightPoint = targetLeft;
            toLeft = false;
            toRight = true;
        }
        else {
            bossRightPoint = targetRight + bossApproxWidth;
            toLeft = true;
            toRight = false;
        }
        frameTime += nextFrameTime;
        teleportTime = currentTime;
        return;
    }

    if (bossRightPoint + bossAttackDistant < targetLeft + approxDistant) {
        toLeft = false;
        toRight = true;
        attackClose = false;
    }
    else if (bossLeftPoint - bossAttackDistant > targetRight - approxDistant) {
        toLeft = true;
        toRight = false;
        attackClose = false;
    }
}


void FinalBoss::move(int targetLeft, int targetRight, int targetFeetPoint, unsigned int currentTime)
{
    attackTarget(targetLeft, targetRight, targetFeetPoint, currentTime);
    selfHealing(currentTime);

    bossRightPoint += plusVelocity;
    bossFeetPoint += bossVelY;
    checkBossLimits();
}

void FinalBoss::receiveAttack(int damage, int currentTime) {
    damageReceived += damage;
    if (damageReceived >= health) {
        die = true;
    }

    if (toRight) {
        bossRightPoint -= bossVelocity;
    }
    else {
        bossRightPoint += bossVelocity;
    }
}

void FinalBoss::selfHealing(int currentTime) {
    if (frame == healing4 && damageReceived > minHealing) {
        if (currentTime > attackTime + nextAttackTime) {
            damageReceived -= minHealing;
            healtime = currentTime;
        }
    }
}

int FinalBoss::getAttackDamage(unsigned int currentTime) {
    attackStrength = 0;
    if (frame == closeAttack4) {
        if (currentTime > attackTime + nextAttackTime) {
            attackStrength = rand() % (maxAttackStrength - minAttackStrength) + minAttackStrength;
            attackTime = currentTime;
        }
    }
    return attackStrength;
}

bool FinalBoss::isDead() {
    return die;
}



int FinalBoss::getBossPosX() {
    return bossLeftPoint;
}
int FinalBoss::getBossFeetPoint() {
    return bossFeetPoint;
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


SDL_Texture* FinalBoss::getBossTexture() {
    return bossTexture;
}
SDL_Rect* FinalBoss::getBossSrcRect() {
    return &bossSpriteClips[frame];
}



void FinalBoss::free()
{
    freeTexture(bossNameTexture);
    freeTexture(bossTexture);
    freeTexture(healthTexture);
    freeTexture(healthBarTexture);
}