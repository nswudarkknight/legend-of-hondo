/*
 * StructureDeedImplementation.cpp
 *
 *  Created on: Feb 20, 2012
 *      Author: xyborn
 */

#include "server/zone/objects/tangible/deed/structure/StructureDeed.h"
#include "server/zone/templates/tangible/StructureDeedTemplate.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/manufactureschematic/craftingvalues/CraftingValues.h"
#include "server/zone/templates/tangible/SharedStructureObjectTemplate.h"
#include "server/zone/managers/templates/TemplateManager.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/managers/structure/StructureManager.h"
#include "server/zone/objects/tangible/deed/Deed.h"
#include "server/zone/objects/tangible/deed/structure/StructureDeed.h"
#include "server/zone/ZoneServer.h"



void StructureDeedImplementation::initializeTransientMembers() {
	DeedImplementation::initializeTransientMembers();

	StructureDeedTemplate* templ = dynamic_cast<StructureDeedTemplate*>(templateObject.get());

	if (templ != NULL)
		placeStructureComponent = templ->getStructurePlacementComponent();

	setLoggingName("StructureDeed");
}

void StructureDeedImplementation::fillObjectMenuResponse(ObjectMenuResponse* menuResponse, CreatureObject* player) {
	// Legend of Hondo
	if (player->getParent() == NULL) {	
		menuResponse->addRadialMenuItem(73, 3, "Advanced Placement");
			menuResponse->addRadialMenuItemToRadialID(73, 70, 3, "Show Ruler");  
			menuResponse->addRadialMenuItemToRadialID(73, 71, 3, "House Plop - Use Caution"); 
	}
}

int StructureDeedImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {

	if (selectedID == 20) {

		if (!isASubChildOf(player))
			return 0;

		player->executeObjectControllerAction(STRING_HASHCODE("placestructuremode"), getObjectID(), "");

		return 0;
	}
	
	// Legend of Hondo
	if (selectedID == 73) {
		player->sendSystemMessage("Advanced Placement: To use this feature, face the direction you want the door of your building to face and select the House Plop option. To help with alignment, use the Show Ruler option to better see where you are facing. While the ruler is active, use the mouse wheel to zoom in then turn your character with the mouse. Zoom out again to see where the ruler is pointing. Be careful using this feature, as it will place a structure wherever you happen to be...");
	}
	// Drop the building on top of the player with the door facing the same direction as the player.
	if (selectedID == 71) {
		if (!isASubChildOf(player))
			return 0;
			
		if (player->getParent() != NULL) {
			player->sendSystemMessage("@player_structure:not_inside"); //You can not place a structure while you are inside a building.
			return 0;
		}
		
		int angle = player->getDirectionAngle();
		float	x = player->getPositionX();
		float	y = player->getPositionY();
		
		player->sendSystemMessage("Building constructed at your location with door facing the way you were facing...");
		
		uint64 deedID;
		
		deedID = getObjectID();
		
		ManagedReference<StructureDeed*> deed = server->getZoneServer()->getObject(deedID).castTo<StructureDeed*>();

		if (deed != NULL)
			deed->placeStructure(player, x, y, angle);
		
		return 0;
	}
	// Display a long green ruler to help with alignment
	if (selectedID == 70) {
		player->sendSystemMessage("Using rulers to help align structure...");
		
		// Execute client effect
		String emoteEffectPath = "clienteffect/house_plop_ruler.cef";
		player->playEffect( emoteEffectPath, "" ); 
	}
		
	return DeedImplementation::handleObjectMenuSelect(player, selectedID);
}

void StructureDeedImplementation::fillAttributeList(AttributeListMessage* alm, CreatureObject* object) {
	DeedImplementation::fillAttributeList(alm, object);

	TemplateManager* templateManager = TemplateManager::instance();

	uint32 structureCRC = generatedObjectTemplate.hashCode();

	SharedStructureObjectTemplate* structureTemplate = dynamic_cast<SharedStructureObjectTemplate*>(templateManager->getTemplate(structureCRC));

	if (structureTemplate == NULL)
		return;

	//Base Maintenance Rate
	int baseMaintenanceRate = structureTemplate->getBaseMaintenanceRate();
	int basePowerRate = structureTemplate->getBasePowerRate();

	if (baseMaintenanceRate > 0)
		alm->insertAttribute("examine_maintenance_rate", String::valueOf(baseMaintenanceRate) + " / hour"); //Base Maintenance Rate

	if (surplusMaintenance > 0)
		alm->insertAttribute("examine_maintenance", String::valueOf(surplusMaintenance)); //Surplus Maintenance

	if (surplusPower > 0)
		alm->insertAttribute("examine_power", String::valueOf(surplusPower)); //Surplus Power

	if (hopperSizeMax > 0)
		alm->insertAttribute("examine_hoppersize", String::valueOf((int)hopperSizeMax));

	if (extractionRate > 0)
		alm->insertAttribute("examine_extractionrate", String::valueOf(Math::getPrecision(extractionRate, 2)));

	for (int i = 0; i < structureTemplate->getTotalAllowedZones(); ++i) {
		String zoneName = structureTemplate->getAllowedZone(i);

		if (zoneName.isEmpty())
			continue;

		alm->insertAttribute("examine_scene", "@planet_n:" + zoneName); //Can Be Built On
	}
}

void StructureDeedImplementation::updateCraftingValues(CraftingValues* values, bool firstUpdate){
	setExtractionRate(values->getCurrentValue("extractrate"));
	setHopperSize(values->getCurrentValue("hoppersize"));
}
