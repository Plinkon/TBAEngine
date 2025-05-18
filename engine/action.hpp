#ifndef ACTION_HPP
#define ACTION_HPP

#include <iostream>
#include <string>
#include <vector>

// Include the full definition of Inventory and Item here
#include "inventory.hpp"

namespace ACTION {
    enum TYPE {
        PICKUP,
        USE,
        NONE
    };

    class Action {
    public:
        TYPE type;

        Action(TYPE actionType) : type(actionType) {}
        Action() = default;
        ~Action() = default;

        // Implementation of executeAction directly in the header
        bool executeAction(INVENTORY::Inventory& inv, const INVENTORY::ItemVec& pickupItems, const INVENTORY::Item& useItem) {
            if (this->type == PICKUP) {
                if (!pickupItems.empty()) {
                    inv.addItems(pickupItems);
                    if (pickupItems.size() == 1) {
                        std::cout << "\n[INFO] You picked up a " << pickupItems[0].name << "!\n";
                    } else {
                        std::cout << "\nYou picked up:\n";
                        for (const auto& item : pickupItems) {
                            std::cout << "- " << item.name << "\n";
                        }
                        std::cout << "\n";
                    }
                    return true;
                }
                return false; // No items to pick up
            } else if (this->type == USE) {
                if (inv.hasItem(useItem.name)) {
                    inv.removeItem(useItem.name);
                    std::cout << "\n[INFO] You used a " << useItem.name << "!\n";
                    return true;
                } else {
                    std::cout << "\n[INFO] You don't have a " << useItem.name << " to use.\n";
                    return false;
                }
            }

            return false; // NONE action or unknown type
        }
    };
}

#endif
