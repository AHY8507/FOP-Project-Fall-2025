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

// my functions
static float find_distance(struct Vec2 v1, struct Vec2 v2) {
    float dx = fabs(v1.x - v2.x), dy = fabs(v1.y - v2.y);
    return sqrt(dx * dx + dy * dy);
}
static struct Vec2 find_max_velocity(struct Vec2 v, float maxi) {
    float leng = sqrt(v.x * v.x + v.y * v.y);
    struct Vec2 res; 
    res.x = v.x;
    res.y = v.y;
    if(leng > maxi) { res.x /= leng;res.x *= maxi; res.y /= leng;res.y *= maxi; }
    return res;
} 
static struct Vec2 find_direction(struct Vec2 v) {
    float leng = sqrt(v.x * v.x + v.y * v.y);
    struct Vec2 res;
    res.x = 0;
    res.y = 0;
    if(leng < 1e-5) return res;
    res.x = v.x / leng;
    res.y = v.y / leng;
    return res;
}

void fix_the_velo_in(struct Player *self, struct Vec2 v1) {
    struct Vec2 res;
    res.x = v1.x - self->position.x;
    res.y = v1.y - self->position.y;
    self->velocity = find_max_velocity(res, (self->talents.agility / ((float)MAX_TALENT_PER_SKILL)) * MAX_PLAYER_VELOCITY);
}

/* Team 1 movement logic */
void movement_logic_1_0(struct Player *self, const struct Scene *scene) { // 3:goal keeper - 1:upper haf - 2:lower haf - 0:middle haf - 4:upper fighter - 5:lower fighter
    struct Ball* ball = scene->ball;
    if(ball->possessor != NULL) {
        if(ball->possessor->team == self->team) {
            struct Vec2 place;
            place.x = 250; place.y = CENTER_Y;
            fix_the_velo_in(self, place);
            return;
        }
    }
    if(ball->position.x > CENTER_X) {
        struct Vec2 place;
        place.x = 250; place.y = CENTER_Y;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y < (SCREEN_HEIGHT / 3.0f)) {
        struct Vec2 place;
        place.x = ball->position.x; place.y = (SCREEN_HEIGHT / 3.0f);
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y >= (SCREEN_HEIGHT / 3.0f) && ball->position.y <= ((SCREEN_HEIGHT / 3.0f) * 2.0f)){
        struct Vec2 place;
        place.x = ball->position.x - 20; place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y > ((SCREEN_HEIGHT / 3.0f) * 2.0f)) {
        struct Vec2 place;
        place.x = ball->position.x; place.y = (SCREEN_HEIGHT / 3.0f) * 2.0f;
        fix_the_velo_in(self, place);
    }
}
void movement_logic_1_1(struct Player *self, const struct Scene *scene) {
    struct Ball* ball = scene->ball;
    if(ball->possessor != NULL) {
        if(ball->possessor->team == self->team) {
            struct Vec2 place;
            place.x = 250; place.y = CENTER_Y - 220;
            fix_the_velo_in(self, place);
            return;
        }
    }
    if(ball->position.x > CENTER_X) {
        struct Vec2 place;
        place.x = 250; place.y = CENTER_Y - 220;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y < (SCREEN_HEIGHT / 3.0f)) {
        struct Vec2 place;
        place.x = ball->position.x - 20; place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y >= (SCREEN_HEIGHT / 3.0f) && ball->position.y <= ((SCREEN_HEIGHT / 3.0f) * 2.0f)){
        struct Vec2 place;
        place.x = ball->position.x; place.y = (SCREEN_HEIGHT / 3.0f);
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y > ((SCREEN_HEIGHT / 3.0f) * 2.0f)) {
        struct Vec2 place;
        place.x = ball->position.x; place.y = (SCREEN_HEIGHT / 3.0f);
        fix_the_velo_in(self, place);
    }
}
void movement_logic_1_2(struct Player *self, const struct Scene *scene) {
    struct Ball* ball = scene->ball;
    if(ball->possessor != NULL) {
        if(ball->possessor->team == self->team) {
            struct Vec2 place;
            place.x = 250; place.y = CENTER_Y + 220;
            fix_the_velo_in(self, place);
            return;
        }
    }
    if(ball->position.x > CENTER_X) {
        struct Vec2 place;
        place.x = 250; place.y = CENTER_Y + 220;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y < (SCREEN_HEIGHT / 3.0f)) {
        struct Vec2 place;
        place.x = ball->position.x; place.y = (SCREEN_HEIGHT / 3.0f) * 2.0f;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y >= (SCREEN_HEIGHT / 3.0f) && ball->position.y <= ((SCREEN_HEIGHT / 3.0f) * 2.0f)){
        struct Vec2 place;
        place.x = ball->position.x; place.y = (SCREEN_HEIGHT / 3.0f) * 2.0f;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y > ((SCREEN_HEIGHT / 3.0f) * 2.0f)) {
        struct Vec2 place;
        place.x = ball->position.x - 20; place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
}
void movement_logic_1_3(struct Player *self, const struct Scene *scene) {
    struct Ball* ball = scene->ball;
    if(ball->position.x > CENTER_X) {
        struct Vec2 place;
        place.x = 70; place.y = CENTER_Y;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.x <= 200) {
        struct Vec2 place;
        place.x = ball->position.x; place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
    else {
        struct Vec2 place;
        place.x = 70; place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
}
void movement_logic_1_4(struct Player *self, const struct Scene *scene) {
}
void movement_logic_1_5(struct Player *self, const struct Scene *scene) {
}

/* Team 2 movement logic */
void movement_logic_2_0(struct Player *self, const struct Scene *scene) {
    struct Ball* ball = scene->ball;
    if(ball->possessor != NULL) {
        if(ball->possessor->team == self->team) {
            struct Vec2 place;
            place.x = SCREEN_WIDTH - 250;
            place.y = CENTER_Y;
            fix_the_velo_in(self, place);
            return;
        }
    }
    if(ball->position.x < CENTER_X) {
        struct Vec2 place;
        place.x = SCREEN_WIDTH - 250;
        place.y = CENTER_Y;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y < (SCREEN_HEIGHT / 3.0f)) {
        struct Vec2 place;
        place.x = ball->position.x;
        place.y = (SCREEN_HEIGHT / 3.0f);
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y >= (SCREEN_HEIGHT / 3.0f) && ball->position.y <= ((SCREEN_HEIGHT / 3.0f) * 2.0f)) {
        struct Vec2 place;
        place.x = ball->position.x + 20;
        place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y > ((SCREEN_HEIGHT / 3.0f) * 2.0f)) {
        struct Vec2 place;
        place.x = ball->position.x;
        place.y = (SCREEN_HEIGHT / 3.0f) * 2.0f;
        fix_the_velo_in(self, place);
    }
}

void movement_logic_2_1(struct Player *self, const struct Scene *scene) {
    struct Ball* ball = scene->ball;
    if(ball->possessor != NULL) {
        if(ball->possessor->team == self->team) {
            struct Vec2 place;
            place.x = SCREEN_WIDTH - 250;
            place.y = CENTER_Y - 220;
            fix_the_velo_in(self, place);
            return;
        }
    }
    if(ball->position.x < CENTER_X) {
        struct Vec2 place;
        place.x = SCREEN_WIDTH - 250;
        place.y = CENTER_Y - 220;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y < (SCREEN_HEIGHT / 3.0f)) {
        struct Vec2 place;
        place.x = ball->position.x + 20;
        place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y >= (SCREEN_HEIGHT / 3.0f) && ball->position.y <= ((SCREEN_HEIGHT / 3.0f) * 2.0f)) {
        struct Vec2 place;
        place.x = ball->position.x;
        place.y = (SCREEN_HEIGHT / 3.0f);
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y > ((SCREEN_HEIGHT / 3.0f) * 2.0f)) {
        struct Vec2 place;
        place.x = ball->position.x;
        place.y = (SCREEN_HEIGHT / 3.0f);
        fix_the_velo_in(self, place);
    }
}

void movement_logic_2_2(struct Player *self, const struct Scene *scene) {
    struct Ball* ball = scene->ball;
    if(ball->possessor != NULL) {
        if(ball->possessor->team == self->team) {
            struct Vec2 place;
            place.x = SCREEN_WIDTH - 250;
            place.y = CENTER_Y + 220;
            fix_the_velo_in(self, place);
            return;
        }
    }
    if(ball->position.x < CENTER_X) {
        struct Vec2 place;
        place.x = SCREEN_WIDTH - 250;
        place.y = CENTER_Y + 220;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y < (SCREEN_HEIGHT / 3.0f)) {
        struct Vec2 place;
        place.x = ball->position.x;
        place.y = (SCREEN_HEIGHT / 3.0f) * 2.0f;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y >= (SCREEN_HEIGHT / 3.0f) && ball->position.y <= ((SCREEN_HEIGHT / 3.0f) * 2.0f)) {
        struct Vec2 place;
        place.x = ball->position.x;
        place.y = (SCREEN_HEIGHT / 3.0f) * 2.0f;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.y > ((SCREEN_HEIGHT / 3.0f) * 2.0f)) {
        struct Vec2 place;
        place.x = ball->position.x + 20;
        place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
}

void movement_logic_2_3(struct Player *self, const struct Scene *scene) {
    struct Ball* ball = scene->ball;
    if(ball->position.x < CENTER_X) {
        struct Vec2 place;
        place.x = SCREEN_WIDTH - 70; place.y = CENTER_Y;
        fix_the_velo_in(self, place);
    }
    else if(ball->position.x >= SCREEN_WIDTH - 200) {
        struct Vec2 place;
        place.x = ball->position.x; place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
    else {
        struct Vec2 place;
        place.x = SCREEN_WIDTH - 70; place.y = ball->position.y;
        fix_the_velo_in(self, place);
    }
}
void movement_logic_2_4(struct Player *self, const struct Scene *scene) {
}
void movement_logic_2_5(struct Player *self, const struct Scene *scene) {
}

/* Team 1 shooting logic */
void shooting_logic_1_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_1_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 2 shooting logic */
void shooting_logic_2_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void shooting_logic_2_5(struct Player *self, const struct Scene *scene) { (void)scene; }

void change_state_logic_all_players(struct Player *self, const struct Scene *scene) {
    int distance_getter;
    switch(self->kit) {
        case 0:
            distance_getter = 25;
            break;
        case 1:
            distance_getter = 40;
            break;
        case 2:
            distance_getter = 40;
            break;
        case 3:
            distance_getter = 40;
            break;
        case 4:
            distance_getter = 30;
            break;
        case 5:
            distance_getter = 30;
            break;
    }
    struct Ball *ball = scene->ball;
    if(ball->possessor == self) self->state = SHOOTING;
    else if(ball->possessor == NULL && find_distance(ball->position, self->position) < distance_getter) self->state = INTERCEPTING;
    else if(ball->possessor != NULL && ball->possessor->team != self->team && find_distance(ball->position, self->position) < distance_getter && self->talents.dribbling > ball->possessor->talents.defence) self->state = INTERCEPTING;
    else self->state = MOVING;
}
// 3:goal keeper - 1:upper haf - 2:lower haf - 0:middle haf - 4:upper fighter - 5:lower fighter

/* Team 1 change_state logic */
void change_state_logic_1_0(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_1_1(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_1_2(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_1_3(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_1_4(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_1_5(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }

/* Team 2 change_state logic */
void change_state_logic_2_0(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_2_1(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_2_2(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_2_3(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_2_4(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }
void change_state_logic_2_5(struct Player *self, const struct Scene *scene) { change_state_logic_all_players(self, scene); }

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
static struct Talents team1_talents[6] = {
    // 0: middle half
    {4, 6, 5, 4},
    // 1: upper half
    {4, 6, 5, 4},
    // 2: lower half
    {4, 6, 5, 4},
    // 3: goalkeeper
    {1, 8, 3, 4},
    // 4: upper fighter
    {7, 3, 6, 3},
    // 5: lower fighter
    {7, 3, 6, 3},
};

/* Team 2 */
static struct Talents team2_talents[6] = {
    // 0: middle half
    {4, 6, 5, 4},
    // 1: upper half
    {4, 6, 5, 4},
    // 2: lower half
    {4, 6, 5, 4},
    // 3: goalkeeper
    {1, 8, 3, 4},
    // 4: upper fighter
    {7, 3, 6, 3},
    // 5: lower fighter
    {7, 3, 6, 3},
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
static struct Vec2 team1_positions[6] = {// 3:goal keeper - 1:upper haf - 2:lower haf - 0:middle haf - 4:upper fighter - 5:lower fighter
    {200, CENTER_Y},
    {250, CENTER_Y - 220},
    {250, CENTER_Y + 220}, 
    {70, CENTER_Y},
    {350, CENTER_Y - 80}, 
    {350, CENTER_Y + 80},
};

/* Team 2 */
static struct Vec2 team2_positions[6] = {
    {SCREEN_WIDTH - 200, CENTER_Y},
    {SCREEN_WIDTH - 250, CENTER_Y - 220},
    {SCREEN_WIDTH - 250, CENTER_Y + 220},
    {SCREEN_WIDTH - 70, CENTER_Y},
    {SCREEN_WIDTH - 350, CENTER_Y - 80},
    {SCREEN_WIDTH - 350, CENTER_Y + 80},
};

struct Vec2 get_positions(int team, int kit) {
    return (team == 1) ? team1_positions[kit] : team2_positions[kit];
}
