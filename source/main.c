// Bunny Garden by kkzero
// Built off Nitro Engine's "with_nflib" template, linked below and licensed CC0-1.0
// https://github.com/AntonioND/nitro-engine/tree/master/examples/templates/using_nflib

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <filesystem.h>
#include <nds.h>
#include <NEMain.h>
#include <nf_lib.h>

#include "const.h"
#include "GameObject3D.h"

#include "cube_bin.h"

NE_Camera *Camera;
NE_Model *Model[6];
NE_Physics *Physics[6];
NE_Model *Level[LEVEL_CNT];
NE_Physics *LevelPhysics[LEVEL_CNT];
NE_Model *Object[OBJECT_CNT];
NE_Physics *ObjectPhysics[OBJECT_CNT];
NE_Material *Material;
GameObject3D PlayerObject;

int mx = 0;
int my = 4;
int mz = 0;

//Level/object params: posx, posy, posz, sizex, sizey, sizez
int levelDefault[LEVEL_CNT][LEVEL_ARG] = {{0, -2, 0, 9, 1, 9},
                                          {-10, 0, 0, 9, 1, 9},
                                          {10, 0, 0, 9, 1, 9},
                                          {0, 3, 0, 3, 1, 3},
                                          {3, 6, 3, 3, 1, 3},
                                          {-3, 6, -3, 3, 1, 3},
                                          {0, 9, 0, 3, 3, 1},};
int objsDefault[OBJECT_CNT][OBJECT_ARG] = {{0, 4, 4, 1, 1, 1},
                                           {-10, 4, 0, 1, 1, 1},
                                           {10, 4, 0, 1, 1, 1},};

void Draw3DScene(void)
{
    NE_CameraUse(Camera);

    NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, 0);
    for (int i = 0; i < OBJECT_CNT; i++)
        NE_ModelDraw(Object[i]);
    NE_PolyFormat(31, 0, NE_LIGHT_1, NE_CULL_BACK, 0);
    for (int i = 0; i < LEVEL_CNT; i++)
        NE_ModelDraw(Level[i]);
}

void WaitLoop(void)
{
    while(1)
        swiWaitForVBlank();
}

void LoadLevel(int level[LEVEL_CNT][LEVEL_ARG]) {
    for(int i = 0; i < LEVEL_CNT; i++) {
        Level[i] = NE_ModelCreate(NE_Static);
        LevelPhysics[i] = NE_PhysicsCreate(NE_BoundingBox);
		NE_ModelLoadStaticMesh(Level[i], (u32 *)cube_bin);

        NE_PhysicsSetModel(LevelPhysics[i], // Physics object
                           (void *)Level[i]); // Model assigned to it

        NE_PhysicsEnable(LevelPhysics[i], false);
        NE_PhysicsSetSize(LevelPhysics[i], level[i][3], level[i][4], level[i][5]);
        NE_ModelScale(Level[i], level[i][3], level[i][4], level[i][5]);
        NE_ModelSetCoord(Level[i], level[i][0], level[i][1], level[i][2]);

    }
}

void LoadObjects(int objects[OBJECT_CNT][OBJECT_ARG], NE_Material *tex) {
    for(int i = 0; i < OBJECT_CNT; i++) {
        Object[i] = NE_ModelCreate(NE_Static);
        ObjectPhysics[i] = NE_PhysicsCreate(NE_BoundingBox);
		NE_ModelLoadStaticMesh(Object[i], (u32 *)cube_bin);

        NE_PhysicsSetModel(ObjectPhysics[i], // Physics object
                           (void *)Object[i]); // Model assigned to it

        NE_PhysicsEnable(ObjectPhysics[i], true);
        NE_PhysicsSetSize(ObjectPhysics[i], objects[i][3], objects[i][4], objects[i][5]);
        NE_ModelScale(Object[i], objects[i][3], objects[i][4], objects[i][5]);
        NE_ModelSetCoord(Object[i], objects[i][0], objects[i][1], objects[i][2]);
        NE_ModelSetMaterial(Object[i], tex);
        NE_PhysicsOnCollision(ObjectPhysics[i], NE_ColBounce);
        NE_PhysicsSetGravity(ObjectPhysics[i], 0.001);
        NE_PhysicsSetBounceEnergy(ObjectPhysics[i], 100);
    }
}

void LoadAndSetupGraphics3D(void)
{
    // When using nflib for the 2D sub screen engine, banks C and H are used for
    // backgrounds and banks D and I are used for sprites. Nitro Engine only
    // uses bank E for palettes, so the only thing we need to do is to tell
    // Nitro Engine to only use banks A and B and leave C and D unused.

    NE_Init3D();
    NE_TextureSystemReset(0, 0, NE_VRAM_AB);

    // Create objects

    Camera = NE_CameraCreate();
    Material = NE_MaterialCreate();

    PlayerObject.model = NE_ModelCreate(NE_Static);
    PlayerObject.physics = NE_PhysicsCreate(NE_BoundingBox);
	NE_ModelLoadStaticMesh(PlayerObject.model, (u32 *)cube_bin);
    NE_PhysicsSetModel(PlayerObject.physics, // Physics object
                        (void *)PlayerObject.model); // Model assigned to it
    NE_PhysicsEnable(PlayerObject.physics, false);
    NE_PhysicsSetSize(PlayerObject.physics, 1, 1, 1);
    PlayerObject.speedvert = 0;
    PlayerObject.speedhorz = 0;
    NE_ModelSetCoord(PlayerObject.model, mx, my, mz);

    NE_PhysicsEnable(PlayerObject.physics, true);
	NE_ModelSetRot(PlayerObject.model, 0, 0, 0);

    NE_PhysicsSetGravity(PlayerObject.physics, 0.001);
    NE_PhysicsOnCollision(PlayerObject.physics, NE_ColStop);

    if (NE_MaterialTexLoadFAT(Material, NE_A1RGB5, 32, 32, NE_TEXGEN_TEXCOORD, "bg/bunnycube.img") == 0)
    {
        consoleDemoInit();
        printf("Couldn't load texture...");
        WaitLoop();
    }

    LoadLevel(levelDefault);
    LoadObjects(objsDefault, Material);

    // Setup light

    NE_LightSet(0, NE_White, -1, -1, 0);
    NE_LightSet(1, NE_DarkGreen, -1, -1, 0);

    // Setup background color
    NE_ClearColorSet(NE_LightBlue, 31, 63);

    // Setup camera
    NE_CameraSet(Camera,
                 0, 4, 2,
                 0, 4, 0,
                 0, 1, 0);
}

void LoadAndSetupGraphics2D(void)
{
    // Initialize sub 2D engine
    NF_Set2D(1, 0);

    // Initialize sprites for sub screen (it uses VRAM D and I)
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(1);

    // Load assets from filesystem to RAM
    NF_LoadSpriteGfx("sprite/bunspr", 1, 64, 64);
    NF_LoadSpritePal("sprite/bunspr", 1);

    // Copy all frames to VRAM
    NF_VramSpriteGfx(1, 1, 0, false);
    NF_VramSpritePal(1, 1, 0);

    // Display sprite on the screen
    NF_CreateSprite(1, 0, 0, 0, 0, 0);

    // Initialize tiled backgrounds and text systems for the 2D sub engine (it
    // uses VRAM C and H)
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(1);
    NF_InitTextSys(1);

    // Load assets from filesystem to RAM
    NF_LoadTiledBg("bg/bunnybg", "capa_3", 256, 256);
    //NF_LoadTiledBg("bg/bg1", "capa_1", 1536, 256);
    //NF_LoadTiledBg("bg/bg0", "capa_0", 2048, 256);
    NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);

    // Create tiled backgrounds
    NF_CreateTiledBg(1, 3, "capa_3");
    //NF_CreateTiledBg(1, 2, "capa_1");
    //NF_CreateTiledBg(1, 1, "capa_0");

    // Create text layer
    NF_CreateTextLayer(1, 0, 0, "normal");
}

void MovePlayer(NE_Model *model, NE_Physics *phys, int x, int y, int z) {
	int move[3];
	
    int sine = sinLerp(model->ry << 6);
    int cosine = cosLerp(model->ry << 6);

    move[0] = mulf32(x, cosine)
                     - mulf32(z, sine);
    move[2] = mulf32(x, sine)
                     + mulf32(z, cosine);
					 
	NE_PhysicsSetSpeedI(phys, move[0], phys->yspeed, move[2]);
}

int main(void)
{
    // Initialize nitroFS before doing anything else
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    consoleDemoInit();
    printf("Starting nitroFS...\n");
    if (!nitroFSInit(NULL))
    {
        printf("Failed to start nitroFS\n");
        printf("Press START to exit\n");

        while (1)
        {
            swiWaitForVBlank();
            scanKeys();
            if (keysHeld() & KEY_START)
                return -1;
        }
    }

    swiWaitForVBlank();

    // Set the root folder to the nitroFS filesystem
    NF_SetRootFolder("NITROFS");

    // Setup interrupt handlers
    irqEnable(IRQ_HBLANK);
    irqSet(IRQ_VBLANK, NE_VBLFunc);
    irqSet(IRQ_HBLANK, NE_HBLFunc);

    // Load and setup graphics
    LoadAndSetupGraphics3D();
    LoadAndSetupGraphics2D();

    // Initialize variables to control the sprite
    int pj_x = 95;
    int pj_y = 63;
    unsigned int pj_frame = 0;
    unsigned int pj_anim_ticks = 0;

    int psx = 0;
    int psz = 0;

    int noisetimer = 0;

    // Initialize variables to control the backgrounds
    int bg_x[4] = {0, 0, 0};
    int bg_y[4] = {0, 0, 0};

    int fov = 2;
    bool pause = false;

    while (1)
    {

        // At this point we are in the vertical blank period. This is where 2D
        // elements have to be updated to avoid flickering.

        // Update the scroll of the backgrounds
        for (int n = 0; n < 3; n ++)
                NF_ScrollBg(1, n + 1, bg_x[n], bg_y[n]);

        // Copy shadow OAM copy to the OAM of the 2D sub engine
        oamUpdate(&oamSub);

        // Start processing a new frame after the 2D elements have been updated.

        scanKeys();
        uint32_t keys = keysHeld();
        uint32_t keysPress = keysDown();
        if (keys & KEY_SELECT)
            exit(0);
        if (keysPress & KEY_START)
            if(!pause) pause = true;
            else pause = false;
        if(!pause) {
            NE_WaitForVBL(NE_UPDATE_ANIMATIONS | NE_UPDATE_PHYSICS);
            NE_PhysicsSetSpeedI(PlayerObject.physics, 0, PlayerObject.physics->yspeed, 0);
            psx = 0;
            psz = 0;
            
            if (keys & KEY_RIGHT)
                //MovePlayer(Model[1], Physics[1], 200, 0, Physics[1]->zspeed);
                psx = 200;
            if (keys & KEY_LEFT)
                //MovePlayer(Model[1], Physics[1], -200, 0, Physics[1]->zspeed);
                psx = -200;
            if (keys & KEY_UP)
                //MovePlayer(Model[1], Physics[1], Physics[1]->xspeed, 0, -200);
                psz = -200;
            if (keys & KEY_DOWN)
                //MovePlayer(Model[1], Physics[1], Physics[1]->xspeed, 0, 200);
                psz = 200;
            if (keys & KEY_L) {
                NE_ModelRotate(PlayerObject.model, 0, -4, 0);
            }
            if (keys & KEY_R) {
                NE_ModelRotate(PlayerObject.model, 0, 4, 0);
            }
            if (keys & KEY_A) {
                if (NE_PhysicsIsColliding(PlayerObject.physics)) {
                    NE_PhysicsSetSpeed(PlayerObject.physics, 0, 0.05, 0);
                }
            }

            MovePlayer(PlayerObject.model, PlayerObject.physics, psx, 0, psz);

            if (PlayerObject.model->y < FALL_POINT) {
                NE_ModelSetCoord(PlayerObject.model, mx, my, mz);
                NE_ModelSetRot(PlayerObject.model, 0, 0, 0);
                NE_SpecialEffectSet(NE_NOISE);
                noisetimer = 10;
            }

            if (noisetimer > 1) {
                noisetimer--;
            }
            else if (noisetimer == 1) {
                NE_SpecialEffectSet(0);
                noisetimer--;
            }
        }
		///*
		NE_CameraSet(Camera,
					 f32tofloat(PlayerObject.model->x), f32tofloat(PlayerObject.model->y), f32tofloat(PlayerObject.model->z),
					 f32tofloat(PlayerObject.model->x), f32tofloat(PlayerObject.model->y), f32tofloat(PlayerObject.model->z) - fov,
					 0, 1, 0);
		//*/
		NE_CameraRotate(Camera, PlayerObject.model->rx, PlayerObject.model->ry, PlayerObject.model->rz);

        NF_UpdateTextLayers();

        NF_MoveSprite(1, 0, pj_x, pj_y);

        // Animate sprite
        pj_anim_ticks++;
        if (pj_anim_ticks > 5)
        {
            pj_anim_ticks = 0;
            pj_frame++;
            if (pj_frame > 3)
                pj_frame = 0;
            NF_SpriteFrame(1, 0, pj_frame);
        }

        // Refresh shadow OAM copy
        NF_SpriteOamSet(1);

        // Draw 3D scene
        NE_Process(Draw3DScene);
    }

    return 0;
}
