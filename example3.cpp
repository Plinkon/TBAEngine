#include <iostream>

#include "engine/play.hpp"

int main() {
    INVENTORY::Inventory playerInv;

    // --- Define Items ---
    INVENTORY::Item itemFlashlightCasing =
        INVENTORY::Item("FlashlightCasing");
    INVENTORY::Item itemPowerCell = INVENTORY::Item("PowerCell");
    INVENTORY::Item itemWorkingFlashlight =
        INVENTORY::Item("WorkingFlashlight");
    INVENTORY::Item itemDataPadCryo = INVENTORY::Item("DataPad_CryoLog");
    INVENTORY::Item itemPrybar = INVENTORY::Item("Prybar");

    // --- Define Nodes ---

    // Part 1: Awakening and Immediate Survival

    // Cryo Bay Area
    NODE::NodePtr nodeCryoBay = NODE::createNode(
        "You awaken with a gasp in a damaged cryo-pod. Emergency alarms "
        "blare intermittently, their red lights casting unsettling shadows. "
        "The air is frigid, and the room is shrouded in oppressive "
        "darkness, save for a faint emergency flicker near what looks like a "
        "control panel. A single door is visible, presumably leading out.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());

    NODE::NodePtr nodeCryoPanelInspect = NODE::createNode(
        "You approach the flickering control panel. It's mostly dead, sparks "
        "occasionally spitting from exposed wiring. Tucked into a small data "
        "slot, you find a data pad. It looks intact.",
        ACTION::TYPE::PICKUP, INVENTORY::Item(),
        INVENTORY::ItemVec({itemDataPadCryo}));

    NODE::NodePtr nodeCryoPodSearch = NODE::createNode(
        "Searching around your cryo-pod in the dim emergency light, your "
        "hand closes on a cold, metallic cylinder. It's a flashlight "
        "casing, but it doesn't turn on. It seems to be missing a power "
        "cell.",
        ACTION::TYPE::PICKUP, INVENTORY::Item(),
        INVENTORY::ItemVec({itemFlashlightCasing}));

    NODE::NodePtr nodeFlashlightCombined = NODE::createNode(
        "You carefully open the flashlight casing and insert the Power Cell. "
        "With a satisfying click, you switch it on. A bright, steady beam "
        "cuts through the darkness! You now have a Working Flashlight.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());

    // Corridor Area
    NODE::NodePtr nodeInitialCorridor = NODE::createNode(
        "You step into a narrow corridor. The air hums with a low, "
        "unsettling thrum, and sparks occasionally fly from a damaged "
        "conduit on the wall. It's very dark, making it hard to see "
        "details. You can vaguely make out a door to the North, another to "
        "the East, and the door to the Cryo Bay is South.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());

    NODE::NodePtr nodeInitialCorridorLit = NODE::createNode(
        "With your flashlight illuminating the way, the narrow corridor is "
        "much clearer. Sparks still fly from the damaged conduit. Doors are "
        "clearly visible: North (labeled 'Storage S-103'), East (labeled "
        "'Life Support Access'), and South (leading back to the Cryo Bay).",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());

    // Storage Room Area
    NODE::NodePtr nodeStorageRoom = NODE::createNode(
        "You push open the North door from the corridor. It's a small "
        "storage room, utterly black. You can hear the rustle of displaced "
        "items as you step in, and the air is stale.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());

    NODE::NodePtr nodeStorageRoomSearchDark = NODE::createNode(
        "You blindly feel around on shelves and the floor in the pitch-dark "
        "storage room. Your hands brush against something long and metallic, "
        "and a small, dense object. You've found a Prybar and a Power Cell!",
        ACTION::TYPE::PICKUP, INVENTORY::Item(),
        INVENTORY::ItemVec({itemPrybar, itemPowerCell}));

    NODE::NodePtr nodeStorageRoomLit = NODE::createNode(
        "Your flashlight cuts through the gloom of Storage S-103. It's "
        "cluttered with overturned shelves and scattered supplies. A Prybar "
        "lies on the floor next to a discarded Power Cell.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());

    NODE::NodePtr nodeStorageRoomTakeItemsLit = NODE::createNode(
        "You pick up the Prybar and the Power Cell from the floor.",
        ACTION::TYPE::PICKUP, INVENTORY::Item(),
        INVENTORY::ItemVec({itemPrybar, itemPowerCell}));

    // Life Support Annex Area
    NODE::NodePtr nodeLifeSupportAnnex = NODE::createNode(
        "The East door from the corridor groans open into what feels like a "
        "larger room. The air is thick and heavy. Warning lights flash "
        "rhythmically from a large console, casting eerie shadows: 'Life "
        "Support Systems - CRITICAL FAILURE'. It's dangerously dark "
        "otherwise.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());

    NODE::NodePtr nodeLifeSupportAnnexConsoleDark = NODE::createNode(
        "It's too dark to make out any useful details on the console, beyond "
        "the insistent flashing of critical warning lights.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());

    NODE::NodePtr nodeLifeSupportAnnexLit = NODE::createNode(
        "Your flashlight beam sweeps across the Life Support Annex. The large "
        "console is indeed covered in warning indicators for oxygen and "
        "atmosphere. Various pipes and machinery line the walls, some "
        "visibly damaged and sparking. The air is stale and carries a faint "
        "metallic tang.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());

    NODE::NodePtr nodeLifeSupportAnnexConsoleLit = NODE::createNode(
        "The console confirms critical failures in oxygen generation and "
        "atmospheric scrubbing. Main system override appears to be offline. A "
        "service panel on the console is slightly ajar and looks like it "
        "could be pried open.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());
    
    NODE::NodePtr nodePryPanelLifeSupport = NODE::createNode(
        "You wedge the prybar into the gap and heave. With a screech of "
        "metal, the panel pops open, revealing a manual bypass lever and "
        "some fried circuits. For now, at least the panel is open.",
        ACTION::TYPE::NONE, INVENTORY::Item(), INVENTORY::ItemVec());


    // --- Define Options ---

    // Cryo Bay Options
    NODE::Option optInspectPanel = NODE::Option(
        "Examine the flickering control panel", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optSearchPod = NODE::Option(
        "Search near your cryo-pod", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optCombineFlashlight = NODE::Option(
        "Attempt to insert Power Cell into Flashlight Casing",
        ACTION::TYPE::USE, INVENTORY::ItemVec({itemWorkingFlashlight}),
        itemPowerCell); // Requires PowerCell, gives WorkingFlashlight
    NODE::Option optExitToCorridor = NODE::Option(
        "Exit to the corridor", ACTION::TYPE::NONE, INVENTORY::ItemVec(),
        INVENTORY::Item());
    NODE::Option optReturnToCryoBay = NODE::Option(
        "Return to the main Cryo Bay area", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());

    // Corridor Options
    NODE::Option optGoNorthStorage = NODE::Option(
        "Go North (towards Storage S-103)", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optGoEastLifeSupport = NODE::Option(
        "Go East (towards Life Support Access)", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optGoSouthCryoBay = NODE::Option(
        "Go South (back to Cryo Bay)", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optUseWorkingFlashlight = NODE::Option(
        "Use Working Flashlight", ACTION::TYPE::USE, INVENTORY::ItemVec(),
        itemWorkingFlashlight); // Requires WorkingFlashlight
    NODE::Option optTurnOffFlashlightCorridor = NODE::Option(
        "Turn off Flashlight", ACTION::TYPE::NONE, INVENTORY::ItemVec(),
        INVENTORY::Item()); // No item use, just changes state

    // Storage Room Options
    NODE::Option optSearchStorageDarkly = NODE::Option(
        "Fumble around for supplies in the dark", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optPickUpStorageItemsLit = NODE::Option(
        "Pick up the Prybar and Power Cell", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optReturnToCorridor = NODE::Option(
        "Return to the Corridor", ACTION::TYPE::NONE, INVENTORY::ItemVec(),
        INVENTORY::Item());
    NODE::Option optReturnToLitCorridor = NODE::Option(
        "Return to the Lit Corridor", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optTurnOffFlashlightStorage = NODE::Option(
        "Turn off Flashlight", ACTION::TYPE::NONE, INVENTORY::ItemVec(),
        INVENTORY::Item());
    NODE::Option optBackToStorageDark = NODE::Option(
        "Step back from your search", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
     NODE::Option optBackToStorageLit = NODE::Option(
        "Step back", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());


    // Life Support Annex Options
    NODE::Option optExamineConsoleDark = NODE::Option(
        "Try to examine the console in the dark", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optExamineConsoleLit = NODE::Option(
        "Examine the console with your flashlight", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());
    NODE::Option optTurnOffFlashlightLifeSupport = NODE::Option(
        "Turn off Flashlight", ACTION::TYPE::NONE, INVENTORY::ItemVec(),
        INVENTORY::Item());
    NODE::Option optUsePrybarOnPanel = NODE::Option(
        "Use Prybar on the service panel", ACTION::TYPE::USE,
        INVENTORY::ItemVec(), itemPrybar); // Requires Prybar
    NODE::Option optBackToLifeSupportLit = NODE::Option(
        "Step back from the console", ACTION::TYPE::NONE,
        INVENTORY::ItemVec(), INVENTORY::Item());


    // --- Connect Nodes using Options ---

    // Cryo Bay Connections
    nodeCryoBay->addNextNode(nodeCryoPanelInspect, optInspectPanel);
    nodeCryoBay->addNextNode(nodeCryoPodSearch, optSearchPod);
    nodeCryoBay->addNextNode(nodeFlashlightCombined, optCombineFlashlight);
    nodeCryoBay->addNextNode(nodeInitialCorridor, optExitToCorridor);

    nodeCryoPanelInspect->addNextNode(nodeCryoBay, optReturnToCryoBay);
    nodeCryoPodSearch->addNextNode(nodeCryoBay, optReturnToCryoBay);
    nodeFlashlightCombined->addNextNode(nodeCryoBay, optReturnToCryoBay);

    // Initial Corridor Connections
    nodeInitialCorridor->addNextNode(nodeStorageRoom, optGoNorthStorage);
    nodeInitialCorridor->addNextNode(nodeLifeSupportAnnex,
                                     optGoEastLifeSupport);
    nodeInitialCorridor->addNextNode(nodeCryoBay, optGoSouthCryoBay);
    nodeInitialCorridor->addNextNode(nodeInitialCorridorLit,
                                     optUseWorkingFlashlight);

    // Initial Corridor Lit Connections
    nodeInitialCorridorLit->addNextNode(nodeStorageRoomLit, optGoNorthStorage);
    nodeInitialCorridorLit->addNextNode(nodeLifeSupportAnnexLit,
                                        optGoEastLifeSupport);
    nodeInitialCorridorLit->addNextNode(nodeCryoBay, optGoSouthCryoBay);
    nodeInitialCorridorLit->addNextNode(nodeInitialCorridor,
                                        optTurnOffFlashlightCorridor);

    // Storage Room (Dark) Connections
    nodeStorageRoom->addNextNode(nodeStorageRoomSearchDark,
                                 optSearchStorageDarkly);
    nodeStorageRoom->addNextNode(nodeStorageRoomLit, optUseWorkingFlashlight);
    nodeStorageRoom->addNextNode(nodeInitialCorridor, optReturnToCorridor);
    nodeStorageRoomSearchDark->addNextNode(nodeStorageRoom, optBackToStorageDark);


    // Storage Room (Lit) Connections
    nodeStorageRoomLit->addNextNode(nodeStorageRoomTakeItemsLit,
                                    optPickUpStorageItemsLit);
    nodeStorageRoomLit->addNextNode(nodeInitialCorridorLit,
                                    optReturnToLitCorridor);
    nodeStorageRoomLit->addNextNode(nodeStorageRoom,
                                    optTurnOffFlashlightStorage);
    nodeStorageRoomTakeItemsLit->addNextNode(nodeStorageRoomLit, optBackToStorageLit);


    // Life Support Annex (Dark) Connections
    nodeLifeSupportAnnex->addNextNode(nodeLifeSupportAnnexConsoleDark,
                                      optExamineConsoleDark);
    nodeLifeSupportAnnex->addNextNode(nodeLifeSupportAnnexLit,
                                      optUseWorkingFlashlight);
    nodeLifeSupportAnnex->addNextNode(nodeInitialCorridor,
                                      optReturnToCorridor);
    nodeLifeSupportAnnexConsoleDark->addNextNode(nodeLifeSupportAnnex, optReturnToCorridor); // Simplified back

    // Life Support Annex (Lit) Connections
    nodeLifeSupportAnnexLit->addNextNode(nodeLifeSupportAnnexConsoleLit,
                                         optExamineConsoleLit);
    nodeLifeSupportAnnexLit->addNextNode(nodeInitialCorridorLit,
                                         optReturnToLitCorridor);
    nodeLifeSupportAnnexLit->addNextNode(nodeLifeSupportAnnex,
                                         optTurnOffFlashlightLifeSupport);
    
    nodeLifeSupportAnnexConsoleLit->addNextNode(nodePryPanelLifeSupport, optUsePrybarOnPanel);
    nodeLifeSupportAnnexConsoleLit->addNextNode(nodeLifeSupportAnnexLit, optBackToLifeSupportLit);
    nodePryPanelLifeSupport->addNextNode(nodeLifeSupportAnnexLit, optBackToLifeSupportLit);


    // --- Start the game ---
    GAME::Game game("Echoes of the Void");
    game.Init();
    game.Run(nodeCryoBay, playerInv);

    return 0;
}