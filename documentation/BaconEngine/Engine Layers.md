# Engine Layers

The engine contains some built in layers.

They're there to help with interactions between engine and client code.

## What are they?

### CurrentUIUpdater

This layer only uses `BE_Layer_OnEvent` to be able to detect what is the currently selected UI element.

Not only will this help make stuff like input boxes easier to use,
it will also make it faster, since it wouldn't have to constantly look for the uiWindow 
when you do an action.

This is split from the other layers to make development easier.

### UIManager

This layer uses both `BE_Layer_OnEvent` and `BE_Layer_OnUpdate` to both render, and
manage the actions for every uiWindow.

It uses `BE_Layer_OnUpdate` to render the uiWindow information on screen.

It was originally supposed to be rendered on UI uiWindow creation, but I deemed it would make it harder to add stuff like UI elements.

It then uses 'BE_Layer_OnEvent' to detect stuff like mouse clicks. This allows the uiWindow to be interactable.