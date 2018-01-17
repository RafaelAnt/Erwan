//
// Created by Rafael on 27/09/2017.
//

#include <jni.h>
#include <android/log.h>

#include <map>

#include <vslibs.h>

#include "Parser.h"
#include "TreeNode.h"
#include "Tree.h"

static const char* mainTAG = "RafaDebugMainCPP";

#define LOGD(...) \
  ((void)__android_log_print(ANDROID_LOG_DEBUG, mainTAG, __VA_ARGS__))
#define PI 3.1415
#define EXPANSIONS_NUMBER 3
#define GROUND_RADIUS 3.0f
// INITIAL -> For the initial rotation system, with only 1 angle. - UNAVAILABLE
// SPHERIC -> For the new rotation systems, using spheric coordinates.
#define ROTATION_SYSTEM_SPHERIC 999
#define ROTATION_SYSTEM_INITIAL 998// Unused...
// FIXED -> Fixed growth, performance friendly.
// ANIMATED -> Animated growth.
#define PLANT_GROWTH_ANIMATED 997
#define PLANT_GROWTH_FIXED 996


static VSMathLib *vsml;

void initVSL() {

    VSResourceLib::setMaterialBlockName("Material");

//	Init VSML
    vsml = VSMathLib::getInstance();
    vsml->setUniformBlockName("Matrices");
    vsml->setUniformName(VSMathLib::PROJ_VIEW_MODEL, "m_pvm");
    vsml->setUniformName(VSMathLib::NORMAL_MODEL, "m_normalM");
    vsml->setUniformName(VSMathLib::MODEL, "m_model");
    vsml->setUniformName(VSMathLib::NORMAL, "m_normal");
    vsml->setUniformName(VSMathLib::VIEW_MODEL, "m_vm");

    vsml->loadIdentity(VSMathLib::AUX1);
    vsml->loadIdentity(VSMathLib::AUX2);
    vsml->loadIdentity(VSMathLib::AUX3);
}

extern "C"
void
Java_rafaelantunes_erwan_classes_GLES3JNILib_init(JNIEnv* env, jobject obj, jobject assetManager) {
    LOGD("Init Started!");

    Parser parser = Parser();
    list<ProductionRule> pr;

    int r = -1;

    if (parser.setFile("grammar.txt") == PARSER_FILE_NOT_FOUND) {
        LOGD("File Not Found!\n");
        return;
    }

    r=parser.parse();
    switch (r){
        case(PARSER_DONE):
            parser.printGrammar();
            break;

        case(PARSER_AXIOM_INVALID_CHARACTERS):
            LOGD("Axiom contains invalid characters!\n");
            return;

        case(PARSER_DEGREE_INVALID_CHARACTERS):
            LOGD("Degree contains invalid characters!\n");
            return;

        case(PARSER_PRODUCTION_RULE_INVALID_CHARACTERS):
            LOGD("One of the production rules contains invalid characters!\n");
            return;

        case(PARSER_SYNTAX_ERROR):
            LOGD("Syntax error (probably some tag \"#\" out of place or missing)...\n");
            return;

        case(PARSER_UNKNOWN_ERROR):
            LOGD("UNKNOWN ERROR (probably some tag \"#\" out of place or missing)...\n");
            return;

        case(PARSER_PRODUCTION_RULE_EQUALS_ERROR):
            printf("One of the production rules has an invalid number of equals \"=\", only one is allowed!\n");
            return;

        case(PARSER_PRODUCTION_RULE_MORE_THAN_1_LEFT):
            LOGD("One of the production rules has more than one symbol before \"=\"!\n");
            return;

        default:
            LOGD("Unexpected error!\n");
            return;
    }


    /*Assimp::Importer *imp = new Assimp::Importer();
    mgr = AAssetManager_fromJava(env, assetManager);
    AAIOSystem* ioSystem = new AAIOSystem(mgr);
    imp->SetIOHandler(ioSystem);
#ifdef __VSL_MODEL_LOADING__
    VSModelLib::SetImporter(imp);
#endif
    VSShaderLib::SetAssetManager(mgr);

    setupShaders();
    LOGD("Shaders ready");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    initVSL();
    LOGD("VSL init done");

#ifdef __VSL_MODEL_LOADING__
    model.load("teapot.obj");
    LOGD("Teapot loaded");
    model.addCubeMapTexture(0,
                            "CM/posx.jpg",
                            "CM/negx.jpg",
                            "CM/posy.jpg",
                            "CM/negy.jpg",
                            "CM/posz.jpg",
                            "CM/negz.jpg");

    LOGD("cubemap loaded");
    plane0.load("plane.obj");
    plane1.addMeshes(plane0);
    plane2.addMeshes(plane0);
    plane3.addMeshes(plane0);
    plane4.addMeshes(plane0);
    plane5.addMeshes(plane0);

    plane0.addTexture(0, "CM/negy.jpg");
    plane1.addTexture(0, "CM/negz.jpg");
    plane2.addTexture(0, "CM/posz.jpg");
    plane3.addTexture(0, "CM/posy.jpg");
    plane4.addTexture(0, "CM/posx.jpg");
    plane5.addTexture(0, "CM/negx.jpg");
#endif
    modelGrid.set(VSGrid::Y, 5, 10);
    float gray[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    modelGrid.setColor(VSResourceLib::EMISSIVE, gray);
    modelAxis.set(3.0f, 0.025f);

    surfRev.createTorus(1,2,16,16);
    pawn.createPawn();*/

    LOGD("Init done");
}