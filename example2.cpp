#include <iostream>
#include "engine/play.hpp"

int main() {
    INVENTORY::Inventory inv;

    // Define Items
    INVENTORY::Item plank = INVENTORY::Item("Plank");
    INVENTORY::Item axe = INVENTORY::Item("Axe"); // Optional item, not strictly needed to win

    // Define Nodes
    // Node_Start: The beginning, a fork in the path.
   NODE::NodePtr nodeStart = NODE::createNode(
        "You find yourself at the start of two diverging paths in a dense forest. "
        "The sun is beginning to set, casting long shadows. Which path do you take?",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec()
    );

    // Node_Clearing: Leads from the left path, contains the essential item.
    NODE::NodePtr nodeClearing = NODE::createNode(
        "You follow the left path and arrive in a small clearing. "
        "In the center, near a mossy, fallen log, you spot something useful.",
        ACTION::TYPE::PICKUP, INVENTORY::Item(), INVENTORY::ItemVec({plank}) // Automatically pick up the Plank on entry
    );

    // Node_Riverbank: Leads from the clearing, obstacle requiring an item.
    NODE::NodePtr nodeRiverbank = NODE::createNode(
        "You head towards the sound of water and reach the bank of a wide, fast-flowing river. "
        "There's no bridge in sight, and the current looks too strong to swim.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec()
    );

    // Node_DeadEnd: Leads from the right path, optional item.
    NODE::NodePtr nodeDeadEnd = NODE::createNode(
        "You follow the right path, but it ends abruptly at a rocky cliff face. "
        "There's nowhere else to go here, but you notice something glinting on the ground.",
        ACTION::TYPE::PICKUP, INVENTORY::Item(), INVENTORY::ItemVec({axe}) // Automatically pick up the Axe on entry
    );

    // Node_Home: The successful end state.
    NODE::NodePtr nodeHome = NODE::createNode(
        "Using the plank, you carefully make your way across the treacherous river, narrowly avoiding the swift current. "
        "As you climb the opposite bank, the trees thin out, and you see the welcoming lights of houses in the distance. "
        "You've made it! You are safe. The journey is over.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec() // End node, no further options
    );


    // Define Options
    // Options from Node_Start
    NODE::Option optGoLeft = NODE::Option("Take the left path", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optGoRight = NODE::Option("Take the right path", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());

    // Options from Node_Clearing
    NODE::Option optGoToRiver = NODE::Option("Head towards the sound of water (to the Riverbank)", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optBackToFork = NODE::Option("Go back to the fork in the path", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());

    // Options from Node_Riverbank
    NODE::Option optBackToClearing = NODE::Option("Go back towards the clearing", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());
    // This option requires the 'plank' item to be in the inventory to be selectable/successful.
    NODE::Option optAttemptCross = NODE::Option("Attempt to cross the river (requires Plank)", ACTION::TYPE::USE, INVENTORY::ItemVec(), plank);

    // Options from Node_DeadEnd
    NODE::Option optBackToForkFromDeadEnd = NODE::Option("Go back to the fork in the path", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());


    // Connect Nodes using Options
    // From Start
    nodeStart->addNextNode(nodeClearing, optGoLeft);
    nodeStart->addNextNode(nodeDeadEnd, optGoRight);

    // From Clearing
    nodeClearing->addNextNode(nodeRiverbank, optGoToRiver);
    nodeClearing->addNextNode(nodeStart, optBackToFork); // Can go back to the start/fork

    // From Riverbank
    nodeRiverbank->addNextNode(nodeClearing, optBackToClearing); // Can go back to clearing
    nodeRiverbank->addNextNode(nodeHome, optAttemptCross); // Requires Plank to reach Home

    // From Dead End
    nodeDeadEnd->addNextNode(nodeStart, optBackToForkFromDeadEnd); // Must go back from dead end

    // Node_Home has no outgoing connections, ending the story when reached.

    // Start the game
    GAME::Game game("The Forest");
    game.Init();
    game.Run(nodeStart, inv);

    return 0;
}