#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include <stdbool.h>
#include <math.h> 

typedef struct Player {
  char name[20];
  Vector2 position;
  Vector2 size;
  Vector2 velocity;
  Texture2D player_texture;
  float scale;
} Player;

typedef struct Ball {
  int radius;
  Vector2 position;
  Vector2 velocity;
  Color color;
} Ball;

const int WIDTH = 800;
const int HEIGHT = 600;
bool show_lines = false;

// const int CANVAS = 150;

struct Player player;
Rectangle player_rec;
Vector2 pj_direction = {0.0f, 0.0f};
const float PLAYER_SPEED = 300.0f;

// -- Textures --

struct Ball ball;
Vector2 ball_direction = {0.0f, -1.0f};
const float BALL_SPEED = 250.0f;


//--- prototypes --- //

void setup_game(void);
void player_input(void);
void update_position(Vector2 *position, Vector2 *velocity, float dt);
// void move_player(struct Player *player, float dt);
// void move_ball(struct Ball *ball, float dt);
void clamp_player(struct Player *player, int WIDTH, int HEIGHT);
void clamp_ball(struct Ball *player, int WIDTH, int HEIGHT);
void move_player(struct Player *player, float dt, int WIDTH, int HEIGHT);
void move_ball(struct Ball *ball, float dt, int WIDTH, int HEIGHT);
// float vec_magnitude(Vector2 vec);
float vec_magnitude(Vector2 vec);
Vector2 vec_normalize(Vector2 vec);
void draw_lines(void);

//--- prototypes --- //


int main(void) {
  // setup //
  setup_game();

  // game loop //
  while (!WindowShouldClose()) {
    
    DrawFPS(WIDTH / 4, HEIGHT / 4);
    float dt = GetFrameTime();

    // update state //
    //
    //inputs
    player_input();

    // normalizar vector para fijar magnitude en 1 
    // if (vec_magnitude(pj_direction) > 0) {
    //   pj_direction = vec_normalize(pj_direction);
    // }
    // calcular velocity
    player.velocity.x = pj_direction.x * PLAYER_SPEED; 
    player.velocity.y = pj_direction.y * PLAYER_SPEED; 

    move_player(&player, dt, WIDTH, HEIGHT);
    move_ball(&ball, dt, WIDTH, HEIGHT);
    printf("Player position: (%.2f, %.2f)\n", player.position.x, player.position.y);

    
    // render //
    BeginDrawing();
    ClearBackground(DARKBLUE);

    // debug
    if (show_lines) {
      draw_lines();
    }

    // draw player 

    // rectangle
    // player_rec = (Rectangle){
    //   player.position.x,
    //   player.position.y,
    //   player.size.x,
    //   player.size.y,
    // };

    // DrawRectangleRec(player_rec, WHITE);

    // draw textured player
    Vector2 current_position = {player.position.x, player.position.y};
    DrawTextureEx(player.player_texture, current_position, 0.0f, player.scale, WHITE);

    // xD
    // player.scale += 0.01f;

    // draw ball
    DrawCircle(
        ball.position.x,
        ball.position.y,
        ball.radius,
        ball.color
        );

    EndDrawing();
  }
  // unload to avoid memory leaks
  UnloadTexture(player.player_texture);
  CloseWindow();

  return 0;
}

void setup_game(void) {
  InitWindow(WIDTH, HEIGHT, "New game");
  SetConfigFlags(FLAG_VSYNC_HINT);
  SetTargetFPS(60);
  
  // probably restart game here
  // set initial directions
  Vector2 pj_direction = {0.0f, 0.0f};
  Vector2 ball_direction = {0.0f, -1.0f};
  // player
  strcpy(player.name, "Pedro");
  // size for rectangle
  // player.size = (Vector2){200.0f, 50.0f};

  // texture
  player.player_texture = LoadTexture("resources/komo.png");
  // texture scaling
  player.scale = 4.0f;

  player.size = (Vector2){player.player_texture.width, player.player_texture.height};
  float player_center_x = (WIDTH * 0.5f) - (player.size.x * 0.5f);
  float player_center_y = (HEIGHT * 0.5f) - (player.size.y * 0.5f);
  // set player start position
  player.position = (Vector2){player_center_x, player_center_y};
  
  ball.radius = 16;
  ball.color = RED;
  // set ball start position
  ball.position = (Vector2){WIDTH * 0.5f, 50.0f};
}

/*
void _player_input(void) {
  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) 
    pj_direction.y += -1.0f;
  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    pj_direction.y += 1.0f;
  if (IsKeyDown(KEY_A)|| IsKeyDown(KEY_LEFT))
    pj_direction.x += -1.0f;
  if (IsKeyDown(KEY_D)|| IsKeyDown(KEY_RIGHT))
    pj_direction.x += 1.0f;
  if (IsKeyDown(KEY_SPACE))
    pj_direction = (Vector2){0.0f, 0.0f};
  if (IsKeyDown(KEY_ESCAPE)) CloseWindow();
    
  printf("Current direction: (%.2f, %.2f)\n", pj_direction.x, pj_direction.y);
}
*/

void player_input(void) {

  if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
      pj_direction = (Vector2){0.0f, -1.0f};

  if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
      pj_direction = (Vector2){0.0f, 1.0f};

  if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      pj_direction = (Vector2){-1.0f, 0.0f};

  if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      pj_direction = (Vector2){1.0f, 0.0f};
    
  printf("Current direction: (%.2f, %.2f)\n", pj_direction.x, pj_direction.y);
}

void clamp_player(Player *player, int WIDTH, int HEIGHT) {
  // for rectangle
  // if (player->position.x <= 0)
  //   player->position.x = 0;
  // else if ((player->position.x + player->size.x) >= WIDTH)
  //   player->position.x = WIDTH - player->size.x;

  // if (player->position.y <= 0)
  //   player->position.y = 0;
  // else if ((player->position.y + player->size.y) >= HEIGHT)
  //   player->position.y = HEIGHT - player->size.y;
  float scaled_width = player->player_texture.width * player->scale;
  float scaled_height = player->player_texture.height * player->scale;

  if (player->position.x <= 0)
    player->position.x = 0;
  else if ((player->position.x + scaled_width) >= WIDTH)
    player->position.x = WIDTH - scaled_width;

  if (player->position.y <= 0)
    player->position.y = 0;
  else if ((player->position.y + scaled_height) >= HEIGHT)
    player->position.y = HEIGHT - scaled_height;
}

void clamp_ball(Ball *ball, int WIDTH, int HEIGHT) {

  if (ball->position.x <= 0)
    ball->position.x = 0;
  else if ((ball->position.x + ball->radius) >= WIDTH)
    ball->position.x = WIDTH - ball->radius;
  if (ball->position.y <= 0)
    ball->position.y = 0;
  else if ((ball->position.y + ball->radius) >= HEIGHT)
    ball->position.y = HEIGHT - ball->radius;
}

/*
void move_player(struct Player *player, float dt) {
  player->position.x += pj_direction.x * PLAYER_SPEED * dt;
  player->position.y += pj_direction.y * PLAYER_SPEED * dt;
  clamp_player(player, WIDTH, HEIGHT);
  printf("Player position: (%.2f, %.2f)\n", player->position.x, player->position.y);
}
*/

/*
void move_ball(struct Ball *ball, float dt) {
  ball->position.x += ball_direction.x * BALL_SPEED * dt;
  ball->position.y += ball_direction.y * BALL_SPEED * dt;
  clamp_ball(ball, WIDTH, HEIGHT);
  printf("Ball position: (%.2f, %.2f)\n", ball->position.x, ball->position.y);
}
*/

void update_position(Vector2 *position, Vector2 *velocity, float dt) {
  position->x += velocity->x * dt;
  position->y += velocity->y * dt;
}

void move_ball(Ball *ball, float dt, int WIDTH, int HEIGHT) {
  update_position(&ball->position, &ball->velocity, dt);
  clamp_ball(ball, WIDTH, HEIGHT);
}

void move_player(Player *player, float dt, int WIDTH, int HEIGHT) {
  update_position(&player->position, &player->velocity, dt);
  clamp_player(player, WIDTH, HEIGHT);
}

void draw_lines(void) {
    DrawLineEx(
        (Vector2){400, 0},
        (Vector2){400, 600},
        4.0f,
        BLACK);
}

// Math

/*
float vec_magnitude(Vector2 vec) {
  float magnitude = sqrtf(vec.x * vec.x + vec.y * vec.y);
  return magnitude;
}
*/

float vec_magnitude(Vector2 vec) {
  float magnitude = sqrtf(vec.x * vec.x + vec.y * vec.y);
  return magnitude;
}

Vector2 vec_normalize(Vector2 vec) {
  float magnitude = vec_magnitude(vec);
  Vector2 norm_vec = vec;
  norm_vec.x = vec.x / magnitude;
  norm_vec.y = vec.y / magnitude;
  return norm_vec;
}
