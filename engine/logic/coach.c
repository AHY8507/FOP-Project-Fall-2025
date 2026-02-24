#include "coach.h"
#include "core/constants.h"
#include "entities/ball.h"
#include "entities/team.h"
#include "game/scene.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Set to false to let the other team use their own logic (if you implement it)
// Set to true to test your logic on both teams
bool coach_both_teams = false;
// Mine VARS
struct Player* ball_last_shooter = NULL;
unsigned int scores[2] = {10000, 10000};
int starting_game = 0;
/* -------------------------------------------------------------------------
 * Logic Functions
 *  TODO 1: You must implement the following functions in Phase 2.
 *        Each player in each team has its own functions.
 *        You can add new functions, but are NOT ALLOWED to remove
 *        the existing functions or change their structure.
 * ------------------------------------------------------------------------- 
 * ⚠️ STUDENT RULES FOR PHASE 2:
 * You are restricted to modifying ONLY specific variables in each function:
 *
 * 1. MOVEMENT FUNCTIONS (movement_logic_X_Y):
 * Allowed: player->velocity
 * Goal:    Determine the direction and speed of movement.
 *
 * 2. SHOOTING FUNCTIONS (shooting_logic_X_Y):
 * Allowed: ball->velocity
 * Goal:    Determine the direction and power of the kick/pass.
 *
 * 3. CHANGE STATE FUNCTIONS (change_state_logic_X_Y):
 * Allowed: player->state
 * Goal:    Switch between IDLE, MOVING, SHOOTING, or INTERCEPTING.
 *
 * NOTE: Directly modifying any other attributes will be flagged as a violation.
 * Thank you for your attention to this matter!
 * ------------------------------------------------------------------------- */

/*MY GENERAL FUNCS*/
static void update_scores(const struct Scene *scene) { scores[0] = scene->first_team->score; scores[1] = scene->second_team->score; }
static float find_distance(struct Vec2 v1, struct Vec2 v2){
    float dx = fabs(v1.x - v2.x), dy = fabs(v1.y - v2.y);
    return sqrtf(dx * dx + dy * dy);
}
static bool is_goaler(struct Player *player) { return player->kit == 3; }
static bool is_haff(struct Player *player) { return (player->kit == 5 || player->kit == 4); }
static bool is_attacker(struct Player *player) { return (player->kit == 0 || player->kit == 1 || player->kit == 2); }
static bool near_opponent_goal(struct Player *self){
    if(self->team == 1) return (SCREEN_WIDTH - self->position.x) < 250;
    else return self->position.x < 250;
}
static bool is_first_team(struct Player *self) { return self->team == 1; }
static bool ball_in_our_half(struct Player *self, const struct Scene *scene) {
    if(is_first_team(self)) return scene->ball->position.x < SCREEN_WIDTH / 2;
    else return scene->ball->position.x > SCREEN_WIDTH / 2;
}
static struct Vec2 find_max_velocity(struct Vec2 v, float maxi) {
    float leng = sqrt(v.x * v.x + v.y * v.y);
    struct Vec2 res; 
    res.x = v.x;
    res.y = v.y;
    if(leng > maxi) { res.x /= leng;res.x *= maxi; res.y /= leng;res.y *= maxi; }
    return res;
}
static void fix_the_velo_in(struct Player *self, struct Vec2 v1) {
    struct Vec2 res;
    res.x = v1.x - self->position.x;
    res.y = v1.y - self->position.y;
    self->velocity = find_max_velocity(res, (self->talents.agility / ((float)MAX_TALENT_PER_SKILL)) * MAX_PLAYER_VELOCITY);
}
/* GENERAL MOVEMENT LOGIC*/
static void movement_goaler(struct Player *self, const struct Scene *scene) {
    struct Vec2 place;
    if(is_first_team(self)) { place.x = 80; place.y = CENTER_Y;}
    else { place.x = SCREEN_WIDTH - 80; place.y = CENTER_Y; }
    if(ball_in_our_half(self, scene)) place.y =  scene->ball->position.y;
    fix_the_velo_in(self, place);
}
static struct Vec2 haf_base_pos(struct Player *self) {
    struct Vec2 player;
    if(is_first_team(self)) player.x = 220;
    else player.x = SCREEN_WIDTH - 220;
    if(self->kit == 4) player.y = CENTER_Y - 100;
    else player.y = CENTER_Y + 100;
    return player;
}

static void movement_haf(struct Player *self, const struct Scene *scene) {
    struct Ball *ball = scene->ball;
    if(ball->possessor == self) {
        self->velocity.x = 0;
        self->velocity.y = 0;
    } 
    else if(ball->possessor != NULL && ball->possessor->team == self->team) fix_the_velo_in(self, haf_base_pos(self));
    else if(ball_in_our_half(self, scene)) fix_the_velo_in(self, ball->position);
    else fix_the_velo_in(self, haf_base_pos(self));
}
static float attacker_y(struct Player *self) {
    if(self->kit == 0) return SCREEN_HEIGHT * 0.3f;
    if(self->kit == 1) return SCREEN_HEIGHT * 0.5f;
    return SCREEN_HEIGHT * 0.7f;
}
static struct Vec2 attacker_base_pos(struct Player *self) {
    struct Vec2 player;
    if(is_first_team(self)) player.x = SCREEN_WIDTH * 0.54f;
    else player.x = SCREEN_WIDTH * 0.46f;
    player.y = attacker_y(self);
    return player;
}
static void movement_attacker(struct Player *self, const struct Scene *scene) {
    struct Ball *ball = scene->ball;
    struct Vec2 place;
    if(ball->possessor != NULL && ball->possessor->team != self->team) {
        place = attacker_base_pos(self);
        fix_the_velo_in(self, place);
    }
    else if(ball->possessor != NULL && ball->possessor->team == self->team) {
        if(!is_attacker(ball->possessor)) fix_the_velo_in(self, ball->possessor->position);
        else {
            place.x = ball->position.x;
            place.y = self->position.y;
            fix_the_velo_in(self, place);
        }
    }
    else fix_the_velo_in(self, attacker_base_pos(self));
}
void movement_logic_general(struct Player *self, const struct Scene *scene) {
    if(is_goaler(self)) movement_goaler(self, scene);
    else if(is_haff(self)) movement_haf(self, scene);
    else if(is_attacker(self)) movement_attacker(self, scene);
}

/* Team 1 movement logic */
void movement_logic_1_0(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_1_1(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_1_2(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_1_3(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_1_4(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_1_5(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }

/* Team 2 movement logic */
void movement_logic_2_0(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_2_1(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_2_2(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_2_3(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_2_4(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }
void movement_logic_2_5(struct Player *self, const struct Scene *scene) { movement_logic_general(self, scene); }

/* GENERAL SHOOTING LOGIC*/
static struct Player *nearest_attacker(const struct Scene *scene, const struct Player *self) {
    struct Player *best = NULL;
    float best_dist = 1e9;
    if(self->team == 1) {
        for(int i = 0; i < 6; i++) {
            struct Player *player = scene->first_team->players[i];
            if(!is_attacker(player)) continue;
            if(player == self) continue;
            float distance = find_distance(self->position, player->position);
            if(distance <= best_dist) { best_dist = distance; best = player; }
        }
    }
    else {
        for(int i = 0; i < 6; i++) {
            struct Player *player = scene->second_team->players[i];
            if(!is_attacker(player)) continue;
            if(player == self) continue;
            float distance = find_distance(self->position, player->position);
            if(distance <= best_dist) { best_dist = distance; best = player; }
        }
    }
    return best;
}
static struct Vec2 opponent_goal_pos(const struct Scene *scene, const struct Player *self) {
    struct Vec2 goal;
    if(self->team == 0) { goal.x = scene->field.width; goal.y = scene->field.height / 2; }
    else { goal.x = 0; goal.y = scene->field.height / 2; }
    return goal;
}
void fix_the_velo_in_ball(const struct Scene *scene, struct Player *self, struct Vec2 v1) {
    struct Vec2 res;
    res.x = v1.x - scene->ball->position.x;
    res.y = v1.y - scene->ball->position.y;
    scene->ball->velocity = find_max_velocity(res, ((float)(self->talents.shooting) / (MAX_TALENT_PER_SKILL)) * MAX_BALL_VELOCITY);
}
void shooting_logic_global(struct Player *self, const struct Scene *scene) {
    if(starting_game == 2 && scene->ball->possessor == self) {
        starting_game = 0;
        printf("the owner of the ball: %d, %d\n", self->team, self->kit);
        printf("the attacker of the ball: %d, %d\n", nearest_attacker(scene, self)->team, nearest_attacker(scene, self)->kit);
        fix_the_velo_in_ball(scene, self, nearest_attacker(scene, self)->position);
        printf("velocity.x %f .y %f\n", scene->ball->velocity.x, scene->ball->velocity.y);
        return;
    }
    if(is_goaler(self)) fix_the_velo_in_ball(scene, self, nearest_attacker(scene, self)->position);
    else if(is_haff(self)) fix_the_velo_in_ball(scene, self, nearest_attacker(scene, self)->position);
    else fix_the_velo_in_ball(scene, self, opponent_goal_pos(scene, self));
}


/* Team 1 shooting logic */
void shooting_logic_1_0(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_1_1(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_1_2(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_1_3(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_1_4(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_1_5(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }

/* Team 2 shooting logic */
void shooting_logic_2_0(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_2_1(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_2_2(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_2_3(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_2_4(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }
void shooting_logic_2_5(struct Player *self, const struct Scene *scene) { shooting_logic_global(self, scene); }

/* GENERAL CHANGE STATE LOGIC*/
void change_state_logic_general(struct Player *self, const struct Scene *scene) {
    if(scores[0] != scene->first_team->score || scores[1] != scene->second_team->score) { 
        self->state = INTERCEPTING;
        // printf("%d %d\n", self->kit, self->team);
        update_scores(scene);
        starting_game = 1;
        return;
    }
    if(starting_game == 1 && self == scene->ball->possessor) {
        self->state = SHOOTING;
        ball_last_shooter = self;
        starting_game = 2;
        return;
    }
    struct Ball *ball = scene->ball;
    float distance = find_distance(self->position, ball->position);
    if(is_goaler(self)) {
        if(ball->possessor == self) { self->state = SHOOTING; ball_last_shooter = self; }
        else if(ball->possessor != self && distance <= 20) { //distance for goaler
            if((ball->possessor == NULL || (ball->possessor != NULL && ball->possessor->team != self->team)) && ball_last_shooter != self) self->state = INTERCEPTING;
            else self->state = MOVING;
        }
        else self->state = MOVING;
    }
    else if(is_haff(self)) {
        if((ball->possessor == NULL || (ball->possessor != NULL && ball->possessor->team != self->team)) && ball_last_shooter != self) {
            if(distance < 50) self->state = INTERCEPTING;
            else self->state = MOVING;
        }
        else if(ball->possessor == self) { self->state = SHOOTING; ball_last_shooter = self; }
        else self->state = MOVING;
    }
    else if(is_attacker(self)) {
        if(ball->possessor == self && near_opponent_goal(self)) { self->state = SHOOTING; ball_last_shooter = self; }
        else if((ball->possessor == NULL || (ball->possessor != NULL && ball->possessor->team != self->team)) && ball_last_shooter != self) {
            if(distance < 50) self->state = INTERCEPTING;
            else self->state = MOVING;
        }
        else self->state = MOVING;
    }
}

/* Team 1 change_state logic */
void change_state_logic_1_0(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_1_1(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_1_2(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_1_3(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_1_4(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_1_5(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }

/* Team 2 change_state logic */
void change_state_logic_2_0(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_2_1(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_2_2(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_2_3(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_2_4(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }
void change_state_logic_2_5(struct Player *self, const struct Scene *scene) { change_state_logic_general(self, scene); }

/* -------------------------------------------------------------------------
 * Lookup tables for factory
 * ------------------------------------------------------------------------- */
static PlayerLogicFn team1_movement[6] = {
    movement_logic_1_0, movement_logic_1_1, movement_logic_1_2,
    movement_logic_1_3, movement_logic_1_4, movement_logic_1_5
};

static PlayerLogicFn team2_movement[6] = {
    movement_logic_2_0, movement_logic_2_1, movement_logic_2_2,
    movement_logic_2_3, movement_logic_2_4, movement_logic_2_5
};

static PlayerLogicFn team1_shooting[6] = {
    shooting_logic_1_0, shooting_logic_1_1, shooting_logic_1_2,
    shooting_logic_1_3, shooting_logic_1_4, shooting_logic_1_5
};

static PlayerLogicFn team2_shooting[6] = {
    shooting_logic_2_0, shooting_logic_2_1, shooting_logic_2_2,
    shooting_logic_2_3, shooting_logic_2_4, shooting_logic_2_5
};

static PlayerLogicFn team1_change_state[6] = {
    change_state_logic_1_0, change_state_logic_1_1, change_state_logic_1_2,
    change_state_logic_1_3, change_state_logic_1_4, change_state_logic_1_5
};

static PlayerLogicFn team2_change_state[6] = {
    change_state_logic_2_0, change_state_logic_2_1, change_state_logic_2_2,
    change_state_logic_2_3, change_state_logic_2_4, change_state_logic_2_5
};

/* -------------------------------------------------------------------------
 * Factory functions
 * ------------------------------------------------------------------------- */
PlayerLogicFn get_movement_logic(int team, int kit) {
    if (coach_both_teams) return team1_movement[kit];
    return (team == 1) ? team1_movement[kit] : team2_movement[kit];
}

PlayerLogicFn get_shooting_logic(int team, int kit) {
    if (coach_both_teams) return team1_shooting[kit];
    return (team == 1) ? team1_shooting[kit] : team2_shooting[kit];
}

PlayerLogicFn get_change_state_logic(int team, int kit) {
    if (coach_both_teams) return team1_change_state[kit];
    return (team == 1) ? team1_change_state[kit] : team2_change_state[kit];
}

/* -------------------------------------------------------------------------
 * TALENTS
 *  TODO 2: Replace these default values with your desired skill points.
 * ------------------------------------------------------------------------- */
/* Team 1 */
static struct Talents team1_talents[6] = { // defence , agility , dribbling , shooting
    {1, 8, 1, 10}, // Attack1
    {1, 8, 1, 10}, // Attack2
    {2, 8, 1, 9}, // Attack3
    {3, 5, 5, 7}, // Goaler
    {3, 7, 2, 7}, // Haf1
    {3, 7, 2, 7}, // Haf2
};

/* Team 2 */
static struct Talents team2_talents[6] = {
    {1, 8, 1, 10}, // Attack1
    {1, 8, 1, 10}, // Attack2
    {2, 8, 1, 9}, // Attack3
    {3, 5, 5, 7}, // Goaler
    {3, 7, 2, 7}, // Haf1
    {3, 7, 2, 7}, // Haf2
};

struct Talents get_talents(int team, int kit) {
    if (coach_both_teams) return team1_talents[kit];
    return (team == 1) ? team1_talents[kit] : team2_talents[kit];
}


/* -------------------------------------------------------------------------
 * Positioning
 *  TODO 3: Decide players positions at kick-off.
 *        Players must stay on their half, outside the center circle.
 *        Keep in mind that the kick-off team's first player will automatically
 *             be placed at the center of the pitch.
 * ------------------------------------------------------------------------- */
/* Team 1 */
static struct Vec2 team1_positions[6] = {
    {300, CENTER_Y}, // Attack
    {350, CENTER_Y - 140}, // Attack
    {350, CENTER_Y + 140}, // Attack
    {80, CENTER_Y}, // Goal
    {220, CENTER_Y - 100}, // Haff
    {220, CENTER_Y + 100}, // Haff
};

/* Team 2 */
static struct Vec2 team2_positions[6] = {
    {SCREEN_WIDTH - 300, CENTER_Y}, // Attack
    {SCREEN_WIDTH - 350, CENTER_Y - 140}, // Attack
    {SCREEN_WIDTH - 350, CENTER_Y + 140}, // Attack
    {SCREEN_WIDTH - 80, CENTER_Y}, // Goal
    {SCREEN_WIDTH - 220, CENTER_Y - 100}, // Haff
    {SCREEN_WIDTH - 220, CENTER_Y + 100}, // Haff
};

struct Vec2 get_positions(int team, int kit) {
    return (team == 1) ? team1_positions[kit] : team2_positions[kit];
}