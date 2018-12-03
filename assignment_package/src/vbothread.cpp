#include "vbothread.h"
#include <iostream>

using namespace std;

VBOThread::VBOThread(Chunk* ch, int x, int z, QHash<int64_t, Chunk*> map, QMutex* m)
    : c(ch), xChunk(x), zChunk(z), chunkMap(map), mutex(m)
{}

void VBOThread::createVertexPosNorUV() {
    c->c_vert_pos_nor_uv_opaque.clear();
    c->c_vert_pos_nor_uv_transparent.clear();
    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 256; y++) {
            for(int z = 0; z < 16; z++) {
                currT = c->getBlockType(x, y, z);
                if(currT != EMPTY) {
                    float cosinePow = 0.0f;
                    float isAnimate = 0;
                    bool isTransparent = false;
                    uvIndicator = 0;
                    bool (VBOThread::*checkFunction)(BlockType);
                    switch(currT) {
                    case DIRT:
                        uvIndicator = 1;
                        cosinePow = 90.0f;
                        isAnimate = 0;
                        isTransparent = false;
                        checkFunction = &VBOThread::shouldRenderSolid;
                        break;
                    case GRASS:
                        uvIndicator = 2;
                        cosinePow = 70.0f;
                        isAnimate = 0;
                        isTransparent = false;
                        checkFunction = &VBOThread::shouldRenderSolid;
                        break;
                    case STONE:
                        uvIndicator = 3;
                        cosinePow = 30.0f;
                        isAnimate = 0;
                        isTransparent = false;
                        checkFunction = &VBOThread::shouldRenderSolid;
                        break;
                    case WOOD:
                        uvIndicator = 4;
                        cosinePow = 90.0f;
                        isAnimate = 0;
                        isTransparent = false;
                        checkFunction = &VBOThread::shouldRenderSolid;
                        break;
                    case LEAF:
                        uvIndicator = 5;
                        cosinePow = 70.0f;
                        isAnimate = 0;
                        isTransparent = false;
                        checkFunction = &VBOThread::shouldRenderSolid;
                        break;
                    case BEDROCK:
                        uvIndicator = 6;
                        cosinePow = 50.0f;
                        isAnimate = 0;
                        isTransparent = false;
                        checkFunction = &VBOThread::shouldRenderSolid;
                        break;
                    case LAVA:
                        uvIndicator = 7;
                        cosinePow = 10.0f;
                        isAnimate = 1;
                        isTransparent = false;
                        checkFunction = &VBOThread::shouldRenderTransp;
                        break;
                    case WATER:
                        uvIndicator = 8;
                        cosinePow = 10.0f;
                        isAnimate = 1;
                        isTransparent = true;
                        checkFunction = &VBOThread::shouldRenderTransp;
                        break;
                    case ICE:
                        uvIndicator = 9;
                        cosinePow = 10.0f;
                        isAnimate = 0;
                        isTransparent = true;
                        checkFunction = &VBOThread::shouldRenderTransp;
                        break;
                    }

                    // top
                    if(checkEmpty(x, y+1, z, checkFunction)) {
                        glm::vec2 uvPoint = getTexture(0); // get the top uv starting from top left vertex
                        glm::vec4 normal = glm::vec4(0, 1, 0, 0);
                        if (isTransparent) {
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        } else {
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        }
                    }
                    // bottom
                    if(checkEmpty(x, y-1, z, checkFunction)) {
                        glm::vec2 uvPoint = getTexture(1); // get the bottom uv starting from top left vertex
                        glm::vec4 normal = glm::vec4(0, -1, 0, 0);
                        if (isTransparent) {
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        } else {
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        }
                    }
                    //right
                    if(checkEmpty(x+1, y, z, checkFunction)) {
                        glm::vec2 uvPoint = getTexture(2); // get the bottom uv starting from top left vertex
                        glm::vec4 normal = glm::vec4(1, 0, 0, 0);
                        if (isTransparent) {
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        } else {
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        }
                    }
                    //left
                    if(checkEmpty(x-1, y, z, checkFunction)) {
                        glm::vec2 uvPoint = getTexture(2); // get the side uv starting from top left vertex
                        glm::vec4 normal = glm::vec4(-1, 0, 0, 0);
                        if (isTransparent) {
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        } else {
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        }
                    }
                    //front
                    if(checkEmpty(x, y, z+1, checkFunction)) {
                        glm::vec2 uvPoint = getTexture(2); // get the side uv starting from top left vertex
                        glm::vec4 normal = glm::vec4(0, 0, 1, 0);
                        if (isTransparent) {
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        } else {
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y+1, z+1, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        }
                    }
                    //back
                    if(checkEmpty(x, y, z-1, checkFunction)) {
                        glm::vec2 uvPoint = getTexture(2); // get the side uv starting from top left vertex
                        glm::vec4 normal = glm::vec4(0, 0, -1, 0);
                        if (isTransparent) {
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(x+1, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_transparent.push_back(normal);
                            c->c_vert_pos_nor_uv_transparent.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        } else {
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y -= 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.x += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                            uvPoint.y += 1.0f / 16.0f;
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(x+1, y+1, z, 1.0f));
                            c->c_vert_pos_nor_uv_opaque.push_back(normal);
                            c->c_vert_pos_nor_uv_opaque.push_back(glm::vec4(uvPoint.x, uvPoint.y, cosinePow, isAnimate));
                        }
                    }
                }
            }
        }
    }
}

glm::vec2 VBOThread::getTexture(int faceNum)
{
    // if dirt
    if (uvIndicator == 1) {
        return glm::vec2(1.0f / 16.0f * 2, 1.0f);
    } else if (uvIndicator == 2) { // if grass
        // check if grass top
        if (faceNum == 0) {
            return glm::vec2(1.0f / 16.0f * 8, 1.0f / 16.0f * 14);
        } else if (faceNum == 1) { // check if grass bottom
            return glm::vec2(1.0f / 16.0f * 2, 1.0f);
        } else { // grass sides
            return glm::vec2(1.0f / 16.0f * 3, 1.0f);
        }
    } else if (uvIndicator == 3) { // if stone
        return glm::vec2(1.0f / 16.0f * 1, 1.0f);
    } else if (uvIndicator == 4) { // if wood
        // check if wood top or bottom
        if (faceNum == 0) {
        return glm::vec2(1.0f / 16.0f * 5, 1.0f / 16.0f * 15);
        } else { // wood sides
        return glm::vec2(1.0f / 16.0f * 4, 1.0f / 16.0f * 15);
        }
    } else if (uvIndicator == 5) { // if leaf
        return glm::vec2(1.0f / 16.0f * 5, 1.0f / 16.0f * 13);
    } else if (uvIndicator == 6) { // if bedrock
        return glm::vec2(1.0f / 16.0f * 1, 1.0f / 16.0f * 15);
    } else if (uvIndicator == 7) { // if lava
        return glm::vec2(1.0f / 16.0f * 13, 1.0f / 16.0f * 2);
    } else if (uvIndicator == 8) { // if water
        return glm::vec2(1.0f / 16.0f * 13, 1.0f / 16.0f * 4);
    } else { // if ice
        return glm::vec2(1.0f / 16.0f * 3, 1.0f / 16.0f * 12);
    }
}

bool VBOThread::shouldRenderSolid(BlockType b) {
    return b == EMPTY || b == WATER || b == ICE || b == LAVA;
}

bool VBOThread::shouldRenderTransp(BlockType b) {
    return (b == EMPTY || ((b == WATER || b == ICE || b == LAVA) && b != currT));
}

bool VBOThread::checkEmpty(int x, int y, int z, bool (VBOThread::*checkFunc)(BlockType)) {
    if(x >= 0 && y >= 0 && z >= 0 && x < 16 && y < 256 && z < 16) {
        return (this->*checkFunc)(c->getBlockType(x, y, z));
    } else {
        if(x < 0) {
            if(chunkMap.contains(getKey(xChunk-1, zChunk, true))) {
                return (this->*checkFunc)(chunkMap[getKey(xChunk-1, zChunk, true)]->getBlockType(15, y, z%16));
            }
        } else if(y < 0) {
            return true; // under terrain, nothing there
        } else if(z < 0) {
            if(chunkMap.contains(getKey(xChunk, zChunk-1, true))) {
                return (this->*checkFunc)(chunkMap[getKey(xChunk, zChunk-1, true)]->getBlockType(x%16, y, 15));
            }
        } else if(x >= 16) {
            if(chunkMap.contains(getKey(xChunk+1, zChunk, true))) {
                return (this->*checkFunc)(chunkMap[getKey(xChunk+1, zChunk, true)]->getBlockType(0, y, z%16));
            }
        } else if(y >= 256) {
            return true;
        } else {
            if(chunkMap.contains(getKey(xChunk, zChunk+1, true))) {
                return (this->*checkFunc)(chunkMap[getKey(xChunk, zChunk+1, true)]->getBlockType(x%16, y, 0));
            }
        }
        return true;
    }
}

int64_t VBOThread::getKey(int x, int z, bool chunked) const {
    int64_t xz = 0xffffffffffffffff;
    int64_t chunkx;
    int64_t chunkz;
    if(chunked) {
        chunkx = x;
        chunkz = z;
    } else {
        float fx = floorf(x / 16.0);
        float fz = floorf(z / 16.0);
        chunkx = int64_t(fx);
        chunkz = int64_t(fz);
    }
    xz = (xz & (chunkx << 32)) | 0x00000000ffffffff;
    chunkz = chunkz | 0xffffffff00000000;
    xz = xz & chunkz;
    return xz;
}

void VBOThread::run() {
    createVertexPosNorUV();
    c->facesOpaque = c->c_vert_pos_nor_uv_opaque.size() / 12;
    c->facesTransparent = c->c_vert_pos_nor_uv_transparent.size() / 12;
}