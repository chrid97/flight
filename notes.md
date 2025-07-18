
useful command for turning all of my sprites into an atlas. thanks chatgpt <3
ill have to find a better way to do this. Maybe writing my own packer. This didnt really do a good job packing

- pack better
- should run at buildtime or precompilation for whenever i add new textures
- ill have to convert these files to asperite files first probably for the packing tool.
- this way i have the information for where each sprite lives in that json file aseprite gives you

chris@localhost ~/g/f/assets (main)> find ./ -type f -name '*.png' -print0 | xargs -0 ~/Applications/Aseprite_1.3.14.4-x64.AppImage --batch --sheet atlas.png --sheet-pack > atlas.json
