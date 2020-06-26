#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"
#include "binary/red.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#define SHADOW_MAP_SIZE 1024

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;

// for plane
Object* Scene::plane = nullptr;
Texture* Scene::planeColor = nullptr;

// for shadow
//Shader* Scene::firstPassVertexShader = nullptr;
//Shader* Scene::firstPassFragmentShader = nullptr;
//Program* Scene::firstPassProgram = nullptr;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");
/*
    Scene::firstPassVertexShader = new Shader(GL_VERTEX_SHADER, "firstPassVertex.glsl");
    Scene::firstPassFragmentShader = new Shader(GL_FRAGMENT_SHADER, "firstPassFragment.glsl");

    GLuint shadowTexture;
    glGenTextures(1, &shadowTexture);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexture, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glDrawBuffer(GL_NONE); // No color buffer is drawn to.

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        // return false;
        LOG_PRINT_DEBUG("GL_FRAMEBUFFER incomplete");
    }


    vec3 lightInvDir = vec3(0,0,80); // Compute the MVP matrix from the light's point of view
    mat4 lightProjMat = ortho<float>(-10,10,-10,10,-10,20);
    mat4 lightViewMat = lookAt(vec3(0,0,80), vec3(0,0,0), vec3(0,1,0));
    mat4 depthModelMatrix = mat4(1.0);
    mat4 depthMVP = lightProjMat * lightViewMat * depthModelMatrix;
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(shadowTexture, 1, GL_FALSE, &depthMVP[0][0]);


    Scene::firstPassProgram = new Program(Scene::firstPassVertexShader, Scene::firstPassFragmentShader);
    */
    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);

    //player
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));

    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
    Scene::lineDraw->worldMat = scale(vec3(1.0f / 3.0f));

    // plane
    Scene::planeColor = new Texture(Scene::program, 0, "textureDiff", redTexels, redSize);
    Scene::plane = new Object(program, material, planeVertices, planeIndices);


//    glUseProgram(firstPassProgram);
//    glUseProgram(program);
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
}

void Scene::update(float deltaTime) {


    Scene::program->use();

    Scene::camera->update();

    const float playTime = 5.0;
    static float time = 0.0f;
    time += deltaTime;
    if (time >= playTime) time -= playTime;

    int prevFrame = (int)floor(time / playTime * motions.size());
    int nextFrame = (int)ceil(time / playTime * motions.size());
    if(nextFrame >= motions.size()) nextFrame = 0;
    float ratio = time / playTime * motions.size() - (float)prevFrame;

    // from bone space to world space
    vector<mat4x4> bone2world = {mat4x4(1.0f)};
    // from world space to bone space
    vector<mat4x4> world2bone = {mat4x4(1.0f)};

    vector<vec3> beforeOffset{{jOffset[0]}};

    for(int jointIdx = 1; jointIdx < jNames.size(); jointIdx ++) {
        //Motion interpolation using an Euler angle or quaternion.
        // Please use prevFrame, nextFrame, ratio, R, bone2world, and world2bone.

        // get character space coordinates(x,y,z) of each joint
        int parentIdx = jParents[jointIdx];

        float x = jOffset[jointIdx][0];
        float y = jOffset[jointIdx][1];
        float z = jOffset[jointIdx][2];

        mat4x4 T = mat4x4{
            1.0f,0.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,0.0f,
            0.0f,0.0f,1.0f,0.0f,
            x,   y,   z,   1.0f
        };
        mat4x4 inverseT = {
            1.0f,0.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,0.0f,
            0.0f,0.0f,1.0f,0.0f,
            -x,  -y,  -z,  1.0f
        };

        beforeOffset.push_back({vec3(column(bone2world[jointIdx],3))});

        // get rotation of each joint
        float rotX;
        float rotY;
        float rotZ;

        if (nextFrame == 0) {
            rotX = motions[prevFrame][3 + 3*jointIdx];
            rotY = motions[prevFrame][4 + 3*jointIdx];
            rotZ = motions[prevFrame][5 + 3*jointIdx];
        } else {
            rotX = mix(motions[prevFrame][3 + 3*jointIdx], motions[nextFrame][3 + 3*jointIdx], ratio);
            rotY = mix(motions[prevFrame][4 + 3*jointIdx], motions[nextFrame][4 + 3*jointIdx], ratio);
            rotZ = mix(motions[prevFrame][5 + 3*jointIdx], motions[nextFrame][5 + 3*jointIdx], ratio);
        }

//        LOG_PRINT_DEBUG("jointIdx: %d\tx: %f, y: %f, z: %f\n", jointIdx, x, y, z);
//        LOG_PRINT_DEBUG("prevFrame: %d  nextFrame: %d\trotx: %f, roty: %f, rotz: %f\n", prevFrame, nextFrame, rotX, rotY, rotZ);

//        vec3 eulerX = {rotX, 0.0f, 0.0f};
//        vec3 eulerY = {0.0f, rotY, 0.0f};
//        vec3 eulerZ = {0.0f, 0.0f, rotZ};
//
//        quat quaternionX = quat(eulerX);
//        quat quaternionY = quat(eulerY);
//        quat quaternionZ = quat(eulerZ);
//
//        mat4x4 rotMatX = toMat4(quaternionX);
//        mat4x4 rotMatY = toMat4(quaternionY);
//        mat4x4 rotMatZ = toMat4(quaternionZ);

        mat4x4 rotMatX = {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cos(radians(rotX)), sin(radians(rotX)), 0.0f,
                0.0f, -sin(radians(rotX)), cos(radians(rotX)), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };
        mat4x4 rotMatY = {
                cos(radians(rotY)), 0.0f, -sin(radians(rotY)), 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                sin(radians(rotY)), 0.0f, cos(radians(rotY)), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };
        mat4x4 rotMatZ = {
                cos(radians(rotZ)), sin(radians(rotZ)), 0.0f, 0.0f,
                -sin(radians(rotZ)), cos(radians(rotZ)), 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };

        mat4x4 R = rotMatZ * rotMatX * rotMatY;

        // With motion
        //bone2world;
        bone2world.push_back(bone2world[parentIdx] * T * R);
        //world2bone;
        world2bone.push_back(inverseT * world2bone[parentIdx]); // without R
    }

    //set new vertex
    vector<Vertex> newVertex;
    float vertexX, vertexY, vertexZ;
    vec4 vertexPos;
    for(int vertexIdx = 0; vertexIdx <= playerVertices.size(); vertexIdx++){
        vec3 result;

        vertexX = playerVertices[vertexIdx].pos[0];
        vertexY = playerVertices[vertexIdx].pos[1];
        vertexZ = playerVertices[vertexIdx].pos[2];

        vertexPos = {vertexX, vertexY, vertexZ, 1.0f};

        for(int boneIter = 0; boneIter < 4; boneIter ++) {
            int boneIndex = playerVertices[vertexIdx].bone[boneIter];
            if (boneIndex != -1) {
                result += vec3(playerVertices[vertexIdx].weight[boneIter] * bone2world[boneIndex] * world2bone[boneIndex] * vertexPos);
            }
        }
        newVertex.push_back( {result, playerVertices[vertexIdx].nor, playerVertices[vertexIdx].tex, playerVertices[vertexIdx].bone, playerVertices[vertexIdx].weight} );
    }
    Scene::player->load(newVertex, playerIndices);
    Scene::player->draw();

    vector<Vertex> skeletonPos = {{jOffset[0]}};
    vector<Index> skeletonIndex;
    for (int jointIdx = 1; jointIdx < jNames.size(); jointIdx++) {
        int parentIdx = jParents[jointIdx];

        skeletonPos.push_back({vec4(column(bone2world[jointIdx], 3))});

        skeletonIndex.push_back(jointIdx);
        skeletonIndex.push_back(parentIdx);
    }

    //If you want to see the skeleton as a line..
//        glLineWidth(20);
//        Scene::lineDraw->load(skeletonPos, skeletonIndex);
//        Scene::lineDraw->draw();

    Scene::plane->draw();
}