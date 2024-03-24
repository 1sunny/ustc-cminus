add_library(cminus_io io.c)

install(
    TARGETS cminus_io
    ARCHIVE DESTINATION lib #ARCHIVE 用于安装静态库文件
)
