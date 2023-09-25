#include <iostream>
#include <chrono>
#include <random>

#include <math.h>
#include <time.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "settings.hpp"
#include "utils.hpp"
#include "puck.hpp"
#include "paddle.hpp"
#include "text.hpp"


void parseArgs(int n, char** args) {
    if (n > 1) {
        for (int i = 0; i<n; i++) {
            std::cout << args[i] << '\n';
        }
    }
}


SDL_Rect rectBorder{
    .x = 0,
    .y = 0,
    .w = W,
    .h = H
};

SDL_Rect rectLine {
    .x = W/2 - 1,
    .y = 0,
    .w = 2,
    .h = H,
};


class App {
    public:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        SDL_Event event;

        const char* fontPath = "./Res/Brickshapers.ttf";
    
        bool is_running = false;

        int scoreLeft;
        int scoreRight;

        Puck puck;
        Paddle lPaddle;
        Paddle rPaddle;

        Text leftScoreText;
        Text rightScoreText;
        Text gameOverText;


        App() {
            SDL_Init(SDL_INIT_VIDEO);
            IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
            TTF_Init();

            window = SDL_CreateWindow(
                "Pong",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                W, H,
                SDL_WINDOW_SHOWN
            );
            SDL_SetWindowBordered(window, SDL_FALSE );


            if (window == nullptr) {
                std::cout << "Can't initiate SDL_window";
                exit(EXIT_FAILURE);
            }
            
            renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            
            if (renderer == nullptr) {
                std::cout << "Can't initiate Renderer";
                exit(EXIT_FAILURE);
            }

            // Game Setup
            setup();
        }

        void destroyApp() {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            
            SDL_Quit();
            IMG_Quit();
            TTF_Quit();
        };

        // Misc.   
        bool gotWinner() {
            if (scoreLeft > WIN_SCORE-1) {
                std::cout << "Left Wins\n";
                return true;
            }

            else if (scoreRight > WIN_SCORE-1) {
                std::cout << "Right Wins\n";
                return true;
            }

            return false;
        }

        void handleKeyboardInput() {
            auto keys = SDL_GetKeyboardState(NULL);  // Array of u8 codes
            // left paddle
            if (keys[SDL_SCANCODE_W]) {
                lPaddle.y -= (PADDLE_VEL);
            }

            if (keys[SDL_SCANCODE_S]) {
                lPaddle.y += PADDLE_VEL;
            }

            // right paddle
            if (keys[SDL_SCANCODE_UP]) {
                rPaddle.y -= PADDLE_VEL;
            }

            if (keys[SDL_SCANCODE_DOWN]) {
                rPaddle.y += PADDLE_VEL;
            }
        }


        // Physics
        void launchPuck() {
            float x = 2.f * (float) rand()/RAND_MAX - 1.f;
            float y = 2.f*(float) rand()/RAND_MAX - 1.f;
            y /= 5.f;            

            float d = sqrtf((x*x) + (y*y));


            x = PUCK_VEL * (x/d);
            y = PUCK_VEL * (y/d);

            puck.velX = x;
            puck.velY = y;
        }

        void collision() {
            // Paddle Bounds
            if (lPaddle.y < PADDING) {
                lPaddle.y = PADDING;
            }

            else if (lPaddle.y > (H-PADDING-PAD_H)) {
                lPaddle.y = H - PADDING-PAD_H;
            }

            if (rPaddle.y < PADDING) {
                rPaddle.y = PADDING;
            }

            else if (rPaddle.y > (H-PADDING-PAD_H)) {
                rPaddle.y = H - PADDING-PAD_H;
            }


            // Collision detection with Walls
            if (puck.x < 0) {
                scoreRight++;
                rightScoreText.update(renderer, std::to_string(scoreRight));
                pause();
            }

            else if (puck.x > (W - PUCK_RAD)) {
                scoreLeft++;
                leftScoreText.update(renderer, std::to_string(scoreLeft));
                pause();
            }

            if (puck.y < PADDING) {
                puck.y = PADDING+1;
                puck.velY *= -1;
            }

            else if (puck.y > (H - PUCK_RAD - PADDING)) {
                puck.y = H - PUCK_RAD - PADDING - 1;
                puck.velY *= -1;
            }

            // Collision detection with Paddles
            if ((puck.x < (PADDING + PAD_W)) & ((puck.y-puck.h) > lPaddle.y) & ((puck.y+puck.h) < lPaddle.y + PAD_H)) {
                puck.x = (PADDING + PAD_W + 1);
                puck.velX *= -1;
            }

            if ((puck.x > (W - PADDING - PAD_W - PUCK_RAD)) & ((puck.y-puck.h) > rPaddle.y) & ((puck.y+puck.h) < rPaddle.y + PAD_H)) {
                puck.x = (W - PADDING - PAD_W - PUCK_RAD - 1);
                puck.velX *= -1;
            }
        }

       void resetPos() {
            puck.velX = 0.f;
            puck.velY = 0.f;

            lPaddle.y = H/2 - PAD_H/2;
            rPaddle.y = H/2 - PAD_H/2;

            puck.x = W/2 - PUCK_RAD/2;
            puck.y = H/2 - PUCK_RAD/2;
            puck.resetTrails();
        }

        void update() {
            collision();

            puck.update();
            lPaddle.update();
            rPaddle.update();
        }

        // Pages
        void homePage() {
            Text textPong = Text(renderer, fontPath, FONT_SIZE_TITLE, SDL_Color{to8BPC(FONT_COLOR_TITLE)} );
            Text textIns = Text(renderer, fontPath, FONT_SIZE_TEXT, SDL_Color{to8BPC(FONT_COLOR_TEXT)} );

            textPong.update(renderer, "PONG");
            textIns.update(renderer, "Press SPACE to play...");


            // Clearing Background
            SDL_SetRenderDrawColor(renderer, to8BPC(BG_COLOR));
            SDL_RenderClear(renderer);

            // Drawing Borders
            SDL_SetRenderDrawColor(renderer, to8BPC(FG_COLOR));
            SDL_RenderDrawRect(renderer, &rectBorder);

            lPaddle.draw(renderer);
            rPaddle.draw(renderer);
            
            textPong.display(renderer, W/2 - textPong.Width()/2, H/2 - textPong.Height()/2);
            textIns.display(renderer, W/2 - textIns.Width()/2, H - H/4);

            SDL_RenderPresent(renderer);

            while (!is_running) {
                while( SDL_PollEvent(&event) ) {
                    if(event.type == SDL_QUIT) {
                        destroyApp();
                        exit(EXIT_SUCCESS);
                    }

                    if(event.type == SDL_KEYDOWN) {
                        auto key = event.key.keysym.sym;
                        if (key == SDLK_SPACE) {
                            is_running = true;
                        }

                        else if (key == SDLK_ESCAPE) {
                            destroyApp();
                            exit(EXIT_SUCCESS);
                        }
                    }
                }

            }

        }

        void pause() {
            is_running = false;
            while (!is_running) {
                while( SDL_PollEvent(&event) ) {
                    if(event.type == SDL_QUIT) {
                        destroyApp();
                        exit(EXIT_SUCCESS);
                    }

                    if(event.type == SDL_KEYDOWN) {
                        auto key = event.key.keysym.sym;
                        if (key == SDLK_SPACE) {
                            is_running = true;
                        }

                        else if (key == SDLK_ESCAPE) {
                            destroyApp();
                            exit(EXIT_SUCCESS);
                        }
                    }
                }
            }

            resetPos();
            launchPuck();
            is_running = true;
        }

        void gameOver() {
            if (scoreLeft > scoreRight){
                gameOverText.update(renderer, "Left Wins");
            }        
            else if (scoreRight > scoreLeft) {
                gameOverText.update(renderer, "Right Wins");
            }
            else {
                gameOverText.update(renderer, "Draw");
            }

            // Clearing Background
            SDL_SetRenderDrawColor(renderer, to8BPC(BG_COLOR));
            SDL_RenderClear(renderer);
            
            // Drawing Borders
            SDL_SetRenderDrawColor(renderer, to8BPC(FG_COLOR));
            SDL_RenderDrawRect(renderer, &rectBorder);

            leftScoreText.display(renderer, L_TEXT_POSX, L_TEXT_POSY);
            rightScoreText.display(renderer, R_TEXT_POSX, R_TEXT_POSY);

            gameOverText.display(renderer, W/2 - gameOverText.Width()/2, H/2 - gameOverText.Height()/2);
            SDL_RenderPresent(renderer);

            while (!is_running) {
                while( SDL_PollEvent(&event) ) {
                    if(event.type == SDL_QUIT) {
                        destroyApp();
                        exit(EXIT_SUCCESS);
                    }

                    if(event.type == SDL_KEYDOWN) {
                        auto key = event.key.keysym.sym;
                        if (key == SDLK_SPACE) {
                            is_running = true;
                        }

                        else if (key == SDLK_ESCAPE) {
                            destroyApp();
                            exit(EXIT_SUCCESS);
                        }
                    }
                }
            }

            scoreLeft = 0;
            scoreRight = 0; 

            leftScoreText.update(renderer, std::to_string(scoreLeft));
            rightScoreText.update(renderer, std::to_string(scoreRight));
        }


        // App
        void setup() {
            scoreLeft = 0;
            scoreRight = 0;

            puck = Puck(W/2 - PUCK_RAD/2, H/2 - PUCK_RAD/2, PUCK_RAD, PUCK_RAD, PUCK_COLOR);

            lPaddle = Paddle(
                PADDING,
                H/2 - PAD_H/2,
                PAD_W,
                PAD_H,
                PADDLE_COLOR
            );

            rPaddle = Paddle(
                W - PAD_W - PADDING,
                H/2 - PAD_H/2,
                PAD_W,
                PAD_H,
                PADDLE_COLOR
            );

            leftScoreText = Text(renderer, fontPath, FONT_SIZE_SCORE, SDL_Color{to8BPC(FONT_COLOR_TEXT)} );
            rightScoreText = Text(renderer, fontPath, FONT_SIZE_SCORE, SDL_Color{to8BPC(FONT_COLOR_TEXT)} );
            gameOverText = Text(renderer, fontPath, FONT_SIZE_TITLE, SDL_Color{to8BPC(FONT_COLOR_TITLE)} );

            leftScoreText.update(renderer, "0");
            rightScoreText.update(renderer, "0");
        }

        void render() {
            // Clearing Surface
            SDL_SetRenderDrawColor(renderer, to8BPC(BG_COLOR));  // clear the background
            SDL_RenderClear(renderer);

            // Drawing Board
            SDL_SetRenderDrawColor(renderer, to8BPC(FG_COLOR));
            SDL_RenderDrawRect(renderer, &rectBorder);
            SDL_RenderFillRect(renderer, &rectLine);
            
            // Drawing Score
            leftScoreText.display(renderer, L_TEXT_POSX, L_TEXT_POSY);
            rightScoreText.display(renderer, R_TEXT_POSX, R_TEXT_POSY);
            
            // Drawing Paddles and Puck
            lPaddle.draw(renderer);
            rPaddle.draw(renderer);
            puck.draw(renderer);

            // Updating Surface
            SDL_RenderPresent(renderer);
        }

        void run() {
            homePage();
            launchPuck();

            std::chrono::high_resolution_clock::time_point t1;
            std::chrono::high_resolution_clock::time_point t2;
            std::chrono::high_resolution_clock::time_point t3;

            int64_t dTuS;  // Render Time (uS)
            float dTmS;

            while( is_running ) {  
                t1 = std::chrono::high_resolution_clock::now();

                // ------ Handling Events ------
                while( SDL_PollEvent(&event) ) {
                    if(event.type == SDL_QUIT) {
                        is_running = false;
                    }

                    if(event.type == SDL_KEYDOWN) {
                        auto key = event.key.keysym.sym;
                        if (key == SDLK_ESCAPE) {
                            is_running = false;
                        }
                    }
                }


                // ------ Validation ------
                if (gotWinner()) {
                    is_running = false;
                    gameOver();
                }


                // ------ Handling Keyboard Input ------
                handleKeyboardInput();

                // ------ Physics ------
                
                update();


                // ------ Rendering ------
                render();

                t2 = std::chrono::high_resolution_clock::now();
                dTuS = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
                dTmS = (float) dTuS / 1000.f;

                SDL_Delay( std::max(0, int(MSPF-dTmS) ) );
            }

        destroyApp();
    }
};

int main(int argc, char** argv) {
    parseArgs(argc, argv);
    srand(time(0));
    
    App pongApp = App();
    pongApp.run();

    return EXIT_SUCCESS;
}
