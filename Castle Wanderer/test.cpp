/*

Fire* test[7] = { nullptr };
    int bulletNum = 0;
    bool fired = false;

    case SDLK_f:
                        std::cout << "pressed f... for respeckt!\n";
                        test[bulletNum]=new Fire(wizard);
                        fired = true;
                        break;

for (int c=0; c <= bulletNum; c++) {
                if (test[bulletNum]->outOfScreen() ) {
                    delete test[bulletNum];
                }
                if (fired && test[bulletNum] != nullptr) {
                    test[bulletNum]->renderBulletPosition(render);
                    std::cout << "rendered bullet, ya saw that?\n";
                    bulletNum++;
                    if (bulletNum >= 7) {
                        bulletNum = 0;
                    }
                }
            }

*/