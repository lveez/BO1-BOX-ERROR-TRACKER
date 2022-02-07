# BO1 Box Error Tracker

## Using The Program

You can download the tracker from the releases section of github, or you can compile it yourself from the source. Just start the tracker at any point throughout your game and it will show how close to the error you are. If you download version 1.0 the value will jump around a lot, version 1.1 instead calculates an average so it's more stable. For testing it's recommended to use BETcli.exe to look at the stack variables, by disabling zombie spawning this value will be very stable and you can easily see if anything you do in game is effecting the error.

At the minute this only works for the steam version of BO1, if someone wants to extend it for bgamer they can do so.

## What Causes Box Error

It's caused by the maximum number of server script variables being exceeded. The tracker works by reading what I think at the minute is the ID of the next free variable location in the game's VM's stack, which is why the value jumps around so much. When this value goes above 24576 it will reset back to 0 and the game will error.

## Can This Crash My Game

I don't want to say it's impossible for it to crash your game, but it's extremely unlikely since all it does is read the processes memory. Nothing is injected and no game code is changed in anyway.

## Todo

- Track the creation of variables and see which ones are not being destroyed
