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

/*MY GENERAL FUNCS*/
static float find_distance(struct Vec2 v1, struct Vec2 v2){
    float dx = fabs(v1.x - v2.x), dy = fabs(v1.y - v2.y);
    return sqrtf(dx * dx + dy * dy);
}
static bool is_goaler(struct Player *player) { return player->kit == 3; }
static bool is_haff(struct Player *player) { return (player->kit == 5 || player->kit == 4); }
static bool is_attacker(struct Player *player) { return (player->kit == 0 || player->kit == 1 || player->kit == 2); }
static bool near_opponent_goal(struct Player *self, const struct Scene *scene){
    if(self->team == scene->first_team) return (SCREEN_WIDTH - self->position.x) < 250;
    else return self->position.x < 250;
}

/* Team 1 movement logic */
void movement_logic_1_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_1_5(struct Player *self, const struct Scene *scene) { (void)scene; }

/* Team 2 movement logic */
void movement_logic_2_0(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_1(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_2(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_3(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_4(struct Player *self, const struct Scene *scene) { (void)scene; }
void movement_logic_2_5(struct Player *self, const struct Scene *scene) { (void)scene; }

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

/* GENERAL CHANGE STATE LOGIC*/
void change_state_logic_general(struct Player *self, const struct Scene *scene) {
    struct Ball *ball = scene->ball;
    float distance = find_distance(self->position, ball->position);
    if(is_goaler(self)) {
        if(ball->possessor == self) self->state = SHOOTING;
        else if(ball->possessor != self && distance <= 20) { //distance for goaler
            if(ball->possessor == NULL || (ball->possessor != NULL && ball->possessor->team != self->team)) self->state = INTERCEPTING;
            else self->state = MOVING;
        }
        else self->state = MOVING;
    }
    else if(is_haff(self)) {
        if(ball->possessor == NULL || (ball->possessor != NULL && ball->possessor->team != self->team)) {
            if(distance < 50) self->state = INTERCEPTING;
            else self->state = MOVING;
        }
        else if(ball->possessor == self) self->state = SHOOTING;
        else self->state = MOVING;
    }
    else if(is_attacker(self)) {
        if(ball->possessor == self && near_opponent_goal(self, scene)) self->state = SHOOTING;
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
static struct Talents team1_talents[6] = {
    {3, 5, 1, 4}, // Attack1
    {3, 5, 1, 4}, // Attack2
    {4, 4, 1, 5}, // Attack3
    {3, 1, 5, 1}, // Goaler
    {4, 3, 3, 3}, // Haf1
    {4, 3, 3, 3}, // Haf2
};

/* Team 2 */
static struct Talents team2_talents[6] = {
    {3, 5, 1, 4}, // Attack1
    {3, 5, 1, 4}, // Attack2
    {4, 4, 1, 5}, // Attack3
    {3, 1, 5, 1}, // Goaler
    {4, 3, 3, 3}, // Haf1
    {4, 3, 3, 3}, // Haf2
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