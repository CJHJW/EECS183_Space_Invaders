#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();

class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
      time = millis();
    }
    // sets values for private data members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      strength = 0;
      time = millis();
    }
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
      time = millis();
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
      time = millis();
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_strength() const {
      return strength;
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
      time = millis();
      if(time > last_time + 1750)
      {
        erase();
        y += 1;
        draw();
        last_time = time;
      }
      
    }

    bool has_been_hit(int x_arg, int y_arg) {
      if (get_strength() > 0) {
        if (x_arg == (x - 1) || x_arg == (x + 2)) {
          if (y_arg == (y + 4)) {
            return true;
          }
        }
        else if (x_arg == x || x_arg == (x + 1)) {
          if (y_arg == (y + 3)) {
            return true;
          }
        }
      }
      return false;
    }

    bool invader_hit_cannon(int x_arg, int y_arg) {
        if (x_arg == (x - 1) || x_arg == (x + 2)) {
          if (y_arg == (y + 3)) {
            return true;
          }
        }
        return false;
    }

    bool has_hit_bottom() {
       if (y >= matrix.height() - 4) {
           return true;
       }
       return false;
    }
    
    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {

      // If the invader has strength, the value of strength is
      // checked and its corresponding color is assigned to body color
      if (strength > 0) {
        Color body_color;
        if (strength == 1) {
          body_color = RED;
        }
        else if (strength == 2) {
          body_color = ORANGE;
        }
        else if (strength == 3) {
          body_color = YELLOW;
        }
        else if (strength == 4) {
          body_color = GREEN;
        }
        else if (strength == 5) {
          body_color = BLUE;
        }
        else if (strength == 6) {
          body_color = PURPLE;
        }
        else {
          body_color = WHITE;
        }
        draw_with_rgb(body_color, BLUE);
      }
      // If the invader does not have strength, the whole invader
      // is drawn with black to erse it
      else
      {
        draw_with_rgb(BLACK, BLACK);
      }
    }
    
    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK, BLACK);
    }    
    
    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      strength -= 1;
      if (strength > 0) {
        draw();
      }
      else {
        erase();
      }
    }

  private:
    int x;
    int y;
    int strength;
    unsigned long time;
    unsigned long last_time = 0;
    
    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
      // draws the body
      matrix.drawPixel(x, y, body_color.to_333());
      matrix.drawPixel(x+1, y, body_color.to_333());
      matrix.drawPixel(x-1, y+1, body_color.to_333());
      matrix.drawPixel(x+2, y+1, body_color.to_333());
      matrix.drawPixel(x-1, y+2, body_color.to_333());
      matrix.drawPixel(x, y+2, body_color.to_333());
      matrix.drawPixel(x+1, y+2, body_color.to_333());
      matrix.drawPixel(x+2, y+2, body_color.to_333());
      matrix.drawPixel(x-1, y+3, body_color.to_333());
      matrix.drawPixel(x+2, y+3, body_color.to_333());
      // draws the eyes
      matrix.drawPixel(x, y+1, eye_color.to_333());
      matrix.drawPixel(x+1, y+1, eye_color.to_333());
    }
};

class Boss1 {
  public:
    Boss1() {
      x = 6;
      y = 9;
      strength = 30;
      time = millis();
    }

    void initialize_boss1(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
      time = millis();
    }
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_strength() const {
      return strength;
    }
    // Moves the Boss 1 down the screen by one row
    // Modifies: y
    void move() {
      time = millis();
      if(time > last_time + 1000)
      {
        erase();
        y += 1;
        if (strength <= 10) {
          weak_draw();
        }
        else {
        draw();
        }

        last_time = time;
      }
      
    }
    void draw(){
      draw_with_rgb(ORANGE, BLUE, WHITE, RED);
    }

    void weak_draw() {
      draw_with_rgb(RED, BLUE, WHITE, ORANGE);
    }

    void erase() {
      draw_with_rgb(BLACK, BLACK, BLACK, BLACK);
    }    

    bool has_been_hit(int x_arg, int y_arg) {
      if (get_strength() > 0) {
        for (int i = 1; i <= 18; i++) {
          if (x_arg == (x + i)) {
            if (y_arg == (y - 2)) {
              return true;
            }
          }
        }
        if (x_arg == x || x_arg == (x + 19)) {
          if (y_arg == y) {
            return true;
          }
        }
      }
      return false;
    }

    void hit() {
      strength -= 1;
      if (strength > 0) {
        if (strength <= 10) {
          weak_draw();
        }
        else {
        draw();
        }
      }
      else {
        erase();
      }

    }

    bool boss1_hit_player(int x_arg, int y_arg) {
        if (x_arg == x || x_arg == (x + 19)) {
          if (y_arg == y) {
            return true;
          }
        }
        else if ((x_arg + 1)  == x || (x_arg - 1) == x) {
          if ((y_arg + 1) == y) {
            return true;
          }
        }
        else if ((x_arg + 1)  == (x + 19) || (x_arg - 1) == (x + 19)) {
          if ((y_arg + 1) == y) {
            return true;
          }
        } 
        return false;
    }

    bool has_hit_bottom() {
       if (y >= matrix.height()) {
           return true;
       }
       return false;
    }

  private:
    int x;
    int y;
    int strength;
    unsigned long time;
    unsigned long last_time = 0;
    void draw_with_rgb(Color body_color, Color eye_color, Color face_color, Color mouth_color) {
      // draws the body
      matrix.drawPixel(x, y, body_color.to_333());
      matrix.drawPixel(x, y-1, body_color.to_333());
      matrix.drawPixel(x, y-2, body_color.to_333());
      matrix.drawPixel(x+19, y, body_color.to_333());
      matrix.drawPixel(x+19, y-1, body_color.to_333());
      matrix.drawPixel(x+19, y-2, body_color.to_333());
      for(int i = 1; i <= 18; i++) {
        matrix.drawPixel(x+i, y-2, body_color.to_333());
      }
      for(int i = 0; i < 3; i++) {
        matrix.drawPixel(x+6, y-3-i, body_color.to_333());
        matrix.drawPixel(x+7, y-3-i, body_color.to_333());
        matrix.drawPixel(x+12, y-3-i, body_color.to_333());
        matrix.drawPixel(x+13, y-3-i, body_color.to_333());
      }
      for(int i = 0; i < 2 ; i++) {
        matrix.drawPixel(x+5, y-4-i, body_color.to_333());
        matrix.drawPixel(x+14, y-4-i, body_color.to_333());
      }
      matrix.drawPixel(x+7, y-6, body_color.to_333());
      matrix.drawPixel(x+12, y-6, body_color.to_333());
      matrix.drawPixel(x+3, y-5, body_color.to_333());
      matrix.drawPixel(x+4, y-5, body_color.to_333());
      matrix.drawPixel(x+15, y-5, body_color.to_333());
      matrix.drawPixel(x+16, y-5, body_color.to_333());
      for(int i = 0; i < 2; i++) {
        matrix.drawPixel(x+2, y-6-i, body_color.to_333());
        matrix.drawPixel(x+4, y-6-i, body_color.to_333());
        matrix.drawPixel(x+15, y-6-i, body_color.to_333());
        matrix.drawPixel(x+17, y-6-i, body_color.to_333());
      }
      for(int i = 0; i < 4; i++) {
        matrix.drawPixel(x+8+i, y-7, body_color.to_333());
      }
      matrix.drawPixel(x+8, y-8, body_color.to_333());
      matrix.drawPixel(x+11, y-8, body_color.to_333());
      matrix.drawPixel(x+6, y-9, body_color.to_333());
      matrix.drawPixel(x+7, y-9, body_color.to_333());
      matrix.drawPixel(x+12, y-9, body_color.to_333());
      matrix.drawPixel(x+13, y-9, body_color.to_333());

      // draws the face
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          matrix.drawPixel(x+8+i, y-6+j, face_color.to_333());
        }
      }

      // draws the eyes
      matrix.drawPixel(x+8, y-6, eye_color.to_333());
      matrix.drawPixel(x+11, y-6, eye_color.to_333());

      // draws the mouth
      matrix.drawPixel(x+9, y-4, mouth_color.to_333());
      matrix.drawPixel(x+10, y-4, mouth_color.to_333());
      matrix.drawPixel(x+9, y-3, mouth_color.to_333());
      matrix.drawPixel(x+10, y-3, mouth_color.to_333());
    }
};

class Boss2 {
  public:
    Boss2() {
      x = 11;
      y = 9;
      strength = 25;
      time = millis();
    }

    void initialize_boss2(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
      time = millis();
    }
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_strength() const {
      return strength;
    }
    // Moves the Boss 1 down the screen by one row
    // Modifies: y
    void move() {
      time = millis();
      if(time > last_time + 1000)
      {
        erase();
        y += 1;
        if (strength <= 10) {
          weak_draw();
        }
        else {
        draw();
        }

        last_time = time;
      }
      
    }
    void draw(){
      draw_with_rgb(GREEN, BLUE, LIME, BLUE);
    }

    void weak_draw() {
      draw_with_rgb(GREEN, RED, LIME, RED);
    }

    void erase() {
      draw_with_rgb(BLACK, BLACK, BLACK, BLACK);
    }    

    bool has_been_hit(int x_arg, int y_arg) {
      if (get_strength() > 0) {
        for (int i = 1; i <= 18; i++) {
          if (x_arg == (x + i)) {
            if (y_arg == (y - 2)) {
              return true;
            }
          }
        }
        if (x_arg == x || x_arg == (x + 19)) {
          if (y_arg == y) {
            return true;
          }
        }
      }
      return false;
    }

    void hit() {
      strength -= 1;
      if (strength > 0) {
        if (strength <= 10) {
          weak_draw();
        }
        else {
        draw();
        }
      }
      else {
        erase();
      }

    }

    bool boss1_hit_player(int x_arg, int y_arg) {
        if (x_arg == x || x_arg == (x + 19)) {
          if (y_arg == y) {
            return true;
          }
        }
        else if ((x_arg + 1)  == x || (x_arg - 1) == x) {
          if ((y_arg + 1) == y) {
            return true;
          }
        }
        else if ((x_arg + 1)  == (x + 19) || (x_arg - 1) == (x + 19)) {
          if ((y_arg + 1) == y) {
            return true;
          }
        } 
        return false;
    }

    bool has_hit_bottom() {
       if (y >= matrix.height()) {
           return true;
       }
       return false;
    }

  private:
    int x;
    int y;
    int strength;
    unsigned long time;
    unsigned long last_time = 0;
    void draw_with_rgb(Color body_color, Color eye_color, Color face_color, Color mouth_color) {
      // draws the body
      for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
          matrix.drawPixel(x+i, y-j, body_color.to_333());
        }
      }

      // draws the face
      for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
          matrix.drawPixel(x+2+i, y-2-j, face_color.to_333());
        }
      }
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          matrix.drawPixel(x+i, y-4-j, face_color.to_333());
          matrix.drawPixel(x+4+i, y-8-j, face_color.to_333());
          matrix.drawPixel(x+4+i, y-j, face_color.to_333());
          matrix.drawPixel(x+8+i, y-4-j, face_color.to_333());
        }
      }

      // draws the eyes
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          matrix.drawPixel(x+2+i, y-5-j, eye_color.to_333());
          matrix.drawPixel(x+6+i, y-5-j, eye_color.to_333());
        }
      }

      // draws the mouth
      for (int i = 0; i < 3; i++) {
        matrix.drawPixel(x+4+i, y-3, mouth_color.to_333());
      }
      matrix.drawPixel(x+5, y-2, mouth_color.to_333());
    }
};


class Cannonball {
  public:
    Cannonball() {
      x = 0;
      y = 0;
      fired = false;
      time = millis();
    }
    
    // resets private data members to initial values
    void reset() {
      x = 0;
      y = 0;
      fired = false;
      time = millis();
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    bool has_been_fired() const {
      return fired;
    }
    
    // sets private data members
    void fire(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      fired = true;
    }
    
    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      time = millis();
      
      // time > last_time_dot_was_moved + 50
      if (fired && ((time > last_time_dot_was_moved + 35))) {
          erase();
          y -= 1;
          if (y < 0) {
            reset();
          }
          else {
            draw();
          }
          last_time_dot_was_moved = time;
      }
    }
    
    // resets private data members to initial values
    void hit() {
      erase();
      reset();
    }
    
    // draws the Cannonball, if it is fired
    void draw() {
      matrix.drawPixel(x, y, ORANGE.to_333());
      matrix.drawPixel(x, y-1, ORANGE.to_333());
    }
    
    // draws black where the Cannonball used to be
    void erase() {
      matrix.drawPixel(x, y, BLACK.to_333());
      matrix.drawPixel(x, y-1, BLACK.to_333());
    }

  private:
    int x;
    int y;
    bool fired;
    unsigned long time;
    unsigned long last_time_dot_was_moved = 0;
};

class Player {
  public:
    Player() {
      initialize(matrix.width() / 2 - 1, matrix.height() - 2);
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_lives() const {
      return lives;
    }
    
    // setter
    void set_x(int x_arg) {
      x = x_arg;
    }
    
    // Modifies: lives
    void die() {
      lives -= 1;
    }
    
    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(AQUA);
    }
    
    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK);
    }

    void restart_player() {
      initialize(matrix.width() / 2 - 1, matrix.height() - 2);
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      lives = 3;
    }
    
    // draws the player
    void draw_with_rgb(Color color) {
      matrix.drawPixel(x, y, color.to_333());
      matrix.drawPixel(x-1, y+1, color.to_333());
      matrix.drawPixel(x, y+1, color.to_333());
      matrix.drawPixel(x+1, y+1, color.to_333());
    }
};

class Game {
  public:
    Game() {
      level = 1;
      time = 0;
    }
    
    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    void setupGame() {
      delay(500);
      btwo.draw();
      delay(10000000);
      // Prints the current level and current number of player lives
      delay(500);
      print_level(level);
      delay(2000);
      print_lives(player.get_lives());
      delay(2000);
      matrix.fillScreen(BLACK.to_333());
      delay(2000);
      int x_arg = 1;
      int y_arg = 0;
      

      // Checks what level the player is on and sets the enemies array
      // to have the invaders in the order expected for levels 1 to 4
      if (level == 1)
      {
        for (int i = 0; i < 8; i++) {
        int power = 1;
        enemies[i].initialize(x_arg, y_arg, power);
        enemies[i].draw();
        x_arg += 4;
        }
        y_arg = 5;
        x_arg = 1;

        for (int i = 8; i < NUM_ENEMIES; i++) {
          
          enemies[i].initialize(x_arg, y_arg, 0);
          enemies[i].draw();
          x_arg += 4;
        }

    
      }
      else if (level == 2)
      {
        for (int i = 0; i < 7; i+=2) {
          enemies[i].initialize(x_arg, y_arg, 1);
          enemies[i].draw();
          x_arg += 4;
          enemies[i+1].initialize(x_arg, y_arg, 2);
          enemies[i+1].draw();
          x_arg += 4;
        }

        y_arg = 5;
        x_arg = 1;

        for (int i = 8; i < NUM_ENEMIES - 1; i+=2) {
          
          enemies[i].initialize(x_arg, y_arg, 2);
          enemies[i].draw();
          x_arg += 4;
          enemies[i+1].initialize(x_arg, y_arg, 1);
          enemies[i+1].draw();
          x_arg += 4;
        }
      }
      else if (level == 3)
      {
        x_arg = 1;
        y_arg = 0;
        int lv3[2][8] = {{1, 2, 3, 4, 5, 1, 2, 3}, {4, 5, 1, 2, 3, 4, 5, 1}};
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 8; j++) {
            enemies[i * 8 + j].initialize(x_arg, y_arg, lv3[i][j]);
            enemies[i * 8 + j].draw();
            x_arg += 4;
          }
          x_arg = 1;
          y_arg += 5;
        }
      }
      else if (level == 4)
      {
        x_arg = 1;
        y_arg = 0;
        int lv4[2][8] = {{5, 4, 5, 4, 5, 4, 5, 4}, {2, 3, 2, 3, 2, 3, 2, 3}};
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 8; j++) {
            enemies[i * 8 + j].initialize(x_arg, y_arg, lv4[i][j]);
            enemies[i * 8 + j].draw();
            x_arg += 4;
          }
          y_arg += 5;
          x_arg = 1;
        }
      }
      else if (level == 6)
      {
        bone.initialize_boss1(6, 9, 30);
        bone.draw();
      }
      // When player goes aboce level 4, the enemies array is filled with invaders
      // of random strength using the random function

      // turn this else if (level > 4) just to an else. We will add more ifs like if level is 6 then do a boss battle
      // so if it is not one of the 4 first levels or the boss levels then just do the randomized level

      else {
        x_arg = 1;
        y_arg = 0;
        int lv_more[2][8];
        randomSeed(micros());
        for (int i = 0; i < 2; i++) {
          for (int j = 0; j < 8; j++) {
            
            lv_more[i][j] = random(1, 8);
          }
        }
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 8; j++) {
              enemies[i * 8 + j].initialize(x_arg, y_arg, lv_more[i][j]);
              enemies[i * 8 + j].draw();
              x_arg += 4;
            }
            y_arg += 5;
            x_arg = 1;
          }
      }
      player.draw();

    }
    
    // advances the game simulation one step and renders the graphics
    // see spec for details of game
    // Modifies: global variable matrix
    void update(int potentiometer_value, bool button_pressed) {
      if (player.get_lives() <= 0)
      {
        game_over();
        delay(1000);
        level = 1;
        player.restart_player();
        setupGame();
      }



      player.erase();
      player.set_x((matrix.width() * potentiometer_value) / 1024 );
      player.draw();

      if(button_pressed && !ball.has_been_fired())
      {
        ball.fire(player.get_x(), 14);
      }
      ball.move();
      
      if (level == 6)  {
        bone.move();
        if (ball.has_been_fired()) {
          if (bone.has_been_hit(ball.get_x(), ball.get_y())) {
            
            ball.hit();
            bone.hit();
          }
        }
      
      if (bone.has_hit_bottom() || bone.boss1_hit_player(player.get_x(),  player.get_y())) {
        player.die();
        matrix.fillScreen(BLACK.to_333());
        matrix.setCursor(1,0);
        matrix.println("Life");
        matrix.print("Lost");
        delay(2000);
         
        setupGame();
        }
      }
      
      else {
      for (int i = 0; i < NUM_ENEMIES; i++) {
        
        if (enemies[i].get_strength() > 0) {
          if (i >= 8) {
            enemies[i].move();
          }
          if ((i < 8) && (bottom_row_cleared())) {
             enemies[i].move();
          }
        }


        if (ball.has_been_fired()) {
          if (enemies[i].has_been_hit(ball.get_x(), ball.get_y())) {
            
            ball.hit();
            enemies[i].hit();
          }
        }

        if (enemies[i].has_hit_bottom() || enemies[i].invader_hit_cannon(player.get_x(),  player.get_y())) {
        player.die();
        matrix.fillScreen(BLACK.to_333());
        matrix.setCursor(1,0);
        matrix.println("Life");
        matrix.print("Lost");
        delay(2000);
         
        setupGame();
        }
      }
      }

      if (level_cleared()) {
        reset_level();
      }
    }

  private:
    int level;
    unsigned long time;
    Player player;
    Cannonball ball;
    Boss1 bone;
    Boss2 btwo;
    Invader enemies[NUM_ENEMIES];

    // checks if the bottom row of Invaders, closest to the invader,
    // is fully elimintaed. If so, function returns true, else it returns false.
    bool bottom_row_cleared() {
      for (int i = 8; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() > 0) {
          return false;
        }
      }
      return true;
    }

    // check if Player defeated all Invaders in current level
    bool level_cleared() {
      if (level == 6)
      {
        if (bone.get_strength() > 0)
        {
          return false;
        }
      }
      else {
        for (int i = 0; i < NUM_ENEMIES; i++) {
          if (enemies[i].get_strength() > 0) {
            return false;
          }
        }
      }
      return true;
    }

    // set up a level
    void reset_level() {
      level += 1;
      setupGame();
    }
};

// a global variable that represents the game Space Invaders
Game game;

// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  // Title is printed
  title(); 
  // setupGame function is called to set up the levels so player can play game
  game.setupGame();
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  // For the loop, potentiometer value and button pressed value is recorded and 
  // sent as arguments to the update function.
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
  game.update(potentiometer_value, button_pressed);


}

// displays Level
void print_level(int level) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(1,0);
  matrix.print("LEVEL:");
  matrix.print(level);
}

// displays number of lives
void print_lives(int lives) {
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(1,0);
  matrix.print("LIVES:");
  matrix.print(lives);
}

// displays "game over"
void game_over() {
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(1,0);
  matrix.println("GAME OVER");
}

void title() {
  // Makes sure the screen starts off black so letters can be printed
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextSize(1);

  // Approximates the width of the title
  int textWidth = 4 * strlen("SPACE INVADERS"); 

  // The starting point of where the text starts off on the screen
  int x = matrix.width(); 

  // While condition is true, each letter is printed with a different color 
  // and after every letter is printed, x is decremented to move text left
  while (x > -textWidth) {
    matrix.fillScreen(BLACK.to_333());
    matrix.setCursor(x - 28, 0);
    matrix.setTextColor(RED.to_333());
    matrix.print("S");
    matrix.setTextColor(ORANGE.to_333());
    matrix.print("P");
    matrix.setTextColor(YELLOW.to_333());
    matrix.print("A");
    matrix.setTextColor(GREEN.to_333());
    matrix.print("C");
    matrix.setTextColor(BLUE.to_333());
    matrix.print("E");
    matrix.print(" ");
    matrix.setTextColor(PURPLE.to_333());
    matrix.print("I");
    matrix.setTextColor(WHITE.to_333());
    matrix.print("N");
    matrix.setTextColor(LIME.to_333());
    matrix.print("V");
    matrix.setTextColor(AQUA.to_333());
    matrix.print("A");
    matrix.setTextColor(RED.to_333());
    matrix.print("D");
    matrix.setTextColor(ORANGE.to_333());
    matrix.print("E");
    matrix.setTextColor(YELLOW.to_333());
    matrix.print("R");
    matrix.setTextColor(GREEN.to_333());
    matrix.print("S");
    matrix.setTextColor(WHITE.to_333());

    // Delay so the title appears to roll off the screen
    delay(150); 
    // x is decremented to move the text one pixel left for each loop
    x--; 
  }
}