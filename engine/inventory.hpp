#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <iostream> // Added for potential future debug printing

namespace INVENTORY {
    // Items

    class Item {
    public:
        std::string name;
        Item() = default;
        Item(std::string name) : name(name) {} // Use member initializer list
        ~Item() = default;

        // Added for easier comparison
        bool operator==(const Item& other) const {
            return name == other.name;
        }
    };

    // typedefs
    typedef std::vector<Item> ItemVec;

    // inventory

    struct Inventory {
        ItemVec items;

        bool hasItem(const std::string& itemName) const { // Made const
            for (const auto& item : items) { // Use range-based for loop
                if (item.name == itemName) return true;
            }
            return false;
        }

        bool hasItems(const ItemVec& itemsToCheck) const { // Made const, renamed parameter
            // This checks if *any* item from itemsToCheck is in the inventory.
            // If you need to check if *all* items are present, the logic needs to change.
            for (const auto& itemToCheck : itemsToCheck) {
                if (hasItem(itemToCheck.name)) { // Reuse hasItem logic
                    return true;
                }
            }
            return false;
        }

        void addItem(const Item& item) { this->items.push_back(item); } // Made const reference

        void addItems(const ItemVec& items) { // Made const reference
            for (const auto& item : items) { // Use range-based for loop
                addItem(item);
            }
        }

        void removeItem(const std::string& itemName) { // Made const reference
            // find the item by its name
            auto it = std::find_if(items.begin(), items.end(), [&](const Item& item){
                return item.name == itemName;
            });

            // if the item is found, erase it
            if (it != items.end()) {
                items.erase(it);
            }
        }

        // Optional: Print inventory contents
        void print() const {
            std::cout << "--- INVENTORY ---\n";
            if (items.empty()) {
                std::cout << "Inventory is empty.\n";
            } else {
                for (size_t i = 0; i < items.size(); ++i) {
                    std::cout << i + 1 << ". " << items[i].name << "\n";
                }
            }
            std::cout << "-----------------\n";
        }
    };
}

#endif
