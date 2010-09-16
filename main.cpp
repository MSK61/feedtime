
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <set>
#include <stdlib.h>
#include <vector>

using std::vector;
/**
 * @brief Row of squares
 */
typedef vector< bool > SquareRow;
namespace
{
    /**
     * @brief Farm square
     */
    class Square
    {
        public:
            /**
             * @brief Creates an uninitialized square
             */
            Square();
            /**
             * @brief Creates a square at the specified coordinates
             *
             * @param row row coordinate of the square
             * @param col column coordinate of the square
             */
            Square(unsigned short int row, unsigned short int col);
            /**
             * @brief Compares this square to another
             *
             * The comparison is based on the sqaure coordinates. First row
             * coordinates are used if different, otherwise column coordinates
             * are engaged.
             *
             * @param rhs other sqaure to compare with
             * @return    true if this square has less coorindates than the
             *            other's, otherwise false
             */
            const bool operator <(const Square& rhs) const;
            /**
             * @brief Row coordinate of this square
             */
            unsigned short int itsRow;
            /**
             * @brief Column coordinate of this square
             */
            unsigned short int itsCol;
    };

    Square::Square()
    {
    }

    Square::Square(unsigned short int row, unsigned short int col):
            itsRow(row), itsCol(col)
    {
    }

    const bool Square::operator <(const Square& rhs) const
    {

        if (itsRow < rhs.itsRow) return true;

        if (itsRow > rhs.itsRow) return false;

        return itsCol < rhs.itsCol;

    }
}

int main(int argc, char *argv[])
{
    using std::max;
    using std::min;
    using std::set;
    set< Square > analysisSet;// set of squares to analyze
    Square centerSquare;
    set< Square >::const_iterator curSquare;
    // child coordinates
    unsigned short int childRow;
    unsigned short int childCol;
    unsigned short int curRow = 0;
    unsigned short int curCol;
    vector< SquareRow > eaten;// eating matrix
    // boundaries of square children
    unsigned short int endRow;
    unsigned short int startCol;
    unsigned short int endCol;
    // square types
    static const char grass = '.';
    static const char rock = '*';
    char sym;
    unsigned long grassCount;
    // I/O files
    static const char inFileName[] = "feedtime.in";
    std::ifstream inFile(inFileName);
    static const char outFileName[] = "feedtime.out";
    std::ofstream outFile(outFileName);
    unsigned long maxGrass = 0;
    // span of the children around a central square
    static const short int startExtent = -1;
    static const short int endExtent = 2;
    // farm dimensions
    unsigned short int w;
    unsigned short int h;

    /// Read the problem description.
    inFile >> w >> h;
    eaten.resize(h, SquareRow(w));

    for (; curRow < h; curRow++) for (curCol = 0; curCol < w; curCol++)
        {

            inFile >> sym;

            switch (sym)
            {

                case grass:

                    eaten[curRow][curCol] = false;
                    break;

                case rock:

                    eaten[curRow][curCol] = true;
                    break;

                default:

                    ;

            }

        }

    inFile.close();

    /// Detect and calculate the size of all possible pastures.
    for (curRow = 0; curRow < h; curRow++)
        for (curCol = 0; curCol < w; curCol++) if (!eaten[curRow][curCol])
            {

                /// Calculate the size of the current pasture.
                grassCount = 0;
                analysisSet.insert(Square(curRow, curCol));

                do
                {

                    curSquare = analysisSet.begin();
                    centerSquare = *curSquare;
                    analysisSet.erase(curSquare);
                    eaten[centerSquare.itsRow][centerSquare.itsCol] = true;
                    grassCount++;
                    /// Calculate the boundaries of adjacent squares around the
                    /// current square.
                    endRow = min((unsigned short int)(
                                     centerSquare.itsRow + endExtent), h);
                    startCol = max(centerSquare.itsCol + startExtent, 0);
                    endCol = min((unsigned short int)(
                                     centerSquare.itsCol + endExtent), w);

                    /// Consider candidate adjacent nodes for future analysis.
                    for (childRow = max(centerSquare.itsRow + startExtent, 0);
                            childRow < endRow; childRow++)
                        for (childCol = startCol; childCol < endCol; childCol++)
                            if (!eaten[childRow][childCol])
                                analysisSet.insert(Square(childRow, childCol));

                }
                while (!analysisSet.empty());

                /// Update the maximum pasture size if necessary.
                if (grassCount > maxGrass) maxGrass = grassCount;

            }

    /// Save the result.
    outFile << maxGrass;
    outFile.close();
    return EXIT_SUCCESS;
}
