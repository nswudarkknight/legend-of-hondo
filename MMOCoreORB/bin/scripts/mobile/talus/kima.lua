kima = Creature:new {
	objectName = "@mob/creature_names:kima",
	socialGroup = "kima",
	faction = "",
	level = 18,
	chanceHit = 0.32,
	damageMin = 160,
	damageMax = 170,
	baseXp = 1257,
	baseHAM = 4500,
	baseHAMmax = 5500,
	armor = 0,
	resists = {115,120,5,-1,5,5,-1,-1,-1},
	meatType = "meat_herbivore",
	meatAmount = 7,
	hideType = "hide_bristley",
	hideAmount = 8,
	boneType = "bone_mammal",
	boneAmount = 7,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/kima.iff"},
	controlDeviceTemplate = "object/intangible/pet/kima_hue.iff",
	lootGroups = {		{			groups = {				{group = "junk", chance = 10000000},			}		}	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"",""},
		{"intimidationattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(kima, "kima")
