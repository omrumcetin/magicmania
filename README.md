The game consist of multiple elements : <br>
I have created widget to able to activate lever in game. Here is the overview of game.
Lever and door interaction can be done by any player.

![20220614_215142](https://user-images.githubusercontent.com/22418431/173666769-852bae8a-39c4-4367-a32e-2f779c246f7c.jpg)

Here is the interaction component widget
![20220614_215244](https://user-images.githubusercontent.com/22418431/173666920-3238a49d-1e67-4790-aefb-9b401b53e7c9.jpg)

Interaction component ray tracing used to able to achieve interactions through player. Can for sure use overlap but since its TPS, it will make much more sense to make it as with raytracing.<br>
This component can be added any kind of actor that is required to be interacted by player, can create individual interact and focus actions, because they are all exposed in blueprints.

I have tried to use multiple variations in code including advancing debugging console commands.
```C++
static bool InteractionDebug = false;
FAutoConsoleVariableRef CVARInteractionDebug(
    TEXT("Interaction.Debug"),
    InteractionDebug,
    TEXT("Draw debug line for interaction casts"),
    ECVF_Cheat);
```

Create and Join session made roughly, you can directly host and join from main menu. Additionally since PIE little bit buggy in unreal network, I have attached a bat file to demonstrate multiplayer in game clearly.
<br>
Please use `GameClient.bat` to open game like client.<br>
I wanted to create much more detailed showcase, but I dont want to deeply dive since this is not complete project, so I keep it simple.
<br>
PS:<br>
Current game mihgt have some couple of bugs, but this assessment aim has to be testing how I'm using the unreal engine with the multiple aspects.
<br>
