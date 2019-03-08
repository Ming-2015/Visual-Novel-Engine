# Introduction
This project was originally intended as an exercise to brush up our C++ skills, while also having fun building something. Eventually, it spinned out of control and the code size got gigantic within a short few weeks.

*Visual Novel Engine* is a versatile Visual Novel engine written in C++, and relies heavily on [SFML](https://www.sfml-dev.org/ "SFML") for rendering and audio (the audio counterpart is actually OpenAL, which SFML is using).

# Design
*Visual Novel Engine* is designed to be easily customizable, allowing creators to add all sorts of features or animations, sound effects, etc. with relative ease. 

We basically created a whole new 'scripting language' that allows the script writer to add various commands, such as show character, set background, play music, display text, fadein/fadeout character sprites, etc. easily, and with a lot of control too! There's a lot of animation commands that accepts a Time argument as input, which allows you to time all your animated scenes perfectly.   

For a list of complete and updated commands, open Script Format Reference.xlsx (note that commands marked in Red or Yellow are not fully functional or implemented yet)

# Demo
[Youtube demo](https://www.youtube.com/watch?v=OfRA-XLtyR0): This is a demo video of a work-in-progress version of the program. At the end of the video there's a sample script that you can reference.

![screenshot]

[screenshot]: https://i.imgur.com/I8XI8sq.png

# Setup instructions after cloning
0. Install Visual Studio 2017 (Community edition should work fine) with C++ support

1. Change environment architecture from x64 to x32 on the top

2. Go to Build on the menu and click "Build Solution" (for both Debug and Release preferably)
..* If you have difficulty building the solution because it is missing some library, it is most likely because the SFML library is not linked properly. Go to https://www.sfml-dev.org/tutorials/2.5/start-vc.php and follow all the steps to properly link the library

3. Copy the .dll files from SFML/SFML-2.5.1/bin folder to where your executable (.exe) file is located (usually Debug and Release folders in your project folder after you built)

4. request from [me](mailto:caiyiming2015@gmail.com) or [Ed](mailto:Kentoruya@gmail.com) for the various asset folders (images, sound, resources), or copy those folders from a working game made with our engine

5. copy all the asset folders into where your executables (.exe) files are. (usually Debug and Release folders)

5. Run the program

# Copyrights
We are using the default Github license, which means that we retain all rights to our source code and no one may reproduce, distribute, or create derivative works from our work. Please notify us if you wish to collaborate in the project.

# TODOs
* Log (or history) function, Log button somewhere on the textbox, and possibly voice playback
* make config, save and load states more pretty
* add 'pages' to the save/load states, so that users can have more data to choose from
* implement the various config elements in the config page, and make sure they are responsive
* add Loop related commands (see Script Command Reference.xlsx which will be complicated)
* add video playback capabilities to the game
