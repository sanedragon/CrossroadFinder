#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "fortressgenerator/FortressGenerator.h"
#include "util/Inputs.h"

#define INT_MAX 2147483647
#define INT_MIN -2147483648

typedef enum {
    DOUBLE = 0,
    TRIPLE_LINE = 1,
    QUAD_LINE = 2,
    QUINT_LINE = 3,
    TRIPLE_CORNER = 4,
    QUAD_SQUARE = 5,
    QUINT_BLOB = 6
} CrossroadShape;

const char *SHAPE_NAMES[] = {
    "DOUBLE",
    "TRIPLE_LINE",
    "QUAD_LINE",
    "QUINT_LINE",
    "TRIPLE_CORNER",
    "QUAD_SQUARE",
    "QUINT_BLOB"
};

#define SHAPE_COUNT (sizeof(SHAPE_NAMES) / sizeof(SHAPE_NAMES[0]))

typedef struct {
    int x;
    int z;
} Offset;

typedef struct {
    Offset *offsets;
} Shape;

typedef struct {
    Shape *shapes;
    int offsetsCount;
    int shapesCount;
} ShapeVariants;

Offset DOUBLE_X_OFFSETS[] = {
    { 19, 0 }
};

Offset DOUBLE_Z_OFFSETS[] = {
    { 0, 19 }
};

Shape DOUBLE_SHAPES[] = {
    { DOUBLE_X_OFFSETS },
    { DOUBLE_Z_OFFSETS }
};

Offset TRIPLE_LINE_X_OFFSETS[] = {
    { 19, 0 },
    { 38, 0 }
};

Offset TRIPLE_LINE_Z_OFFSETS[] = {
    { 0, 19 },
    { 0, 38 }
};

Shape TRIPLE_LINE_SHAPES[] = {
    { TRIPLE_LINE_X_OFFSETS },
    { TRIPLE_LINE_Z_OFFSETS }
};

Offset QUAD_LINE_X_OFFSETS[] = {
    { 19, 0 },
    { 38, 0 },
    { 57, 0 }
};

Offset QUAD_LINE_Z_OFFSETS[] = {
    { 0, 19 },
    { 0, 38 },
    { 0, 57 }
};

Shape QUAD_LINE_SHAPES[] = {
    { QUAD_LINE_X_OFFSETS },
    { QUAD_LINE_Z_OFFSETS }
};

Offset QUINT_LINE_X_OFFSETS[] = {
    { 19, 0 },
    { 38, 0 },
    { 57, 0 },
    { 76, 0 }
};

Offset QUINT_LINE_Z_OFFSETS[] = {
    { 0, 19 },
    { 0, 38 },
    { 0, 57 },
    { 0, 76 }
};

Shape QUINT_LINE_SHAPES[] = {
    { QUINT_LINE_X_OFFSETS },
    { QUINT_LINE_Z_OFFSETS }
};

Offset TRIPLE_CORNER_NN_OFFSETS[] = {
    { 19, 0 },
    { 19, -19 }
};

Offset TRIPLE_CORNER_NP_OFFSETS[] = {
    { 19, 0 },
    { 19, 19 }
};

Offset TRIPLE_CORNER_PP_OFFSETS[] = {
    { 19, 0 },
    { 0, 19 }
};

Offset TRIPLE_CORNER_PN_OFFSETS[] = {
    { 0, 19 },
    { 19, 19 }
};

Shape TRIPLE_CORNER_SHAPES[] = {
    { TRIPLE_CORNER_NN_OFFSETS },
    { TRIPLE_CORNER_NP_OFFSETS },
    { TRIPLE_CORNER_PP_OFFSETS },
    { TRIPLE_CORNER_PN_OFFSETS }
};

Offset QUAD_SQUARE_OFFSETS[] = {
    { 0, 19 },
    { 19, 0 },
    { 19, 19 }
};

Shape QUAD_SQUARE_SHAPES[] = {
    { QUAD_SQUARE_OFFSETS }
};

Offset QUINT_BLOB_0_OFFSETS[] = {
    { 0, 19 },
    { 0, 38 },
    { 19, 19 },
    { 19, 38 }
};

Offset QUINT_BLOB_1_OFFSETS[] = {
    { 0, 19 },
    { 19, -19 },
    { 19, 0 },
    { 19, 19 }
};

Offset QUINT_BLOB_2_OFFSETS[] = {
    { 0, 19 },
    { 19, 0 },
    { 19, 19 },
    { 38, 0 }
};

Offset QUINT_BLOB_3_OFFSETS[] = {
    { 0, 19 },
    { 19, 0 },
    { 19, 19 },
    { 38, 19 }
};

Offset QUINT_BLOB_4_OFFSETS[] = {
    { 0, 19 },
    { 19, 0 },
    { 19, 19 },
    { 19, 38 }
};

Offset QUINT_BLOB_5_OFFSETS[] = {
    { 0, 19 },
    { 0, 38 },
    { 19, 0 },
    { 19, 19 }
};

Offset QUINT_BLOB_6_OFFSETS[] = {
    { 19, 0 },
    { 19, -19 },
    { 38, 0 },
    { 38, -19 }
};

Offset QUINT_BLOB_7_OFFSETS[] = {
    { 19, 0 },
    { 19, 19 },
    { 38, 0 },
    { 38, 19 }
};

Shape QUINT_BLOB_SHAPES[] = {
    { QUINT_BLOB_0_OFFSETS },
    { QUINT_BLOB_1_OFFSETS },
    { QUINT_BLOB_2_OFFSETS },
    { QUINT_BLOB_3_OFFSETS },
    { QUINT_BLOB_4_OFFSETS },
    { QUINT_BLOB_5_OFFSETS },
    { QUINT_BLOB_6_OFFSETS },
    { QUINT_BLOB_7_OFFSETS },
};

ShapeVariants SHAPES[] = {
    { DOUBLE_SHAPES, 1, 2 },
    { TRIPLE_LINE_SHAPES, 2, 2 },
    { QUAD_LINE_SHAPES, 3, 2 },
    { QUINT_LINE_SHAPES, 4, 2 },
    { TRIPLE_CORNER_SHAPES, 2, 4 },
    { QUAD_SQUARE_SHAPES, 3, 1 },
    { QUINT_BLOB_SHAPES, 4, 8 }
};

typedef struct {
    Version version;
    long long structureSeed;
    int salt;
    CrossroadShape crossroadShape;
    int maxY;
    int searchRadius;
    int searchCenterX;
    int searchCenterZ;
    int threadCount;
} InputData;

void processRegion(InputData *inputData, FortressGenerator *fortressGenerator, int32_t regionX, int32_t regionZ) {
    fortressGenerator_generateForRegion(fortressGenerator, inputData->structureSeed, regionX, regionZ, inputData->salt, inputData->version);
    if (fortressGenerator->piecesCount == 0) return;

    BlockBox *crossroadsByHeight[5][5];
    int crossroadCounts[5] = { 0 };
    int height[5];
    int heightCount = 0;

    Piece *lastPiece = fortressGenerator->pieces + fortressGenerator->piecesCount - 1;
    for (Piece *piece = fortressGenerator->pieces; piece <= lastPiece; piece++) {
        if (piece->pieceType == BRIDGE_CROSSING && piece->boundingBox.minY <= inputData->maxY) {
            int i;
            for (i = 0; i < heightCount; i++) {
                if (height[i] == piece->boundingBox.minY) {
                    break;
                }
            }
            crossroadsByHeight[i][crossroadCounts[i]++] = &(piece->boundingBox);
            height[i] = piece->boundingBox.minY;
            if (i == heightCount) heightCount++;
        }
    }

    ShapeVariants *shapeVariants = &(SHAPES[inputData->crossroadShape]);

    for (int heightId = 0; heightId < heightCount; heightId++) {
        int crossroadCount = crossroadCounts[heightId];
        if(crossroadCount <= shapeVariants->offsetsCount) continue;
        
        BlockBox **crossroads = crossroadsByHeight[heightId];

        for (int mainId = 0; mainId < crossroadCount; mainId++) {
            BlockBox *main = crossroads[mainId];
            for (int shapeId = 0; shapeId < shapeVariants->shapesCount; shapeId++) {
                Shape *shape = &(shapeVariants->shapes[shapeId]);
                int good = 0;
                for (int otherId = 0; otherId < crossroadCount; otherId++) {
                    if (otherId == mainId) continue;

                    BlockBox *other = crossroads[otherId];
                    int32_t offsetX = other->minX - main->minX;
                    int32_t offsetZ = other->minZ - main->minZ;
                    if ((offsetX % 19) != 0 || (offsetZ % 19) != 0) continue;

                    for (int offsetId = 0; offsetId < shapeVariants->offsetsCount; offsetId++) {
                        if (offsetX == shape->offsets[offsetId].x && offsetZ == shape->offsets[offsetId].z) {
                            good++;
                            break;
                        }
                    }
                }
                if (good >= shapeVariants->offsetsCount) {
                    printf("Found a good shape at /tp %i %i %i\n", main->minX, main->minY, main->minZ);
                    return;
                }
            }
        }
    }
}

int getInputData(InputData *inputData) {
    if (!getIntEnum("closest lower version", (int*)&inputData->version, VERSION_NAMES, VERSION_COUNT)) {
        return 0;
    }

    if (!getLongNormal("numeric world seed", &inputData->structureSeed)) {
        return 0;
    }

    if (inputData->version >= v1_16_1) {
        if (!getIntNormal("fortress salt", &inputData->salt, 1, FORTRESS_SALT, INT_MIN, INT_MAX)) {
            return 0;
        }
    }

    if (!getIntEnum("crossroad shape", (int*)&inputData->crossroadShape, SHAPE_NAMES, SHAPE_COUNT)) {
        return 0;
    }

    if (!getIntNormal("maxY", &inputData->maxY, 1, 255, 48, INT_MAX)) {
        return 0;
    }
    
    if (!getIntNormal("search radius", &inputData->searchRadius, 0, 0, 0, 30000000)) {
        return 0;
    }

    if (!getIntNormal("search center X", &inputData->searchCenterX, 1, 0, -30000000, 30000000)) {
        return 0;
    }
    
    if (!getIntNormal("search center Z", &inputData->searchCenterZ, 1, 0, -30000000, 30000000)) {
        return 0;
    }

    if (!getIntNormal("number of threads to use", &inputData->threadCount, 1, 1, 1, 10000)) {
        return 0;
    }

    return 1;
}

InputData inputData;
int32_t regionSize;
int32_t regionRadius;
int32_t regionCenterX;
int32_t regionCenterZ;
int32_t regionMinX;
int32_t regionMaxX;
int32_t regionMinZ;
int32_t regionMaxZ;
int32_t currentRegionX;
int32_t currentRegionZ;

pthread_mutex_t regionSelectLock;

/* This is the one critical section of code that needs a lock */
bool selectNextRegion(Offset *output) {
    pthread_mutex_lock(&regionSelectLock);

    if(currentRegionX > regionMaxX) {
        /* if x coordinate is out of bounds, exit */
        pthread_mutex_unlock(&regionSelectLock);
        return false;
    }
    output->x = currentRegionX;
    output->z = currentRegionZ;

    if(currentRegionZ >= regionMaxZ) {
        currentRegionZ = regionMinZ;
        currentRegionX++;
    } else {
        currentRegionZ++;
    }

    pthread_mutex_unlock(&regionSelectLock);

    return true;
}

void *thread_main(void *unused) {
    FortressGenerator fortressGenerator;

    Offset region;
    while(selectNextRegion(&region)) {
        processRegion(&inputData, &fortressGenerator, region.x, region.z);
    }
    return NULL;
}

int main() {

    if (getInputData(&inputData)) {
        regionSize = fortressGenerator_getRegionSize(inputData.version);

        regionRadius = inputData.searchRadius / regionSize / 16 + 1;
        regionCenterX = inputData.searchCenterX / regionSize / 16;
        regionCenterZ = inputData.searchCenterZ / regionSize / 16;

        regionMinX = regionCenterX - regionRadius;
        regionMaxX = regionCenterX + regionRadius;
        regionMinZ = regionCenterZ - regionRadius;
        regionMaxZ = regionCenterZ + regionRadius;

        currentRegionX = regionMinX;
        currentRegionZ = regionMinZ;

        pthread_mutex_init(&regionSelectLock, NULL);

        pthread_t *threads = calloc(inputData.threadCount, sizeof(pthread_t));
	for(int32_t i = 0; i < inputData.threadCount; i++) {
            pthread_create(threads + i, NULL, thread_main, NULL);
        }
	for(int32_t i = 0; i < inputData.threadCount; i++) {
            pthread_join(threads[i], NULL);
        }
	pthread_mutex_destroy(&regionSelectLock);
    }

    system("pause");

    return 0;
}
