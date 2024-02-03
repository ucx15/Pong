#include <iostream>
#include <chrono>
#include <cmath>
#include <time.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "settings.hpp"
#include "utils.hpp"
#include "puck.hpp"
#include "paddle.hpp"
#include "text.hpp"


// MACROS
#define TIME_PT std::chrono::high_resolution_clock::time_point
#define TIME_NOW() std::chrono::high_resolution_clock::now()
#define TIME_DUR(b,a) std::chrono::duration_cast<std::chrono::microseconds>(b-a).count()

#define randF() ((float)rand()/RAND_MAX)



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
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;
        SDL_Surface *surface = NULL;
        SDL_Texture *texture = NULL;
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
            SDL_Init(SDL_INIT_EVERYTHING);
            TTF_Init();

            window = SDL_CreateWindow(
                "Pong",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                W, H,
                SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

            if ( !window ) {
                std::cout << "Can't initiate SDL_window\n";
                exit(EXIT_FAILURE);
            }

            renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if ( !renderer ) {
                std::cout << "Can't initiate Renderer\n";
                exit(EXIT_FAILURE);
            }

            surface = SDL_CreateRGBSurface(0, W, H, 32, 0xFF000000,  0x00FF0000, 0x0000FF00, 0x000000FF);
            if ( !surface ) {
                std::cout << "Can't initiate Surface\n";
                exit(EXIT_FAILURE);
            }

            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, W, H);
            if ( !texture ) {
                std::cout << "SDL_CreateTexture failed\n";
                exit(EXIT_FAILURE);
            }

            // Game Setup
            setup();
        }

        void destroyApp() {
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);

            TTF_Quit();
            SDL_Quit();
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
            float x,y;
            x = PUCK_VEL;
            y = H*randF();

            float d = sqrtf(sq(W/2.f - x) + sq(H/2.f - y)); 
            y = PUCK_VEL*y/d;

            puck.velX += x;
            puck.velY += y;
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
                rightScoreText.update(std::to_string(scoreRight));
                pause();
            }

            else if (puck.x > (W - PUCK_RAD)) {
                scoreLeft++;
                leftScoreText.update(std::to_string(scoreLeft));
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
            if ((puck.x < (PADDING + PAD_W)) & ((puck.y-puck.r) > lPaddle.y) & ((puck.y+puck.r) < lPaddle.y + PAD_H)) {
                puck.x = (PADDING + PAD_W + 1);
                puck.velX *= -1;
            }

            if ((puck.x > (W - PADDING - PAD_W - PUCK_RAD)) & ((puck.y-puck.r) > rPaddle.y) & ((puck.y+puck.r) < rPaddle.y + PAD_H)) {
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
            Text textPong = Text(fontPath, FONT_SIZE_TITLE, SDL_Color{to8BPC(FONT_COLOR_TITLE)} );
            Text textIns  = Text(fontPath, FONT_SIZE_TEXT, SDL_Color{to8BPC(FONT_COLOR_TEXT)} );

            textPong.update("PONG");
            textIns.update("Press SPACE to play...");


            // Clearing Background
            fillSurface(BG_COLOR);

            // Drawing Borders
            this->drawBorders();
            // drawRect((uint32_t *)surface->pixels, PADDING,PADDING, W-PADDING, H-PADDING, FG_COLOR);

            lPaddle.draw(surface);
            rPaddle.draw(surface);
            
            textPong.display(surface, W/2 - textPong.Width()/2, H/2 - textPong.Height()/2);
            textIns.display(surface, W/2 - textIns.Width()/2, H - H/4);

            this->present();

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
                gameOverText.update("Left Wins");
            }
            else if (scoreRight > scoreLeft) {
                gameOverText.update("Right Wins");
            }
            else {
                gameOverText.update("Draw");
            }

            // Clearing Background
            fillSurface(BG_COLOR);
            
            // Drawing Borders
            // drawRect((uint32_t*)surface->pixels, 0,0, W,H, FG_COLOR);
            this->drawBorders();

            // Draw Score
            leftScoreText.display(surface, L_TEXT_POSX, L_TEXT_POSY);
            rightScoreText.display(surface, R_TEXT_POSX, R_TEXT_POSY);
            gameOverText.display(surface, W/2 - gameOverText.Width()/2, H/2 - gameOverText.Height()/2);
            
            this->present();

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

            leftScoreText.update(std::to_string(scoreLeft));
            rightScoreText.update(std::to_string(scoreRight));
        }


        // App
        void setup() {
            scoreLeft = 0;
            scoreRight = 0;

            puck = Puck(W/2 - PUCK_RAD/2, H/2 - PUCK_RAD/2, PUCK_RAD, PUCK_COLOR);

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

            leftScoreText  = Text(fontPath, FONT_SIZE_SCORE, SDL_Color{to8BPC(FONT_COLOR_TEXT)} );
            rightScoreText = Text(fontPath, FONT_SIZE_SCORE, SDL_Color{to8BPC(FONT_COLOR_TEXT)} );
            gameOverText   = Text(fontPath, FONT_SIZE_TITLE, SDL_Color{to8BPC(FONT_COLOR_TITLE)} );

            leftScoreText.update("0");
            rightScoreText.update("0");
        }
    
        void fillSurface(uint32_t color) {
            for (int i=0; i<W*H; i++) {
                ((uint32_t*)surface->pixels)[i] = color;
            }
        }


        void present() {
            SDL_UpdateTexture(texture, nullptr, surface->pixels, W*4);

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
        }

        void drawBorders() {
            drawRect((uint32_t*)surface->pixels, PADDING,PADDING, W-2*PADDING,H-2*PADDING, FG_COLOR);
        }


        void render() {
            // Clearing Surface
            fillSurface(BG_COLOR);

            // Drawing Board
            this->drawBorders();            
            fillRect((uint32_t*)surface->pixels, W/2 -1, PADDING, 2, H-2*PADDING, FG_COLOR);           


            // Drawing Score
            leftScoreText.display(surface, L_TEXT_POSX, L_TEXT_POSY);
            rightScoreText.display(surface, R_TEXT_POSX, R_TEXT_POSY);

            // Drawing Paddles and Puck
            lPaddle.draw(surface);
            rPaddle.draw(surface);
            puck.draw(surface);

            this->present();
        }

        void run() {
            homePage();
            launchPuck();

            float dTmS, dT;
            int tLastLogged=0;

            TIME_PT t1, t2, t3;
            t1 = TIME_NOW();


            while( is_running ) {  

                t2 = TIME_NOW();
                dTmS = TIME_DUR(t2, t1) / 1E3F;
                dT = dTmS / 1E3F;
                t1 = TIME_NOW();


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

                SDL_Delay( std::max(0, int(MSPF-dTmS)) );
                tLastLogged += dTmS;

                if ( tLastLogged > UPDATE_TIME ) {
                    tLastLogged = 0;
                    std::cout << "FPS: " << 1/dT << '\n';
                }
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
