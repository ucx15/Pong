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


class App {
    public:
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;
        SDL_Surface *surface = NULL;
        SDL_Texture *texture = NULL;
        SDL_Event event;


        TIME_PT t1, t2, t3;

        const char* fontPath = "./Res/Brickshapers.ttf";
    
        bool is_running = false;
        
        float dTmS, dT;
        int scoreLeft;
        int scoreRight;
        int paddleHits;

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

        void setup() {
            scoreLeft = 0;
            scoreRight = 0;
            paddleHits = 0;

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
                lPaddle.y -= PADDLE_VEL*dT + (PADDLE_VEL*paddleHits)/PADDLE_SPEEDING_FACTOR;
            }

            if (keys[SDL_SCANCODE_S]) {
                lPaddle.y += PADDLE_VEL*dT + (PADDLE_VEL*paddleHits)/PADDLE_SPEEDING_FACTOR;
            }

            // right paddle
            if (keys[SDL_SCANCODE_UP]) {
                rPaddle.y -= PADDLE_VEL*dT + (PADDLE_VEL*paddleHits)/PADDLE_SPEEDING_FACTOR;
            }

            if (keys[SDL_SCANCODE_DOWN]) {
                rPaddle.y += PADDLE_VEL*dT + (PADDLE_VEL*paddleHits)/PADDLE_SPEEDING_FACTOR;
            }
        }


        // Physics
        void launchPuck() {
            float angle = M_PI/2.f;
            int ctr = 0;

            while (fabsf(cosf(angle))<ANGLE_CUTOFF && ctr++<100) {
                angle = 2.0f * M_PI * randF();
            }

            puck.velX = PUCK_VEL*cosf(angle);
            puck.velY = PUCK_VEL*sinf(angle);
            
            t1 = TIME_NOW();
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
            if (puck.x < PUCK_RAD + PADDING) {
                scoreRight++;
                rightScoreText.update(std::to_string(scoreRight));
                pause();
            }

            else if (puck.x > (W - PUCK_RAD - PADDING)) {
                scoreLeft++;
                leftScoreText.update(std::to_string(scoreLeft));
                pause();
            }

            if (puck.y < PADDING + PUCK_RAD) {
                puck.y = PADDING + PUCK_RAD + 1;
                puck.velY *= -1;
            }

            else if (puck.y > (H - PUCK_RAD - PADDING)) {
                puck.y = H - PUCK_RAD - PADDING - 1;
                puck.velY *= -1;
            }


            // Collision detection with Paddles
            bool leftCond  = (puck.x < 0+PADDING+PAD_W+PUCK_RAD)  &  (puck.y+puck.r/2 > lPaddle.y)  &  (puck.y-puck.r/2 < lPaddle.y+PAD_H);
            bool rightCond = (puck.x > W-PADDING-PAD_W-PUCK_RAD)  &  (puck.y+puck.r/2 > rPaddle.y)  &  (puck.y-puck.r/2 < rPaddle.y+PAD_H);

            if (leftCond) {
                puck.x = 0 + PADDING + PAD_W + PUCK_RAD + 1;
                puck.velX *= -1;
                puck.velX += PUCK_VEL*PUCK_SPEEDING_FACTOR;
                paddleHits++;
            }

            else if (rightCond) {
                puck.x = W - PADDING - PAD_W - PUCK_RAD - 1;
                puck.velX *= -1;
                puck.velX -= PUCK_VEL*PUCK_SPEEDING_FACTOR;
                paddleHits++;
            }
        }

       void resetPos() {
            puck.x = (float) W/2.f;
            puck.y = (float) H/2.f;
            puck.resetTrails();

            lPaddle.y = H/2 - PAD_H/2;
            rPaddle.y = H/2 - PAD_H/2;
        }

        void update() {
            puck.update(dT);
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
            this->fillSurface(BG_COLOR);

            // Drawing Borders
            this->drawBorders();

            lPaddle.draw(surface);
            rPaddle.draw(surface);
            
            textPong.draw(surface, W/2 - textPong.Width()/2, H/2 - textPong.Height()/2);
            textIns.draw(surface, W/2 - textIns.Width()/2, H - H/4);

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
            paddleHits = 0;
            is_running = false;
            this->render();

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

            this->resetPos();
            this->launchPuck();

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
            this->drawBorders();

            // Draw Score
            leftScoreText.draw(surface, L_TEXT_POSX, L_TEXT_POSY);
            rightScoreText.draw(surface, R_TEXT_POSX, R_TEXT_POSY);
            gameOverText.draw(surface, W/2 - gameOverText.Width()/2, H/2 - gameOverText.Height()/2);
            
            this->present();

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

            scoreLeft = 0;
            scoreRight = 0;
            paddleHits = 0;

            leftScoreText.update(std::to_string(scoreLeft));
            rightScoreText.update(std::to_string(scoreRight));
            this->resetPos();
            this->launchPuck();
        }


        // App  
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
            leftScoreText.draw(surface, L_TEXT_POSX, L_TEXT_POSY);
            rightScoreText.draw(surface, R_TEXT_POSX, R_TEXT_POSY);

            // Drawing Paddles and Puck
            lPaddle.draw(surface);
            rPaddle.draw(surface);
            puck.draw(surface);

            this->present();
        }

        void run() {
            homePage();
            launchPuck();

            int tLastLogged=0;

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
                    gameOver();
                }


                // ------ Handling Keyboard Input ------
                handleKeyboardInput();

                // ------ Physics ------
                collision();
                update();

                // ------ Rendering ------
                render();

                SDL_Delay( std::max(0, int(MSPF-dTmS)) );
                tLastLogged += dTmS;

                if ( tLastLogged > UPDATE_TIME ) {
                    tLastLogged = 0;
                    printf("%6.2f\n", 1.f/dT);
                }
            }

        destroyApp();
        }
};


int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    srand(time(0));
    
    App pongApp = App();
    pongApp.run();

    return EXIT_SUCCESS;
}
