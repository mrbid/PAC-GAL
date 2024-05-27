/*
    James William Fletcher ( github.com/mrbid )
        May 2024

    You say it best when you say nothing at all.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define uint GLuint
#define sint GLint

#define GLAD_GL_IMPLEMENTATION
#include "inc/gl.h"
#define GLFW_INCLUDE_NONE
#include "inc/glfw3.h"
#define fTime() (float)glfwGetTime()

//#define GL_DEBUG
#define MAX_MODELS 5 // hard limit, be aware and increase if needed
#include "inc/esAux7.h"

#include "inc/res.h"
#include "assets/high/level.h"  //0
#include "assets/high/gal.h"    //1
#include "assets/high/ghost.h"  //2
#include "assets/high/star.h"   //3
#include "assets/high/blu.h"    //4


//*************************************
// globals
//*************************************
const char appTitle[]="PAC-GAL";
GLFWwindow* window;
uint winw=1024, winh=768;
float t=0.f, dt=0.f, lt=0.f, fc=0.f, lfct=0.f, aspect;

// render state
mat projection, view, model, modelview;

// game vars
#define FAR_DISTANCE 333.f
#define MOVE_SPEED 6.f
#define TOTAL_STARS 95
#define MAX_GHOSTS 4
uint ks[4]={0};
uint ts;
uint level = 0;
float rwait = 0.f;

vec pp, tp;
uint pd;
float pb;

vec gp[MAX_GHOSTS];
vec tgp[MAX_GHOSTS];
uint gd[MAX_GHOSTS];
uint ga[MAX_GHOSTS]={0};
float gt[MAX_GHOSTS];

vec sp[TOTAL_STARS];
uint sa[TOTAL_STARS]={0};
float st[TOTAL_STARS];

uint ba[4]={0};
float bt[4];

//*************************************
// game functions
//*************************************
void addStar(float x, float y)
{
    static uint i = 0;
    if(x == 2.f && y == 6.5f){i=0;}
    sp[i] = (vec){x, y};
    sa[i] = 1;
    st[i] = 0.f;
    i++;
    //printf("%u\n", i);
}
void resetGame(uint mode)
{
    if(mode == 0){level = 0;}
    rwait = t+1.f;
    pp = (vec){-4.f, 0.5f, 0.f};
    tp = pp;
    pd = 0;
    pb = 0.f;
    ts = 0;
    for(int i=0; i<4; i++){ba[i]=1;}
    
    gp[0] = (vec){0.f, -1.5f, 0.f};
    tgp[0] = gp[0];
    gd[0] = 3;
    ga[0] = 1;
    gt[0] = 0.f;
    
    gp[1] = (vec){0.f, 1.5f, 0.f};
    tgp[1] = gp[1];
    gd[1] = 3;
    ga[1] = 1;
    gt[1] = 0.f;
    
    gp[2] = (vec){0.f, 0.5f, 0.f};
    tgp[2] = gp[2];
    gd[2] = 3;
    ga[2] = 1;
    gt[2] = 0.f;
    
    gp[3] = (vec){0.f, -0.5f, 0.f};
    tgp[3] = gp[3];
    gd[3] = 3;
    ga[3] = 1;
    gt[3] = 0.f;

    addStar(2.f, 6.5f); // this always has to be the first star created
    addStar(2.f, 5.5f);
    addStar(2.f, 4.5f);
    addStar(2.f, 3.5f);
    addStar(2.f, 2.5f);
    addStar(2.f, 1.5f);
    addStar(2.f, 0.5f);
    addStar(2.f, -0.5f);
    addStar(2.f, -1.5f);
    addStar(2.f, -2.5f);
    addStar(2.f, -3.5f);
    addStar(2.f, -4.5f);
    addStar(2.f, -5.5f);
    addStar(2.f, -6.5f);

    addStar(4.f, 3.5f);
    addStar(4.f, 2.5f);
    addStar(4.f, 1.5f);
    addStar(4.f, 0.5f);
    addStar(4.f, -0.5f);
    addStar(4.f, -1.5f);
    addStar(4.f, -2.5f);
    addStar(4.f, -3.5f);

    addStar(4.f, -5.5f);
    addStar(4.f, -6.5f);
    addStar(4.f, -7.5f);
    //addStar(4.f, -8.5f);
    addStar(3.f, -5.5f);

    addStar(-4.f, -5.5f);
    addStar(-4.f, -6.5f);
    addStar(-4.f, -7.5f);
    //addStar(-4.f, -8.5f);
    addStar(-3.f, -5.5f);

    addStar(-3.f, -8.5f);
    addStar(-2.f, -8.5f);
    addStar(-1.f, -8.5f);
    addStar(0.f, -8.5f);
    addStar(1.f, -8.5f);
    addStar(2.f, -8.5f);
    addStar(3.f, -8.5f);

    addStar(0.f, 3.5f);
    addStar(1.f, 3.5f);
    addStar(-1.f, 3.5f);

    addStar(0.f, 7.5f);
    addStar(0.f, 6.5f);
    addStar(1.f, 6.5f);
    addStar(-1.f, 6.5f);

    addStar(3.f, 3.5f);
    addStar(3.f, -3.5f);

    addStar(0.f, 5.5f);
    addStar(0.f, 4.5f);
//
    addStar(-2.f, 6.5f);
    addStar(-2.f, 5.5f);
    addStar(-2.f, 4.5f);
    addStar(-2.f, 3.5f);
    addStar(-2.f, 2.5f);
    addStar(-2.f, 1.5f);
    addStar(-2.f, 0.5f);
    addStar(-2.f, -0.5f);
    addStar(-2.f, -1.5f);
    addStar(-2.f, -2.5f);
    addStar(-2.f, -3.5f);
    addStar(-2.f, -4.5f);
    addStar(-2.f, -5.5f);
    addStar(-2.f, -6.5f);

    addStar(-4.f, 3.5f);
    addStar(-4.f, 2.5f);
    addStar(-4.f, 1.5f);
    //addStar(-4.f, 0.5f); // player spawns here
    addStar(-4.f, -0.5f);
    addStar(-4.f, -1.5f);
    addStar(-4.f, -2.5f);
    addStar(-4.f, -3.5f);

    addStar(4.f, 5.5f);
    addStar(4.f, 6.5f);
    addStar(4.f, 7.5f);
    //addStar(4.f, 8.5f);
    addStar(3.f, 5.5f);

    addStar(-4.f, 5.5f);
    addStar(-4.f, 6.5f);
    addStar(-4.f, 7.5f);
    //addStar(-4.f, 8.5f);
    addStar(-3.f, 5.5f);

    addStar(-3.f, 8.5f);
    addStar(-2.f, 8.5f);
    addStar(-1.f, 8.5f);
    addStar(0.f, 8.5f);
    addStar(1.f, 8.5f);
    addStar(2.f, 8.5f);
    addStar(3.f, 8.5f);

    addStar(0.f, -3.5f);
    addStar(1.f, -3.5f);
    addStar(-1.f, -3.5f);

    addStar(0.f, -7.5f);
    addStar(0.f, -6.5f);
    addStar(1.f, -6.5f);
    addStar(-1.f, -6.5f);

    addStar(-3.f, 3.5f);
    addStar(-3.f, -3.5f);

    addStar(0.f, -5.5f);
    addStar(0.f, -4.5f);
}
uint iscollision(vec fp)
{
    if     (fp.y ==  9.5f){return 1;}
    else if(fp.x ==  5.f ){return 1;}
    else if(fp.y == -9.5f){return 1;}
    else if(fp.x == -5.f ){return 1;}
    //
    else if(fp.x == 0.f && fp.y ==  2.5f){return 1;}
    else if(fp.x == 0.f && fp.y == -2.5f){return 1;}
    else if(fp.x == -1.f && fp.y >= -2.5f && fp.y <= 2.5f){return 1;}
    //
    else if(fp.x == 1.f && fp.y >=  1.5f && fp.y <=  2.5f){return 1;}
    else if(fp.x == 1.f && fp.y >= -2.5f && fp.y <= -1.5f){return 1;}
    //
    else if(fp.x ==  3.f && fp.y >= -2.5f && fp.y <= 2.5f){return 1;}
    else if(fp.x == -3.f && fp.y >= -2.5f && fp.y <= 2.5f){return 1;}
    //
    else if(fp.x == 1.f && fp.y >=  4.5f && fp.y <=  5.5f){return 1;}
    else if(fp.x == 1.f && fp.y >= -5.5f && fp.y <= -4.5f){return 1;}
    //
    else if(fp.x == -1.f && fp.y >=  4.5f && fp.y <=  5.5f){return 1;}
    else if(fp.x == -1.f && fp.y >= -5.5f && fp.y <= -4.5f){return 1;}
    //
    else if(fp.y == 4.5f && fp.x >=  3.f && fp.x <=  4.f){return 1;}
    else if(fp.y == 4.5f && fp.x >= -4.f && fp.x <= -3.f){return 1;}
    //
    else if(fp.y == -4.5f && fp.x >=  3.f && fp.x <=  4.f){return 1;}
    else if(fp.y == -4.5f && fp.x >= -4.f && fp.x <= -3.f){return 1;}
    //
    else if(fp.x == 3.f && fp.y >=  6.5f && fp.y <=  7.5f){return 1;}
    else if(fp.x == 3.f && fp.y >= -7.5f && fp.y <= -6.5f){return 1;}
    //
    else if(fp.x == -3.f && fp.y >=  6.5f && fp.y <=  7.5f){return 1;}
    else if(fp.x == -3.f && fp.y >= -7.5f && fp.y <= -6.5f){return 1;}
    //
    else if(fp.y == 7.5f && fp.x >=  1.f && fp.x <=  2.f){return 1;}
    else if(fp.y == 7.5f && fp.x >= -2.f && fp.x <= -1.f){return 1;}
    //
    else if(fp.y == -7.5f && fp.x >=  1.f && fp.x <=  2.f){return 1;}
    else if(fp.y == -7.5f && fp.x >= -2.f && fp.x <= -1.f){return 1;}

    return 0;
}
uint move(uint d)
{
    // turn state
    if(ts == 1){return 0;}
    ts = 1;

    // set direction
    pd = d;

    // bound check (superfluous early terminator)
    if     (d == 0 && pp.y ==  8.5f){goto badending;}
    else if(d == 1 && pp.x ==  4.f ){goto badending;}
    else if(d == 2 && pp.y == -8.5f){goto badending;}
    else if(d == 3 && pp.x == -4.f ){goto badending;}

    // future position
    vec fp = pp;
    if(d == 0)     {fp.y += 1.f;}
    else if(d == 1){fp.x += 1.f;}
    else if(d == 2){fp.y -= 1.f;}
    else if(d == 3){fp.x -= 1.f;}

    // collisions
    if(iscollision(fp) == 1){goto badending;}

    // hit a star?
    uint living_stars = 0;
    for(uint i=0; i < TOTAL_STARS; i++)
    {
        if(sa[i] == 1)
        {
            living_stars++;
            if(fabsf(fp.x-sp[i].x) < 1.f && fabsf(fp.y-sp[i].y) < 1.f)
            {
                sa[i] = 0;
                st[i] = t;
            }
        }
    }
    if(living_stars == 0)
    {
        level++;
        if(level == 10)
            resetGame(0);
        else
            resetGame(1);
        goto badending;
    }

    // hit a blu star?
    if     (ba[0] == 1 && pp.x ==  4.f && pp.y ==  8.5f){ba[0]=2;bt[0]=t;pb=t+6.f;}
    else if(ba[1] == 1 && pp.x == -4.f && pp.y ==  8.5f){ba[1]=2;bt[1]=t;pb=t+6.f;}
    else if(ba[2] == 1 && pp.x ==  4.f && pp.y == -8.5f){ba[2]=2;bt[2]=t;pb=t+6.f;}
    else if(ba[3] == 1 && pp.x == -4.f && pp.y == -8.5f){ba[3]=2;bt[3]=t;pb=t+6.f;}

    // ghost colliding? (cheaper to put it in player move since in sync)
    for(uint i=0; i < MAX_GHOSTS; i++)
    {
        if(ga[i] == 1) // is alive
        {
            vec fgp = gp[i];
            if(randf() < 0.3f){gd[i] = fRand(0, 3);}
            if(gd[i] == 0){fgp.y += 1;}
            if(gd[i] == 1){fgp.x += 1;}
            if(gd[i] == 2){fgp.y -= 1;}
            if(gd[i] == 3){fgp.x -= 1;}
            while(iscollision(fgp) == 1)
            {
                gd[i] = fRand(0, 3);
                fgp = gp[i];
                if(gd[i] == 0){fgp.y += 1;}
                if(gd[i] == 1){fgp.x += 1;}
                if(gd[i] == 2){fgp.y -= 1;}
                if(gd[i] == 3){fgp.x -= 1;}
            }
            tgp[i] = fgp;
        }
    }
    

    // can move
    if     (d == 0 || d == 2){tp.y = fp.y;}
    else if(d == 1 || d == 3){tp.x = fp.x;}

    //printf("P: %.1f %.1f\n", pp.x, pp.y);
    return 1;

    // collison
    badending:
    ts=0;
    return 0;
}

//*************************************
// utility functions
//*************************************
void timestamp(char* ts)
{
    const time_t tt = time(0);
    strftime(ts, 16, "%H:%M:%S", localtime(&tt));
}
void updateModelView()
{
    mMul(&modelview, &model, &view);
    glUniformMatrix4fv(modelview_id, 1, GL_FALSE, (float*)&modelview.m[0][0]);
}

//*************************************
// update & render
//*************************************
void main_loop()
{
//*************************************
// core logic
//*************************************
    fc++;
    glfwPollEvents();
    t = fTime();
    dt = t-lt;
    lt = t;

//*************************************
// game logic
//*************************************

    // inputs
    if(t > rwait){for(uint i=0; i < 4; i++){if(ks[i] == 1){move(i);}}}

    // simulate gal
    if(ts == 1)
    {
        if(pp.y != tp.y)
        {
            if(pd == 0)
            {
                pp.y += MOVE_SPEED*dt;
                if(pp.y >= tp.y){pp.y=tp.y; ts=0;}
            }
            else if(pd == 2)
            {
                pp.y -= MOVE_SPEED*dt;
                if(pp.y <= tp.y){pp.y=tp.y; ts=0;}
            }
        }

        if(pp.x != tp.x)
        {
            if(pd == 1)
            {
                pp.x += MOVE_SPEED*dt;
                if(pp.x >= tp.x){pp.x=tp.x; ts=0;}
            }
            else if(pd == 3)
            {
                pp.x -= MOVE_SPEED*dt;
                if(pp.x <= tp.x){pp.x=tp.x; ts=0;}
            }
        }
    }

    // camera
    mIdent(&view);
    mRotY(&view, 40.f*DEG2RAD);
    mRotZ(&view, 270.f*DEG2RAD);
    mSetPos(&view, (vec){0.f, 0.f, -32.f});

//*************************************
// render
//*************************************

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ///

    // render level
    shadeFullbright(&position_id, &projection_id, &modelview_id, &color_id, &lightness_id, &opacity_id);
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, (float*)&projection.m[0][0]);
    glUniform1f(lightness_id, 0.75f);
    //
    glUniformMatrix4fv(modelview_id, 1, GL_FALSE, (float*)&view.m[0][0]);
    esBindRenderF(0);

    ///

    // render player
    shadeLambert(&position_id, &projection_id, &modelview_id, &lightpos_id, &normal_id, &color_id, &ambient_id, &saturate_id, &opacity_id);
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, (float*)&projection.m[0][0]);
    glUniform1f(ambient_id, 0.4f);
    glUniform1f(saturate_id, 0.5f);
    glUniform3f(lightpos_id, 0.f, -6.f, -26.f);
    //
    mIdent(&model);
    if(pb > t)
    {
        const float d = pb-t;
        const float s = 7.f-d;
        if(d >= 1.f && d <= 2.f)     {mScale1(&model, d);  }
        else if(s <= 2.f)            {mScale1(&model, s);  }
        else if(d >= 2.f && d <= 5.f){mScale1(&model, 2.f);}
    }
    mSetPos(&model, pp);
    if(pd == 1)     {mRotZ(&model, 90.f*DEG2RAD); }
    else if(pd == 2){mRotZ(&model, 180.f*DEG2RAD);}
    else if(pd == 3){mRotZ(&model, 270.f*DEG2RAD);}
    updateModelView();
    esBindRender(1);
    
    ///

    // render ghosts
    esBindModel(2);
    for(uint i = 0; i < MAX_GHOSTS; i++)
    {
        if(ga[i] == 0){continue;}
        if(ga[i] == 1)
        {
            if(fabsf(pp.x-gp[i].x) < 0.5f && fabsf(pp.y-gp[i].y) < 0.5f)
            {
                if(pb > t)
                {
                    ga[i] = 2;
                    gt[i] = t;
                }
                else{resetGame(0);}
            }
            if(gp[i].y != tgp[i].y)
            {
                if(gd[i] == 0)
                {
                    gp[i].y += MOVE_SPEED*dt;
                    if(gp[i].y >= tgp[i].y){gp[i].y=tgp[i].y;}
                }
                else if(gd[i] == 2)
                {
                    gp[i].y -= MOVE_SPEED*dt;
                    if(gp[i].y <= tgp[i].y){gp[i].y=tgp[i].y;}
                }
            }
            if(gp[i].x != tgp[i].x)
            {
                if(gd[i] == 1)
                {
                    gp[i].x += MOVE_SPEED*dt;
                    if(gp[i].x >= tgp[i].x){gp[i].x=tgp[i].x;}
                }
                else if(gd[i] == 3)
                {
                    gp[i].x -= MOVE_SPEED*dt;
                    if(gp[i].x <= tgp[i].x){gp[i].x=tgp[i].x;}
                }
            }
            mIdent(&model);
            mSetPos(&model, gp[i]);
            if(gd[i] == 1)     {mRotZ(&model, 90.f*DEG2RAD); }
            else if(gd[i] == 2){mRotZ(&model, 180.f*DEG2RAD);}
            else if(gd[i] == 3){mRotZ(&model, 270.f*DEG2RAD);}
            updateModelView();
            esRenderModel();
        }
        else if(ga[i] == 2)
        {
            const float gopa = 1.f-((t-gt[i])*0.5f);
            if(gopa <= 0.f)
            {
                if(i == 0)
                {
                    gp[0] = (vec){0.f, -1.5f, 0.f};
                    tgp[0] = gp[0];
                    gd[0] = 0;
                    ga[0] = 1;
                }
                else if(i == 1)
                {
                    gp[1] = (vec){0.f, 1.5f, 0.f};
                    tgp[1] = gp[1];
                    gd[1] = 2;
                    ga[1] = 1;
                }
                else if(i == 2)
                {
                    gp[2] = (vec){0.f, 0.5f, 0.f};
                    tgp[2] = gp[2];
                    gd[2] = 3;
                    ga[2] = 1;
                }
                else if(i == 3)
                {
                    gp[3] = (vec){0.f, -0.5f, 0.f};
                    tgp[3] = gp[3];
                    gd[3] = 3;
                    ga[3] = 1;
                }
            }
            mIdent(&model);
            mSetPos(&model, gp[i]);
            if(gd[i] == 1)     {mRotZ(&model, 90.f*DEG2RAD); }
            else if(gd[i] == 2){mRotZ(&model, 180.f*DEG2RAD);}
            else if(gd[i] == 3){mRotZ(&model, 270.f*DEG2RAD);}
            updateModelView();
            glUniform1f(opacity_id, gopa);
            glEnable(GL_BLEND);
                esRenderModel();
            glDisable(GL_BLEND);
        }
    }

    ///

    // render stars
    esBindModel(3);
    for(uint i=0; i < TOTAL_STARS; i++)
    {
        if(sa[i] == 1)
        {
            mIdent(&model);
            mSetPos(&model, sp[i]);
            updateModelView();
            esRenderModel();
        }
        else if(sa[i] == 0)
        {
            const float td = t-st[i];
            mIdent(&model);
            mSetPos(&model, (vec){sp[i].x, sp[i].y, sp[i].z+(td*2.f)});
            mRotZ(&model, td*9.f);
            const float ns = 1.f-(td*td*0.4f);
            mScale1(&model, ns);
            if(ns <= 0.f){sa[i]=2;}
            updateModelView();
            esRenderModel();
        }
    }

    ///

    // render level count
    if(level > 0)
    {
        for(uint i=0; i < level; i++)
        {
            mIdent(&model);
            mSetPos(&model, (vec){6.f, 8.f-(((float)(i))*2.f), 0.f});
            mRotX(&model, -20.f*DEG2RAD);
            mScale1(&model, 3.f);
            updateModelView();
            esRenderModel();
        }
    }

    ///

    // render blu's
    esBindModel(4);
    if(ba[0] == 1)
    {
        mIdent(&model);
        mSetPos(&model, (vec){4.f, 8.5f, 0.f});
        mRotZ(&model, 90.f*DEG2RAD);
        updateModelView();
        esRenderModel();
    }
    else if(ba[0] == 2)
    {
        const float td = t-bt[0];
        mIdent(&model);
        mSetPos(&model, (vec){4.f, 8.5f, 0.f});
        mRotZ(&model, (90.f*DEG2RAD)+(td*12.f));
        updateModelView();
        glUniform1f(opacity_id, 1.f-(td*1.3f));
        glEnable(GL_BLEND);
            esRenderModel();
        glDisable(GL_BLEND);
    }
    //
    if(ba[1] == 1)
    {
        mIdent(&model);
        mSetPos(&model, (vec){-4.f, 8.5f, 0.f});
        updateModelView();
        esRenderModel();
    }
    else if(ba[1] == 2)
    {
        const float td = t-bt[1];
        mIdent(&model);
        mSetPos(&model, (vec){-4.f, 8.5f, 0.f});
        mRotZ(&model, (td*12.f));
        updateModelView();
        glUniform1f(opacity_id, 1.f-(td*1.3f));
        glEnable(GL_BLEND);
            esRenderModel();
        glDisable(GL_BLEND);
    }
    //
    if(ba[2] == 1)
    {
        mIdent(&model);
        mSetPos(&model, (vec){4.f, -8.5f, 0.f});
        mRotZ(&model, 180.f*DEG2RAD);
        updateModelView();
        esRenderModel();
    }
    else if(ba[2] == 2)
    {
        const float td = t-bt[2];
        mIdent(&model);
        mSetPos(&model, (vec){4.f, -8.5f, 0.f});
        mRotZ(&model, (180.f*DEG2RAD)+(td*12.f));
        updateModelView();
        glUniform1f(opacity_id, 1.f-(td*1.3f));
        glEnable(GL_BLEND);
            esRenderModel();
        glDisable(GL_BLEND);
    }
    //
    if(ba[3] == 1)
    {
        mIdent(&model);
        mSetPos(&model, (vec){-4.f, -8.5f, 0.f});
        mRotZ(&model, 270.f*DEG2RAD);
        updateModelView();
        esRenderModel();
    }
    else if(ba[3] == 2)
    {
        const float td = t-bt[3];
        mIdent(&model);
        mSetPos(&model, (vec){-4.f, -8.5f, 0.f});
        mRotZ(&model, (270.f*DEG2RAD)+(td*12.f));
        updateModelView();
        glUniform1f(opacity_id, 1.f-(td*1.3f));
        glEnable(GL_BLEND);
            esRenderModel();
        glDisable(GL_BLEND);
    }

    ///

    // display render
    glfwSwapBuffers(window);
}

//*************************************
// input
//*************************************
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        if(     key == GLFW_KEY_LEFT  || key == GLFW_KEY_A){ks[0]=1;}
        else if(key == GLFW_KEY_UP    || key == GLFW_KEY_W){ks[1]=1;}
        else if(key == GLFW_KEY_RIGHT || key == GLFW_KEY_D){ks[2]=1;}
        else if(key == GLFW_KEY_DOWN  || key == GLFW_KEY_S){ks[3]=1;}
        else if(key == GLFW_KEY_F) // show average fps
        {
            if(t-lfct > 2.0)
            {
                char strts[16];
                timestamp(&strts[0]);
                printf("[%s] FPS: %g\n", strts, fc/(t-lfct));
                lfct = t;
                fc = 0;
            }
        }
        else if(key == GLFW_KEY_R) // reset game
        {
            resetGame(0);
        }
    }
    else if(action == GLFW_RELEASE)
    {
        if(     key == GLFW_KEY_LEFT  || key == GLFW_KEY_A){ks[0]=0;}
        else if(key == GLFW_KEY_UP    || key == GLFW_KEY_W){ks[1]=0;}
        else if(key == GLFW_KEY_RIGHT || key == GLFW_KEY_D){ks[2]=0;}
        else if(key == GLFW_KEY_DOWN  || key == GLFW_KEY_S){ks[3]=0;}
    }
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
    winw = width, winh = height;
    glViewport(0, 0, winw, winh);
    aspect = (float)winw / (float)winh;
    mIdent(&projection);
    mPerspective(&projection, 30.0f, aspect, 0.1f, FAR_DISTANCE);
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, (float*)&projection.m[0][0]);
}

//*************************************
// process entry point
//*************************************
int main(int argc, char** argv)
{
    // allow custom msaa level
    int msaa = 16;
    if(argc >= 2){msaa = atoi(argv[1]);}

    // help
    printf("----\n");
    printf("James William Fletcher (github.com/mrbid)\n");
    printf("%s - You say it best when you say nothing at all.\n", appTitle);
    printf("----\n");
    printf("A feminized PAC-MAN clone.\n");
    printf("----\n");
    printf("One command line argument, msaa 0-16.\n");
    printf("e.g; ./pacgal 16\n");
    printf("----\n");
    printf("Arrows/WASD = Move\n");
    printf("F = FPS to console.\n");
    printf("R = Reset game.\n");
    printf("----\n");
    printf("All assets where generated using LUMA GENIE (https://lumalabs.ai/genie).\n");
    printf("----\n");
    printf("Dedicated to Tōru Iwatani (https://en.wikipedia.org/wiki/T%%C5%%8Dru_Iwatani).\n");
    printf("----\n");
    printf("%s\n", glfwGetVersionString());
    printf("----\n");

    // init glfw
    if(!glfwInit()){printf("glfwInit() failed.\n"); exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, msaa);
    window = glfwCreateWindow(winw, winh, appTitle, NULL, NULL);
    if(!window)
    {
        printf("glfwCreateWindow() failed.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    const GLFWvidmode* desktop = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(window, (desktop->width/2)-(winw/2), (desktop->height/2)-(winh/2)); // center window on desktop
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1); // 0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync

    // set icon
    glfwSetWindowIcon(window, 1, &(GLFWimage){16, 16, (unsigned char*)icon_image});

//*************************************
// bind vertex and index buffers
//*************************************
    register_level();
    register_gal();
    register_ghost();
    register_star();
    register_blu();

//*************************************
// configure render options
//*************************************
    makeLambert();
    makeFullbright();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.f, 0.f, 0.f, 0.f);

    shadeFullbright(&position_id, &projection_id, &modelview_id, &color_id, &lightness_id, &opacity_id);
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, (float*)&projection.m[0][0]);
    window_size_callback(window, winw, winh);

#ifdef GL_DEBUG
    esDebug(1);
#endif

//*************************************
// execute update / render loop
//*************************************

    // init
    srand(time(0));
    srandf(time(0));
    t = fTime();
    lfct = t;

    // game init
    resetGame(0);

    // loop
    while(!glfwWindowShouldClose(window)){main_loop();}

    // done
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}
