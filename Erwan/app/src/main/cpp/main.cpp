//
// Created by Rafael on 27/09/2017.
//

#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>

#include <map>

#include <vslibs.h>

#include "Parser.h"
#include "TreeNode.h"
#include "Tree.h"

static const char* mainTAG = "RafaDebugMainCPP";

#define LOGD(...) \
  ((void)__android_log_print(ANDROID_LOG_DEBUG, mainTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, mainTAG, __VA_ARGS__))
// #define LOGD_VAR(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, mainTAG, "test int = %d", testInt))

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
string pathToGrammar;

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

string ConvertJString(JNIEnv* env, jstring str)
{
    //if ( !str ) LString();

    const jsize len = env->GetStringUTFLength(str);
    const char* strChars = env->GetStringUTFChars(str, (jboolean *)0);

    std::string Result(strChars, len);

    env->ReleaseStringUTFChars(str, strChars);

    return Result;
}

extern "C"
void
Java_rafaelantunes_erwan_classes_GLES3JNILib_sendPath(JNIEnv* env, jobject obj, jstring path) {
    std::string pathConv = ConvertJString( env, path );
    pathToGrammar = pathConv;
    LOGD("Received grammmar path on android: \"%s\"", pathToGrammar.c_str());

}

extern "C"
void
Java_rafaelantunes_erwan_classes_GLES3JNILib_init(JNIEnv* env, jobject obj, jobject assetManager) {
    LOGD("Init Started!");

    Parser parser = Parser();
    list<ProductionRule> pr;

    //jobject assetManager = state_param->activity->assetManager;
    //AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    //AAssetManager* mgr = app->activity->assetManager;

    int r = -1;

    /*AAssetDir* assetDir = AAssetManager_openDir(mgr, "");
    const char* filename = (const char*)NULL;
    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) {
        AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_STREAMING);
        char buf[BUFSIZ];
        int nb_read = 0;
        FILE* out = fopen(filename, "w");
        while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
            fwrite(buf, nb_read, 1, out);
        fclose(out);
        AAsset_close(asset);
    }
    AAssetDir_close(assetDir);*/

    if (parser.setFile(pathToGrammar + "/grammar.txt") == PARSER_FILE_NOT_FOUND) {
        LOGE("File Not Found!\n");
        return;
    }

    LOGE("RAFA ERROR: Unexpected error!\n");
    r=parser.parse();
    LOGE("ACABEI O PARSE!\n");

    switch (r){
        case(PARSER_DONE):
            parser.printGrammar();
            break;

        case(PARSER_AXIOM_INVALID_CHARACTERS):
            LOGE("Axiom contains invalid characters!\n");
            return;

        case(PARSER_DEGREE_INVALID_CHARACTERS):
            LOGE("Degree contains invalid characters!\n");
            return;

        case(PARSER_PRODUCTION_RULE_INVALID_CHARACTERS):
            LOGE("One of the production rules contains invalid characters!\n");
            return;

        case(PARSER_SYNTAX_ERROR):
            LOGE("Syntax error (probably some tag \"#\" out of place or missing)...\n");
            return;

        case(PARSER_UNKNOWN_ERROR):
            LOGE("UNKNOWN ERROR (probably some tag \"#\" out of place or missing)...\n");
            return;

        case(PARSER_PRODUCTION_RULE_EQUALS_ERROR):
            LOGE("One of the production rules has an invalid number of equals \"=\", only one is allowed!\n");
            return;

        case(PARSER_PRODUCTION_RULE_MORE_THAN_1_LEFT):
            LOGE("One of the production rules has more than one symbol before \"=\"!\n");
            return;

        default:
            LOGE("Unexpected error!\n");
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