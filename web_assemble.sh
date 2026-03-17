mkdir -p ./bin-web
emcc -o ./bin-web/index.html \
  ./game/code/*.cpp \
  ./game/code/core/*.cpp \
  ./game/code/utils/*.cpp \
-Os -Wall ./web/libraylib_web.a \
  -I./game/external/raylib/src \
  -I./game/code \
  -I./game/code/core \
  -I./game/code/render \
  -I./game/code/utils \
-DPLATFORM_WEB -s GL_ENABLE_GET_PROC_ADDRESS -s ALLOW_MEMORY_GROWTH=1 -s USE_GLFW=3 -std=c++23 --shell-file ./web/shell.html --preload-file ./game/resources@resources
echo "done."