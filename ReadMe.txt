!!Run on Debug or Release x64!!

--Threading--
I've implemented async model loader (just for ply now)
using non-blocking approach, storing meshdata in game object,
giving object simplified meshes (for now just wireframe boxes)
and when models are loaded - changing game object meshes to high res versions
-----------



--Animation---
Instead of rolling I'm also made "strafe with casting spell" animation
since I'm going to use it for Game Jam

There is also "pushing" animation 
I've made all spheres ridiculously heavy, so try to push them!

Also I left debugging lines for ray cast

Controls:

-----------Character------------


Third Person:
WASD + Mouse - Move
S/D - Strafe
Hold Shift + W - run
Hold Mouse left button - Prepare to cast spell

9/8 - change character


-----------Character------------

-----------other-------------
Enter - switch FLY/THIRD PERSON camera
T - switch between objects
ctrl + WASD - apply velocity in camera looking direction

Additional controls:
Arrows Up/Down change gravity
G - lift All rigidbodies up
-----------other-------------

you can cofigure objects in GameEngine/scenes/physics.json



