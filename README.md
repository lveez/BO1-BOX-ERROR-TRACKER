# BO1 Box Error Tracker

## Using The Program

You can download the tracker from the releases section of github, or you can compile it yourself from the source. Just start the tracker at any point throughout your game and it will show how close to the error you are. Due to the nature of box error the value isn't constant so it will jump around quite a lot. This can be used to see how close you are to box error, or to test other things in the game to see if they effect the error. One issue is that because the value is so volatile you still have to test for an hour or two to see if the value is increasing, I've found it's easiest to test with ai_disablespawn set to 1 as this stops the value jumping around as much.

At the minute this only works for the steam version of BO1, if someone wants to extend it for bgamer they can do so.

## What Causes Box Error

It's caused by the maximum number of server script variables being exceeded. The tracker works by reading a value that once it hits 24576 will cause the game to error. I'm not sure yet exactly what the value is, but it represents the current number of server script variables. Some script functions leak variables in BO1, and this is what causes the error. I'm not sure yet if this is because of the VM that runs the script or the actual scripts themselves.

## Can This Crash My Game

I don't want to say it's impossible for it to crash your game, but it's extremely unlikely since all it does is read the processes memory.

## Todo

- Track the creation of variables and see which ones are not being destroyed
