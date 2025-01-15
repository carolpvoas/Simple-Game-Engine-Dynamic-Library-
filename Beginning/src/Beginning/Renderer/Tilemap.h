#pragma once
#include <vector>
#include <memory>
#include "Sprite.h"

//CLASS DEFINITION: Hold the tilemap data for the RenderComponent to use it
namespace Beginning {

    class Tilemap {
    public:
        Tilemap(int rows, int cols, int tileWidth, int tileHeight)
            : rows(rows), cols(cols), tileWidth(tileWidth), tileHeight(tileHeight) {
            //Resizing the tiles vector array to hold rows number of vectors, each containing cols number of nullptr elements (indicating empty tiles).
            
            tiles.resize(rows, std::vector<std::shared_ptr<Beginning::Sprite>>(cols, nullptr));
        }

        void SetTile(int row, int col, std::shared_ptr<Beginning::Sprite> tile) {
            //Sets the tile in the tilemap
            if (row >= 0 && row < rows && col >= 0 && col < cols) {
                tiles[row][col] = tile;
            }
        }

        std::shared_ptr<Beginning::Sprite> GetTile(int row, int col) const {
            if (row >= 0 && row < rows && col >= 0 && col < cols) {
                return tiles[row][col];
            }
            return nullptr;
        }

        int GetRows() const { return rows; }
        int GetCols() const { return cols; }
        int GetTileWidth() const { return tileWidth; }
        int GetTileHeight() const { return tileHeight; }


    private:
        int rows, cols;
        int tileWidth, tileHeight;

        //This is a 2D vector that holds the tiles in the tilemap.Each element in the outer vector represents a row, and each element in the inner vector represents a column.
        std::vector<std::vector<std::shared_ptr<Beginning::Sprite>>> tiles;
    };

}