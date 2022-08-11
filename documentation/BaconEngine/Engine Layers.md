# Engine Layers

The engine contains some built in layers.

They're there to help with interactions between engine and client code.

## What are they?

### CurrentUIUpdater

This layer only uses `LayerOnEvent` to be able to detect what is the currently selected UI element.

Not only will this help make stuff like input boxes easier to use,
it will also make it faster, since it wouldn't have to constantly look for the window 
when you do an action.

This is split from the other layers to make development easier.

### UIManager

This layer uses both `LayerOnEvent` and `LayerOnUpdate` to both render, and
manage the actions for every window.

It uses `LayerOnUpdate` to render the window information on screen.

It was originally supposed to be rendered on UI window creation, but I deemed it would make it harder to add stuff like UI elements.

It then uses 'LayerOnEvent' to detect stuff like mouse clicks. This allows the window to be interactable.