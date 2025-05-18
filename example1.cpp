#include <iostream>
#include "engine/play.hpp"

int main() {
    INVENTORY::Inventory inv;

    NODE::NodePtr Begining = NODE::createNode("Welcome to the begining, you can walk in any direction", ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());
    NODE::NodePtr walkLeft = NODE::createNode("You walked left, now there is a dead end and the game is over.", ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());
    NODE::NodePtr walkRight = NODE::createNode("You walked right, and there is a path. Do you go down it or go back?", ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());
    NODE::NodePtr downPath = NODE::createNode("You went down the path, and there is a treasure chest! Do you open it or not?", ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());
    NODE::NodePtr openChest = NODE::createNode("You opened the chest and there was a cube inside!", ACTION::TYPE::PICKUP, INVENTORY::Item(), INVENTORY::ItemVec({ INVENTORY::Item("CUBE") }));

    NODE::Option goDownPathOption = NODE::Option("Go down the path", ACTION::TYPE::PICKUP, INVENTORY::ItemVec({ INVENTORY::Item("Key"), INVENTORY::Item("Mushroom") }), INVENTORY::Item());
    NODE::Option goBackOption = NODE::Option("Go back", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option openChestOption = NODE::Option("Open the chest", ACTION::TYPE::USE, INVENTORY::ItemVec(), INVENTORY::Item("Key"));
    NODE::Option walkRightOption = NODE::Option("Walk right", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option walkLeftOption = NODE::Option("Walk left", ACTION::TYPE::NONE, INVENTORY::ItemVec(), INVENTORY::Item());

    walkRight->addNextNode(downPath, goDownPathOption);
    walkRight->addNextNode(Begining, goBackOption);
    downPath->addNextNode(openChest, openChestOption);
    Begining->addNextNode(walkRight, walkRightOption);
    Begining->addNextNode(walkLeft, walkLeftOption);

    GAME::Game game("TEST GAME");
    game.Init();
    game.Run(Begining, inv);

    return 0;
}