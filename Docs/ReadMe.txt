/tarship - Qixuan(Kayson) Liang
/~~~~~~~~~~~~~~~~~~~~~~~~~~~
A simple 2D space starship shooting game written by my custom C++ (C33SD) game engine.


SOURCE CODE PATH

You can check all my codes in the folder \Personal_Build\SD\Starship\Code\Game

When you open the whole solution, you can use the F5 to run my project.

------------------------------------------------------------------------------------------------------------
For SD1_A4 Vesion(Lastest released at 10/10/2023 ):

CONTROLS

- Turn: S or F / Left joystick
- Move: E / Left joystick
- Fire: Space bar / Right trigger
- Pause: P 
- Exit: ESC

KNOWN ISSUES

- The buffs from Pickups are here to stay and won’t disappear anytime soon. (FIXED AT 10/10/2023)
- Equipped weapons are not displayed correctly on the ship. (FIXED AT 10/09/2023)
- If a missile is finding its target and then target dead, the missile will suddenly move to a new position.

DEEP LEARNING

At the beginning of my programming for the math part of the whole engine, I enjoy the feeling of solving "math puzzles" by myself. Just like I prefer to lock myself in my dark bedroom to program, I have an excessive love for solving problems alone because it makes me feel a sense of accomplishment and the feeling of "I am so smart" when solving problems. But when I encounter a more difficult or unfamiliar problem, I often spend a lot of time thinking and "convincing myself." If I didn't figure it out in the end, I would become too frustrated to continue. For example when I first wrote the "Angle utilities" function section, I couldn't figure out exactly what "GetTurnedTowardDegrees" meant and how to implement it. As I began programming the AudioSystem and the left parts of the assignment, I try to follow the "DONT THINK MORE THAN 15 MINS" rules, and my solution is to find someone's help or just discuss with them and get the ideas which I did't consider before. And at the same time, I will choose to implement the parts that I currently have no ideas for last, so as to ensure that I can complete the engine and various parts of the game efficiently. When I do this, I gradually reduce my anxiety and fear of facing problems that I have no ideas about, and I can reserve more time before the deadline to think about these problems without burden. Although I still think for a long time sometimes, the frequency of this has become much lower than before. I think I need to stick to this approach more closely especially when I have more assignments and programming tasks to complete on time.

Another thing that impressed me deeply about "Deep Learning" is I need to try my best to make my code readable and easy to understand rather than seem to increase efficiency or be more "smart". In my previous programming experience and the programming process of the Assignment 1, I did not have a unified and easy-to-understand naming convention, and sometimes I would use some abbreviations like "i" or "a", especially when I use for loop, the frequency of "i" will become very high. So when my classmates and sometimes even myself feel struggle to read my code. So when I heard Squirrel's discussion about this part in class, I decided to make this a strict requirement in every line of my code thereafter, that is, make every function and variable simple to get its meaning only by reading its names instead of comments. The most obvious change is that I replaced "i" in all indexes in the for loop with very clear naming rules such as "asteroidIndex". I also structure my code to be clearer and easier to understand, not just to make it look "shorter" or "smarter", since this is the performance of "junior programmers":(. So when I share the code with other people in the company and studio, I don't need to write long comments or explain it to them by myself, and they can easily help me check out bugs in the code.

-------------------------------------------------------------------------------------------------------------

For SD1_A3 Vesion(Lastest released at 09/27/2023 ), there are all new features:

1.New enemies: Beetles, Wasps
2.Debris for all entities collision and dead effects
3.Xbox controller for input, now you can use your controller to move the space ship
4.UIElements added: Player now has 4 lives and the live icon will be listed on the left top corner of the screen
5.Attract mode of the game, by now you need to firstly go to the attract mode and then enter the game
6.Inputsysterm added, now all the inputs will be handled by the inputsystem (same as the renderer)
-------------------------------------------------------------------------------------------------------------

