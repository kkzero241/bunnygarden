#include "NEMain.h"
#include "GameObject3D.h"

static GameObject3D **GameObject3DPointers;

GameObject3D *GameObject3DCreate(NE_ModelType modeltype, NE_PhysicsTypes physicstype) {

    GameObject3D *go3d = calloc(1, sizeof(GameObject3D));
    if (go3d == NULL)
    {
        NE_DebugPrint("Not enough memory");
        return NULL;
    }

    go3d->model = *NE_ModelCreate(modeltype);
    go3d->physics = *NE_PhysicsCreate(physicstype);

    NE_PhysicsSetModel(&go3d->physics, // Physics object
                        (void *)&go3d->model); // Model assigned to it
    
    if (&go3d->model == NULL) {
        NE_DebugPrint("Model failed to load");
        return NULL;
    }

    if (&go3d->physics == NULL) {
        NE_DebugPrint("Physics failed to load");
        return NULL;
    }

    int i = 0;
    while (1)
    {
        if (GameObject3DPointers[i] == NULL)
        {
            GameObject3DPointers[i] = go3d;
            break;
        }
        i++;
    }

    go3d->speedvert = 0;
    go3d->speedhorz = 0;

    return go3d;
}

void GameObject3DDelete(GameObject3D *object) {
    NE_AssertPointer(object, "NULL pointer");

    NE_ModelDelete(&object->model);
    NE_PhysicsDelete(&object->physics);

    free(object);

}