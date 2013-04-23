//       _________ __                 __
//      /   _____//  |_____________ _/  |______     ____  __ __  ______
//      \_____  \\   __\_  __ \__  \\   __\__  \   / ___\|  |  \/  ___/
//      /        \|  |  |  | \// __ \|  |  / __ \_/ /_/  >  |  /\___ |
//     /_______  /|__|  |__|  (____  /__| (____  /\___  /|____//____  >
//             \/                  \/          \//_____/            \/
//  ______________________                           ______________________
//                        T H E   W A R   B E G I N S
//         Stratagus - A free fantasy real time strategy game engine
//
/**@name tileset.h - The tileset headerfile. */
//
//      (c) Copyright 1998-2005 by Lutz Sammer and Jimmy Salmon
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; only version 2 of the License.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//      02111-1307, USA.
//

#ifndef TILESET_H
#define TILESET_H

//@{

/*----------------------------------------------------------------------------
--  Declarations
----------------------------------------------------------------------------*/

#include "vec2i.h"
#include <vector>

struct lua_State;

/**
**  These are used for lookup tiles types
**  mainly used for the FOW implementation of the seen woods/rocks
**
**  @todo I think this can be removed, we can use the flags?
**  I'm not sure, if we have seen and real time to considere.
*/
enum TileType {
	TileTypeUnknown,    /// Unknown tile type
	TileTypeWood,       /// Any wood tile
	TileTypeRock,       /// Any rock tile
	TileTypeCoast,      /// Any coast tile
	TileTypeHumanWall,  /// Any human wall tile
	TileTypeOrcWall,    /// Any orc wall tile
	TileTypeWater       /// Any water tile
};

/// Single tile definition
struct CTileInfo {
public:
	CTileInfo() : BaseTerrain(0), MixTerrain(0)
	{}
	CTileInfo(unsigned char base, unsigned char mix) : BaseTerrain(base), MixTerrain(mix)
	{}

	bool operator ==(const CTileInfo &rhs) const
	{
		return BaseTerrain == rhs.BaseTerrain && MixTerrain == rhs.MixTerrain;
	}
	bool operator !=(const CTileInfo &rhs) const { return !(*this == rhs); }

public:
	unsigned char BaseTerrain; /// Basic terrain of the tile
	unsigned char MixTerrain;  /// Terrain mixed with this
};

/// Definition for a terrain type
struct SolidTerrainInfo {
	std::string TerrainName;  /// Name of the terrain
	// TODO: When drawing with the editor add some kind fo probabilities for every tile.
};

class CTile
{
public:
	unsigned short tile;  /// graphical pos
	unsigned short flag;  /// Flag
	CTileInfo tileinfo;   /// Tile descriptions
};

/// Tileset definition
class CTileset
{
public:
	void clear();

	unsigned int getTileCount() const { return tiles.size(); }

	bool isAWallTile(unsigned tileIndex) const;
	bool isARaceWallTile(unsigned tileIndex, bool human) const;
	bool isAWoodTile(unsigned tileIndex) const;
	bool isARockTile(unsigned tileIndex) const;

	const PixelSize &getPixelTileSize() const { return pixelTileSize; }

	unsigned getRemovedRockTile() const { return removedRockTile; }
	unsigned getRemovedTreeTile() const { return removedTreeTile; }
	unsigned getBottomOneTreeTile() const { return botOneTreeTile; }
	unsigned getTopOneTreeTile() const { return topOneTreeTile; }

	unsigned getHumanWallTile(int index) const;
	unsigned getOrcWallTile(int index) const;
	unsigned getHumanWallTile_broken(int index) const;
	unsigned getOrcWallTile_broken(int index) const;
	unsigned getHumanWallTile_destroyed(int index) const;
	unsigned getOrcWallTile_destroyed(int index) const;

	unsigned int getSolidTerrainCount() const;

	const std::string &getTerrainName(int solidTerrainIndex) const;
	int findTileIndex(unsigned char baseTerrain, unsigned char mixTerrain = 0) const;
	int getTileIndex(unsigned char baseTerrain, unsigned char mixTerrain, unsigned int quad) const;

	int findTileIndexByTile(unsigned int tile) const;
	unsigned int getTileNumber(int basic, bool random, bool filler) const;
	void fillSolidTiles(std::vector<unsigned int> *tiles) const;

	unsigned getQuadFromTile(unsigned int tile) const;
	int getTileIndexBySurrounding(unsigned short type,
								  int up, int right,
								  int bottom, int left) const;
	int tileFromQuad(unsigned fixed, unsigned quad) const;
	bool isEquivalentTile(unsigned int tile1, unsigned int tile2) const;

	void parse(lua_State *l);
	void buildTable(lua_State *l);

private:
	unsigned int getOrAddSolidTileIndexByName(const std::string &name);
	void buildWallReplacementTable();
	void parseSlots(lua_State *l, int t);
	void parseSpecial(lua_State *l);
	void parseSolid(lua_State *l);
	void parseMixed(lua_State *l);
	int findTilePath(int base, int goal, int length, std::vector<char> &marks, int *tileIndex) const;
public:
	std::string Name;           /// Nice name to display
	std::string ImageFile;      /// File containing image data

public:
	std::vector<CTile> tiles;

	// TODO: currently hardcoded
	std::vector<unsigned char> TileTypeTable;  /// For fast lookup of tile type
private:
	PixelSize pixelTileSize;    /// Size of a tile in pixel
	std::vector<SolidTerrainInfo> solidTerrainTypes; /// Information about solid terrains.
#if 1
	std::vector<int> mixedLookupTable;  /// Lookup for what part of tile used
	unsigned topOneTreeTile;   /// Tile for one tree top
	unsigned midOneTreeTile;   /// Tile for one tree middle
	unsigned botOneTreeTile;   /// Tile for one tree bottom
	unsigned removedTreeTile;  /// Tile placed where trees are gone
	int woodTable[20];     /// Table for tree removable
	unsigned topOneRockTile;   /// Tile for one rock top
	unsigned midOneRockTile;   /// Tile for one rock middle
	unsigned botOneRockTile;   /// Tile for one rock bottom
	unsigned removedRockTile;  /// Tile placed where rocks are gone
	int rockTable[20];     /// Removed rock placement table
	unsigned humanWallTable[16];  /// Human wall placement table
	unsigned orcWallTable[16];    /// Orc wall placement table
#endif
};

/*----------------------------------------------------------------------------
--  Functions
----------------------------------------------------------------------------*/

extern void TilesetCclRegister(); /// Register CCL features for tileset

//@}

#endif // !TILESET_H
