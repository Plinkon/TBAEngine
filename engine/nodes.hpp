#ifndef NODES_HPP
#define NODES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Include the full definition of Action and Inventory here
#include "action.hpp"
#include "inventory.hpp"

namespace NODE {
    // some stuff to make code easier to read
    class Node;

    typedef std::shared_ptr<Node> NodePtr;
    template<typename... Args>
    NodePtr createNode(Args&&... args) {
        return std::make_shared<Node>(std::forward<Args>(args)...);
    }

    // Nodes/progression

    class Option {
    public:
        std::string text;
        ACTION::Action useAction = ACTION::Action(ACTION::TYPE::NONE);
        INVENTORY::ItemVec pickupItems;
        INVENTORY::Item useItem; // Represents the item to be used for the USE action

        // Use member initializer list
        Option(std::string optionText, ACTION::Action useAction, INVENTORY::ItemVec pickupItems, INVENTORY::Item useItem)
            : text(optionText), useAction(useAction), pickupItems(pickupItems), useItem(useItem) {}
        ~Option() = default;
    };

    class Node {
    public:
        std::string text;
        std::vector<Option> options;
        std::vector<NodePtr> nextNodes;

        ACTION::Action onEnterAction = ACTION::Action(ACTION::TYPE::NONE);
        INVENTORY::Item onEnterUseItem; // Item to be used for the ON_ENTER_USE action
        INVENTORY::ItemVec onEnterPickupItems; // Items to be picked up for the ON_ENTER_PICKUP action

        // Use member initializer list
        Node(std::string text, ACTION::Action onEnterAction, INVENTORY::Item onEnterUseItem, INVENTORY::ItemVec onEnterPickupItems)
            : text(text), onEnterAction(onEnterAction), onEnterUseItem(onEnterUseItem), onEnterPickupItems(onEnterPickupItems) {}
        ~Node() = default;

        void addNextNode(NodePtr nextNode, Option option) {
            this->nextNodes.push_back(nextNode);
            this->options.push_back(option);
        }

        bool isEndNode() const { // Made const
            return nextNodes.empty();
        }
    };
}

#endif
