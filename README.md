### Game Engine Usage Dictionary

This guide outlines the core functions and structures available for creating games with your engine. (probably kinda outdated i forgot)

---

#### `INVENTORY::Item`

Represents an item that the player can possess.

*   **Creation:**
    ```c++
    INVENTORY::Item myItem = INVENTORY::Item("Item Name");
    ```
*   **Parameters:**
    *   `std::string name`: The name of the item. This is primarily used for identification within the inventory and potentially for display to the player.

---

#### `ACTION::TYPE`

An enumeration defining different types of actions that can occur.

*   **Available Types:**
    *   `ACTION::TYPE::NONE`: Indicates no specific action is associated.
    *   `ACTION::TYPE::PICKUP`: Used to signify that an item or items are being picked up (primarily used with `onEnterPickupItems` in `createNode`).
    *   `ACTION::TYPE::USE`: Used to signify that an item is being used (primarily used with `useAction` and `useItem` in `NODE::Option`).

---

#### `ACTION::Action`

Represents an action associated with a node or an option.

*   **Creation:**
    ```c++
    ACTION::Action myAction = ACTION::Action(ACTION::TYPE::SOME_TYPE);
    ```
*   **Parameters:**
    *   `ACTION::TYPE actionType`: The type of action this object represents.

---

#### `INVENTORY::ItemVec`

A type alias for a `std::vector` of `INVENTORY::Item` objects. Useful for representing collections of items, such as those picked up.

*   **Usage:**
    ```c++
    typedef std::vector<Item> INVENTORY::ItemVec; // Definition (already provided in engine files)

    INVENTORY::ItemVec itemsToPickup;
    itemsToPickup.push_back(plank);
    itemsToPickup.push_back(axe);
    ```

---

#### `NODE::Option`

Represents a choice or option available to the player from a specific node. Selecting an option transitions the player to a different node.

*   **Creation:**
    ```c++
    NODE::Option myOption = NODE::Option(
        "Option Text",         // The text displayed for this option
        ACTION::TYPE::SOME_ACTION, // Action associated with choosing this option
        INVENTORY::ItemVec(),    // Items picked up when choosing this option
        INVENTORY::Item()        // Item required from inventory to choose this option
    );
    ```
*   **Parameters:**
    *   `std::string optionText`: The text displayed to the player as a choice.
    *   `ACTION::Action useAction`: The action associated with selecting this option. Currently, `ACTION::TYPE::USE` is the primary type used here to indicate an item requirement.
    *   `INVENTORY::ItemVec pickupItems`: A vector of items that are added to the player's inventory when this option is chosen.
    *   `INVENTORY::Item useItem`: An `INVENTORY::Item` object representing the item required in the player's inventory to successfully select this option. If the player doesn't have this item, the option might be unavailable or fail (depending on the engine's internal logic). An empty `INVENTORY::Item()` means no item is required.

---

#### `NODE::createNode`

A factory function to create a new game node.

*   **Creation:**
    ```c++
    NODE::NodePtr newNode = NODE::createNode(
        "Node Description",    // Text for this node
        ACTION::TYPE::SOME_ACTION, // Action on entering this node
        INVENTORY::Item(),     // Item used on entering this node
        INVENTORY::ItemVec()     // Items picked up on entering this node
    );
    ```
*   **Parameters:**
    *   `std::string text`: The narrative or descriptive text displayed when the player enters this node.
    *   `ACTION::Action onEnterAction`: An action that occurs automatically when the player enters this node. `ACTION::TYPE::PICKUP` is used here to trigger the automatic pickup of items specified in `onEnterPickupItems`.
    *   `INVENTORY::Item onEnterUseItem`: An item that is automatically used from the player's inventory upon entering this node. This is less commonly used than item pickups on entry in the provided example.
    *   `INVENTORY::ItemVec onEnterPickupItems`: A vector of items that are automatically added to the player's inventory when they enter this node, assuming `onEnterAction` is `ACTION::TYPE::PICKUP`.

*   **Return Value:** Returns a `NODE::NodePtr`, which is a smart pointer to the newly created node.

---

#### `Node::addNextNode` (Member function of a `Node` object)

Connects the current node to another node via a specific option. This defines the possible transitions from this node.

*   **Usage:**
    ```c++
    currentNode->addNextNode(nextNode, option);
    ```
*   **Parameters:**
    *   `NodePtr nextNode`: A smart pointer to the node the player will transition to if they choose the provided `option`.
    *   `Option option`: The `NODE::Option` object that the player selects to move from the `currentNode` to the `nextNode`.

---

#### `GAME::Game`

Represents the overall game instance. Manages the game loop and player state.

*   **Creation:**
    ```c++
    GAME::Game myGame = GAME::Game("Game Title");
    ```
*   **Parameters:**
    *   `std::string title`: The title of the game, likely displayed at the start.

---

#### `Game::Init` (Member function of a `Game` object)

Initializes the game. Based on the example, this might set up internal game state or display introductory messages.

*   **Usage:**
    ```c++
    myGame.Init();
    ```
*   **Parameters:** None.

---

#### `Game::Run` (Member function of a `Game` object)

Starts and manages the main game loop. The game will continue until a node with no outgoing options is reached.

*   **Usage:**
    ```c++
    myGame.Run(startNode, playerInventory);
    ```
*   **Parameters:**
    *   `NodePtr startNode`: The node where the game begins.
    *   `INVENTORY::Inventory playerInventory`: The player's inventory object.

---

#### `INVENTORY::Inventory`

Represents the player's inventory, holding their collected items.

*   **Creation:**
    ```c++
    INVENTORY::Inventory playerInv;
    ```
*   **Usage:** This object is passed to `GAME::Run` and managed internally by the game loop as the player picks up and uses items. You don't typically directly manipulate the inventory from your game definition code.

---

### Usage Example

This example demonstrates how to define items, nodes, options, connect them, and start a simple game.

```c++
#include <iostream>

#include "play.hpp"

int main() {
    INVENTORY::Inventory inv;

    // Define Items
    INVENTORY::Item key = INVENTORY::Item("Key");
    INVENTORY::Item door = INVENTORY::Item("Door"); // Example of an item that represents an obstacle

    // Define Nodes
    NODE::NodePtr nodeStart = NODE::createNode(
        "You are in a dark room. There is a faint light to the north and a heavy wooden door to the east.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec()
    );

    NODE::NodePtr nodeLight = NODE::createNode(
        "You move towards the light and find a small pedestal. On the pedestal rests a rusty key.",
        ACTION::TYPE::PICKUP, INVENTORY::Item(), INVENTORY::ItemVec({key}) // Automatically pick up the key
    );

    NODE::NodePtr nodeDoor = NODE::createNode(
        "You stand before the heavy wooden door. It's locked.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec()
    );

    NODE::NodePtr nodeOutside = NODE::createNode(
        "The key turns in the lock with a click. The door swings open, revealing a bright, sunny day outside!",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec() // End node
    );

    // Define Options
    NODE::Option optGoNorth = NODE::Option("Go north towards the light", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optGoEastDoor = NODE::Option("Examine the heavy wooden door (to the East)", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optBackToStart = NODE::Option("Go back to the dark room", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optUseKeyOnDoor = NODE::Option("Use the Key on the Door", ACTION::TYPE::USE, INVENTORY::ItemVec(), key); // Requires the key

    // Connect Nodes using Options
    // From Start
    nodeStart->addNextNode(nodeLight, optGoNorth);
    nodeStart->addNextNode(nodeDoor, optGoEastDoor);

    // From Light
    nodeLight->addNextNode(nodeStart, optBackToStart);

    // From Door
    nodeDoor->addNextNode(nodeStart, optBackToStart); // Can go back
    nodeDoor->addNextNode(nodeOutside, optUseKeyOnDoor); // Requires the key to proceed

    // Start the game
    GAME::Game game("Escape the Room");
    game.Init();
    game.Run(nodeStart, inv);

    return 0;
}
```
