#include "simulation/ElementCommon.h"

static void create(ELEMENT_CREATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);
static void changeType(ELEMENT_CHANGETYPE_FUNC_ARGS);

int RADIUS = 3;

void Element::Element_PKUP()
{
	Identifier = "DEFAULT_PT_PKUP";
	Name = "PKUP";
	Colour = 0xFD9D18_rgb;
	MenuVisible = 1;
	MenuSection = SC_SENSOR;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 0;
	Description = "Pickup. Generic item that can be picked up by the player.";

	Properties = TYPE_SOLID;
	CarriesTypeIn = 1U << FIELD_CTYPE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	DefaultProperties.ctype = 1;

	// Update = &update;
	Graphics = &graphics;
	Create = &create;
	ChangeType = &changeType;
}

static void create(ELEMENT_CREATE_FUNC_ARGS)
{
	printf("created dtec_id: %d\n", i);

	auto parts = sim->parts;
	for (int cx = -RADIUS; cx <= RADIUS; cx++)
        for (int cy = -RADIUS; cy <= RADIUS; cy++)
		{
			if (floor(sqrt(cx*cx + cy*cy)) == RADIUS)
			{
				int dtec = sim->create_part(-1, x + cx, y + cy, PT_DTEC);
				parts[dtec].ctype = parts[i].ctype;
				parts[dtec].tmp3 = i; // every DTEC knows of its PKUP
				// parts[i].tmp = dtec; // PKUP knows its last point
			}
		}
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	*pixel_mode = PMODE_NONE; //Don't draw pixel
	//Returning 1 to cache
	return 1;
}

// handle deletion
static void changeType(ELEMENT_CHANGETYPE_FUNC_ARGS)
{
	if (to != PT_NONE)
		return;

	auto parts = sim->parts;
	for (int cx = -RADIUS; cx <= RADIUS; cx++)
        for (int cy = -RADIUS; cy <= RADIUS; cy++)
		{
			if (floor(sqrt(cx*cx + cy*cy)) == RADIUS)
			{
				sim->delete_part(x+cx, y+cy);
				int embr = sim->create_part(-1, x + cx, y + cy, PT_EMBR);
				if (embr<0)
					continue;
				parts[embr].ctype = sim->elements[parts[i].ctype].Colour.Pack();
				parts[embr].life = 50;
				parts[embr].vx = 0;
				parts[embr].vy = -4;
			}
		}
}