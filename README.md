The game consist of multiple elements : 
Interaction component ray tracing used to able to achieve interactions through player. Can for sure use overlap but since its TPS, it will make much more sense to make it as with raytracing.<br>
I have created widget to able to activate lever in game. Here is the overview of game.

![20220614_215142](https://user-images.githubusercontent.com/22418431/173666769-852bae8a-39c4-4367-a32e-2f779c246f7c.jpg)

Here is the interaction component widget
![20220614_215244](https://user-images.githubusercontent.com/22418431/173666920-3238a49d-1e67-4790-aefb-9b401b53e7c9.jpg)

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
