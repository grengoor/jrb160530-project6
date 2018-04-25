/*
 * John Bernard
 * jrb160530
 * CS3377.501
 */

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "cdk.h"


#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 18
#define MATRIX_NAME_STRING "Test Matrix"

using namespace std;

class BinaryFileHeader
{
public:
    uint32_t magicNumber;
    uint32_t versionNumber;
    uint64_t numRecords;
};

/*
 * Records inn the file have a fixed length buffer that will hold a C-Style
 * string. This is the size of the fixed length buffer.
 */
const int maxRecordStringLength = 25;

class BinaryFileRecord
{
public:
    uint8_t strLength;
    char stringBuffer[maxRecordStringLength];
};


template<class T>
std::string int_to_hex(T i)
{
    std::stringstream stream;
    stream << "0x"
           << std::setfill('0') << std::setw(sizeof(T)*2)
           << std::hex << i;
    return stream.str();
}


int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "R1", "R2", "R3", "R4", "R5"};
  const char 		*columnTitles[] = {"C0", "C1", "C2", "C3", "C4", "C5"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  ifstream file;
  BinaryFileHeader header;
  BinaryFileRecord *records;

  file.open("cs3377.bin", ios::in | ios::binary);

  /* Read the header record */
  file.read((char *) &header.magicNumber, sizeof(header.magicNumber));
  file.read((char *) &header.versionNumber, sizeof(header.versionNumber));
  file.read((char *) &header.numRecords, sizeof(header.numRecords));
  if (header.numRecords > maxRecordStringLength) {
      file.close();
      cerr << "The number of records " << header.numRecords
           << "is too long.\n";
      return 1;
  }

  /* Read string records */
  //records = new BinaryFileRecord[header.numRecords];

  file.close();

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */
  string dispStr1 = "Magic: " + int_to_hex(header.magicNumber);
  string dispStr2 = "Version: " + to_string(header.versionNumber);
  string dispStr3 = "NumRecords: " + to_string(header.numRecords);
  /* std::string dispStr2 = */ 
  setCDKMatrixCell(myMatrix, 1, 1, dispStr1.c_str());
  setCDKMatrixCell(myMatrix, 1, 2, dispStr2.c_str());
  setCDKMatrixCell(myMatrix, 1, 3, dispStr3.c_str());
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  //delete[] records;

  // Cleanup screen
  endCDK();
}
