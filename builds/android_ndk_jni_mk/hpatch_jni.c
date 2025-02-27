// hpatch_jni.c
// Created by sisong on 2019-09-30.
#include <jni.h>
#include "hpatch.h"
#ifdef __cplusplus
extern "C" {
#endif
    #define HPATCH_OPTIONS_ERROR 1
    #define _check_rt(v)              do { if (!(v)) { result=HPATCH_OPTIONS_ERROR; goto _clear; }; } while(0)
    #define __j2cstr_(jstr,cstr)      do { (cstr)=(*jenv)->GetStringUTFChars(jenv,jstr,NULL); _check_rt(cstr); } while(0)
    #define _check_j2cstr(jstr,cstr)  do { _check_rt(jstr); __j2cstr_(jstr,cstr); } while(0)
    #define _check_jn2cstr(jstr,cstr) do { if (jstr) __j2cstr_(jstr,cstr); else (cstr)=0; } while(0)
    #define _jrelease_cstr(jstr,cstr) do { if (cstr) (*jenv)->ReleaseStringUTFChars(jenv,jstr,cstr); } while(0)


    static size_t getCacheMemory(jlong cacheMemory){
        #define kPatchCacheSize_default  (1024*256)
        #define kPatchCacheSize_max ((jlong)((size_t)(~(size_t)0)))
        if (cacheMemory<0) return kPatchCacheSize_default;
        if (sizeof(jlong)<=sizeof(size_t)) return (size_t)cacheMemory;
        return (size_t)((cacheMemory<kPatchCacheSize_max)?cacheMemory:kPatchCacheSize_max);
    }

    JNIEXPORT jint
    Java_com_github_sisong_HPatch_patch(JNIEnv* jenv,jobject jobj,
                                        jstring oldFileName,jstring diffFileName,
                                        jstring outNewFileName,jlong cacheMemory){
        const char* cOldFileName   =0;
        const char* cDiffFileName  =0;
        const char* cOutNewFileName=0;
        size_t cCacheMemory=getCacheMemory(cacheMemory);
        int result=0;

        _check_jn2cstr(oldFileName,cOldFileName);
        _check_j2cstr(diffFileName,cDiffFileName);
        _check_j2cstr(outNewFileName,cOutNewFileName);
        result=hpatchz(cOldFileName,cDiffFileName,cOutNewFileName,cCacheMemory);
    _clear:
        _jrelease_cstr(outNewFileName,cOutNewFileName);
        _jrelease_cstr(diffFileName,cDiffFileName);
        _jrelease_cstr(oldFileName,cOldFileName);
        return result;
    }
    
#ifdef __cplusplus
}
#endif

