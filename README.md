# Introduction
This project was originally intended as an exercise to brush up our C++ skills, while also having fun building something. Eventually, it spinned out of control and the code size got gigantic within a short few weeks.

*Visual Novel Engine* is a versatile Visual Novel engine written in C++, and relies heavily on (https://www.sfml-dev.org/ "SFML") for rendering and audio (the audio counterpart is actually OpenAL, which SFML is using).

# Design
*Visual Novel Engine* is designed to be easily customizable, allowing creators to add all sorts of features or animations, sound effects, etc. with relative ease. 

We basically created a whole new 'scripting language' that allows the script writer to add various commands, such as show character, set background, play music, display text, fadein/fadeout character sprites, etc. easily, and with a lot of control too! There's a lot of animation commands that accepts a Time argument as input, which allows you to time all your animated scenes perfectly.   

# Setup instructions after cloning
0. Install Visual Studio 2017 (Community edition should work fine) with C++ support

1. Change environment architecture from x64 to x32 on the top

2. Go to Build on the menu and click "Build Solution" (for both Debug and Release preferably)
..* If you have difficulty building the solution because it is missing some library, it is most likely because the SFML library is not linked properly. Go to https://www.sfml-dev.org/tutorials/2.5/start-vc.php and follow all the steps to properly link the library

3. Copy the .dll files from SFML/SFML-2.5.1/bin folder to where your executable .exe file is located (usually Debug and Release folder in your project folder after you built)

4. request from (mailto:caiyiming2015@gmail.com "me") or (mailto:Kentoruya@gmail.com "Ed") for the various asset folders (images, sound, resources), or copy those folders from a working game made with our engine

5. Run the program