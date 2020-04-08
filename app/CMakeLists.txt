cmake_minimum_required(VERSION 3.4.1)

# 需要引入我们头文件,以这个配置的目录为基准
include_directories(src/main/jniLibs/include)
include_directories(src/main/jniLibs/include/inc)

# 添加共享库搜索路径
link_directories(${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})

##设置一个变量
#set(libs ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
##-L：引入库查找路径
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${libs}")

# 指定源文件目录
#AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/main/cpp SRC_LIST)
FILE(GLOB SRC_LIST "src/main/cpp/*.cpp")

add_library(
        voiceStudy
        SHARED
        ${SRC_LIST}
)

target_link_libraries(
        # 链接额外的 ffmpeg 的编译
        voiceStudy
        # 编解码(最重要的库)
        avcodec-57
        # 设备信息
        avdevice-57
        # 滤镜特效处理库
        avfilter-6
        # 封装格式处理库
        avformat-57
        # 工具库(大部分库都需要这个库的支持)
        avutil-55
        # 后期处理
        postproc-54
        # 音频采样数据格式转换库L
        swresample-2
        # 视频像素数据格式转换
        swscale-4
        # 变声lib
        fmod
        fmodL
        # 链接 android ndk 自带的一些库
        android
        log)