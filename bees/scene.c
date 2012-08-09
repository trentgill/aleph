/* scene.c 
   bees
   aleph

   scene management module
   includes operator network, DSP patch
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "net_protected.h"
#include "preset.h"
#include "types.h"
#include "scene.h"

typedef struct _scene {
  char sceneName[SCENE_NAME_LEN];
  char moduleName[MODULE_NAME_LEN];
} scene_t;

static FILE* pSceneFile;
static char sceneFilename[32];
// scene storage memory
static scene_t* sceneData;

void scene_init(void) {
  u8 i;
    sceneData = (scene_t*)malloc(SCENE_COUNT * sizeof(scene_t));
  
  for(i=0; i<SCENE_COUNT; i++) {
    snprintf(sceneData[i].sceneName, SCENE_NAME_LEN, " scene %d", i);
    snprintf(sceneData[i].moduleName, MODULE_NAME_LEN, " no module");
  }
  
}

void scene_deinit(void) {
    free(sceneData);
}

// store 
void scene_store(u32 idx) {
  //  memcpy(&(sceneData[idx].net), &net, sizeof(net));
  // open file  
  snprintf(sceneFilename, 32, "aleph_scene_%d", (int)idx);
  pSceneFile = fopen(sceneFilename, "wb");
  // store scene name
  fwrite(sceneData[idx].sceneName, 1, SCENE_NAME_LEN, pSceneFile);
  // store module name
  fwrite(sceneData[idx].moduleName, 1, MODULE_NAME_LEN, pSceneFile);
  // store network
  fwrite((u8*)(&net), 1, sizeof(ctlnet_t), pSceneFile);
  // store presets 
  fwrite((u8*)preset_get_presets(), 1, sizeof(preset_t) * NET_PRESETS_MAX, pSceneFile);
  // close file
  fclose(pSceneFile);
}

// recall
void scene_recall(u32 idx) {
  // open file  
  snprintf(sceneFilename, 32, "aleph_scene_%d", (int)idx);
  pSceneFile = fopen(sceneFilename, "rb"); 
  // load scene name
  fread(sceneData[idx].sceneName, 1, SCENE_NAME_LEN, pSceneFile);
  // load module name
  fread(sceneData[idx].moduleName, 1, MODULE_NAME_LEN, pSceneFile);
  // load network
  fread((u8*)(&net), 1, sizeof(ctlnet_t), pSceneFile);
  // loda  presets 
  fread((u8*)preset_get_presets(), 1, sizeof(preset_t) * NET_PRESETS_MAX, pSceneFile);
  // close file
  fclose(pSceneFile);
  //  memcpy(&net, &(sceneData[idx].net), sizeof(net));
}

// get scene name
const char* scene_name(const s16 idx) {
  if (idx >=0 && idx < SCENE_COUNT) {
    return sceneData[idx].sceneName;
  } else { return 0; }
}


// get module name
const char* scene_module_name(const s16 idx) {
  if (idx >=0 && idx < SCENE_COUNT) {
    return sceneData[idx].moduleName;
  } else { return 0; }
}
