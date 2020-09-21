/*********************************************************************
(c) Matt Marchant & contributors 2016 - 2019
http://trederia.blogspot.com

tmxlite - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

/*
Creates an SFML drawable from an Orthogonal tmx map layer.
This is an example of drawing with SFML - not all features
are implemented.
*/

#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/detail/Log.hpp>

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <limits>
#include <iostream>
#include <cmath>

namespace space
{
    class ResourceManager;

    class MapLayer final : public sf::Drawable
    {
    public:

        // Constructor
        MapLayer(const tmx::Map& map, ResourceManager &resourceManager, std::size_t idx);
        ~MapLayer() = default;
        MapLayer(const MapLayer&) = delete;
        MapLayer& operator = (const MapLayer&) = delete;

        // Methods
        const sf::FloatRect& getGlobalBounds() const { return m_globalBounds; }

        void setTile(int tileX, int tileY, tmx::TileLayer::Tile tile, bool refresh = true);
        tmx::TileLayer::Tile getTile(int tileX, int tileY);

        void setColor(int tileX, int tileY, sf::Color color, bool refresh = true);
        sf::Color getColor(int tileX, int tileY);

        void update(sf::Time elapsed);

    private:
        class Chunk;
        using ChunkPtr = std::unique_ptr<Chunk>;

        //increasing m_chunkSize by 4; fixes render problems when mapsize != chunksize
        sf::Vector2f m_chunkSize = sf::Vector2f(512.f, 512.f);
        sf::Vector2u m_chunkCount;
        sf::Vector2u m_MapTileSize;   // general Tilesize of Map
        sf::FloatRect m_globalBounds;

        struct AnimationState
        {
            sf::Vector2u tileCords;
            sf::Time startTime;
            tmx::Tileset::Tile animTile;
            std::uint8_t flipFlags;
        };

        std::vector<ChunkPtr> m_chunks;
        mutable std::vector<Chunk*> m_visibleChunks;
        mutable std::vector<Chunk*> m_visibleChunksTemp;
        ChunkPtr& getChunkAndTransform(int x, int y, sf::Vector2u& chunkRelative);
        void createChunks(const tmx::Map& map, ResourceManager &resourceManager, const tmx::TileLayer& layer);

        void updateVisibility(const sf::View& view) const;
        void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;

        class Chunk final : public sf::Transformable, public sf::Drawable
        {
        public:
            using Tile = std::array<sf::Vertex, 6u>;

            Chunk(const tmx::TileLayer& layer, const std::vector<const tmx::Tileset*> &tilesets,
                const sf::Vector2f& position, const sf::Vector2f& tileCount, const sf::Vector2u& tileSize,
                std::size_t rowSize, ResourceManager& tr, const std::map<std::uint32_t, tmx::Tileset::Tile>& animTiles);

            ~Chunk() = default;
            Chunk(const Chunk&) = delete;

            Chunk& operator = (const Chunk&) = delete;

            std::vector<AnimationState>& getActiveAnimations() { return m_activeAnimations; }
            bool empty() const { return m_chunkArrays.empty(); }

            void generateTiles(bool registerAnimation = false);
            tmx::TileLayer::Tile getTile(int x, int y) const;
            void setTile(int x, int y, tmx::TileLayer::Tile tile, bool refresh);

            sf::Color getColor(int x, int y) const;
            void setColor(int x, int y, sf::Color color, bool refresh);

            void maybeRegenerate(bool refresh);
            int calcIndexFrom(int x, int y) const;

            void flipY(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3);
            void flipX(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3);
            void flipD(sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3);
            void doFlips(std::uint8_t bits, sf::Vector2f *v0, sf::Vector2f *v1, sf::Vector2f *v2, sf::Vector2f *v3);

        private:
            class ChunkArray final : public sf::Drawable
            {
            public:
                // Fields
                using Ptr = std::unique_ptr<ChunkArray>;
                tmx::Vector2u tileSetSize;
                sf::Vector2u tsTileCount;
                std::uint32_t m_firstGID, m_lastGID;

                // Constructor
                explicit ChunkArray(const sf::Texture& t, const tmx::Tileset& ts);

                ~ChunkArray() = default;
                ChunkArray(const ChunkArray&) = delete;
                ChunkArray& operator = (const ChunkArray&) = delete;

                // Methods
                void reset();
                void addTile(const Chunk::Tile& tile);
                sf::Vector2u getTextureSize() const { return m_texture.getSize(); }

            private:
                const sf::Texture& m_texture;
                std::vector<sf::Vertex> m_vertices;
                void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
            };

            // Fields
            sf::Uint8 layerOpacity;     // opacity of the layer
            sf::Vector2f layerOffset;   // Layer offset
            sf::Vector2u mapTileSize;   // general Tilesize of Map
            sf::Vector2f chunkTileCount;   // chunk tilecount
            std::vector<tmx::TileLayer::Tile> m_chunkTileIDs; // stores all tiles in this chunk for later manipulation
            std::vector<sf::Color> m_chunkColors; // stores colors for extended color effects
            std::map<std::uint32_t, tmx::Tileset::Tile> m_animTiles;    // animation catalogue
            std::vector<AnimationState> m_activeAnimations;     // Animations to be done in this chunk
            std::vector<ChunkArray::Ptr> m_chunkArrays;

            // Methods
            void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
        };

    };

} // namespace space