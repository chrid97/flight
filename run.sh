gcc main.c -I./lib/raylib/include -L./lib/raylib/lib -Wl,-rpath=./lib/raylib/lib \
  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o flight.exe
./flight.exe
