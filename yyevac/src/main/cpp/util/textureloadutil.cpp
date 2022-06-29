//
// Created by zengjiale on 2022/4/21.
//


#include <bean/evasrc.h>
#include <EGL/egl.h>
#include "textureloadutil.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "util/stb_image_write.h"

#define LOG_TAG "TextureLoadUtil"
#define ELOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ELOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

GLuint TextureLoadUtil::loadTexture(unsigned char *bitmap, AndroidBitmapInfo* info) {
    if (bitmap == nullptr) {
        ELOGE("bitmap = null");
        return 0;
    }

    GLuint textureObjectIds = 0;
    glGenTextures(1, &textureObjectIds);
    if (textureObjectIds == 0) {
        ELOGE("textureObjectIds = 0");
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureObjectIds);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint format = GL_RGB;
    if (info->format == ANDROID_BITMAP_FORMAT_RGB_565) { //RGB三通道，例如jpg格式
        format = GL_RGB;
    } else if (info->format == ANDROID_BITMAP_FORMAT_RGBA_8888) {  //RGBA四通道，例如png格式 android图片是ARGB排列，所以还是要做图片转换的
        format = GL_RGBA;
    }
    //将图片数据生成一个2D纹理
    glTexImage2D(GL_TEXTURE_2D, 0, format, info->width, info->height, 0, format, GL_UNSIGNED_BYTE,
                 reinterpret_cast<const void *>(*bitmap));

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureObjectIds;
}

GLuint TextureLoadUtil::loadTexture(EvaSrc* src) {
    if (src->bitmap == nullptr) {
        ELOGE("bitmap = null");
        return 0;
    }

    GLuint textureObjectIds = 0;
    glGenTextures(1, &textureObjectIds);
    if (textureObjectIds == 0) {
        ELOGE("textureObjectIds = 0");
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureObjectIds);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint format = GL_RGB;
    if (src->bitmapFormat == ANDROID_BITMAP_FORMAT_RGB_565) { //RGB三通道，例如jpg格式
        format = GL_RGB;
    } else if (src->bitmapFormat == ANDROID_BITMAP_FORMAT_RGBA_8888) {  //RGBA四通道，例如png格式 android图片是ARGB排列，所以还是要做图片转换的
        format = GL_RGBA;
    }
    //将图片数据生成一个2D纹理
    glTexImage2D(GL_TEXTURE_2D, 0, format, src->bitmapWidth, src->bitmapHeight, 0, format, GL_UNSIGNED_BYTE,
                 src->bitmap);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
//
//    if (stbi_write_png(src->saveAddress.c_str(), src->bitmapWidth, src->bitmapHeight, 4,
//                       src->bitmap, 0)) {
//        ELOGV("save address = %s success", src->saveAddress.c_str());
//    } else {
//        ELOGE("save address = %s fail", src->saveAddress.c_str());
//    }
    src->srcTextureId = textureObjectIds;
    return textureObjectIds;
}

void TextureLoadUtil::releaseTexture(GLuint textureId) {
    if (textureId != 0) {
        glDeleteTextures(1,&textureId);
    }
}
