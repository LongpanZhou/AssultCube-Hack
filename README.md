# AssaultCube Hack
This project is dedicated to educational purposes, aimed at providing insights into C++ programming and the techniques involved in game hacking. It serves as a learning tool for developers interested in understanding how to interact with game memory and manipulate game behavior, focusing on two primary approaches: external and internal hacks.

## Branches Overview:
There are two main branches of the project: External Hack and Internal Hack.

### [External Hack](https://github.com/LongpanZhou/AssultCube-Hack/tree/external):

The external hacking method operates outside of the game's process. It uses an overlay to render visual elements, such as ESP (Extra Sensory Perception) or other custom features, on top of the game window.
This method interacts with the game's memory through external tools or custom scripts that read and write to the memory locations (often referred to as memread and memwrite). By doing so, it can alter game variables such as health, ammo, or player position without directly modifying the game's codebase.
The advantage of external hacks is that they are generally safer to implement since they don't require direct injection into the game process. However, they can be slower or less efficient compared to internal hacks due to the overhead of reading and writing memory from outside the game's process.

### [Internal Hack](https://github.com/LongpanZhou/AssultCube-Hack/tree/internal):

The internal hacking technique, in contrast, works by injecting a Dynamic Link Library (DLL) directly into the game's memory space. This allows the hack to run within the game process itself.
Once injected, the internal hack can intercept the game’s rendering pipeline by hooking functions like SwapBuffers, enabling it to draw visuals directly within the game and manipulate game objects more efficiently.
This method allows direct access to the game's memory, making it more powerful and versatile. Since the hack operates inside the game, it can manipulate game functions and variables with lower latency and greater control. However, internal hacks are often riskier, as they are more easily detected by anti-cheat systems.
