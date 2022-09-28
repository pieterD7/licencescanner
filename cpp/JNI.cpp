#include <android/log.h>

#include <jni.h>
#include "recognize.h"
#include "analyse.h"
#include "util.h"

#define LOG_TAG "JNI Native"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

typedef struct fieldIDs{
    jfieldID rectTop;
    jfieldID rectLeft;
    jfieldID rectWidth;
    jfieldID rectHeight;
} fieldIDs;

typedef struct field{
    const char * class_name;
    const char * field_name;
    const char * field_type;
    jfieldID * jfield;
    field(const char * class_name, const char * field_name, const char * field_type, jfieldID* fieldID){
        this->class_name = class_name;
        this->field_name = field_name;
        this->field_type = field_type;
        this->jfield = fieldID;
    }
} field;

fieldIDs* fields = new fieldIDs();

string licence = "";

// Utility function to get ID's of android.graphics.rect
static int find_fields(JNIEnv *env, field *fields, int count)
{
	jclass clazz;
    for (int i = 0; i < count; i++) {
        field *f = &fields[i];
        clazz = env->FindClass(f->class_name);
        if (env->ExceptionCheck()) {
          LOGD("JNI error");
          return -1;
        }
        if (clazz == NULL) {
            LOGD("Can't find %s", f->class_name);
            return -1;
        }

        jfieldID field = env->GetFieldID(clazz, f->field_name, f->field_type);
        if (field == NULL || env->ExceptionCheck()) {
            // Somehow this line line is never visible in the logs
            LOGD("Can't find %s.%s", f->class_name, f->field_name);
            return -1;
        }

        *(f->jfield) = field;
    }
    env->DeleteLocalRef(clazz);

    return 0;
}

jobject setRectangle(JNIEnv* env, jobject objRect, Rect rect){
    (env)->SetIntField(objRect, fields->rectTop, rect.y);
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    (env)->SetIntField(objRect, fields->rectLeft, rect.x);
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    (env)->SetIntField(objRect, fields->rectWidth, rect.width);
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    (env)->SetIntField(objRect, fields->rectHeight, rect.height);
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    return jobject();
}

jobject createRectangles(JNIEnv* env, Rect rect ){

    jclass clsArrayList = (env)->FindClass("java/util/ArrayList");
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    jmethodID constructor = (env)->GetMethodID(clsArrayList, "<init>", "()V");
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    jobject objArrayList = (env)->NewObject(clsArrayList, constructor, "");
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    jobject objArrayList0 = (env)->NewObject(clsArrayList, constructor, "");
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
        return jobject();
    }
    jobject objArrayList2 = (env)->NewObject(clsArrayList, constructor, "");
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    jobject objArrayList3 = (env)->NewObject(clsArrayList, constructor, "");
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    jmethodID arrayListAdd = (env)->GetMethodID(clsArrayList, "add", "(Ljava/lang/Object;)Z");
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    jclass clsRect = (env)->FindClass("android/graphics/Rect");
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }
    jmethodID constructorRect = (env)->GetMethodID(clsRect, "<init>", "()V");
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }

    jobject objRect0 = (env)->NewObject(clsRect, constructorRect, "");

    setRectangle(env, objRect0, rect);

    env->CallBooleanMethod(objArrayList0, arrayListAdd, objRect0);
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }

    env->CallBooleanMethod(objArrayList2, arrayListAdd, objArrayList0);
    if (env->ExceptionCheck()) {
      LOGD("JNI error");
      return jobject();
    }

    vector<MatrixElement*> dmatrix = get_dmatrix();

    vector<Rect*> type = get_type();

    for(int n = 0; n < dmatrix.size(); n++){
        jobject objRect = (env)->NewObject(clsRect, constructorRect, "");

        jobject objRect2 = (env)->NewObject(clsRect, constructorRect, "");

        MatrixElement* el = dmatrix[n];
        Rect rect = *type[el->b];
        Rect rect2 = *type[el->bb];

        setRectangle(env, objRect, rect);
        setRectangle(env, objRect2, rect2);

        env->CallBooleanMethod(objArrayList, arrayListAdd, objRect);
        if (env->ExceptionCheck()) {
          LOGD("JNI error");
          return jobject();
        }
        env->CallBooleanMethod(objArrayList, arrayListAdd, objRect2);
        if (env->ExceptionCheck()) {
          LOGD("JNI error");
          return jobject();
        }

        env->DeleteLocalRef(objRect);
        if (env->ExceptionCheck()) {
            LOGD("JNI error");
            return jobject();
        }
        env->DeleteLocalRef(objRect2);
        if (env->ExceptionCheck()) {
             LOGD("JNI error");
             return jobject();
         }
    }

    env->CallBooleanMethod(objArrayList2, arrayListAdd, objArrayList);

    for(int n = 0; n < type.size(); n++){
        Rect rect = *type[n];

        jobject objRect3 = (env)->NewObject(clsRect, constructorRect, "");

        setRectangle(env, objRect3, rect);

        env->CallBooleanMethod(objArrayList3, arrayListAdd, objRect3);
        if (env->ExceptionCheck()) {
          LOGD("JNI error");
          return jobject();
        }

        env->DeleteLocalRef(objRect3);
        if (env->ExceptionCheck()) {
            LOGD("JNI error");
            return jobject();
        }
    }

    env->CallBooleanMethod(objArrayList2, arrayListAdd, objArrayList3);

    if(clsArrayList != NULL)
    {
        env->DeleteLocalRef(clsArrayList);
        if (env->ExceptionCheck()) {
            LOGD("JNI error");
            return jobject();
        }
    }

    return objArrayList2;
}

void init(
    JNIEnv* env,
    jobject thiz,
    jstring alphabet,
    jstring model
){
    LOGD("Init called : %s", env->GetStringUTFChars(alphabet, 0));

    try{
        init_recognition(
            env->GetStringUTFChars(alphabet, 0),
            env->GetStringUTFChars(model, 0)
        );
    }
    catch(const exception&){
        LOGD("Error init recognition");
    }
}

void analyse_exit(JNIEnv* env, jobject thiz){
}

Rect* androidRectToOpenCVRect(JNIEnv* env, jobject rect){
    int x = env->GetIntField(rect, fields->rectLeft);
    int y = env->GetIntField(rect, fields->rectTop);
    int width = env->GetIntField(rect, fields->rectWidth);
    int height = env->GetIntField(rect, fields->rectHeight);

    return new Rect(x, y, width, height);
}

jobject analyse_native(
    JNIEnv* env,
    jobject thiz,
    jlong addrGray,
    jobject rectH/*,
    jint orientation*/
){
    Rect* recth = androidRectToOpenCVRect(env, rectH);

    licence = "";

    Mat& mGray = *(Mat*)addrGray;
    int width = mGray.cols;
    int height = mGray.rows;

    try{
        threshold(mGray, mGray, 40, 255, 3);
        Rect rect = *recth;
        licence = analyse_mat(mGray(rect));
    }
    catch(const exception&){
    }

    vector<vector<Rect *>> bxs = get_bxs();

    Rect r;
    if(bxs.size() > 0)
        r = bounding_rect(bxs[0]);

    mGray.release();

    return createRectangles(env, r);
}

jstring get_licence(
    JNIEnv* env,
    jobject thiz){

    LOGD("Get licence");
    return env->NewStringUTF(licence.c_str());
}

/* This function will be called when the library gets loaded */
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGD("JNI Error");
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("nl/nr8/licencescanner/Analyser");
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
        {"init", "(Ljava/lang/String;Ljava/lang/String;)V",
            reinterpret_cast<void*>(init)},
        //{"analyse_exit", "()V", reinterpret_cast<void*>(analyse_exit)},
        {"analyse", "(JLandroid/graphics/Rect;)Ljava/util/ArrayList;",
            reinterpret_cast<void*>(analyse_native)},
        {"get_licence", "()Ljava/lang/String;",
            reinterpret_cast<void*>(get_licence)}
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    /// An array for collecting all the fields
    jfieldID test = jfieldID();
    field fields_to_find[] = {
        //{ "org/opencv/core/Rect", "x", "I",  &test},
        { "android/graphics/Rect", "top", "I", &fields->rectTop },
        { "android/graphics/Rect", "left",   "I", &fields->rectLeft },
        { "android/graphics/Rect", "right",  "I", &fields->rectWidth },
        { "android/graphics/Rect", "bottom",    "I", &fields->rectHeight },
    };

    /// Find all the field IDs
    if (find_fields(env, fields_to_find, sizeof(fields_to_find)/sizeof(fields_to_find[0])))
        return -1;

    //LOGD("TEST %d", test);

    return JNI_VERSION_1_6;
}